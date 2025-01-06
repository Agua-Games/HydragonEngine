"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Tool to collect Python function declarations and calls.
"""

from pathlib import Path
from typing import Dict, List, Set
import re
import ast
from Engine.Tools.CodeAnalysis.base_collector import BaseFunctionCollector

class PythonFunctionCollector(BaseFunctionCollector):
    """Collects Python function declarations and calls"""
    
    def get_supported_extensions(self) -> List[str]:
        return ['.py']
        
    def __init__(self):
        super().__init__()
        # Regex patterns for Python function detection
        self.func_def_pattern = re.compile(
            r'^\s*def\s+(\w+)\s*\('  # Function definitions
        )
        self.func_call_pattern = re.compile(
            r'\b(\w+)\s*\('  # Function calls
        )
        self.class_pattern = re.compile(
            r'^\s*class\s+(\w+)'  # Class declarations
        )
        
    def collect_function_declarations(self, root_dir: Path) -> Dict[str, Path]:
        """Collect all Python function declarations"""
        function_locations = {}
        
        for file_path in root_dir.rglob('*.py'):
            if file_path.is_file():
                self._process_python_file(file_path, function_locations)
                
        return function_locations
        
    def _process_python_file(self, file_path: Path, 
                           function_locations: Dict[str, Path]) -> None:
        """Process a single Python file for function declarations"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                current_class = None
                
                for line in f:
                    # Check for class declarations
                    class_match = self.class_pattern.match(line)
                    if class_match:
                        current_class = class_match.group(1)
                        continue
                        
                    # Check for function declarations
                    func_match = self.func_def_pattern.match(line)
                    if func_match:
                        func_name = func_match.group(1)
                        if current_class:
                            func_name = f"{current_class}.{func_name}"
                        function_locations[func_name] = file_path
                        
        except Exception as e:
            self.log_error(f"Error processing {file_path}: {e}")
            raise
            
    def collect_function_calls(self, root_dir: Path) -> Dict[str, List[Path]]:
        """Collect all Python function calls"""
        function_calls = {}
        
        for file_path in root_dir.rglob('*.py'):
            if file_path.is_file():
                self._process_python_calls(file_path, function_calls)
                
        return function_calls
        
    def _process_python_calls(self, file_path: Path, 
                            function_calls: Dict[str, List[Path]]) -> None:
        """Process a single Python file for function calls"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
                tree = ast.parse(content)
                
                # Find all function calls
                for node in ast.walk(tree):
                    if isinstance(node, ast.Call) and isinstance(node.func, ast.Name):
                        func_name = node.func.id
                        if func_name not in function_calls:
                            function_calls[func_name] = []
                        if file_path not in function_calls[func_name]:
                            function_calls[func_name].append(file_path)
                            
        except Exception as e:
            self.log_error(f"Error processing calls in {file_path}: {e}")
            raise
            
    def collect_implementation_status(self, root_dir: Path) -> Dict[str, str]:
        """
        Collect implementation status of functions:
        - 'orphaned': No declaration found
        - 'stubbed': Declaration exists but no implementation
        - 'implemented': Full implementation exists
        """
        declarations = self.collect_function_declarations(root_dir)
        all_calls = self.collect_function_calls(root_dir)
        
        status_map = {}
        
        for func_name, call_locations in all_calls.items():
            if func_name not in declarations:
                status_map[func_name] = 'orphaned'
            else:
                status_map[func_name] = 'implemented'
                
        return status_map
