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
        self._current_source = ""
        self._current_scope = None
        self._declarations = {
            'functions': {},
            'variables': {},
            'classes': {}
        }
        self._references = {
            'functions': set(),
            'variables': set(),
            'classes': set()
        }
        # Track both our code and third-party separately
        self._third_party_stats = {
            'functions': 0,
            'variables': 0,
            'classes': 0
        }
        self.excluded_paths = {
            '.venv', 
            'site-packages',
            '__pycache__',
            'dist',
            'build'
        }
        self._load_cache()

    def _should_analyze_file(self, file_path: Path) -> bool:
        """Determine if a file should be analyzed"""
        # Skip files in excluded directories
        parts = file_path.parts
        return not any(excluded in parts for excluded in self.excluded_paths)

    def analyze_codebase(self, root_dir: Union[str, Path]) -> OrphanedCallsResult:
        root_dir = Path(root_dir)
        result = OrphanedCallsResult()
        
        try:
            # Find all Python files, excluding third-party packages
            python_files = [
                f for f in root_dir.rglob('*.py')
                if self._should_analyze_file(f)
            ]
            logger.info(f"Found {len(python_files)} Python files to analyze")
            
            # Process files in parallel
            with ThreadPoolExecutor() as executor:
                for file_path in python_files:
                    try:
                        self._analyze_file(file_path, result)
                    except Exception as e:
                        logger.error(f"Error analyzing file {file_path}: {e}")
                        
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
            
            # First pass: collect declarations
            for node in ast.walk(tree):
                if isinstance(node, ast.FunctionDef):
                    self._add_python_function(node, file_path, result)
                elif isinstance(node, ast.ClassDef):
                    self._add_python_class(node, file_path, result)
                elif isinstance(node, ast.Name) and isinstance(node.ctx, ast.Store):
                    self._add_python_variable(node, file_path, result)
            
            # Second pass: collect references
            for node in ast.walk(tree):
                if isinstance(node, ast.Name) and isinstance(node.ctx, ast.Load):
                    self._add_reference(node.id)
                elif isinstance(node, ast.Call):
                    if isinstance(node.func, ast.Name):
                        self._add_reference(node.func.id)
                    elif isinstance(node.func, ast.Attribute):
                        self._add_reference(node.func.attr)
                        
        except Exception as e:
            logger.error(f"Error analyzing Python file {file_path}: {e}")

    def _add_reference(self, name: str) -> None:
        """Add a reference to a name"""
        # Check all categories since we don't know the type
        for category in ['functions', 'variables', 'classes']:
            if name in self._declarations[category]:
                self._references[category].add(name)

    def _update_statistics(self, result: OrphanedCallsResult) -> None:
        """Update result statistics"""
        for category in ['functions', 'variables', 'classes']:
            declarations = self._declarations[category]
            references = self._references[category]
            
            # Update total counts
            result.statistics[f'total_{category}'] = len(declarations)
            
            # Find truly orphaned elements (declared but never referenced)
            orphaned = [
                elem for name, elem in declarations.items()
                if name not in references and not elem.is_public
            ]
            result.orphaned_elements[category] = orphaned
            result.statistics[f'orphaned_{category}'] = len(orphaned)

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
            locations=[location],
            is_public=not node.name.startswith('_')
        )
        self._declarations['functions'][node.name] = element

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
            locations=[location],
            is_public=not node.name.startswith('_')
        )
        self._declarations['classes'][node.name] = element

    def _add_python_variable(self, node: ast.Name, file_path: Path, result: OrphanedCallsResult) -> None:
        """Add Python variable to results"""
        # Track all variables, including function/class scope
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
            locations=[location],
            is_public=not node.id.startswith('_')
        )
        self._declarations['variables'][node.id] = element
        logger.debug(f"Added variable: {node.id} in scope {self._current_scope}")

    def _load_cache(self) -> None:
        """Load analysis cache"""
        try:
            self.cache_dir.mkdir(parents=True, exist_ok=True)
        except Exception as e:
            logger.error(f"Error creating cache directory: {e}") 

    def analyze_third_party(self, root_dir: Union[str, Path]) -> Dict[str, int]:
        """Analyze third-party packages for element counts
        
        Args:
            root_dir: Root directory containing site-packages
            
        Returns:
            Dict with counts of functions, variables, and classes
        """
        root_dir = Path(root_dir)
        stats = {'functions': 0, 'variables': 0, 'classes': 0}
        
        try:
            # Look in site-packages directories
            site_packages = list(root_dir.rglob('site-packages'))
            for site_pkg in site_packages:
                logger.info(f"Analyzing third-party packages in: {site_pkg}")
                for py_file in site_pkg.rglob('*.py'):
                    try:
                        with open(py_file, 'r', encoding='utf-8') as f:
                            tree = ast.parse(f.read())
                            
                        for node in ast.walk(tree):
                            if isinstance(node, ast.FunctionDef):
                                stats['functions'] += 1
                            elif isinstance(node, ast.ClassDef):
                                stats['classes'] += 1
                                logger.debug(f"Found third-party class: {node.name} in {py_file}")
                            elif isinstance(node, ast.Name) and isinstance(node.ctx, ast.Store):
                                stats['variables'] += 1
                                
                    except Exception as e:
                        logger.error(f"Error analyzing third-party file {py_file}: {e}")
                        
            return stats
            
        except Exception as e:
            logger.error(f"Error analyzing third-party packages: {e}")
            return stats 