"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Test script to verify file path finding functionality
"""

import os
from pathlib import Path

# Setup paths
SCRIPT_DIR = Path(__file__).parent.parent.parent
ENGINE_ROOT = SCRIPT_DIR.parent
SOURCE_DIR = ENGINE_ROOT / "Source"

def find_source_files():
    """Find and return all source files in Engine/Source directory"""
    source_files = []
    try:
        # Walk through source directory
        for root, dirs, files in os.walk(SOURCE_DIR):
            for file in files:
                if file.endswith(('.h', '.hpp', '.cpp', '.py')):
                    full_path = Path(root) / file
                    source_files.append(full_path)
        
        return source_files
    
    except Exception as e:
        print(f"Error finding source files: {str(e)}")
        return []

def main():
    print("Starting file path check...")
    
    # Find and display source files
    files = find_source_files()
    
    if files:
        print(f"\nFound {len(files)} source files:")
        for file in files:
            print(f"- {file}")
    else:
        print("\nNo source files found!")
    
    print("\nFile path check complete.")

if __name__ == "__main__":
    main()
