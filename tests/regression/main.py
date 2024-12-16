import sys
import argparse

from regression.generator import OutputGenerator
from regression.runner import TestRunner
from regression.helpers import print_header, print_footer, print_info, print_success, print_error, print_summary

from colorama import Fore, Style, init

init(autoreset=True)


def main():
    parser = argparse.ArgumentParser(description="Regression Test Runner")
    parser.add_argument("--generate", action="store_true", help="Generate expected outputs.")
    parser.add_argument("--run", action="store_true", help="Run regression tests.")
    
    args = parser.parse_args()

    if not any(vars(args).values()):
        parser.print_help()
        sys.exit(1)

    # Header
    print_header(
        "Regression Test Runner", 
        " A regression testing tool that validates compiler output against predefined test cases."
        " It automates the comparison of actual and expected outputs to ensure the compiler's correctness and prevent regressions after changes."
        " The tool can generate expected outputs or run tests to verify the compiler’s behavior.")

    if args.generate:
        print_info("Generating expected outputs...")
        try:
            OutputGenerator.generate_expected_outputs()
            print_success("Expected outputs generated successfully.")
        except Exception as e:
            print_error(f"Error generating outputs: {e}")

    if args.run:
        print_info("Running regression tests...")
        
        runner = TestRunner()
        all_passed = runner.run_tests()
        
        print_summary(runner.tests_passed, runner.tests_failed, runner.tests_skipped)
        if not all_passed: sys.exit(1) # Return non-zero exit code if any test failed

    print_footer("End of Regression Test Run", width=100)

if __name__ == "__main__":
    main()