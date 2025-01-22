"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

TypeScript function collector implementation.
"""

import re
from pathlib import Path
from typing import Dict, List
from .base_collector import BaseFunctionCollector

class TypeScriptFunctionCollector(BaseFunctionCollector):
    """Collects TypeScript function declarations and calls"""
    
    def get_supported_extensions(self) -> List[str]:
        return ['.ts', '.tsx']
        
    def collect_function_declarations(self, root_dir: Path) -> Dict[str, Path]:
        """Collect TypeScript function declarations"""
        function_locations = {}
        pattern = re.compile(
            r'^\s*(?:export\s+)?(?:async\s+)?function\s+(\w+)\s*\('
            r'|^\s*(?:export\s+)?(?:const|let|var)\s+(\w+)\s*=\s*\([^)]*\)\s*=>'
        )
        
        for file_path in root_dir.rglob('*.[tj]sx?'):
            if file_path.suffix in self.file_extensions:
                self._process_ts_file(file_path, function_locations, pattern)
                
        return function_locations
        
    def _process_ts_file(self, file_path: Path, function_locations: Dict[str, Path], pattern: re.Pattern) -> None:
        """Process a single TypeScript file"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                for line in f:
                    match = pattern.match(line)
                    if match:
                        func_name = match.group(1) or match.group(2)
                        if func_name:
                            function_locations[func_name] = file_path
        except Exception:
            pass
            
    def collect_function_calls(self, root_dir: Path) -> Dict[str, List[Path]]:
        """Collect TypeScript function calls"""
        function_calls = {}
        pattern = re.compile(r'\b(\w+)\s*\(')
        
        for file_path in root_dir.rglob('*.[tj]sx?'):
            if file_path.suffix in self.file_extensions:
                self._process_ts_calls(file_path, function_calls, pattern)
                
        return function_calls
        
    def _process_ts_calls(self, file_path: Path, function_calls: Dict[str, List[Path]], pattern: re.Pattern) -> None:
        """Process a single TypeScript file for function calls"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                for line in f:
                    matches = pattern.findall(line)
                    for func_name in matches:
                        if func_name not in function_calls:
                            function_calls[func_name] = []
                        if file_path not in function_calls[func_name]:
                            function_calls[func_name].append(file_path)
        except Exception:
            pass
