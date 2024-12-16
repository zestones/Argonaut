from colorama import Fore, Style, init, Back
import textwrap
import re

init(autoreset=True)

ansi_escape = re.compile(r'\x1b\[[0-9;]*[mK]')
def remove_ansi_codes(text):
    """Remove ANSI escape codes from a string."""
    return ansi_escape.sub('', text)


def print_header(title: str, description: str, width: int = 90):
    """Print a styled header with a title and program description."""
    
    # Calculate the border based on the total desired width
    border_line = f"{Fore.MAGENTA}{Style.BRIGHT}{'~' * width}"

    # Center the title based on the total width
    centered_title = title.center(width)

    # Wrap the description to fit within the desired width
    wrapped_description = textwrap.fill(description, width=width - 4)

    # Print the border, centered title, and wrapped description
    print(f"\n{border_line}")
    print(f"{Fore.CYAN}{Style.BRIGHT}{'  ' + centered_title + '  '}")
    
    # Print wrapped description line by line, each centered
    for line in wrapped_description.splitlines():
        print(f"{Fore.WHITE}{Style.NORMAL}{'  ' + line.center(width - 4) + '  '}")

    print(f"{border_line}\n")
    
def print_footer(footer_text: str, width: int = 90):
    """Print a styled footer with a message."""
    
    # Calculate the border based on the total desired width
    border_line = f"{Fore.MAGENTA}{Style.BRIGHT}{'~' * width}"

    # Center the footer text based on the total width
    centered_footer = footer_text.center(width)

    # Print the border and centered footer text
    print(f"\n{border_line}")
    print(f"{Fore.CYAN}{Style.BRIGHT}{'  ' + centered_footer + '  '}")

    # Print the footer border line
    print(f"{border_line}\n")

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


def print_summary(tests_passed: int, tests_failed: int, tests_skipped: int, width: int = 90):
    """Print a styled summary of the test run with passed, failed, skipped counts, and total tests."""
    
    # Calculate total tests
    total_tests = tests_passed + tests_failed + tests_skipped
    
    # Ensure percentages are calculated only if total tests are greater than zero
    passed_percentage = (tests_passed / total_tests) * 100 if total_tests else 0
    failed_percentage = (tests_failed / total_tests) * 100 if total_tests else 0
    skipped_percentage = (tests_skipped / total_tests) * 100 if total_tests else 0
    
    # Adjust total width to accommodate the labels, values, and percentages
    total_width = width
    
    # Add padding for borders and spacing between columns
    padding = 4  # A bit more space between columns for visual clarity
    
    # Divide the total width, subtracting space for borders and padding
    label_width = (total_width - padding * 2) // 3  # Split width evenly
    value_width = (total_width - padding * 2) // 3
    percentage_width = (total_width - padding * 2) // 3
    
    # Border line for the summary box (same as header/footer)
    border_line = f"{'=' * total_width}"
    
    # Center the title based on the specified width
    centered_title = f"{Fore.CYAN}{Style.BRIGHT}{'Test Summary'.center(total_width)}{Style.RESET_ALL}"
    
    # Print the header with separator
    print(f"\n{border_line}")
    print(f"{centered_title}")
    print(f"{border_line}")
    
    # Column headers, with labels matching the cell width
    print(f"| {Back.BLACK}{'Metric'.center(label_width)}{Style.RESET_ALL} | {Back.BLACK}{'Count'.center(value_width)}{Style.RESET_ALL} | {Back.BLACK}{'Percentage'.center(percentage_width)}{Style.RESET_ALL} |")
    print(f"{'+' + '-' * (label_width + 2) + '+' + '-' * (value_width + 2) + '+' + '-' * (percentage_width + 2) + '|'}")
    
    # Print the results in a table-like format with proper alignment
    print(f"| {Back.BLACK}{Fore.GREEN}{'Tests Passed:'.ljust(label_width)}{Style.RESET_ALL} | {Back.BLACK}{Fore.GREEN}{str(tests_passed).rjust(value_width)}{Style.RESET_ALL} | {Back.BLACK}{Fore.GREEN}{f'{passed_percentage:6.2f}%'.rjust(percentage_width)}{Style.RESET_ALL} |")
    print(f"| {Back.BLACK}{Fore.RED}{'Tests Failed:'.ljust(label_width)}{Style.RESET_ALL} | {Back.BLACK}{Fore.RED}{str(tests_failed).rjust(value_width)}{Style.RESET_ALL} | {Back.BLACK}{Fore.RED}{f'{failed_percentage:6.2f}%'.rjust(percentage_width)}{Style.RESET_ALL} |")
    print(f"| {Back.BLACK}{Fore.YELLOW}{'Tests Skipped:'.ljust(label_width)}{Style.RESET_ALL} | {Back.BLACK}{Fore.YELLOW}{str(tests_skipped).rjust(value_width)}{Style.RESET_ALL} | {Back.BLACK}{Fore.YELLOW}{f'{skipped_percentage:6.2f}%'.rjust(percentage_width)}{Style.RESET_ALL} |")
    
    # Print the total tests row
    print(f"{'+' + '-' * (label_width + 2) + '+' + '-' * (value_width + 2) + '+' + '-' * (percentage_width + 2) + '|'}")
    print(f"| {Back.BLACK}{Fore.MAGENTA}{'Total Tests:'.ljust(label_width)}{Style.RESET_ALL} | {Back.BLACK}{Fore.MAGENTA}{str(total_tests).rjust(value_width)}{Style.RESET_ALL} | {Back.BLACK}{Fore.MAGENTA}{'100.00%'.rjust(percentage_width)}{Style.RESET_ALL} |")
    
    # Bottom border to close the table
    print(f"{border_line}")
