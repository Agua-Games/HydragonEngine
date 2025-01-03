"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Enhanced orphaned calls detector with Python and Lua support.
"""

import ast
import logging
import re
import json
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Set, Optional, Union
from concurrent.futures import ThreadPoolExecutor
import luaparser.ast
from luaparser import ast as lua_ast

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
    """Detects orphaned code elements in Python and Lua files"""

    def __init__(self, config: Optional[Dict] = None):
        self.config = config or {}
        self.cache_dir = Path(self.config.get('cache_dir', 'Engine/Tools/CodeAnalysis/cache'))
        self.supported_extensions = {
            'python': {'.py'},
            'lua': {'.lua'}
        }
        self._load_cache()

    def analyze_codebase(self, root_dir: Union[str, Path]) -> OrphanedCallsResult:
        """Analyze codebase for orphaned elements
        
        Args:
            root_dir: Root directory to analyze
            
        Returns:
            OrphanedCallsResult containing analysis results
        """
        try:
            root_path = Path(root_dir)
            result = OrphanedCallsResult()
            
            # Collect all supported files
            python_files = list(root_path.rglob("*.py"))
            lua_files = list(root_path.rglob("*.lua"))
            
            # Process files in parallel
            with ThreadPoolExecutor() as executor:
                python_futures = [
                    executor.submit(self._analyze_python_file, file)
                    for file in python_files
                ]
                lua_futures = [
                    executor.submit(self._analyze_lua_file, file)
                    for file in lua_files
                ]
                
                # Collect results
                for future in python_futures + lua_futures:
                    try:
                        file_result = future.result()
                        self._merge_results(result, file_result)
                    except Exception as e:
                        logger.error(f"Error processing file: {e}")
            
            self._update_statistics(result)
            return result
            
        except Exception as e:
            logger.error(f"Error analyzing codebase: {e}")
            return OrphanedCallsResult()

    def _analyze_python_file(self, file_path: Path) -> OrphanedCallsResult:
        """Analyze Python file for code elements"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                tree = ast.parse(f.read(), filename=str(file_path))
            
            result = OrphanedCallsResult()
            for node in ast.walk(tree):
                if isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef)):
                    self._add_python_function(node, file_path, result)
                elif isinstance(node, ast.ClassDef):
                    self._add_python_class(node, file_path, result)
                elif isinstance(node, ast.Name) and isinstance(node.ctx, ast.Store):
                    self._add_python_variable(node, file_path, result)
            
            return result
            
        except Exception as e:
            logger.error(f"Error analyzing Python file {file_path}: {e}")
            return OrphanedCallsResult()

    def _analyze_lua_file(self, file_path: Path) -> OrphanedCallsResult:
        """Analyze Lua file for code elements"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                tree = luaparser.ast.parse(f.read())
            
            result = OrphanedCallsResult()
            self._traverse_lua_ast(tree, file_path, result)
            return result
            
        except Exception as e:
            logger.error(f"Error analyzing Lua file {file_path}: {e}")
            return OrphanedCallsResult()

    def _merge_results(self, target: OrphanedCallsResult, source: OrphanedCallsResult) -> None:
        """Merge analysis results"""
        for category in target.orphaned_elements:
            target.orphaned_elements[category].extend(source.orphaned_elements[category])

    def _update_statistics(self, result: OrphanedCallsResult) -> None:
        """Update analysis statistics"""
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