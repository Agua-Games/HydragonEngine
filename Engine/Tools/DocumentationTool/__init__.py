"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Documentation Tool package initialization.
"""

from pathlib import Path
import sys

# Add package root to Python path
package_root = Path(__file__).parent
if str(package_root) not in sys.path:
    sys.path.append(str(package_root)) 