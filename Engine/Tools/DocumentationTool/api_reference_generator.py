"""
API Reference documentation generator implementation
"""
import re
from pathlib import Path
from typing import Dict, Optional
from .generate_documentation import DocumentationGenerator

class APIReferenceGenerator(DocumentationGenerator):
    """Generates API reference documentation from source files"""
    
    def __init__(self, config_path: Path):
        super().__init__(config_path)
        self.source_extensions = ['.h', '.hpp', '.cpp', '.py']
        
    def parse_doc(self, source_path: Path) -> Optional[Dict]:
        """Parse API documentation from source file"""
        try:
            content = source_path.read_text(encoding='utf-8')
            self.logger.info(f"Parsing API documentation from: {source_path.name}")
            
            if source_path.suffix in ['.h', '.hpp', '.cpp']:
                return self._parse_cpp_file(content)
            elif source_path.suffix == '.py':
                return self._parse_python_file(content)
            return None
        except Exception as e:
            self.logger.error(f"Failed to parse {source_path}: {str(e)}")
            return None
            
    def _parse_cpp_file(self, content: str) -> Dict:
        """Parse C++ source file for API documentation"""
        result = {
            'namespace': '',
            'classes': [],
            'functions': [],
            'variables': [],
            'structs': []
        }
        
        # Parse namespace
        namespace_match = re.search(r'namespace\s+(\w+)\s*{', content)
        if namespace_match:
            result['namespace'] = namespace_match.group(1)
            
        # Parse classes
        class_matches = re.finditer(
            r'(?:/\*\*([^*]|(\*(?!/)))*?\*/|//.*?\n|/\*.*?\*/)?\s*'  # Optional comments
            r'class\s+(\w+)\s*{', 
            content
        )
        for match in class_matches:
            result['classes'].append({
                'name': match.group(3),
                'docstring': match.group(1).strip() if match.group(1) else ""
            })
            
        # Parse functions
        func_matches = re.finditer(
            r'(?:/\*\*([^*]|(\*(?!/)))*?\*/|//.*?\n|/\*.*?\*/)?\s*'  # Optional comments
            r'(\w[\w\s\*&]+)\s+(\w+)\s*\(([^)]*)\)\s*;', 
            content
        )
        for match in func_matches:
            result['functions'].append({
                'name': match.group(4),
                'return_type': match.group(3).strip(),
                'params': match.group(5).strip(),
                'docstring': match.group(1).strip() if match.group(1) else ""
            })
            
        return result
        
    def _parse_python_file(self, content: str) -> Dict:
        """Parse Python source file for API documentation"""
        result = {
            'module_doc': '',
            'classes': [],
            'functions': []
        }
        
        # Parse module docstring
        module_doc = re.search(r'^(""".*?""")', content, re.DOTALL)
        if module_doc:
            result['module_doc'] = module_doc.group(1).strip('"')
            
        # Parse classes
        class_matches = re.finditer(
            r'class\s+(\w+).*?:\s*(""".*?""")', 
            content, re.DOTALL
        )
        for match in class_matches:
            result['classes'].append({
                'name': match.group(1),
                'docstring': match.group(2).strip('"')
            })
            
        # Parse functions
        func_matches = re.finditer(
            r'def\s+(\w+)\s*\(([^)]*)\)\s*:\s*(""".*?""")', 
            content, re.DOTALL
        )
        for match in func_matches:
            result['functions'].append({
                'name': match.group(1),
                'params': match.group(2).strip(),
                'docstring': match.group(3).strip('"')
            })
            
        return result
        
    def generate_markdown(self, 
                         source_path: Path, 
                         output_path: Path) -> bool:
        """Generate API reference markdown documentation"""
        try:
            doc_data = self.parse_doc(source_path)
            if not doc_data:
                return False
                
            markdown = []
            
            # Add module/namespace documentation
            if doc_data.get('module_doc'):
                markdown.append(f"# Module Documentation\n\n{doc_data['module_doc']}\n")
            elif doc_data.get('namespace'):
                markdown.append(f"# Namespace {doc_data['namespace']}\n")
                
            # Add classes section
            if doc_data.get('classes'):
                markdown.append("\n## Classes\n")
                for cls in doc_data['classes']:
                    markdown.append(f"### {cls['name']}\n")
                    if cls.get('docstring'):
                        markdown.append(f"{cls['docstring']}\n")
                        
            # Add functions section
            if doc_data.get('functions'):
                markdown.append("\n## Functions\n")
                for func in doc_data['functions']:
                    markdown.append(f"### {func['name']}\n")
                    if func.get('docstring'):
                        markdown.append(f"{func['docstring']}\n")
                    markdown.append(f"- **Parameters:** {func['params']}\n")
                    markdown.append(f"- **Returns:** {func['return_type']}\n")
                    
            # Write markdown file
            output_path.write_text("\n".join(markdown))
            return True
            
        except Exception as e:
            self.logger.error(f"Failed to generate markdown: {str(e)}")
            return False
            
    def setup_doc_api_reference(self, **kwargs) -> None:
        """Setup API reference documentation generation"""
        self.logger.info("Setting up API reference documentation")
        # Additional API-specific setup can be added here
