"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Simple script to collect all function declarations from Python files.
"""

import ast
import logging
from pathlib import Path
from typing import Dict, Set, Tuple

logger = logging.getLogger(__name__)

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

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    
    engine_root = Path(__file__).parent.parent.parent
    functions = collect_function_declarations(engine_root)
    
    logger.info(f"\nFound {len(functions)} function declarations:")
    for func_name, file_path in sorted(functions.items()):
        logger.info(f"- {func_name} in {file_path}")
        
    logger.info(f"\nTotal function declarations found: {len(functions)}") 