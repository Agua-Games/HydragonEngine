"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Helper script to run documentation tests from any location.
"""

import os
import sys
from pathlib import Path

def find_doc_tool_dir():
    """Find the DocumentationTool directory from current location"""
    current = Path.cwd()
    
    # Try to find Engine/Tools/DocumentationTool
    while current.parts:
        doc_tool_dir = current / "Engine" / "Tools" / "DocumentationTool"
        if doc_tool_dir.exists():
            return doc_tool_dir
        current = current.parent
        
    return None

def main():
    doc_tool_dir = find_doc_tool_dir()
    if not doc_tool_dir:
        print("Error: Could not find Engine/Tools/DocumentationTool directory.")
        print("Please run this script from within the HydragonEngine project.")
        sys.exit(1)
        
    # Change to the correct directory and run tests
    os.chdir(doc_tool_dir)
    print(f"Changed directory to: {doc_tool_dir}")
    
    # Import and run tests
    from run_tests import run_tests
    success = run_tests()
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main() 