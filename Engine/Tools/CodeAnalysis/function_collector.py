"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Simple script to collect function declarations and calls using string operations.
"""

from pathlib import Path
from typing import Dict, List
import re

def collect_function_declarations(root_dir: Path) -> Dict[str, Path]:
    """Collect all function declarations from Python files
    
    Args:
        root_dir: Root directory to search for Python files
        
    Returns:
        Dictionary mapping function names to their file paths
    """
    function_locations = {}
    func_def_pattern = re.compile(r'^\s*def\s+(\w+)\s*\(')
    
    for file_path in root_dir.rglob('*.py'):
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                for line in f:
                    match = func_def_pattern.search(line)
                    if match:
                        function_locations[match.group(1)] = file_path
        except Exception:
            continue
            
    return function_locations

def collect_function_calls(root_dir: Path) -> Dict[str, List[Path]]:
    """Collect all function calls from Python files
    
    Args:
        root_dir: Root directory to search for Python files
        
    Returns:
        Dictionary mapping called function names to lists of files where they're called
    """
    function_calls = {}
    func_call_pattern = re.compile(r'\b(\w+)\s*\(')
    
    for file_path in root_dir.rglob('*.py'):
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                for line in f:
                    matches = func_call_pattern.findall(line)
                    for func_name in matches:
                        if func_name not in function_calls:
                            function_calls[func_name] = []
                        if file_path not in function_calls[func_name]:
                            function_calls[func_name].append(file_path)
        except Exception:
            continue
            
    return function_calls

def collect_orphaned_function_calls(root_dir: Path) -> Dict[str, List[Path]]:
    """Collect function calls that have no matching declarations
    
    Args:
        root_dir: Root directory to search for Python files
        
    Returns:
        Dictionary mapping orphaned function names to lists of files where they're called
    """
    declarations = collect_function_declarations(root_dir)
    all_calls = collect_function_calls(root_dir)
    
    orphaned_calls = {
        func_name: call_locations 
        for func_name, call_locations in all_calls.items()
        if func_name not in declarations
    }
    
    return orphaned_calls
