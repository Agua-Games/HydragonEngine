"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Tests for documentation testing functionality.
"""

import pytest
from pathlib import Path
from ..doc_tester import DocumentationTester, DocTestConfig, CoverageResult, LinkCheckResult

def test_coverage_check_empty_file(clean_test_env):
    """Test coverage check with empty file"""
    tester = DocumentationTester()
    test_file = clean_test_env / "source/empty.py"
    test_file.write_text("")
    
    result = tester.check_coverage(str(test_file))
    assert result.total_elements == 0
    assert result.coverage_ratio == 0

def test_coverage_check_fully_documented(clean_test_env):
    """Test coverage check with fully documented code"""
    tester = DocumentationTester()
    test_file = clean_test_env / "source/documented.py"
    test_file.write_text('''
        """Module docstring."""
        
        class TestClass:
            """Class docstring."""
            
            def test_method(self):
                """Method docstring."""
                pass
    ''')
    
    result = tester.check_coverage(str(test_file))
    assert result.coverage_ratio == 1.0
    assert not result.missing_docs

def test_spell_check_custom_dictionary(clean_test_env):
    """Test spell checking with custom dictionary"""
    config = DocTestConfig(spell_check_lang="en_US")
    tester = DocumentationTester(config)
    
    # Add custom technical terms
    tester.spell_checker.add("pytest")
    tester.spell_checker.add("docstring")
    
    content = "This is a pytest docstring with mispeled words."
    misspelled = tester.spell_check(content)
    assert "mispeled" in misspelled
    assert "pytest" not in misspelled
    assert "docstring" not in misspelled

def test_link_validation_with_timeout(clean_test_env, mocker):
    """Test link validation with timeout handling"""
    tester = DocumentationTester()
    mock_requests = mocker.patch("requests.get")
    mock_requests.side_effect = TimeoutError("Request timed out")
    
    test_file = clean_test_env / "docs/test.md"
    test_file.write_text("[Test Link](https://example.com)")
    
    results = tester.validate_links(str(test_file))
    assert any(not r.valid and "timeout" in r.error.lower() for r in results)

def test_style_compliance_complex(clean_test_env):
    """Test style compliance with complex documentation"""
    tester = DocumentationTester()
    test_file = clean_test_env / "source/complex.py"
    test_file.write_text('''
        """
        This module needs style fixes.
        
        @param x: Wrong style param
        @return: Wrong style return
        """
        
        def test_function(x):
            # Incorrect comment style
            pass
    ''')
    
    violations = tester._check_style(str(test_file))
    assert len(violations) > 0
    assert any("param" in v.rule for v in violations)
    assert any("comment" in v.rule for v in violations)

@pytest.mark.parametrize("file_pattern", [
    "*.py",
    "*.cpp",
    "*.h",
    "*.md",
    "*.rst"
])
def test_file_pattern_recognition(clean_test_env, file_pattern):
    """Test recognition of different file patterns"""
    tester = DocumentationTester()
    test_file = clean_test_env / f"test{file_pattern[1:]}"
    test_file.touch()
    
    assert tester._should_check_file(test_file) 