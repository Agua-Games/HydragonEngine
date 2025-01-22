"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Base class for orphaned call resolvers.
"""

from pathlib import Path
from typing import Dict, List
import abc

class BaseOrphanedCallResolver(abc.ABC):
    """Abstract base class for resolving orphaned function calls"""
    
    def __init__(self, root_dir: Path):
        self.root_dir = root_dir
        
    @abc.abstractmethod
    def generate_report(self) -> str:
        """Generate report of orphaned calls"""
        pass
        
    @abc.abstractmethod 
    def resolve_orphaned_calls(self, strategy: str = 'create') -> None:
        """
        Resolve orphaned calls using specified strategy
        
        Args:
            strategy: Resolution strategy - 'create', 'remove', or 'link'
        """
        pass
        
    def _confirm_action(self, message: str) -> bool:
        """Confirm an action with the user"""
        # Default implementation - always confirm
        return True
        
    def _determine_implementation_location(self, call_locations: List[Path]) -> Path:
        """Determine best location for new implementation"""
        return call_locations[0].parent / "orphaned_implementations.py"
        
    def _add_function_to_file(self, file_path: Path, function_code: str) -> None:
        """Add function implementation to file"""
        try:
            with open(file_path, 'a', encoding='utf-8') as f:
                f.write(function_code)
        except Exception as e:
            print(f"Error writing to {file_path}: {e}")
