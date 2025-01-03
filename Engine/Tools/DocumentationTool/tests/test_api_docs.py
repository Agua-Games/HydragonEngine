"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Test suite for API documentation generation.
"""

import unittest
import logging
from pathlib import Path
import tempfile
import shutil
from typing import Dict, List

# Use absolute imports
from Engine.Tools.DocumentationTool.parsers.python_parser import PythonParser
from Engine.Tools.DocumentationTool.parsers.cpp_parser import CppParser
from Engine.Tools.DocumentationTool.generate_api_docs import generate_api_docs, setup_logging

logger = logging.getLogger(__name__)

class TestAPIDocumentation(unittest.TestCase):
    """Test cases for API documentation generation"""
    
    def setUp(self):
        """Set up test environment"""
        try:
            # Create temporary test directories
            self.test_dir = Path(tempfile.mkdtemp())
            self.source_dir = self.test_dir / "Source"
            self.docs_dir = self.test_dir / "Docs"
            self.source_dir.mkdir(parents=True)
            self.docs_dir.mkdir(parents=True)
            
            # Setup test logging
            self.log_dir = self.test_dir / "Logs"
            self.log_dir.mkdir(parents=True)
            setup_logging(self.log_dir / "test_api_docs.log")
            
            logger.info(f"Test environment set up in {self.test_dir}")
            
        except Exception as e:
            logger.error(f"Error setting up test environment: {e}")
            raise
            
    def tearDown(self):
        """Clean up test environment"""
        try:
            shutil.rmtree(self.test_dir)
            logger.info("Test environment cleaned up")
        except Exception as e:
            logger.error(f"Error cleaning up test environment: {e}")
            
    def create_test_file(self, path: str, content: str) -> Path:
        """Create a test source file"""
        file_path = self.source_dir / path
        file_path.parent.mkdir(parents=True, exist_ok=True)
        file_path.write_text(content)
        return file_path
        
    def test_python_file_parsing(self):
        """Test Python file parsing and documentation generation"""
        try:
            # Create test Python file with proper documentation
            test_content = '''"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Test module for documentation generation.
"""

class TestClass:
    """A test class for documentation.
    
    This class demonstrates proper documentation standards.
    """
    
    def test_method(self, param1: str, param2: int) -> bool:
        """Test method with documentation.
        
        Args:
            param1: First parameter description
            param2: Second parameter description
            
        Returns:
            Boolean result
            
        Raises:
            ValueError: If parameters are invalid
        """
        pass
'''
            test_file = self.create_test_file("Test/test_module.py", test_content)
            
            # Parse the file
            parser = PythonParser()
            doc_data = parser.parse_file(test_file, self.docs_dir)
            
            # Verify documentation content
            self.assertIn('module_doc', doc_data)
            self.assertIn('classes', doc_data)
            self.assertEqual(len(doc_data['classes']), 1)
            self.assertEqual(doc_data['classes'][0]['name'], 'TestClass')
            
            # Verify standards compliance
            self.assertEqual(len(doc_data['violations']), 0)
            
            logger.info("Python file parsing test passed successfully")
            
        except Exception as e:
            logger.error(f"Error in Python parsing test: {e}")
            raise
            
    def test_cpp_file_parsing(self):
        """Test C++ file parsing and documentation generation"""
        try:
            # Create test C++ header file
            test_content = '''/*
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0
*/

#pragma once

namespace Engine {

/**
 * @brief Test class for documentation
 * @details This class demonstrates proper documentation standards
 */
class TestClass {
public:
    /**
     * @brief Test method with documentation
     * @param param1 First parameter description
     * @param param2 Second parameter description
     * @return Boolean result
     * @throws std::invalid_argument If parameters are invalid
     */
    bool TestMethod(const std::string& param1, int param2);
};

} // namespace Engine
'''
            test_file = self.create_test_file("Test/test_module.hpp", test_content)
            
            # Parse the file
            parser = CppParser()
            doc_data = parser.parse_file(test_file, self.docs_dir)
            
            # Verify documentation content
            self.assertIn('classes', doc_data)
            self.assertEqual(len(doc_data['classes']), 1)
            self.assertEqual(doc_data['classes'][0]['name'], 'TestClass')
            
            # Verify standards compliance
            self.assertEqual(len(doc_data['violations']), 0)
            
            logger.info("C++ file parsing test passed successfully")
            
        except Exception as e:
            logger.error(f"Error in C++ parsing test: {e}")
            raise
            
    def test_full_documentation_generation(self):
        """Test complete documentation generation process"""
        try:
            # Create test files
            self.create_test_file("Test/test_module.py", "# Python test file")
            self.create_test_file("Test/test_module.hpp", "// C++ test file")
            
            # Generate documentation
            generate_api_docs(
                source_dir=self.source_dir,
                output_dir=self.docs_dir,
                log_dir=self.log_dir
            )
            
            # Verify output files
            self.assertTrue((self.docs_dir / "Test/test_module.html").exists())
            self.assertTrue((self.docs_dir / "Test/test_module.md").exists())
            
            logger.info("Full documentation generation test passed successfully")
            
        except Exception as e:
            logger.error(f"Error in full documentation generation test: {e}")
            raise

if __name__ == '__main__':
    unittest.main() 