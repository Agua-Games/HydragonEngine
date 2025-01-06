"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Tool to collect C++ function declarations and calls.
"""

from pathlib import Path
from typing import Dict, List, Set
import re
from base_collector import BaseFunctionCollector

class CppFunctionCollector(BaseFunctionCollector):
    """Collects C++ function declarations and calls"""
    
    def get_supported_extensions(self) -> List[str]:
        return ['.h', '.hpp', '.cpp', '.cxx', '.cc']
        
    def __init__(self):
        super().__init__()
        # Regex patterns for C++ function detection
        self.func_def_pattern = re.compile(
            r'^\s*(?:template\s*<[^>]+>\s*)?'  # Template prefix
            r'(?:\w+\s+)*'  # Return type and qualifiers
            r'(\w+)\s*\([^)]*\)'  # Function name and parameters
        )
        self.func_call_pattern = re.compile(
            r'\b(\w+)\s*\([^)]*\)'  # Function calls
        )
        self.class_pattern = re.compile(
            r'^\s*class\s+(\w+)'  # Class declarations
        )
        
    def collect_function_declarations(self, root_dir: Path) -> Dict[str, Path]:
        """Collect all C++ function declarations"""
        function_locations = {}
        
        for file_path in root_dir.rglob('*.[hc]pp'):
            if file_path.is_file():
                self._process_cpp_file(file_path, function_locations)
                
        return function_locations
        
    def _process_cpp_file(self, file_path: Path, function_locations: Dict[str, Path]) -> None:
        """Process a single C++ file for function declarations"""
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
                            func_name = f"{current_class}::{func_name}"
                        function_locations[func_name] = file_path
                        
        except Exception:
            pass
            
    def collect_function_calls(self, root_dir: Path) -> Dict[str, List[Path]]:
        """Collect all C++ function calls"""
        function_calls = {}
        
        for file_path in root_dir.rglob('*.[hc]pp'):
            if file_path.is_file():
                self._process_cpp_calls(file_path, function_calls)
                
        return function_calls
        
    def _process_cpp_calls(self, file_path: Path, function_calls: Dict[str, List[Path]]) -> None:
        """Process a single C++ file for function calls"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                for line in f:
                    matches = self.func_call_pattern.findall(line)
                    for func_name in matches:
                        if func_name not in function_calls:
                            function_calls[func_name] = []
                        if file_path not in function_calls[func_name]:
                            function_calls[func_name].append(file_path)
                            
        except Exception:
            pass
            
    def collect_implementation_status(self, root_dir: Path) -> Dict[str, str]:
        """
        Collect implementation status of functions:
        - 'orphaned': No declaration found
        - 'stubbed': Declaration exists but no implementation
        - 'implemented': Full implementation exists
        """
        declarations = self.collect_function_declarations(root_dir)
        all_calls = self.collect_function_calls(root_dir)
        implementations = self.collect_function_implementations(root_dir)
        
        status_map = {}
        
        for func_name, call_locations in all_calls.items():
            if func_name not in declarations:
                status_map[func_name] = 'orphaned'
            elif func_name in declarations and func_name not in implementations:
                status_map[func_name] = 'stubbed'
            else:
                status_map[func_name] = 'implemented'
                
        return status_map

    def collect_function_implementations(self, root_dir: Path) -> Dict[str, Path]:
        """Collect actual function implementations from .cpp files"""
        implementations = {}
        
        for file_path in root_dir.rglob('*.cpp'):
            if file_path.is_file():
                with open(file_path, 'r', encoding='utf-8') as f:
                    content = f.read()
                    # Match function implementations
                    matches = re.finditer(r'\b\w+\s+\w+::?\w+\s*\([^)]*\)\s*\{', content)
                    for match in matches:
                        func_name = match.group().split('(')[0].strip()
                        implementations[func_name] = file_path
                        
        return implementations

    def collect_namespaces(self, root_dir: Path) -> Dict[str, List[Path]]:
        """Collect all namespace declarations and their locations"""
        namespaces = {}
        
        try:
            for file_path in root_dir.rglob('*.[hc]pp'):
                if file_path.is_file():
                    with open(file_path, 'r', encoding='utf-8') as f:
                        for line in f:
                            match = re.match(r'namespace\s+(\w+)', line)
                            if match:
                                namespace = match.group(1)
                                if namespace not in namespaces:
                                    namespaces[namespace] = []
                                namespaces[namespace].append(file_path)
        except Exception as e:
            self.log_error(f"Error collecting namespaces: {e}")
            raise
                            
        return namespaces

    def collect_orphaned_namespaces(self, root_dir: Path) -> Dict[str, List[Path]]:
        """Collect references to non-existent namespaces"""
        existing_namespaces = set()
        orphaned_namespaces = {}
        
        try:
            # First collect all declared namespaces
            namespaces = self.collect_namespaces(root_dir)
            existing_namespaces = set(namespaces.keys())
            
            # Then check all namespace references
            for file_path in root_dir.rglob('*.[hc]pp'):
                if file_path.is_file():
                    with open(file_path, 'r', encoding='utf-8') as f:
                        for line in f:
                            matches = re.finditer(r'(?<!namespace\s)\b(\w+)::', line)
                            for match in matches:
                                namespace = match.group(1)
                                if namespace not in existing_namespaces:
                                    if namespace not in orphaned_namespaces:
                                        orphaned_namespaces[namespace] = []
                                    orphaned_namespaces[namespace].append(file_path)
        except Exception as e:
            self.log_error(f"Error collecting orphaned namespaces: {e}")
            raise
                            
        return orphaned_namespaces
