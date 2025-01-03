"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Python code orphaned calls detector.
"""

import ast
import logging
import re
import json
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Set, Optional, Union
from concurrent.futures import ThreadPoolExecutor

logger = logging.getLogger(__name__)

@dataclass
class CodeLocation:
    """Detailed source location information"""
    file_path: Path
    line_number: int
    column: int
    context: str  # Surrounding code snippet
    scope: Optional[str] = None

@dataclass
class CodeElement:
    """Code element representation"""
    name: str
    element_type: str
    locations: List[CodeLocation]  # All occurrences
    namespace: Optional[str] = None
    is_public: bool = True
    dependencies: List[str] = field(default_factory=list)

@dataclass
class OrphanedCallsResult:
    """Analysis results"""
    orphaned_elements: Dict[str, List[CodeElement]] = field(default_factory=lambda: {
        'functions': [], 
        'variables': [], 
        'classes': []
    })
    statistics: Dict[str, int] = field(default_factory=dict)
    suggestions: List[str] = field(default_factory=list)

class OrphanedCallsDetector:
    """Detects orphaned code elements in Python files"""

    def __init__(self, config: Optional[Dict] = None):
        self.config = config or {}
        self.cache_dir = Path(self.config.get('cache_dir', 'Engine/Tools/CodeAnalysis/cache'))
        self.supported_extensions = {'.py'}
        self._current_source = ""  # For storing current file source
        self._current_scope = None  # For tracking current namespace/scope
        self._load_cache()

    def analyze_codebase(self, root_dir: Union[str, Path]) -> OrphanedCallsResult:
        """Analyze codebase for orphaned elements
        
        Args:
            root_dir: Root directory to analyze
            
        Returns:
            OrphanedCallsResult: Analysis results
        """
        root_dir = Path(root_dir)
        result = OrphanedCallsResult()
        
        try:
            # Find all Python files
            python_files = list(root_dir.rglob('*.py'))
            logger.info(f"Found {len(python_files)} Python files to analyze")
            
            # Process files in parallel
            with ThreadPoolExecutor() as executor:
                for file_path in python_files:
                    try:
                        self._analyze_file(file_path, result)
                    except Exception as e:
                        logger.error(f"Error analyzing file {file_path}: {e}")
                        
            # Update statistics
            self._update_statistics(result)
            return result
            
        except Exception as e:
            logger.error(f"Error analyzing codebase: {e}")
            return result

    def _analyze_file(self, file_path: Path, result: OrphanedCallsResult) -> None:
        """Analyze a single Python file"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                self._current_source = f.read()
                
            tree = ast.parse(self._current_source)
            for node in ast.walk(tree):
                if isinstance(node, ast.FunctionDef):
                    self._add_python_function(node, file_path, result)
                elif isinstance(node, ast.ClassDef):
                    self._add_python_class(node, file_path, result)
                elif isinstance(node, ast.Name) and isinstance(node.ctx, ast.Store):
                    self._add_python_variable(node, file_path, result)
                    
        except Exception as e:
            logger.error(f"Error analyzing Python file {file_path}: {e}")

    def _add_python_function(self, node: ast.FunctionDef, file_path: Path, result: OrphanedCallsResult) -> None:
        """Add Python function to results"""
        location = CodeLocation(
            file_path=file_path,
            line_number=node.lineno,
            column=node.col_offset,
            context=ast.get_source_segment(self._current_source, node),
            scope=self._current_scope
        )
        
        element = CodeElement(
            name=node.name,
            element_type='function',
            locations=[location]
        )
        result.orphaned_elements['functions'].append(element)

    def _add_python_class(self, node: ast.ClassDef, file_path: Path, result: OrphanedCallsResult) -> None:
        """Add Python class to results"""
        location = CodeLocation(
            file_path=file_path,
            line_number=node.lineno,
            column=node.col_offset,
            context=ast.get_source_segment(self._current_source, node),
            scope=self._current_scope
        )
        
        element = CodeElement(
            name=node.name,
            element_type='class',
            locations=[location]
        )
        result.orphaned_elements['classes'].append(element)

    def _add_python_variable(self, node: ast.Name, file_path: Path, result: OrphanedCallsResult) -> None:
        """Add Python variable to results"""
        location = CodeLocation(
            file_path=file_path,
            line_number=node.lineno,
            column=node.col_offset,
            context=ast.get_source_segment(self._current_source, node),
            scope=self._current_scope
        )
        
        element = CodeElement(
            name=node.id,
            element_type='variable',
            locations=[location]
        )
        result.orphaned_elements['variables'].append(element)

    def _update_statistics(self, result: OrphanedCallsResult) -> None:
        """Update result statistics"""
        for category, elements in result.orphaned_elements.items():
            result.statistics[f'total_{category}'] = len(elements)
            result.statistics[f'orphaned_{category}'] = len(
                [e for e in elements if not e.dependencies]
            )

    def _load_cache(self) -> None:
        """Load analysis cache"""
        try:
            self.cache_dir.mkdir(parents=True, exist_ok=True)
        except Exception as e:
            logger.error(f"Error creating cache directory: {e}") 