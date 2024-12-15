from colorama import Fore, Style, init
import re

init(autoreset=True)

ansi_escape = re.compile(r'\x1b\[[0-9;]*[mK]')
def remove_ansi_codes(text):
    """Remove ANSI escape codes from a string."""
    return ansi_escape.sub('', text)

def print_header(title: str):
    """Print a styled header with a title."""
    line_length = len(title) + 4
    print(f"\n{Fore.MAGENTA}{Style.BRIGHT}{'=' * line_length}")
    print(f"{Fore.MAGENTA}{Style.BRIGHT}| {title} |")
    print(f"{Fore.MAGENTA}{Style.BRIGHT}{'=' * line_length}\n")

def print_success(message: str):
    """Print a success message in green."""
    print(f"{Fore.GREEN}{Style.BRIGHT}✅ {message}{Style.RESET_ALL}")

def print_warning(message: str):
    """Print a warning message in yellow."""
    print(f"{Fore.YELLOW}{Style.BRIGHT}⚠️ {message}{Style.RESET_ALL}")

def print_error(message: str):
    """Print an error message in red."""
    print(f"{Fore.RED}{Style.BRIGHT}❌ {message}{Style.RESET_ALL}")

def print_info(message: str):
    """Print an informational message in cyan."""
    print(f"{Fore.CYAN}{Style.BRIGHT}ℹ️ {message}{Style.RESET_ALL}")

def print_progress(message: str):
    """Print a progress update (e.g., test running)."""
    print(f"{Fore.YELLOW}{Style.BRIGHT}⏳ {message}...{Style.RESET_ALL}")


def print_summary(tests_passed: int, tests_failed: int, tests_skipped: int):
    """Print a refined summary of the test run with passed, failed, and skipped counts."""
    
    # Calculate total tests
    total_tests = tests_passed + tests_failed + tests_skipped
    
    # Ensure percentages are calculated only if total tests are greater than zero
    passed_percentage = (tests_passed / total_tests) * 100 if total_tests else 0
    failed_percentage = (tests_failed / total_tests) * 100 if total_tests else 0
    skipped_percentage = (tests_skipped / total_tests) * 100 if total_tests else 0
    
    # Calculate dynamic widths
    label_width = max(len("Tests Passed:"), len("Tests Failed:"), len("Tests Skipped:"))
    value_width = max(len(str(tests_passed)), len(str(tests_failed)), len(str(tests_skipped)))
    percentage_width = max(len(f"{passed_percentage:6.2f}%"), len(f"{failed_percentage:6.2f}%"), len(f"{skipped_percentage:6.2f}%"))
    
    # Calculate space between columns dynamically
    column_space = 2  # Space between label, value, and percentage columns
    
    # Total width for formatting
    total_width = label_width + value_width + percentage_width + 2 * column_space + 4 

    # Print the header with separators
    print(f"\n{Fore.MAGENTA}{Style.BRIGHT}{'=' * total_width}{Style.RESET_ALL}")
    print(f"{Fore.MAGENTA}{Style.BRIGHT}|{'Test Summary'.center(total_width)}|")
    print(f"{Fore.MAGENTA}{Style.BRIGHT}{'=' * total_width}")

    # Print each category with labels, values, and percentages, ensuring alignment
    print(f"{Fore.GREEN}{Style.BRIGHT}| {'Tests Passed:'.ljust(label_width)} {str(tests_passed).rjust(value_width)} ({passed_percentage:6.2f}%) |")
    print(f"{Fore.RED}{Style.BRIGHT}| {'Tests Failed:'.ljust(label_width)} {str(tests_failed).rjust(value_width)} ({failed_percentage:6.2f}%) |")
    print(f"{Fore.YELLOW}{Style.BRIGHT}| {'Tests Skipped:'.ljust(label_width)} {str(tests_skipped).rjust(value_width)} ({skipped_percentage:6.2f}%) |")

    # Print footer with separator for clarity
    print(f"{Fore.MAGENTA}{Style.BRIGHT}{'=' * total_width}{Style.RESET_ALL}\n")
