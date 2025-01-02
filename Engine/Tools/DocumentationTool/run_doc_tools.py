"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Wrapper script to run documentation tools using virtual environment
"""

import subprocess
import sys
import os
from pathlib import Path

# Get Engine root directory relative to this script
ENGINE_ROOT = Path(__file__).parent.parent.parent
TOOL_ROOT = Path(__file__).parent
VENV_DIR = TOOL_ROOT / ".venv"

def ensure_venv():
    """Ensure virtual environment exists"""
    if not VENV_DIR.exists():
        print("Virtual environment not found. Running installer...")
        subprocess.run([sys.executable, str(TOOL_ROOT / "install_dependencies.py")])

def run_tool(tool_name: str, args: list = None):
    """Run a documentation tool using the virtual environment"""
    if args is None:
        args = []
        
    python_path = VENV_DIR / ("Scripts" if os.name == "nt" else "bin") / "python"
    tool_path = TOOL_ROOT / tool_name
    
    if not tool_path.exists():
        print(f"Tool not found: {tool_path}")
        print("Available tools:")
        for tool in TOOL_ROOT.glob("*.py"):
            if tool.name not in ["run_doc_tools.py", "install_dependencies.py"]:
                print(f"  {tool.name}")
        sys.exit(1)
        
    subprocess.run([str(python_path), str(tool_path)] + args)

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