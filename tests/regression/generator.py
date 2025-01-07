import os
import subprocess
from regression.config import TEST_DIR, OUTPUTS_DIR, COMPILER_CMD, COMPILER_OPTIONS, ensure_directory_exists
from regression.helpers import print_success, print_error, print_progress, remove_ansi_codes

class OutputGenerator:
    @staticmethod
    def generate_expected_outputs():
        """
        Generates expected outputs for all test cases.
        """
        print_progress("Starting output generation")

        for root, _, files in os.walk(TEST_DIR):
            for file in files:
                if file.endswith(".arn"):
                    input_file = os.path.join(root, file)
                    relative_path = os.path.relpath(input_file, TEST_DIR)
                    expected_output_file = os.path.join(OUTPUTS_DIR, relative_path.replace(".arn", ".out"))

                    ensure_directory_exists(os.path.dirname(expected_output_file))

                    try:
                        with open(expected_output_file, "w") as output_file:
                            result = subprocess.run([COMPILER_CMD, COMPILER_OPTIONS, input_file], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                            
                            output = remove_ansi_codes(result.stdout.decode())
                            output_file.write(output)

                        print_success(f"[Generated] {input_file}")  

                    except Exception as e:
                        print_error(f"[Error] {input_file}: {e}")
                        continue