"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Test runner for documentation tool with detailed reporting.
"""

import unittest
import logging
import sys
from pathlib import Path
import pytest
from typing import Optional
from datetime import datetime

# Setup logging
logger = logging.getLogger(__name__)

def verify_directory():
    """Verify we're running from the correct directory"""
    current_dir = Path.cwd()
    expected_path = Path("Engine/Tools/DocumentationTool")
    
    if not any(part == "DocumentationTool" for part in current_dir.parts):
        logger.error(
            "\nError: Tests must be run from the DocumentationTool directory.\n"
            f"Current directory: {current_dir}\n"
            "Please run:\n"
            "  cd Engine/Tools/DocumentationTool\n"
            "  python run_tests.py"
        )
        return False
    return True

def setup_test_logging() -> Optional[Path]:
    """Configure logging for test execution"""
    try:
        log_dir = Path("Logs/DocumentationTool/Tests")
        log_dir.mkdir(parents=True, exist_ok=True)
        
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        log_file = log_dir / f"test_run_{timestamp}.log"
        
        file_handler = logging.FileHandler(log_file)
        file_handler.setFormatter(
            logging.Formatter('%(asctime)s - %(levelname)s - %(name)s - %(message)s')
        )
        logger.addHandler(file_handler)
        logger.setLevel(logging.DEBUG)
        
        return log_file
    except Exception as e:
        logger.error(f"Failed to setup logging: {e}")
        return None

def run_tests() -> bool:
    """Run all documentation tool tests"""
    try:
        if not verify_directory():
            return False
            
        log_file = setup_test_logging()
        if not log_file:
            return False
            
        logger.info(f"Starting test run, logging to {log_file}")
        
        # Use pytest for test execution
        test_args = [
            "--verbose",
            "--tb=short",
            "--strict-markers",
            "--import-mode=importlib",
            "-p", "no:warnings",  # Suppress warnings
            str(Path(__file__).parent / "tests")
        ]
        
        result = pytest.main(test_args)
        success = result == pytest.ExitCode.OK
        
        logger.info(f"Test run {'succeeded' if success else 'failed'}")
        return success
        
    except Exception as e:
        logger.error(f"Error running tests: {e}")
        return False

if __name__ == "__main__":
    success = run_tests()
    sys.exit(0 if success else 1) 