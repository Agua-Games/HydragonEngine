"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Generates API Reference documentation by scanning Engine/Source/ directory.
"""

import sys
import logging
from pathlib import Path
import subprocess
import re
from datetime import datetime
import yaml

# Use standard imports for local modules
import version
from template_manager import TemplateManager, TemplateConfig
from format_converter import FormatConverter, ConversionConfig
sys.path.append(str(Path(__file__).parent.parent))
from Common.config_manager import EngineConfig

# Setup paths relative to this script
SCRIPT_DIR = Path(__file__).parent
ENGINE_ROOT = SCRIPT_DIR.parent.parent
SOURCE_DIR = ENGINE_ROOT / "Source"
API_DOCS_DIR = ENGINE_ROOT / "Docs" / "APIReference"
BUILD_OUTPUT = ENGINE_ROOT / "BuildOutput" / "DocumentationTool"
LOGS_DIR = ENGINE_ROOT / "Shared" / "Logs" / "DocumentationTool"
TEMPLATES_DIR = SCRIPT_DIR / "templates"

# Setup logging
logger = logging.getLogger(__name__)

def setup_logging():
    """Configure logging for API documentation generation"""
    LOGS_DIR.mkdir(parents=True, exist_ok=True)
    log_file = LOGS_DIR / "api_docs.log"
    
    file_handler = logging.FileHandler(log_file)
    file_handler.setFormatter(
        logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
    )
    logger.addHandler(file_handler)
    
    console_handler = logging.StreamHandler()
    console_handler.setFormatter(
        logging.Formatter('%(levelname)s: %(message)s')
    )
    logger.addHandler(console_handler)
    
    logger.setLevel(logging.INFO)

def scan_source_files(directory: Path) -> list:
    """Recursively scan source directory for C++ and Python files"""
    source_files = []
    for path in directory.rglob('*'):
        if path.is_file() and path.suffix in ['.h', '.hpp', '.cpp', '.py']:
            source_files.append(path)
    return source_files

def parse_cpp_file(file_path: Path) -> dict:
    """Parse C++ source file for documentation"""
    try:
        content = file_path.read_text()
        # Extract documentation comments, class definitions, etc.
        # TODO: Implement proper C++ parsing
        return {
            'path': file_path,
            'content': content
            # Add parsed documentation data
        }
    except Exception as e:
        logger.error(f"Error parsing {file_path}: {e}")
        return None

def generate_api_docs():
    """Generate API reference documentation"""
    try:
        setup_logging()
        logger.info("Generating API reference documentation...")
        
        # Create output directories
        API_DOCS_DIR.mkdir(parents=True, exist_ok=True)
        BUILD_OUTPUT.mkdir(parents=True, exist_ok=True)
        
        # Initialize converter
        converter = FormatConverter(ConversionConfig(
            preserve_links=True,
            output_dir=str(API_DOCS_DIR),
            temp_dir=str(BUILD_OUTPUT / "temp")
        ))
        
        # Scan source files
        source_files = scan_source_files(SOURCE_DIR)
        logger.info(f"Found {len(source_files)} source files")
        
        # Load config for template variables
        config_path = ENGINE_ROOT / "Config" / "engine_config.yaml"
        with open(config_path, 'r', encoding='utf-8') as f:
            config = yaml.safe_load(f)
        
        # Process each source file
        for source_file in source_files:
            try:
                rel_path = source_file.relative_to(SOURCE_DIR)
                output_dir = API_DOCS_DIR / rel_path.parent
                output_dir.mkdir(parents=True, exist_ok=True)
                
                # Parse source file
                if source_file.suffix in ['.h', '.hpp', '.cpp']:
                    doc_data = parse_cpp_file(source_file)
                # TODO: Add Python file parsing
                
                if doc_data:
                    # Generate markdown
                    md_path = output_dir / f"{source_file.stem}.md"
                    html_path = output_dir / f"{source_file.stem}.html"
                    
                    # TODO: Generate markdown content from doc_data
                    
                    # Convert to HTML
                    extra_args = [
                        '--metadata', f'title={rel_path}',
                        '--metadata', f'current_year={datetime.now().year}',
                        '--metadata', f'company_name={config["engine"]["company"]}',
                        '--metadata', f'engine_name={config["engine"]["name"]}',
                        '--metadata', f'engine_version={config["engine"]["version"]}',
                        '--metadata', f'tool_name={config["documentation"]["tool_name"]}',
                        '--metadata', f'tool_version={config["documentation"]["tool_version"]}',
                        '--metadata', f'website_name={config["documentation"]["website_name"]}',
                        '--metadata', f'website_url={config["documentation"]["website_url"]}',
                        '--css', str(TEMPLATES_DIR / "style.css"),
                        '--standalone',
                        '--template', str(TEMPLATES_DIR / "html5_template.html"),
                        '--toc',
                        '--toc-depth=3'
                    ]
                    
                    converter.convert_file(
                        md_path,
                        html_path,
                        from_format='markdown',
                        to_format='html5',
                        extra_args=extra_args
                    )
                    
            except Exception as e:
                logger.error(f"Error processing {source_file}: {e}")
                continue
        
        logger.info("\nAPI documentation generated successfully!")
        
    except Exception as e:
        logger.error(f"Error generating API documentation: {e}")
        raise

if __name__ == "__main__":
    generate_api_docs() 