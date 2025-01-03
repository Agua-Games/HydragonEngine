"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Simple script to collect all function declarations from Python files.
"""

import ast
import logging
import site
from pathlib import Path
from typing import Dict, Set, Tuple, List

logger = logging.getLogger(__name__)

def _is_third_party_path(file_path: Path) -> bool:
    """Check if path is from third-party libraries
    
    Args:
        file_path: Path to check
        
    Returns:
        bool: True if path is in site-packages or dist-packages
    """
    str_path = str(file_path).lower()
    return any(
        x in str_path 
        for x in ['site-packages', 'dist-packages', '.local/lib/python']
    )

def collect_function_declarations(root_dir: Path) -> Dict[str, Path]:
    """Collect all function declarations from Python files
    
    Args:
        root_dir: Root directory to search for Python files
        
    Returns:
        Dictionary mapping function names to their file paths
    """
    try:
        function_locations = {}
        
        # Find all Python files
        for file_path in root_dir.rglob('*.py'):
            # Skip third-party libraries
            if _is_third_party_path(file_path):
                continue
                
            try:
                with open(file_path, 'r', encoding='utf-8') as f:
                    tree = ast.parse(f.read())
                
                # Collect function names from AST
                for node in ast.walk(tree):
                    if isinstance(node, ast.FunctionDef):
                        function_locations[node.name] = file_path
                        
            except Exception as e:
                logger.error(f"Error parsing {file_path}: {e}")
                continue
                
        return function_locations
        
    except Exception as e:
        logger.error(f"Error collecting function declarations: {e}")
        return {}

def collect_function_calls(root_dir: Path) -> Dict[str, List[Path]]:
    """Collect all function calls from Python files
    
    Args:
        root_dir: Root directory to search for Python files
        
    Returns:
        Dictionary mapping called function names to lists of files where they're called
    """
    try:
        function_calls = {}
        
        # Find all Python files
        for file_path in root_dir.rglob('*.py'):
            if _is_third_party_path(file_path):
                continue
                
            try:
                with open(file_path, 'r', encoding='utf-8') as f:
                    tree = ast.parse(f.read())
                
                # Collect function calls from AST
                for node in ast.walk(tree):
                    if isinstance(node, ast.Call):
                        # Get the function name being called
                        if isinstance(node.func, ast.Name):
                            # Direct function call like: my_function()
                            func_name = node.func.id
                        elif isinstance(node.func, ast.Attribute):
                            # Method call like: obj.method()
                            func_name = node.func.attr
                        else:
                            continue
                            
                        # Add to our collection
                        if func_name not in function_calls:
                            function_calls[func_name] = []
                        if file_path not in function_calls[func_name]:
                            function_calls[func_name].append(file_path)
                        
            except Exception as e:
                logger.error(f"Error parsing {file_path}: {e}")
                continue
                
        return function_calls
        
    except Exception as e:
        logger.error(f"Error collecting function calls: {e}")
        return {}

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    
    engine_root = Path(__file__).parent.parent.parent
    declarations = collect_function_declarations(engine_root)
    calls = collect_function_calls(engine_root)
    
    logger.info(f"\nFound {len(declarations)} function declarations:")
    for func_name, file_path in sorted(declarations.items()):
        logger.info(f"- {func_name} in {file_path}")
        
    logger.info(f"\nFound {len(calls)} unique function calls:")
    for func_name, file_paths in sorted(calls.items()):
        logger.info(f"- {func_name} called in {len(file_paths)} files:")
        for path in file_paths:
            logger.info(f"  - {path}")
            
    logger.info(f"\nTotal function declarations found: {len(declarations)}")
    logger.info(f"Total unique function calls found: {len(calls)}") 