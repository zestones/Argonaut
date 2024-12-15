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
            result = subprocess.run([COMPILER_CMD, COMPILER_OPTIONS, input_file],
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            
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
                if file.endswith(".txt"):
                    input_file = os.path.join(root, file)

                    # Determine corresponding output files
                    relative_path = os.path.relpath(input_file, TEST_DIR)
                    expected_output_file = os.path.join(OUTPUTS_DIR, relative_path.replace(".txt", ".out"))
                    actual_output_file = os.path.join(ACTUAL_OUTPUTS_DIR, relative_path.replace(".txt", ".out"))

                    # Ensure output directory exists
                    ensure_directory_exists(os.path.dirname(actual_output_file))

                    # Run the test
                    if os.path.exists(expected_output_file):
                        if self.run_test(input_file, expected_output_file, actual_output_file):
                            print_success(f"[Test Passed] {relative_path}")
                            self.tests_passed += 1
                        else:
                            print_error(f"[Test Failed] {relative_path}")
                            self.tests_failed += 1
                            all_passed = False
                    else:
                        print_warning(f" [Test Skipped] {relative_path} (no expected output)")
                        self.tests_skipped += 1

        return all_passed