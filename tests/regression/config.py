import os

TEST_DIR = "examples/compilation/"
OUTPUTS_DIR = "examples/compilation/.out/expected_outputs/"
ACTUAL_OUTPUTS_DIR = "examples/compilation/.out/actual_outputs/"
COMPILER_CMD = os.path.join(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'bin')), 'argoc')
COMPILER_OPTIONS = "-va"

def ensure_directory_exists(path: str):
    """
    Ensures that the directory at the given path exists.

    Args:
        path (str): The path to the directory to ensure exists.
    """
    os.makedirs(path, exist_ok=True)
