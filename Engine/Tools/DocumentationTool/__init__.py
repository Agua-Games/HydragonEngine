"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Documentation Tool package initialization.
"""

import os
import sys
from pathlib import Path

# Add both engine root and package root to Python path
engine_root = Path(__file__).parent.parent.parent.parent
package_root = Path(__file__).parent

for path in [str(engine_root), str(package_root)]:
    if path not in sys.path:
        sys.path.insert(0, path) 