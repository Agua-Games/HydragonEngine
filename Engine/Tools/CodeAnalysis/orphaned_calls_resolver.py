"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Tool to resolve orphaned function calls identified by function_collector.py
"""

import subprocess
import sys
from pathlib import Path
from typing import Dict, List, Optional
import requests
from declarations_calls_collector import collect_orphaned_function_calls

# PyPI API endpoint for package verification
PYPI_API_URL = "https://pypi.org/pypi/{package}/json"

class ImportVerificationError(Exception):
    """Exception for failed import verification"""
    pass

class OrphanedCallResolver:
    """Main class for resolving orphaned function calls"""
    
    def __init__(self, root_dir: Path):
        self.root_dir = root_dir
        self.orphaned_calls = collect_orphaned_function_calls(root_dir)
        
    def _verify_package(self, package_name: str) -> bool:
        """Verify a package exists on PyPI and is from official source"""
        try:
            response = requests.get(PYPI_API_URL.format(package=package_name), timeout=5)
            if response.status_code == 200:
                package_info = response.json()
                return package_info.get('info', {}).get('package_url', '').startswith('https://pypi.org/project/')
            return False
        except Exception:
            return False
            
    def _verify_import(self, import_name: str) -> bool:
        """Verify if an import is safe to add"""
        if import_name in sys.stdlib_module_names:
            return True
        if self._verify_package(import_name):
            return True
        if (self.root_dir / f"{import_name.replace('.', '/')}.py").exists():
            return True
        raise ImportVerificationError(f"Unverified import: {import_name}")

    def _confirm_action(self, message: str) -> bool:
        """Get user confirmation for potentially risky actions"""
        # In auto mode, always confirm actions
        if hasattr(self, '_mode') and self._mode == 'auto':
            return True
            
        print(f"ACTION REQUIRED: {message} [y/N]")
        try:
            response = input().strip().lower()
            return response == 'y'
        except Exception:
            return False
            
    def set_mode(self, mode: str) -> None:
        """Set the confirmation mode"""
        self._mode = mode
        
    def _update_call_sites(self, func_name: str, call_locations: List[Path], target_function: Dict) -> None:
        """Update call sites to reference correct function with import verification"""
        target_module = self._get_module_path(target_function['file'])
        
        try:
            if not self._verify_import(target_module):
                return
                
            for call_file in call_locations:
                try:
                    with open(call_file, 'r', encoding='utf-8') as f:
                        content = f.readlines()
                        
                    modified = False
                    with open(call_file, 'w', encoding='utf-8') as f:
                        for line in content:
                            if func_name in line:
                                if not any(f"from {target_module}" in l for l in content):
                                    f.write(f"from {target_module} import {func_name}\n")
                                modified = True
                            f.write(line)
                            
                except Exception:
                    continue
                    
        except ImportVerificationError:
            return
            
    def _get_module_path(self, file_path: Path) -> str:
        """Convert file path to importable module path"""
        rel_path = file_path.relative_to(self.root_dir)
        return '.'.join(rel_path.with_suffix('').parts)

    def generate_report(self) -> str:
        """Generate report of orphaned calls and resolution status"""
        report = ["Orphaned Function Call Report"]
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
        """Create new function implementation for orphaned call with safety checks"""
        if not func_name.isidentifier():
            return
            
        if func_name in collect_function_declarations(self.root_dir):
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
        if self._confirm_action(f"Create implementation for {func_name}?"):
            self._add_function_to_file(target_file, function_code)

    def _remove_orphaned_calls(self, func_name: str, call_locations: List[Path]) -> None:
        """Remove orphaned function calls with safety checks"""
        if not self._confirm_action(f"Remove all calls to {func_name}?"):
            return
        
        for call_file in call_locations:
            try:
                # Read file content
                with open(call_file, 'r', encoding='utf-8') as f:
                    lines = f.readlines()
                
                # Process each line
                new_lines = [line for line in lines if func_name not in line]
                
                # Only write back if changes were made
                if len(new_lines) != len(lines):
                    with open(call_file, 'w', encoding='utf-8') as f:
                        f.writelines(new_lines)
                    
            except Exception as e:
                print(f"Error processing {call_file}: {e}")
                continue

    def _link_to_existing_function(self, func_name: str, call_locations: List[Path]) -> None:
        """Link orphaned calls to existing functions with safety checks"""
        if not func_name.isidentifier():
            return
            
        existing_functions = self._find_existing_functions(func_name)
        
        if not existing_functions:
            return
            
        valid_functions = self._validate_function_signatures(func_name, existing_functions)
        
        if not valid_functions:
            return
            
        if not self._confirm_action(f"Link calls to {func_name}?"):
            return
            
        self._update_call_sites(func_name, call_locations, valid_functions[0])

    def _find_existing_functions(self, func_name: str) -> List[Dict]:
        """Find existing functions with matching names"""
        existing_functions = []
        
        for py_file in self.root_dir.rglob("*.py"):
            try:
                with open(py_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                    tree = ast.parse(content)
                    
                for node in ast.walk(tree):
                    if isinstance(node, ast.FunctionDef) and node.name == func_name:
                        params = [arg.arg for arg in node.args.args]
                        returns = isinstance(node.returns, ast.Name) and node.returns.id or None
                        
                        existing_functions.append({
                            'file': py_file,
                            'name': node.name,
                            'params': params,
                            'returns': returns,
                            'lineno': node.lineno
                        })
                        
            except Exception:
                continue
                
        return existing_functions
        
    def _validate_function_signatures(self, func_name: str, functions: List[Dict]) -> List[Dict]:
        """Validate function signatures match call sites"""
        valid_functions = []
        
        first_call = next(iter(self.orphaned_calls[func_name]), None)
        if not first_call:
            return []
            
        try:
            with open(first_call, 'r', encoding='utf-8') as f:
                tree = ast.parse(f.read())
                
            call_args = 0
            call_kwargs = 0
            found_call = False
            
            for node in ast.walk(tree):
                if isinstance(node, ast.Call) and isinstance(node.func, ast.Name) and node.func.id == func_name:
                    call_args = len(node.args)
                    call_kwargs = len(node.keywords)
                    found_call = True
                    break
                    
            if not found_call:
                return []
                
            for func in functions:
                if len(func['params']) == call_args:
                    valid_functions.append(func)
                    
        except Exception:
            pass
            
        return valid_functions

# Default confirmation mode
DEFAULT_CONFIRM_MODE = 'atomic-actions'  # Options: atomic-actions, batched-actions, auto

if __name__ == "__main__":
    import argparse
    
    parser = argparse.ArgumentParser(description='Resolve orphaned function calls')
    parser.add_argument('--path', type=str, default='Engine/Source/',
                       help='Path to analyze (default: Engine/Source/)')
    parser.add_argument('--strategy', type=str, default='create',
                       choices=['create', 'remove', 'link'],
                       help='Resolution strategy (create/remove/link)')
    parser.add_argument('--mode', type=str, default=DEFAULT_CONFIRM_MODE,
                       choices=['atomic-actions', 'batched-actions', 'auto'],
                       help='Confirmation mode (atomic-actions, batched-actions, auto)')
                       
    args = parser.parse_args()
    
    resolver = OrphanedCallResolver(Path(args.path))
    print(resolver.generate_report())
    
    try:
        if args.mode == 'auto':
            # Auto mode - no confirmations needed
            resolver.resolve_orphaned_calls(strategy=args.strategy)
        elif args.mode == 'batched-actions':
            # Batched mode - single confirmation per strategy
            if input(f"Apply {args.strategy} strategy to all orphaned calls? [y/N] ").strip().lower() == 'y':
                resolver.resolve_orphaned_calls(strategy=args.strategy)
        else:
            # Atomic mode - default behavior with individual confirmations
            resolver.resolve_orphaned_calls(strategy=args.strategy)
    except Exception as e:
        print(f"Error during resolution: {e}")
