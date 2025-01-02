"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Installs required dependencies for documentation tools.
"""

import subprocess
import sys
import venv
from pathlib import Path

# Get Engine root directory relative to this script
ENGINE_ROOT = Path(__file__).parent.parent.parent
TOOL_ROOT = Path(__file__).parent
VENV_DIR = TOOL_ROOT / ".venv"

def create_venv():
    """Create virtual environment"""
    print(f"Creating virtual environment in {VENV_DIR}...")
    venv.create(VENV_DIR, with_pip=True)

def install_requirements():
    """Install required packages"""
    if sys.platform == "win32":
        python = VENV_DIR / "Scripts" / "python.exe"
        pip = VENV_DIR / "Scripts" / "pip.exe"
    else:
        python = VENV_DIR / "bin" / "python"
        pip = VENV_DIR / "bin" / "pip"
    
    print("Installing required packages...")
    try:
        subprocess.run([str(pip), "install", "-U", "pip"], check=True)
        subprocess.run([str(pip), "install", "pypandoc", "jinja2"], check=True)
        print("\nDependencies installed successfully!")
    except subprocess.CalledProcessError as e:
        print(f"\nError installing dependencies: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if not VENV_DIR.exists():
        create_venv()
    install_requirements() 