import subprocess
import filecmp
import os

from regression.config import TEST_DIR, OUTPUTS_DIR, ACTUAL_OUTPUTS_DIR, COMPILER_CMD, COMPILER_OPTIONS, ensure_directory_exists
from regression.helpers import print_success, print_error, print_warning, print_progress, remove_ansi_codes

class TestRunner:
    def __init__(self):
        self.tests_passed = 0
        self.tests_failed = 0
        self.tests_skipped = 0
        
    @staticmethod
    def run_test(input_file: str, expected_output_file: str, actual_output_file: str) -> bool:
        """
        Runs a single test case and compares output.

        Args:
            input_file (str): The input file to compile.
            expected_output_file (str): The expected output file.
            actual_output_file (str): The actual output file.

        Returns:
            bool: True if the test passed, False otherwise.
        """
        # Run the compiler
        with open(actual_output_file, "w") as output_file:
            result = subprocess.run([COMPILER_CMD, COMPILER_OPTIONS, input_file], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

            output = remove_ansi_codes(result.stdout.decode())
            output_file.write(output)        
            
        # Compare actual output to expected output
        return filecmp.cmp(expected_output_file, actual_output_file, shallow=False)

    def run_tests(self) -> bool:
        """
        Iterates through all error examples and runs tests.

        Returns:
            bool: True if all tests passed, False otherwise.
        """
        all_passed = True
        print_progress("Starting regression tests")

        for root, _, files in os.walk(TEST_DIR):
            for file in files:
                if file.endswith(".arn"):
                    input_file = os.path.join(root, file)
                    relative_path = os.path.relpath(input_file, TEST_DIR)

                    expected_output_file, actual_output_file = self._get_output_files(relative_path)

                    # Ensure output directory exists
                    ensure_directory_exists(os.path.dirname(actual_output_file))

                    # Run the test and report result
                    all_passed &= self._run_and_report_test(input_file, expected_output_file, actual_output_file, relative_path)

        return all_passed

    def _get_output_files(self, relative_path: str) -> tuple[str, str]:
        """
        Determines the corresponding expected and actual output files.

        Args:
            relative_path (str): The relative path of the input file.

        Returns:
            tuple: A tuple containing the expected and actual output file paths.
        """
        expected_output_file = os.path.join(OUTPUTS_DIR, relative_path.replace(".arn", ".out"))
        actual_output_file = os.path.join(ACTUAL_OUTPUTS_DIR, relative_path.replace(".arn", ".out"))
        return expected_output_file, actual_output_file

    def _run_and_report_test(self, input_file: str, expected_output_file: str, actual_output_file: str, relative_path: str) -> bool:
        """
        Runs a single test and reports the result.

        Args:
            input_file (str): The path to the input file.
            expected_output_file (str): The path to the expected output file.
            actual_output_file (str): The path to the actual output file.
            relative_path (str): The relative path of the test file (used for printing).

        Returns:
            bool: True if the test passed, False otherwise.
        """
        if os.path.exists(expected_output_file):
            if self.run_test(input_file, expected_output_file, actual_output_file):
                print_success(f"[Test Passed] {relative_path}")
                self.tests_passed += 1
                return True
            else:
                print_error(f"[Test Failed] {relative_path}")
                self.tests_failed += 1
                return False
        else:
            print_warning(f" [Test Skipped] {relative_path} (no expected output)")
            self.tests_skipped += 1
            return True  # Treat skipped tests as not failing, they don't affect the result
