"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Common documentation types and structures.
"""

from dataclasses import dataclass
from typing import Dict, Optional, List

@dataclass
class PythonDocComment:
    """Python documentation comment structure"""
    brief: str
    details: Optional[str] = None
    params: Dict[str, str] = None
    returns: Optional[str] = None
    raises: Dict[str, str] = None 