"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Simplified dependency installer for documentation tools.
"""

import subprocess
import sys
import logging
from pathlib import Path

# Setup logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# Get Engine root directory relative to this script
ENGINE_ROOT = Path(__file__).parent.parent.parent
TOOL_ROOT = Path(__file__).parent

def install_requirements():
    """Install required packages"""
    try:
        requirements_file = TOOL_ROOT / "requirements.txt"
        if not requirements_file.exists():
            logger.error(f"requirements.txt not found at: {requirements_file}")
            return False
            
        logger.info(f"Installing requirements from: {requirements_file}")
        result = subprocess.run(
            [sys.executable, "-m", "pip", "install", "-r", str(requirements_file)],
            capture_output=True,
            text=True
        )
        
        if result.returncode != 0:
            logger.error(f"Failed to install requirements:\n{result.stderr}")
            return False
        
        logger.info("Dependencies installed successfully!")
        return True
            
    except Exception as e:
        logger.error(f"Error installing Python dependencies: {e}")
        return False

def verify_installation():
    """Verify that all dependencies are correctly installed"""
    try:
        import luaparser
        logger.info("All core dependencies verified successfully!")
        return True
    except ImportError as e:
        logger.error(f"Dependency verification failed: {e}")
        return False

if __name__ == "__main__":
    if install_requirements() and verify_installation():
        logger.info("Setup completed successfully!")
        sys.exit(0)
    else:
        logger.error("Setup failed - please check the logs above")
        sys.exit(1) 