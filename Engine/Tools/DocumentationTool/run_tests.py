"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Test runner for documentation tool with detailed reporting.
"""

import unittest
import logging
import sys
from pathlib import Path
from datetime import datetime
import json
import os

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

def setup_test_logging():
    """Configure logging for test execution"""
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

def run_tests():
    """Run all documentation tool tests with detailed reporting"""
    try:
        # Verify correct directory first
        if not verify_directory():
            return False
            
        log_file = setup_test_logging()
        logger.info("Starting documentation tool test suite")
        
        # Discover and run tests
        loader = unittest.TestLoader()
        start_dir = Path(__file__).parent / "tests"
        suite = loader.discover(start_dir, pattern="test_*.py")
        
        # Setup test result collection
        results = {
            'total': 0,
            'passed': 0,
            'failed': 0,
            'errors': 0,
            'skipped': 0,
            'failures': []
        }
        
        # Run tests with result collection
        runner = unittest.TextTestRunner(verbosity=2)
        test_results = runner.run(suite)
        
        # Collect results
        results['total'] = test_results.testsRun
        results['passed'] = results['total'] - len(test_results.failures) - len(test_results.errors)
        results['failed'] = len(test_results.failures)
        results['errors'] = len(test_results.errors)
        results['skipped'] = len(test_results.skipped)
        
        # Log results
        logger.info("\nTest Results Summary:")
        logger.info(f"Total Tests: {results['total']}")
        logger.info(f"Passed: {results['passed']}")
        logger.info(f"Failed: {results['failed']}")
        logger.info(f"Errors: {results['errors']}")
        logger.info(f"Skipped: {results['skipped']}")
        
        if test_results.failures:
            logger.error("\nFailures:")
            for test, traceback in test_results.failures:
                logger.error(f"\n{test}")
                logger.error(traceback)
                results['failures'].append({
                    'test': str(test),
                    'traceback': traceback
                })
        
        # Save results to JSON
        results_file = log_file.parent / f"test_results_{timestamp}.json"
        with open(results_file, 'w') as f:
            json.dump(results, f, indent=2)
            
        logger.info(f"\nTest logs saved to: {log_file}")
        logger.info(f"Test results saved to: {results_file}")
        
        return len(test_results.failures) + len(test_results.errors) == 0
        
    except Exception as e:
        logger.error(f"Error running tests: {e}")
        return False

if __name__ == "__main__":
    success = run_tests()
    sys.exit(0 if success else 1) 