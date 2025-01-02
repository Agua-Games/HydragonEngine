"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Test configuration and shared fixtures.
"""

import pytest
import shutil
from pathlib import Path

@pytest.fixture(scope="session")
def test_root():
    """Create and manage test root directory"""
    root = Path("test_files")
    root.mkdir(exist_ok=True)
    yield root
    shutil.rmtree(root)

@pytest.fixture(scope="function")
def clean_test_env(test_root):
    """Provide clean test environment for each test"""
    # Clean existing files
    for item in test_root.glob("*"):
        if item.is_file():
            item.unlink()
        elif item.is_dir():
            shutil.rmtree(item)
    
    # Create standard test directories
    dirs = ["docs", "source", "examples", "templates", "output"]
    for dir_name in dirs:
        (test_root / dir_name).mkdir(exist_ok=True)
    
    yield test_root

@pytest.fixture
def mock_logger(mocker):
    """Mock logger for testing log messages"""
    return mocker.patch("logging.getLogger") 