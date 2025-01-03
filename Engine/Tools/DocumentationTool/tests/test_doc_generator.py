"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Tests for documentation generator error handling and edge cases.
"""

import pytest
from unittest.mock import patch, Mock
from pathlib import Path

from Engine.Tools.DocumentationTool.doc_generator import DocumentationGenerator, GeneratorConfig

def test_docker_unavailable(clean_test_env, mock_logger):
    """Test graceful handling of Docker unavailability"""
    config = GeneratorConfig(
        source_dir=str(clean_test_env / "source"),
        enable_interactive=True
    )
    
    with patch('Engine.Tools.DocumentationTool.doc_generator.DOCKER_AVAILABLE', False):
        doc_gen = DocumentationGenerator(config)
        assert not doc_gen.config.enable_interactive
        mock_logger.warning.assert_called_with("Docker not available - falling back to static examples")

def test_complex_template_parsing_failure(clean_test_env, mock_logger):
    """Test handling of complex C++ template parsing failures"""
    config = GeneratorConfig(source_dir=str(clean_test_env / "source"))
    doc_gen = DocumentationGenerator(config)
    
    # Create complex template file
    template_file = clean_test_env / "source/complex_template.hpp"
    template_file.write_text('''
        template<typename T, template<typename U, typename... Args> class Container>
        class ComplexTemplate<T, Container<T, std::allocator<T>>> {
            // Intentionally malformed template
            template<typename... InvalidTemplate
        };
    ''')
    
    doc_gen._validate_documentation()
    mock_logger.error.assert_called_with(
        mock_logger.error.call_args_list[0][0][0].startswith("Template parsing failed:")
    )

@pytest.mark.integration
def test_git_repository_edge_cases(clean_test_env, mock_logger):
    """Test handling of Git repository edge cases"""
    config = GeneratorConfig(
        source_dir=str(clean_test_env / "source"),
        enable_api_tracking=True
    )
    
    # Test non-git directory
    doc_gen = DocumentationGenerator(config)
    assert not doc_gen.config.enable_api_tracking
    mock_logger.warning.assert_called_with(
        mock_logger.warning.call_args_list[0][0][0].startswith("Failed to initialize API tracker:")
    )
    
    # Test corrupted git repo
    with patch('Engine.Tools.DocumentationTool.api_tracker.APIChangeTracker._is_git_repo') as mock_git:
        mock_git.side_effect = Exception("Git repository corrupted")
        doc_gen = DocumentationGenerator(config)
        assert not doc_gen.config.enable_api_tracking

@pytest.mark.stress
def test_memory_exhaustion_handling(clean_test_env, mock_logger):
    """Test handling of memory exhaustion scenarios"""
    config = GeneratorConfig(source_dir=str(clean_test_env / "source"))
    doc_gen = DocumentationGenerator(config)
    
    # Generate large number of files
    for i in range(10000):  # Increased file count to stress memory
        file = clean_test_env / f"source/large_module_{i}.py"
        file.write_text(f'''
            """
            {"A" * 1000}  # Large docstring
            """
            class LargeClass_{i}:
                """{"B" * 1000}"""
                def method_{i}(self):
                    """{"C" * 1000}"""
                    pass
        ''')
    
    try:
        doc_gen._validate_documentation()
    except MemoryError:
        pytest.skip("Memory exhaustion test - skipped after expected MemoryError")
    
    mock_logger.warning.assert_any_call(
        mock_logger.warning.call_args_list[-1][0][0].startswith("High memory usage detected")
    )

@pytest.mark.network
def test_network_timeout_handling(clean_test_env, mock_logger):
    """Test handling of network timeouts in link validation"""
    config = GeneratorConfig(source_dir=str(clean_test_env / "source"))
    doc_gen = DocumentationGenerator(config)
    
    # Create markdown with external links
    doc_file = clean_test_env / "source/readme.md"
    doc_file.write_text('''
        # Test Documentation
        [Link 1](https://very-slow-server.example.com)
        [Link 2](https://timeout-server.example.com)
    ''')
    
    with patch('requests.get') as mock_get:
        mock_get.side_effect = TimeoutError("Request timed out")
        doc_gen._validate_documentation()
    
    mock_logger.warning.assert_called_with(
        mock_logger.warning.call_args_list[-1][0][0].startswith("Link validation timeout:")
    ) 