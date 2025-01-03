"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Python source code parser for API documentation generation.
"""

import re
import ast
import logging
from pathlib import Path
from typing import Dict, List, Optional, Set, Tuple
from dataclasses import dataclass
import os
import traceback

from .doc_types import PythonDocComment

logger = logging.getLogger(__name__)

class ParserError(Exception):
    """Custom exception for parser-specific errors"""
    pass

class PythonParser:
    def __init__(self):
        """Initialize parser with PEP 8 standards"""
        self.standards = {
            'copyright_pattern': r'"""[\s\S]*?Copyright \(c\) \d{4} Agua Games',
            'max_line_length': 100,
            'indent_size': 4
        }
        self.processed_files: Set[Path] = set()
        self._setup_logging()
        
    def _setup_logging(self) -> None:
        """Configure detailed logging for the parser"""
        self.log_file = Path("Logs/DocumentationTool/python_parser.log")
        os.makedirs(self.log_file.parent, exist_ok=True)
        
        file_handler = logging.FileHandler(self.log_file)
        file_handler.setFormatter(
            logging.Formatter(
                '%(asctime)s - %(levelname)s - [%(filename)s:%(lineno)d] - %(message)s\n'
                'Context: %(context)s\n'
                '%(details)s\n'
            )
        )
        logger.addHandler(file_handler)
        logger.setLevel(logging.DEBUG)
        
    def _log_error(self, error: Exception, context: str, file_path: Optional[Path] = None, 
                   line_num: Optional[int] = None, code_snippet: Optional[str] = None) -> None:
        """Log detailed error information with context"""
        details = []
        
        if file_path:
            details.append(f"File: {file_path}")
        if line_num:
            details.append(f"Line number: {line_num}")
        if code_snippet:
            details.append("Code context:")
            details.append("```python")
            details.append(code_snippet)
            details.append("```")
            
        details.append("Traceback:")
        details.extend(traceback.format_exc().split('\n'))
        
        logger.error(
            str(error),
            extra={
                'context': context,
                'details': '\n'.join(details)
            }
        )
        
    def parse_file(self, file_path: Path, docs_dir: Path) -> Dict:
        """Parse a Python source file and extract API documentation"""
        try:
            self.processed_files.add(file_path)
            content = file_path.read_text(encoding='utf-8')
            
            # Log file processing start
            logger.info(f"Processing file: {file_path}", extra={
                'context': 'File Processing Start',
                'details': f"Output directory: {docs_dir}"
            })
            
            violations = self._verify_standards_compliance(content, file_path)
            if violations:
                logger.warning(
                    f"Found {len(violations)} standards violations",
                    extra={
                        'context': 'Standards Compliance',
                        'details': '\n'.join(f"- Line {v['line']}: {v['message']}" 
                                           for v in violations)
                    }
                )
            
            tree = ast.parse(content)
            doc_data = {
                'module_doc': self._extract_module_doc(tree),
                'classes': self._extract_classes(tree),
                'functions': self._extract_functions(tree),
                'violations': violations,
                'source_file': file_path
            }
            
            # Log successful parsing
            logger.debug(
                f"Successfully parsed {file_path}",
                extra={
                    'context': 'Parse Success',
                    'details': (f"Found: {len(doc_data['classes'])} classes, "
                              f"{len(doc_data['functions'])} functions")
                }
            )
            
            return doc_data
            
        except SyntaxError as e:
            self._log_error(
                e, 
                "Python Syntax Error",
                file_path,
                e.lineno,
                self._get_code_context(content, e.lineno)
            )
            raise ParserError(f"Syntax error in {file_path}: {str(e)}")
            
        except Exception as e:
            self._log_error(e, "File Parsing Error", file_path)
            raise ParserError(f"Error parsing {file_path}: {str(e)}")
            
    def _get_code_context(self, content: str, line_num: int, context_lines: int = 3) -> str:
        """Get code context around an error"""
        lines = content.splitlines()
        start = max(0, line_num - context_lines - 1)
        end = min(len(lines), line_num + context_lines)
        
        context = []
        for i in range(start, end):
            prefix = '-> ' if i == line_num - 1 else '   '
            context.append(f"{prefix}{i+1:4d} | {lines[i]}")
            
        return '\n'.join(context)
        
    def cleanup_orphaned_docs(self, source_dir: Path, docs_dir: Path) -> None:
        """Remove documentation files for non-existent source files"""
        try:
            for doc_file in docs_dir.rglob('*.html'):
                # Convert doc path back to potential source path
                rel_path = doc_file.relative_to(docs_dir)
                source_path = source_dir / rel_path.with_suffix('.py')
                
                # If source doesn't exist or wasn't processed, remove doc
                if not source_path.exists() or source_path not in self.processed_files:
                    logger.info(f"Removing orphaned doc file: {doc_file}")
                    doc_file.unlink()
                    
                    # Also remove markdown if it exists
                    md_file = doc_file.with_suffix('.md')
                    if md_file.exists():
                        md_file.unlink()
                        
            # Clean up empty directories
            for dir_path in sorted(docs_dir.rglob('*'), reverse=True):
                if dir_path.is_dir() and not any(dir_path.iterdir()):
                    logger.info(f"Removing empty directory: {dir_path}")
                    dir_path.rmdir()
                    
        except Exception as e:
            logger.error(f"Error cleaning up orphaned docs: {e}")
            raise

    def _verify_standards_compliance(self, content: str, file_path: Path) -> List[Dict]:
        """Verify basic PEP 8 compliance"""
        violations = []
        
        # Check copyright notice
        if not re.match(self.standards['copyright_pattern'], content):
            violations.append({
                'line': 1,
                'message': 'Missing or incorrect copyright notice'
            })
            
        # Check line length
        for i, line in enumerate(content.splitlines(), 1):
            if len(line) > self.standards['max_line_length']:
                violations.append({
                    'line': i,
                    'message': f'Line exceeds {self.standards["max_line_length"]} characters'
                })
                
        return violations
        
    def _extract_module_doc(self, tree: ast.AST) -> Optional[PythonDocComment]:
        """Extract module-level documentation"""
        docstring = ast.get_docstring(tree)
        return self._parse_docstring(docstring) if docstring else None
        
    def _parse_docstring(self, docstring: str) -> PythonDocComment:
        """Parse docstring into structured format"""
        lines = docstring.split('\n')
        brief = lines[0].strip() if lines else ""
        details = ""
        params = {}
        returns = None
        raises = {}
        
        current_section = 'details'
        for line in lines[1:]:
            line = line.strip()
            if line.startswith('Args:'):
                current_section = 'args'
            elif line.startswith('Returns:'):
                current_section = 'returns'
            elif line.startswith('Raises:'):
                current_section = 'raises'
            elif line and current_section == 'details':
                details += line + '\n'
            elif current_section == 'args' and ':' in line:
                param, desc = line.split(':', 1)
                params[param.strip()] = desc.strip()
            elif current_section == 'returns' and line:
                returns = line
            elif current_section == 'raises' and ':' in line:
                error, desc = line.split(':', 1)
                raises[error.strip()] = desc.strip()
                
        return PythonDocComment(
            brief=brief,
            details=details.strip(),
            params=params,
            returns=returns,
            raises=raises
        ) 

    def generate_navigation_links(self, source_file: Path, docs_dir: Path) -> Dict[str, str]:
        """Generate relative navigation links for documentation"""
        try:
            rel_path = source_file.relative_to(Path('Engine/Source'))
            current_doc_path = docs_dir / rel_path.with_suffix('.html')
            
            nav_links = {
                'user_guide': self._get_relative_path(current_doc_path, docs_dir / 'UserGuide/index.html'),
                'developer_guide': self._get_relative_path(current_doc_path, docs_dir / 'DeveloperGuide/index.html'),
                'api_reference': self._get_relative_path(current_doc_path, docs_dir / 'APIReference/index.html'),
                'standards': self._get_relative_path(current_doc_path, docs_dir / 'DeveloperGuide/Standards/coding_standards.html'),
                'module_index': self._get_relative_path(current_doc_path, docs_dir / 'APIReference' / rel_path.parent / 'index.html')
            }
            
            # Add links to parent modules
            parent_links = []
            current = rel_path.parent
            while current.name:
                index_path = docs_dir / 'APIReference' / current / 'index.html'
                parent_links.append({
                    'name': current.name,
                    'path': self._get_relative_path(current_doc_path, index_path)
                })
                current = current.parent
            
            nav_links['parent_modules'] = parent_links
            
            return nav_links
            
        except Exception as e:
            logger.error(f"Error generating navigation links for {source_file}: {e}")
            raise
            
    def _get_relative_path(self, from_path: Path, to_path: Path) -> str:
        """Calculate relative path between two documentation files"""
        try:
            # Handle non-existent paths for new files
            if not from_path.parent.exists():
                from_path.parent.mkdir(parents=True, exist_ok=True)
            
            rel_path = os.path.relpath(str(to_path), str(from_path.parent))
            # Ensure forward slashes for web paths
            return rel_path.replace(os.sep, '/')
            
        except Exception as e:
            logger.error(f"Error calculating relative path from {from_path} to {to_path}: {e}")
            # Fall back to absolute path from docs root
            return f"/{to_path.relative_to(docs_dir)}"
            
    def generate_markdown(self, doc_data: Dict, nav_links: Dict[str, str]) -> str:
        """Generate markdown with proper navigation links"""
        try:
            md_lines = []
            
            # Add navigation metadata for template
            md_lines.extend([
                "---",
                f"user_guide_link: {nav_links['user_guide']}",
                f"developer_guide_link: {nav_links['developer_guide']}",
                f"api_reference_link: {nav_links['api_reference']}",
                f"standards_link: {nav_links['standards']}",
                f"module_index_link: {nav_links['module_index']}"
            ])
            
            # Add parent module navigation
            if nav_links['parent_modules']:
                md_lines.append("parent_modules:")
                for parent in nav_links['parent_modules']:
                    md_lines.append(f"  - name: {parent['name']}")
                    md_lines.append(f"    path: {parent['path']}")
                    
            md_lines.append("---\n")
            
            # Continue with existing markdown generation
            source_file = doc_data['source_file'].relative_to(Path('Engine/Source'))
            md_lines.extend([
                f"# {source_file.stem}",
                f"**Source**: `{source_file}`\n"
            ])
            
            # Add module documentation
            if doc_data['module_doc']:
                md_lines.extend([
                    "## Overview",
                    doc_data['module_doc'].brief,
                    "",
                    doc_data['module_doc'].details,
                    ""
                ])
            
            # Add classes
            if doc_data['classes']:
                md_lines.append("## Classes\n")
                for class_info in doc_data['classes']:
                    md_lines.extend(self._format_class_doc(class_info))
            
            # Add functions
            if doc_data['functions']:
                md_lines.append("## Functions\n")
                for func_info in doc_data['functions']:
                    md_lines.extend(self._format_function_doc(func_info))
            
            # Add standards violations if any
            if doc_data['violations']:
                md_lines.extend([
                    "## Standards Violations",
                    "| Line | Message |",
                    "|------|---------|",
                ])
                for violation in doc_data['violations']:
                    md_lines.append(f"| {violation['line']} | {violation['message']} |")
            
            return '\n'.join(line for line in md_lines if line is not None)
            
        except Exception as e:
            logger.error(f"Error generating markdown with navigation: {e}")
            raise
            
    def _format_class_doc(self, class_info: Dict) -> List[str]:
        """Format class documentation as markdown"""
        lines = [
            f"### {class_info['name']}",
            class_info['doc'].brief if class_info['doc'] else "",
            "",
            class_info['doc'].details if class_info['doc'] else "",
            ""
        ]
        
        # Add methods
        if class_info['methods']:
            lines.append("#### Methods\n")
            for method in class_info['methods']:
                lines.extend(self._format_function_doc(method, is_method=True))
                
        return lines
        
    def _format_function_doc(self, func_info: Dict, is_method: bool = False) -> List[str]:
        """Format function documentation as markdown"""
        prefix = "##### " if is_method else "### "
        lines = [
            f"{prefix}{func_info['name']}",
            func_info['doc'].brief if func_info['doc'] else "",
            ""
        ]
        
        if func_info['doc']:
            # Add parameters
            if func_info['doc'].params:
                lines.append("**Parameters:**")
                for param, desc in func_info['doc'].params.items():
                    lines.append(f"- `{param}`: {desc}")
                lines.append("")
            
            # Add return value
            if func_info['doc'].returns:
                lines.extend([
                    "**Returns:**",
                    func_info['doc'].returns,
                    ""
                ])
            
            # Add exceptions
            if func_info['doc'].raises:
                lines.append("**Raises:**")
                for exc, desc in func_info['doc'].raises.items():
                    lines.append(f"- `{exc}`: {desc}")
                lines.append("")
                
        return lines 