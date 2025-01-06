"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Base class for function collectors.
"""

from pathlib import Path
from typing import Dict, List, Set, Type
import abc

class BaseFunctionCollector(abc.ABC):
    """Abstract base class for function collectors"""
    
    def __init__(self):
        self.file_extensions = self.get_supported_extensions()
        
    @abc.abstractmethod
    def get_supported_extensions(self) -> List[str]:
        """Get supported file extensions for this language"""
        pass
        
    @abc.abstractmethod
    def collect_function_declarations(self, root_dir: Path) -> Dict[str, Path]:
        """Collect all function declarations"""
        pass
        
    @abc.abstractmethod
    def collect_function_calls(self, root_dir: Path) -> Dict[str, List[Path]]:
        """Collect all function calls"""
        pass
        
    def collect_orphaned_calls(self, root_dir: Path) -> Dict[str, List[Path]]:
        """Collect function calls that have no matching declarations"""
        declarations = self.collect_function_declarations(root_dir)
        all_calls = self.collect_function_calls(root_dir)
        
        return {
            func_name: call_locations 
            for func_name, call_locations in all_calls.items()
            if func_name not in declarations
        }

class CollectorFactory:
    """Factory for creating appropriate collectors"""
    
    _collectors: Dict[str, Type[BaseFunctionCollector]] = {}
    
    @classmethod
    def register_collector(cls, language: str, collector: Type[BaseFunctionCollector]):
        """Register a new collector type"""
        cls._collectors[language] = collector
        
    @classmethod
    def create_collector(cls, language: str) -> BaseFunctionCollector:
        """Create a collector for the specified language"""
        if language not in cls._collectors:
            raise ValueError(f"No collector registered for language: {language}")
        return cls._collectors[language]()
