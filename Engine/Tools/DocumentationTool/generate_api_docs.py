"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Generates API Reference documentation by scanning Engine/Source/ directory.
"""
import os
import sys
import logging
from pathlib import Path
import re
from datetime import datetime
import yaml
from typing import Dict, List, Optional

# Local imports
import version
from template_manager import TemplateManager, TemplateConfig
from format_converter import FormatConverter, ConversionConfig
sys.path.append(str(Path(__file__).parent.parent))
from Common.config_manager import EngineConfig

# Setup paths
SCRIPT_DIR = Path(__file__).parent.parent
ENGINE_ROOT = SCRIPT_DIR.parent
SOURCE_DIR = ENGINE_ROOT / "Source"
API_DOCS_DIR = ENGINE_ROOT / "Docs" / "APIReference"
BUILD_OUTPUT = ENGINE_ROOT / "BuildOutput" / "DocumentationTool"
LOGS_DIR = ENGINE_ROOT / "Shared" / "Logs" / "DocumentationTool"
TEMPLATES_DIR = SCRIPT_DIR / "templates"

# Setup concise logging
logger = logging.getLogger(__name__)

def setup_logging():
    """Configure minimal logging for API docs"""
    LOGS_DIR.mkdir(parents=True, exist_ok=True)
    log_file = LOGS_DIR / "api_docs.log"
    
    file_handler = logging.FileHandler(log_file)
    file_handler.setFormatter(logging.Formatter('%(message)s'))
    logger.addHandler(file_handler)
    
    logger.setLevel(logging.INFO)

class APIParser:
    """Handles parsing of source files for API documentation"""
    
    @staticmethod
    def parse_cpp_file(file_path: Path) -> Optional[Dict]:
        """Parse C++ file and extract basic API elements"""
        try:
            content = file_path.read_text(encoding='utf-8')
            logger.info(f"Parsing: {file_path.name}")
            
            # Extract namespace
            namespace = ""
            namespace_match = re.search(r'namespace\s+(\w+)\s*{', content)
            if namespace_match:
                namespace = namespace_match.group(1)
            
            # Handle template classes and macros
            content = re.sub(r'#.*?\n', '', content)  # Remove preprocessor directives
            content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)  # Remove block comments
            
            # Extract classes
            classes = []
            class_matches = re.finditer(
                r'(?:/\*\*([^*]|(\*(?!/)))*?\*/|//.*?\n|/\*.*?\*/)?\s*'  # Optional comments
                r'class\s+(\w+)\s*{', 
                content
            )
            for match in class_matches:
                class_name = match.group(3)
                class_doc = match.group(1).strip() if match.group(1) else ""
                classes.append({
                    'name': class_name,
                    'docstring': class_doc,
                    'methods': [],
                    'variables': [],
                    'structs': []
                })
            
            # Extract functions
            functions = []
            func_matches = re.finditer(
                r'(?:/\*\*([^*]|(\*(?!/)))*?\*/|//.*?\n|/\*.*?\*/)?\s*'  # Optional comments
                r'(\w[\w\s\*&]+)\s+(\w+)\s*\(([^)]*)\)\s*;', 
                content
            )
            for match in func_matches:
                return_type = match.group(3).strip()
                func_name = match.group(4)
                params = match.group(5).strip()
                func_doc = match.group(1).strip() if match.group(1) else ""
                functions.append({
                    'name': func_name,
                    'return_type': return_type,
                    'params': params,
                    'docstring': func_doc
                })
            
            # Extract variables with default values
            variables = []
            var_matches = re.finditer(
                r'(?:/\*\*([^*]|(\*(?!/)))*?\*/|//.*?\n|/\*.*?\*/)?\s*'  # Optional comments
                r'(\w[\w\s\*&]+)\s+(\w+)\s*(?:=\s*([^;]+))?\s*;', 
                content
            )
            for match in var_matches:
                var_type = match.group(3).strip()
                var_name = match.group(4)
                default_value = match.group(5).strip() if match.group(5) else None
                var_doc = match.group(1).strip() if match.group(1) else ""
                var_data = {
                    'name': var_name,
                    'type': var_type,
                    'docstring': var_doc
                }
                if default_value:
                    var_data['default_value'] = default_value
                variables.append(var_data)
            
            # Extract structs with members
            structs = []
            struct_matches = re.finditer(
                r'(?:/\*\*([^*]|(\*(?!/)))*?\*/|//.*?\n|/\*.*?\*/)?\s*'  # Optional comments
                r'struct\s+(\w+)\s*{([^}]*)}', 
                content, re.DOTALL
            )
            for match in struct_matches:
                struct_name = match.group(3)
                struct_doc = match.group(1).strip() if match.group(1) else ""
                struct_body = match.group(4)
                
                # Extract struct members
                members = []
                member_matches = re.finditer(
                    r'(?:/\*\*([^*]|(\*(?!/)))*?\*/|//.*?\n|/\*.*?\*/)?\s*'  # Optional comments
                    r'(\w[\w\s\*&]+)\s+(\w+)\s*;', 
                    struct_body
                )
                for member_match in member_matches:
                    member_type = member_match.group(3).strip()
                    member_name = member_match.group(4)
                    member_doc = member_match.group(1).strip() if member_match.group(1) else ""
                    members.append({
                        'name': member_name,
                        'type': member_type,
                        'description': member_doc
                    })
                
                structs.append({
                    'name': struct_name,
                    'docstring': struct_doc,
                    'members': members
                })
            
            return {
                'namespace': namespace,
                'classes': classes,
                'functions': functions,
                'variables': variables,
                'structs': structs
            }
            
        except Exception as e:
            logger.error(f"CPP Parse Error: {file_path.name}")
            return None

    @staticmethod        
    def parse_python_file(file_path: Path) -> Optional[Dict]:
        """Parse Python file with docstrings"""
        try:
            content = file_path.read_text()
            
            # Parse classes
            class_pattern = re.compile(
                r'class\s+(\w+).*?:\s*'
                r'(""".*?""")',  # Docstring
                re.DOTALL
            )
            classes = [
                {
                    'name': match.group(1),
                    'docstring': match.group(2).strip('"')
                }
                for match in class_pattern.finditer(content)
            ]
            
            # Parse functions
            func_pattern = re.compile(
                r'def\s+(\w+)\s*\(([^)]*)\)\s*:\s*'
                r'(""".*?""")',  # Docstring
                re.DOTALL
            )
            functions = [
                {
                    'name': match.group(1),
                    'params': match.group(2).strip(),
                    'docstring': match.group(3).strip('"')
                }
                for match in func_pattern.finditer(content)
            ]
            
            # Parse module docstring
            module_doc = re.search(
                r'^(""".*?""")',  # Module docstring
                content,
                re.DOTALL
            )
            
            return {
                'module_doc': module_doc.group(1).strip('"') if module_doc else '',
                'classes': classes,
                'functions': functions
            }
            
        except Exception as e:
            logger.error(f"Python Parse Error: {file_path.name}")
            return None

class DocGenerator:
    """Handles documentation generation"""
    
    def __init__(self):
        self.converter = FormatConverter(ConversionConfig(
            preserve_links=True,
            output_dir=str(API_DOCS_DIR),
            temp_dir=str(BUILD_OUTPUT / "temp")
        ))
        
    def generate_markdown(self, doc_data: Dict, output_path: Path, source_file: Path) -> bool:
        """Generate markdown from parsed documentation"""
        try:
            markdown = []
            
            # Add classes section
            if doc_data.get('classes'):
                markdown.append("# Classes\n---\n")
                for cls in doc_data['classes']:
                    markdown.append(f"## {cls['name']}\n---\n")
                    if 'docstring' in cls:
                        markdown.append(f"{cls['docstring']}\n")
            
            # Add variables section
            if doc_data.get('variables'):
                markdown.append("\n# Variables\n---\n")
                for var in doc_data['variables']:
                    markdown.append(f"### `{var['name']}`\n")
                    markdown.append(f"- **Type:** `{var['type']}`\n")
                    if 'default_value' in var:
                        markdown.append(f"- **Default Value:** `{var['default_value']}`\n")
                    markdown.append("\n")
            
            # Add structs section
            if doc_data.get('structs'):
                markdown.append("\n# Structs\n---\n")
                for struct in doc_data['structs']:
                    markdown.append(f"### `{struct['name']}`\n")
                    if 'docstring' in struct:
                        markdown.append(f"- **Description:** {struct['docstring']}\n")
                    if 'members' in struct:
                        markdown.append("- **Members:**\n")
                        for member in struct['members']:
                            markdown.append(f"  - `{member['name']}`: `{member['type']}` - {member['description']}\n")
                    markdown.append("\n")
            
            # Add functions section
            if doc_data.get('functions'):
                markdown.append("\n# Functions\n---\n")
                for func in doc_data['functions']:
                    markdown.append(f"## {func['name']}\n")
                    if 'docstring' in func:
                        markdown.append(f"{func['docstring']}\n")
                    markdown.append(f"- **Parameters:** {func['params']}\n")
                    if 'return_type' in func:
                        markdown.append(f"- **Return:** `{func['return_type']}`\n")
                    markdown.append("---\n")
            
            # Create output directory if it doesn't exist
            output_path.mkdir(parents=True, exist_ok=True)
            
            # Write markdown file using source file name
            output_file = output_path / f"{source_file.stem}.md"
            output_file.write_text("\n".join(markdown))
            logger.info(f"Generated: {output_file}")
            return True
            
        except Exception as e:
            logger.error(f"Markdown Error: {output_path.name}")
            return False

def generate_api_docs():
    """Generate API reference documentation"""
    setup_logging()
    logger.info("Starting API docs generation")
    
    try:
        # Create output directories
        API_DOCS_DIR.mkdir(parents=True, exist_ok=True)
        BUILD_OUTPUT.mkdir(parents=True, exist_ok=True)
        
        # Load config
        config_path = ENGINE_ROOT / "Config" / "engine_config.yaml"
        with open(config_path, 'r', encoding='utf-8') as f:
            config = yaml.safe_load(f)
        
        # Initialize components
        parser = APIParser()
        generator = DocGenerator()
        
        # Process source files
        # Get all source files from Engine/Source directory
        source_files = []
        for root, dirs, files in os.walk(SOURCE_DIR):
            for file in files:
                if file.endswith(('.h', '.hpp', '.cpp', '.py')):
                    file_path = Path(root) / file
                    if file_path.exists():
                        source_files.append(file_path)
                    else:
                        logger.warning(f"Source file not found: {file_path}")
        
        logger.info(f"Processing {len(source_files)} files")
        
        success_count = 0
        for source_file in source_files:
            try:
                if not source_file.exists():
                    logger.error(f"Source file missing: {source_file}")
                    continue
                    
                # Parse file
                if source_file.suffix in ['.h', '.hpp', '.cpp']:
                    doc_data = parser.parse_cpp_file(source_file)
                else:
                    doc_data = parser.parse_python_file(source_file)
                
                if doc_data:
                    # Generate documentation
                    rel_path = source_file.relative_to(SOURCE_DIR)
                    output_dir = API_DOCS_DIR / rel_path.parent
                    
                    try:
                        output_dir.mkdir(parents=True, exist_ok=True)
                        if generator.generate_markdown(doc_data, output_dir, source_file):
                            success_count += 1
                    except OSError as e:
                        logger.error(f"Directory creation failed: {output_dir} - {str(e)}")
                        continue
                        
            except Exception as e:
                logger.error(f"Processing Error: {source_file.name} - {str(e)}")
                continue
                
        logger.info(f"Successfully processed {success_count}/{len(source_files)} files")
        return success_count == len(source_files)
        
    except Exception as e:
        logger.error(f"Generation Failed: {str(e)}")
        return False

if __name__ == "__main__":
    if generate_api_docs():
        logger.info("API docs generation completed successfully")
        sys.exit(0)
    else:
        logger.error("API docs generation completed with errors")
        sys.exit(1)
