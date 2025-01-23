#!/usr/bin/env python3

"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Script to check for missing or incorrect copyright notices in source files.

This tool helps maintain consistent copyright notices across the codebase by:
1. Scanning source files for missing or incorrect copyright notices
2. Creating backups before any modifications
3. Adding standardized copyright headers with proper comment syntax
4. Supporting multiple programming languages
5. Providing both check-only and auto-fix modes

Usage:
    1. Check files without modifying:
       python Tools/check_copyright.py
       
    2. Check and fix missing notices:
       python Tools/check_copyright.py --fix
       
    3. Scan a different directory:
       python Tools/check_copyright.py --dir path/to/directory

Arguments:
    --fix            Add missing copyright notices to files
    --dir PATH       Directory to scan (default: Engine)

Safety Features:
    - Creates backups before modifying any file
    - Restores from backup if any error occurs
    - Only modifies files when --fix flag is used
    - Supports language-specific comment syntax
    - UTF-8 encoding for proper character handling

Example:
    # Check for missing notices
    python Tools/check_copyright.py
    
    # Fix missing notices in the Tools directory
    python Tools/check_copyright.py --fix --dir Tools
"""

import os
import re
from typing import List, Dict, Tuple
import shutil
from datetime import datetime

# Language-specific comment syntax
LANGUAGE_COMMENTS = {
    '.py': {
        'start': '"""',
        'prefix': '',
        'end': '"""'
    },
    '.cpp': {
        'start': '/*',
        'prefix': ' *',
        'end': ' */'
    },
    '.h': {
        'start': '/*',
        'prefix': ' *',
        'end': ' */'
    },
    '.hpp': {
        'start': '/*',
        'prefix': ' *',
        'end': ' */'
    },
    '.c': {
        'start': '/*',
        'prefix': ' *',
        'end': ' */'
    },
    '.cs': {
        'start': '/*',
        'prefix': ' *',
        'end': ' */'
    },
    '.java': {
        'start': '/*',
        'prefix': ' *',
        'end': ' */'
    }
}

def get_comment_syntax(filepath: str) -> Dict[str, str]:
    """Get the appropriate comment syntax for the file extension."""
    ext = os.path.splitext(filepath)[1].lower()
    return LANGUAGE_COMMENTS.get(ext, LANGUAGE_COMMENTS['.cpp'])  # Default to C++ style

def generate_copyright_notice(filepath: str) -> str:
    """Generate copyright notice with appropriate comment syntax."""
    comment = get_comment_syntax(filepath)
    
    notice = [
        f"{comment['start']}",
        f"{comment['prefix']} Copyright (c) 2024 Agua Games. All rights reserved.",
        f"{comment['prefix']} Licensed under the Agua Games License 1.0",
        f"{comment['prefix']} ",
        f"{comment['prefix']} This software is the confidential and proprietary information of Agua Games.",
        f"{comment['prefix']} You shall not disclose such confidential information and shall use it only",
        f"{comment['prefix']} in accordance with the terms of the license agreement.",
        f"{comment['prefix']} ",
        f"{comment['prefix']} File: {os.path.basename(filepath)}",
        f"{comment['prefix']} Author: Agua Games Engine Team",
        f"{comment['prefix']} Created: 2024",
        f"{comment['end']}\n\n"
    ]
    
    return '\n'.join(notice)

def should_check_file(filename: str) -> bool:
    """Determine if a file should be checked for copyright notice."""
    if filename.endswith(('.empty', '.md', '.gitignore')):
        return False
    
    ext = os.path.splitext(filename)[1].lower()
    return ext in LANGUAGE_COMMENTS

def check_copyright(filepath: str) -> bool:
    """Check if a file has the correct copyright notice."""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read(500)  # Read first 500 chars
            return "Copyright (c) 2024 Agua Games" in content
    except Exception as e:
        print(f"Error reading {filepath}: {e}")
        return False

def backup_file(filepath: str) -> str:
    """Create a backup of the file before modifying it."""
    backup_path = f"{filepath}.bak"
    shutil.copy2(filepath, backup_path)
    return backup_path

def safely_add_copyright(filepath: str) -> Tuple[bool, str]:
    """Safely add copyright notice to a file."""
    try:
        # Create backup
        backup_path = backup_file(filepath)
        
        # Read original content
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Generate notice
        notice = generate_copyright_notice(filepath)
        
        # Write new content
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(notice + content)
        
        return True, ""
        
    except Exception as e:
        # Restore from backup if anything goes wrong
        if os.path.exists(backup_path):
            shutil.copy2(backup_path, filepath)
        return False, str(e)
    finally:
        # Clean up backup
        if os.path.exists(backup_path):
            os.remove(backup_path)

def scan_and_fix_directory(directory: str, fix: bool = False) -> List[str]:
    """Scan directory for files missing copyright notice and optionally fix them."""
    missing_copyright = []
    
    for root, _, files in os.walk(directory):
        for filename in files:
            filepath = os.path.join(root, filename)
            
            if should_check_file(filepath):
                if not check_copyright(filepath):
                    missing_copyright.append(filepath)
                    
                    if fix:
                        success, error = safely_add_copyright(filepath)
                        if not success:
                            print(f"Failed to add copyright to {filepath}: {error}")
                        else:
                            print(f"Added copyright notice to {filepath}")
                    
    return missing_copyright

def main():
    import argparse
    parser = argparse.ArgumentParser(description='Check and fix copyright notices in source files')
    parser.add_argument('--fix', action='store_true', help='Add missing copyright notices')
    parser.add_argument('--dir', default='Engine', help='Directory to scan (default: Engine)')
    args = parser.parse_args()
    
    if not os.path.exists(args.dir):
        print(f"Error: {args.dir} directory not found!")
        return
    
    print(f"Scanning {args.dir} for files missing copyright notice...")
    missing = scan_and_fix_directory(args.dir, args.fix)
    
    if missing:
        print("\nFiles missing proper copyright notice:")
        for filepath in missing:
            print(f"- {filepath}")
        print(f"\nTotal files missing copyright: {len(missing)}")
        if not args.fix:
            print("\nRun with --fix to add missing copyright notices")
    else:
        print("\nAll source files have proper copyright notice!")

if __name__ == "__main__":
    main() 