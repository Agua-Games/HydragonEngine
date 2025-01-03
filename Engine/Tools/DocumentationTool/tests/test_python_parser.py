"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Tests for Python documentation parser edge cases.
"""

import pytest
from pathlib import Path
from Engine.Tools.DocumentationTool.parsers.python_parser import PythonParser
from Engine.Tools.DocumentationTool.parsers.doc_types import PythonDocComment

def test_complex_type_hints(clean_test_env, mock_logger):
    """Test parsing of complex type hints in docstrings"""
    parser = PythonParser()
    test_file = clean_test_env / "source/complex_types.py"
    test_file.write_text('''
        from typing import Dict, List, Optional, Union, Callable, TypeVar, Generic

        T = TypeVar('T')
        class ComplexClass(Generic[T]):
            """Complex class with type parameters.
            
            Args:
                data (Dict[str, Union[List[T], Optional[Callable[[T], bool]]]]):
                    Complex nested type
                callback (Callable[[T, ...], None]): Variadic callback
            """
            def __init__(self, data, callback):
                self.data = data
                self.callback = callback
    ''')
    
    result = parser.parse_file(str(test_file))
    assert any(doc.params and 'data' in doc.params for doc in result.doc_comments)
    assert any(doc.params and 'callback' in doc.params for doc in result.doc_comments)

def test_malformed_docstrings(clean_test_env, mock_logger):
    """Test handling of malformed docstrings"""
    parser = PythonParser()
    test_file = clean_test_env / "source/malformed.py"
    test_file.write_text('''
        def broken_doc():
            """Incomplete section
            Args:
                x: Missing type
            Returns:
            Raises:
                Exception
            """
            pass

        def mixed_styles():
            """Mixed docstring styles
            @param x: ReST style
            :param y: Sphinx style
            Args:
                z: Google style
            """
            pass
    ''')
    
    result = parser.parse_file(str(test_file))
    assert mock_logger.warning.called
    assert any('incomplete section' in str(call).lower() 
              for call in mock_logger.warning.call_args_list)

def test_nested_functions_and_classes(clean_test_env):
    """Test parsing of nested functions and classes"""
    parser = PythonParser()
    test_file = clean_test_env / "source/nested.py"
    test_file.write_text('''
        def outer():
            """Outer function"""
            class Inner:
                """Inner class"""
                def nested_method():
                    """Nested method"""
                    def deeply_nested():
                        """Deeply nested function"""
                        pass
                    return deeply_nested
            return Inner
    ''')
    
    result = parser.parse_file(str(test_file))
    assert len(result.doc_comments) == 4  # All levels should be documented
    assert any('Outer function' in doc.brief for doc in result.doc_comments)
    assert any('Deeply nested function' in doc.brief for doc in result.doc_comments)

def test_async_and_decorators(clean_test_env):
    """Test parsing of async functions and decorators"""
    parser = PythonParser()
    test_file = clean_test_env / "source/async_decorated.py"
    test_file.write_text('''
        def decorator(func):
            """Decorator docstring"""
            return func

        @decorator
        async def async_function():
            """Async function docstring
            
            Returns:
                Coroutine: An async result
            """
            return await some_coroutine()
    ''')
    
    result = parser.parse_file(str(test_file))
    assert len(result.doc_comments) == 2
    assert any('Decorator docstring' in doc.brief for doc in result.doc_comments)
    assert any('Async function docstring' in doc.brief for doc in result.doc_comments)

def test_property_and_magic_methods(clean_test_env):
    """Test parsing of properties and magic methods"""
    parser = PythonParser()
    test_file = clean_test_env / "source/properties.py"
    test_file.write_text('''
        class PropertyTest:
            @property
            def computed(self) -> int:
                """Property docstring
                
                Returns:
                    int: Computed value
                """
                return 42

            def __str__(self) -> str:
                """String representation
                
                Returns:
                    str: Object description
                """
                return "PropertyTest instance"
    ''')
    
    result = parser.parse_file(str(test_file))
    assert any('Property docstring' in doc.brief for doc in result.doc_comments)
    assert any('String representation' in doc.brief for doc in result.doc_comments)
    assert any(doc.returns and 'int' in doc.returns for doc in result.doc_comments)

def test_error_recovery(clean_test_env, mock_logger):
    """Test parser recovery from syntax errors"""
    parser = PythonParser()
    test_file = clean_test_env / "source/syntax_errors.py"
    test_file.write_text('''
        def valid_function():
            """Valid docstring"""
            pass

        def invalid_syntax(
            """Invalid placement"""
            x: int  # Missing closing parenthesis
            pass

        def another_valid():
            """Should still parse this"""
            pass
    ''')
    
    result = parser.parse_file(str(test_file))
    assert mock_logger.error.called
    assert any('Valid docstring' in doc.brief for doc in result.doc_comments)
    assert any('Should still parse this' in doc.brief for doc in result.doc_comments) 