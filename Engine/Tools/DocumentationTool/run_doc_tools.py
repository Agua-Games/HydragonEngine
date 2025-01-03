"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Wrapper script to run documentation tools using virtual environment
"""

import subprocess
import sys
import os
from pathlib import Path

# Use relative paths
SCRIPT_DIR = Path(__file__).parent
VENV_DIR = SCRIPT_DIR / ".venv"

def ensure_venv():
    """Ensure virtual environment exists"""
    if not VENV_DIR.exists():
        print("Virtual environment not found. Running installer...")
        subprocess.run([sys.executable, str(SCRIPT_DIR / "install_dependencies.py")])

def run_tool(tool_name: str, args: list = None):
    """Run a documentation tool using the virtual environment"""
    if args is None:
        args = []
        
    python_path = VENV_DIR / ("Scripts" if os.name == "nt" else "bin") / "python"
    tool_path = SCRIPT_DIR / tool_name
    
    # Add the Engine/Tools directory to PYTHONPATH
    env = os.environ.copy()
    tools_path = SCRIPT_DIR.parent
    if 'PYTHONPATH' in env:
        env['PYTHONPATH'] = f"{tools_path}{os.pathsep}{env['PYTHONPATH']}"
    else:
        env['PYTHONPATH'] = str(tools_path)
    
    subprocess.run([str(python_path), str(tool_path)] + args, env=env)

if __name__ == "__main__":
    ensure_venv()
    
    if len(sys.argv) < 2:
        print("Usage: python run_doc_tools.py <tool_name> [args...]")
        print("Available tools:")
        print("  generate_standards_doc.py - Generate coding standards documentation")
        sys.exit(1)
        
    tool_name = sys.argv[1]
    args = sys.argv[2:]
    run_tool(tool_name, args) 