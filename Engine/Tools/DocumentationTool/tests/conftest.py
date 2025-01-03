"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Pytest configuration and fixtures.
"""

import pytest
import logging
from pathlib import Path
import shutil
import tempfile

@pytest.fixture
def clean_test_env(tmp_path):
    """Create a clean test environment"""
    test_dir = tmp_path / "doc_tool_test"
    test_dir.mkdir(parents=True, exist_ok=True)
    
    # Create standard directories
    (test_dir / "source").mkdir()
    (test_dir / "docs").mkdir()
    (test_dir / "examples").mkdir()
    
    yield test_dir
    
    # Cleanup
    shutil.rmtree(test_dir, ignore_errors=True)

@pytest.fixture
def mock_logger(mocker):
    """Mock logger for testing"""
    return mocker.patch("logging.getLogger") 