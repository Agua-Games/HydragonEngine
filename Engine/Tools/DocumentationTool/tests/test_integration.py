"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Integration, performance and stress tests for documentation system.
"""

import pytest
import time
from pathlib import Path
from ..doc_generator import DocumentationGenerator
from ..doc_tester import DocumentationTester
from ..api_tracker import APIChangeTracker
from ..example_manager import CodeExampleManager

# Integration Tests
def test_full_documentation_workflow(clean_test_env, mock_logger):
    """Test complete documentation workflow with all components"""
    doc_gen = DocumentationGenerator()
    doc_test = DocumentationTester()
    api_tracker = APIChangeTracker()
    
    # Setup test files
    source_file = clean_test_env / "source/test_module.cpp"
    source_file.write_text('''
        /**
         * @brief Test function
         * @param x Input parameter
         * @return Result value
         */
        int test_function(int x) {
            return x * 2;
        }
    ''')
    
    # Run complete workflow
    api_tracker.track_changes()
    doc_gen._validate_documentation()
    doc_gen._integrate_examples({})
    
    # Verify outputs
    assert mock_logger.info.called
    assert (clean_test_env / "docs").exists()

# Performance Tests
@pytest.mark.performance
def test_documentation_generation_performance(clean_test_env):
    """Test documentation generation performance"""
    doc_gen = DocumentationGenerator()
    
    # Generate test files
    for i in range(100):
        file = clean_test_env / f"source/test_{i}.py"
        file.write_text(f'''
            """Module {i} documentation."""
            def function_{i}():
                """Function {i} documentation."""
                pass
        ''')
    
    start_time = time.time()
    doc_gen._validate_documentation()
    end_time = time.time()
    
    duration = end_time - start_time
    assert duration < 5.0  # Should complete within 5 seconds

# Stress Tests
@pytest.mark.stress
def test_large_documentation_set(clean_test_env):
    """Test handling of large documentation sets"""
    doc_test = DocumentationTester()
    
    # Generate large test dataset
    for i in range(1000):
        file = clean_test_env / f"source/module_{i}.py"
        file.write_text(f'''
            """
            Module {i} documentation with multiple sections.
            
            This is a detailed description.
            
            Examples:
                >>> example_code()
                expected_output
            
            Notes:
                Additional notes about usage.
            """
            
            def function_{i}():
                """Complex function documentation."""
                pass
        ''')
    
    # Test memory usage and processing time
    import psutil
    import os
    
    process = psutil.Process(os.getpid())
    initial_memory = process.memory_info().rss
    
    start_time = time.time()
    coverage = doc_test.check_coverage(str(clean_test_env / "source"))
    end_time = time.time()
    
    final_memory = process.memory_info().rss
    memory_usage = (final_memory - initial_memory) / 1024 / 1024  # MB
    
    assert coverage.coverage_ratio > 0.9  # Should maintain accuracy
    assert memory_usage < 512  # Should use less than 512MB additional memory
    assert end_time - start_time < 30  # Should complete within 30 seconds

@pytest.mark.stress
def test_concurrent_operations(clean_test_env):
    """Test concurrent documentation operations"""
    doc_gen = DocumentationGenerator()
    example_manager = CodeExampleManager()
    
    # Setup test data
    for i in range(100):
        example_file = clean_test_env / f"examples/example_{i}.py"
        example_file.write_text(f'''
            """Example {i}"""
            print("Hello from example {i}")
        ''')
    
    from concurrent.futures import ThreadPoolExecutor
    with ThreadPoolExecutor(max_workers=4) as executor:
        futures = []
        futures.append(executor.submit(doc_gen._validate_documentation))
        futures.append(executor.submit(example_manager.update_examples))
        futures.append(executor.submit(doc_gen._generate_search_index))
        
        # All operations should complete without errors
        for future in futures:
            assert future.exception() is None 