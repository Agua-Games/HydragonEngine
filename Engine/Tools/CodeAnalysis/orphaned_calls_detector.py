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
import git
from enum import Enum
from datetime import datetime
import html

logger = logging.getLogger(__name__)

class LogFormat(Enum):
    """Output format for logs"""
    TERMINAL = "terminal"
    HTML = "html"
    PLAIN = "plain"

@dataclass
class LoggingConfig:
    """Configuration for log output"""
    format: LogFormat = LogFormat.TERMINAL
    output_file: Optional[Path] = None
    use_colors: bool = True

class ColorScheme:
    """ANSI and HTML color definitions"""
    # Terminal colors
    TERMINAL = {
        'high': '\033[91m',  # Red
        'medium': '\033[93m',  # Yellow
        'low': '\033[92m',  # Green
        'reset': '\033[0m',
        'bold': '\033[1m',
        'header': '\033[95m',  # Purple
        'info': '\033[94m',  # Blue
    }
    
    # HTML colors
    HTML = {
        'high': '#ff4444',
        'medium': '#ffbb33',
        'low': '#00C851',
        'header': '#9933CC',
        'info': '#33b5e5',
    }

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
    last_used: Optional[str] = None  # Git timestamp of last usage
    severity: str = "unknown"  # high/medium/low/unknown

    def calculate_severity(self) -> str:
        """Calculate severity based on various factors"""
        if self.is_public:
            self.severity = "low"  # Public APIs might be used externally
        elif not self.last_used:
            self.severity = "high"  # Never used
        else:
            self.severity = "medium"  # Default for private used elements
        return self.severity

@dataclass
class OrphanedCallsResult:
    """Analysis results"""
    orphaned_elements: Dict[str, List[CodeElement]] = field(default_factory=lambda: {
        'function_calls': [], 
        'variable_calls': [], 
        'class_calls': []
    })
    statistics: Dict[str, int] = field(default_factory=dict)
    suggestions: List[str] = field(default_factory=list)

class OrphanedCallsDetector:
    """Detects orphaned calls in Python code"""
    
    def __init__(self):
        """Initialize detector"""
        self.excluded_paths = {'__pycache__', '.git', '.venv', 'venv', 'env'}
        self.repo = None
        try:
            self.repo = git.Repo(search_parent_directories=True)
        except git.InvalidGitRepositoryError:
            logger.warning("Not running from a git repository")
            
        # Initialize tracking dictionaries
        self._declarations = {
            'function_calls': {},
            'variable_calls': {},
            'class_calls': {}
        }
        self._references = {
            'function_calls': set(),
            'variable_calls': set(),
            'class_calls': set()
        }
        self._current_source = ""
        self._current_scope = None

    def _collect_references(self, file_path: Path) -> None:
        """Collect all references from a file"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                tree = ast.parse(f.read())
                
            for node in ast.walk(tree):
                # Function calls and references
                if isinstance(node, ast.Call):
                    if isinstance(node.func, ast.Name):
                        self._add_orphaned_call('function_calls', node.func.id)
                    elif isinstance(node.func, ast.Attribute):
                        self._add_orphaned_call('function_calls', node.func.attr)
                
                # Variable references
                elif isinstance(node, ast.Name) and isinstance(node.ctx, ast.Load):
                    self._add_orphaned_call('variable_calls', node.id)
                
                # Class references
                elif isinstance(node, ast.Name) and not isinstance(node.ctx, ast.Store):
                    self._add_orphaned_call('class_calls', node.id)
                elif isinstance(node, ast.Attribute):
                    self._add_orphaned_call('class_calls', node.attr)
                        
        except Exception as e:
            logger.error(f"Error analyzing Python file {file_path}: {e}") 

    def analyze_codebase(self, root_dir: Path) -> OrphanedCallsResult:
        """Analyze Python codebase for orphaned calls
        
        Args:
            root_dir: Root directory of the codebase to analyze
            
        Returns:
            OrphanedCallsResult containing analysis results
        """
        try:
            result = OrphanedCallsResult()
            logger.info(f"Found {sum(1 for _ in root_dir.rglob('*.py'))} Python files to analyze")

            # First pass: collect all declarations
            for file_path in root_dir.rglob('*.py'):
                if any(excluded in file_path.parts for excluded in self.excluded_paths):
                    continue
                    
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        tree = ast.parse(f.read())
                        
                    for node in ast.walk(tree):
                        if isinstance(node, ast.FunctionDef):
                            self._declarations['function_calls'][node.name] = node
                        elif isinstance(node, ast.ClassDef):
                            self._declarations['class_calls'][node.name] = node
                        elif isinstance(node, ast.Name) and isinstance(node.ctx, ast.Store):
                            self._declarations['variable_calls'][node.id] = node
                            
                except Exception as e:
                    logger.error(f"Error collecting declarations from {file_path}: {e}")

            # Second pass: collect references and identify orphaned calls
            for file_path in root_dir.rglob('*.py'):
                if any(excluded in file_path.parts for excluded in self.excluded_paths):
                    continue
                self._collect_references(file_path)

            # Update statistics
            self._update_statistics(result)
            
            return result
            
        except Exception as e:
            logger.error(f"Error analyzing codebase: {e}")
            raise 

    def _add_orphaned_call(self, category: str, name: str) -> None:
        """Add an orphaned call to the results
        
        Args:
            category: Category of the call ('function_calls', 'variable_calls', 'class_calls')
            name: Name of the referenced element
        """
        try:
            # Skip built-in names and common imports
            if name in dir(__builtins__) or name in {'logging', 'Path', 'Optional', 'List', 'Set', 'Dict'}:
                return
                
            # Skip if the name is declared
            if name in self._declarations[category]:
                return
                
            # Add to references set
            self._references[category].add(name)
                
        except Exception as e:
            logger.error(f"Error adding orphaned call {name}: {e}")

    def _update_statistics(self, result: OrphanedCallsResult) -> None:
        """Update result statistics
        
        Args:
            result: OrphanedCallsResult to update
        """
        try:
            for category in ['function_calls', 'variable_calls', 'class_calls']:
                # Count total declarations
                result.statistics[f'total_{category}'] = len(self._declarations[category])
                
                # Count orphaned calls (referenced but not declared)
                orphaned = self._references[category] - set(self._declarations[category].keys())
                result.statistics[f'orphaned_{category}'] = len(orphaned)
                
                # Add orphaned elements to result
                for name in orphaned:
                    element = CodeElement(
                        name=name,
                        element_type=category,
                        locations=[],  # Locations would be added when found
                        severity="high"  # Undefined references are high severity
                    )
                    result.orphaned_elements[category].append(element)
                    
        except Exception as e:
            logger.error(f"Error updating statistics: {e}") 

    def analyze_third_party(self, root_dir: Path) -> Dict[str, int]:
        """Analyze third-party packages for declarations
        
        Args:
            root_dir: Root directory containing third-party packages
            
        Returns:
            Dict mapping categories to total declaration counts
        """
        try:
            # Look for site-packages in common virtual environment locations
            venv_paths = [
                root_dir / 'Tools' / 'DocumentationTool' / '.venv' / 'Lib' / 'site-packages',
                root_dir / '.venv' / 'Lib' / 'site-packages',
                root_dir / 'venv' / 'Lib' / 'site-packages'
            ]
            
            site_packages = next((p for p in venv_paths if p.exists()), None)
            if not site_packages:
                logger.warning("No site-packages directory found")
                return {}
                
            logger.info(f"Analyzing third-party packages in: {site_packages}")
            
            # Track only declaration counts for third-party
            stats = {
                'functions': 0,
                'variables': 0,
                'classes': 0
            }
            
            # Analyze .py files in site-packages
            for file_path in site_packages.rglob('*.py'):
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        tree = ast.parse(f.read())
                        
                    for node in ast.walk(tree):
                        if isinstance(node, ast.FunctionDef):
                            stats['functions'] += 1
                        elif isinstance(node, ast.ClassDef):
                            stats['classes'] += 1
                        elif isinstance(node, ast.Name) and isinstance(node.ctx, ast.Store):
                            stats['variables'] += 1
                            
                except Exception as e:
                    logger.debug(f"Error analyzing third-party file {file_path}: {e}")
                    continue
                    
            return stats
            
        except Exception as e:
            logger.error(f"Error analyzing third-party packages: {e}")
            return {} 

    def _suggest_action(self, element: CodeElement) -> str:
        """Suggest an action for handling an orphaned element
        
        Args:
            element: CodeElement to analyze
            
        Returns:
            str: Suggested action for handling the element
        """
        try:
            # Base suggestion on severity and element type
            if element.severity == "high":
                if element.element_type == "function_calls":
                    return "Remove call or implement missing function"
                elif element.element_type == "variable_calls":
                    return "Define variable or remove reference"
                elif element.element_type == "class_calls":
                    return "Implement missing class or update reference"
                    
            elif element.severity == "medium":
                return "Review usage and consider deprecation if unused"
                
            elif element.severity == "low":
                return "Document usage or mark as intentionally undefined"
                
            return "Review and determine appropriate action"
            
        except Exception as e:
            logger.error(f"Error suggesting action for {element.name}: {e}")
            return "Unable to determine action - review manually" 