"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Python orphaned call resolver implementation.
"""

from pathlib import Path
from typing import Dict, List
from .base_resolver import BaseOrphanedCallResolver
from .python_function_collector import PythonFunctionCollector

class PythonOrphanedCallResolver(BaseOrphanedCallResolver):
    """Resolves orphaned Python function calls"""
    
    def __init__(self, root_dir: Path):
        super().__init__(root_dir)
        self.collector = PythonFunctionCollector()
        self.orphaned_calls = self.collector.collect_orphaned_calls(root_dir)
        
    def generate_report(self) -> str:
        """Generate report of orphaned calls"""
        report = ["Orphaned Python Function Call Report"]
        report.append(f"Total orphaned calls: {len(self.orphaned_calls)}")
        
        for func_name, call_locations in self.orphaned_calls.items():
            report.append(f"\nFunction: {func_name}")
            report.append(f"Call locations: {len(call_locations)}")
            for loc in call_locations:
                report.append(f"- {loc}")
                
        return "\n".join(report)

    def resolve_orphaned_calls(self, strategy: str = 'create') -> None:
        """
        Resolve orphaned calls using specified strategy
        
        Args:
            strategy: Resolution strategy - 'create', 'remove', or 'link'
        """
        if not self.orphaned_calls:
            return
            
        for func_name, call_locations in self.orphaned_calls.items():
            if strategy == 'create':
                self._create_function_implementation(func_name, call_locations)
            elif strategy == 'remove':
                self._remove_orphaned_calls(func_name, call_locations)
            elif strategy == 'link':
                self._link_to_existing_function(func_name, call_locations)
            else:
                raise ValueError(f"Invalid strategy: {strategy}")

    def _create_function_implementation(self, func_name: str, call_locations: List[Path]) -> None:
        """Create new function implementation for orphaned call"""
        if not self._confirm_action(f"Create implementation for {func_name}?"):
            return
            
        target_file = self._determine_implementation_location(call_locations)
        
        if not target_file.exists():
            return
            
        function_code = f"""
def {func_name}(*args, **kwargs):
    \"\"\"
    Auto-generated implementation for previously orphaned function
    
    WARNING: This is a placeholder implementation. Actual functionality needs to be implemented.
    
    Args:
        *args: Variable length argument list
        **kwargs: Arbitrary keyword arguments
        
    Returns:
        None (implementation pending)
        
    Raises:
        NotImplementedError: Until proper implementation is added
    \"\"\"
    raise NotImplementedError(
        f"Placeholder implementation for {func_name}. "
        "Actual functionality needs to be implemented."
    )
"""
        if self._confirm_action(f"Add implementation to {target_file.name}?"):
            self._add_function_to_file(target_file, function_code)

    def _remove_orphaned_calls(self, func_name: str, call_locations: List[Path]) -> None:
        """Remove orphaned function calls"""
        if not self._confirm_action(f"Remove all calls to {func_name}?"):
            return
            
        for call_file in call_locations:
            try:
                with open(call_file, 'r', encoding='utf-8') as f:
                    lines = f.readlines()
                    
                new_lines = [line for line in lines if func_name not in line]
                
                if len(new_lines) != len(lines):
                    with open(call_file, 'w', encoding='utf-8') as f:
                        f.writelines(new_lines)
                        
            except Exception as e:
                print(f"Error processing {call_file}: {e}")

    def _link_to_existing_function(self, func_name: str, call_locations: List[Path]) -> None:
        """Link orphaned calls to existing functions"""
        existing_functions = self._find_existing_functions(func_name)
        
        if not existing_functions:
            return
            
        if not self._confirm_action(f"Link calls to {func_name}?"):
            return
            
        self._update_call_sites(func_name, call_locations, existing_functions[0])

    def _find_existing_functions(self, func_name: str) -> List[Dict]:
        """Find existing functions with matching names"""
        existing_functions = []
        declarations = self.collector.collect_function_declarations(self.root_dir)
        
        for decl_name, file_path in declarations.items():
            if decl_name == func_name:
                existing_functions.append({
                    'name': decl_name,
                    'file': file_path
                })
                
        return existing_functions

    def _update_call_sites(self, func_name: str, call_locations: List[Path], target_function: Dict) -> None:
        """Update call sites to reference correct function"""
        target_file = target_function['file']
        
        for call_file in call_locations:
            try:
                with open(call_file, 'r', encoding='utf-8') as f:
                    content = f.readlines()
                    
                modified = False
                with open(call_file, 'w', encoding='utf-8') as f:
                    for line in content:
                        if func_name in line:
                            # Add import if needed
                            if f"from {target_file.stem} import {func_name}" not in content:
                                f.write(f"from {target_file.stem} import {func_name}\n")
                            modified = True
                        f.write(line)
                        
            except Exception as e:
                print(f"Error updating {call_file}: {e}")

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
