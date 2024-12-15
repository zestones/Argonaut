import os

ERRORS_DIR = "example/compilation/errors/"
OUTPUTS_DIR = "example/compilation/.out/expected_outputs/"
ACTUAL_OUTPUTS_DIR = "example/compilation/.out/actual_outputs/"
COMPILER_CMD = os.path.join(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..')), 'compiler.exe')
COMPILER_OPTIONS = "-vf"

def ensure_directory_exists(path: str):
    """
    Ensures that the directory at the given path exists.

    Args:
        path (str): The path to the directory to ensure exists.
    """
    os.makedirs(path, exist_ok=True)
