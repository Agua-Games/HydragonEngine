"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Generates coding standards documentation.
"""

import sys
import logging
from pathlib import Path
import subprocess
import re

# Get Engine root directory relative to this script
ENGINE_ROOT = Path(__file__).parent.parent.parent
TOOL_ROOT = Path(__file__).parent

# Import local modules
from template_manager import TemplateManager, TemplateConfig
from format_converter import FormatConverter, ConversionConfig

# Standardize paths
DOCS_ROOT = ENGINE_ROOT / "Docs"
STANDARDS_DIR = DOCS_ROOT / "DeveloperGuide" / "Standards"
BUILD_OUTPUT = ENGINE_ROOT / "BuildOutput" / "DocumentationTool"
LOGS_DIR = ENGINE_ROOT / "Shared" / "Logs" / "DocumentationTool"
TEMPLATES_DIR = TOOL_ROOT / "Templates"

# Configure logging first
logger = logging.getLogger(__name__)

def setup_logging():
    """Configure logging for documentation generation"""
    LOGS_DIR.mkdir(parents=True, exist_ok=True)
    log_file = LOGS_DIR / "standards_doc.log"
    
    file_handler = logging.FileHandler(log_file)
    file_handler.setFormatter(
        logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
    )
    logger.addHandler(file_handler)
    
    # Also log to console
    console_handler = logging.StreamHandler()
    console_handler.setFormatter(
        logging.Formatter('%(levelname)s: %(message)s')
    )
    logger.addHandler(console_handler)
    
    logger.setLevel(logging.INFO)

def parse_standards(file_path: Path) -> dict:
    """Parse coding standards from header file"""
    try:
        content = file_path.read_text()
        
        # Extract sections using regex
        general_pattern = r"FOR ANY CODE:\s*\n(.*?)(?=\n\w+\s*SPECIFIC:|\*/)"
        cpp_pattern = r"C\+\+ SPECIFIC:\s*\n(.*?)(?=\n\w+\s*SPECIFIC:|\*/)"
        python_pattern = r"PYTHON SPECIFIC:\s*\n(.*?)(?=\n\w+\s*SPECIFIC:|\*/)"
        
        def extract_standards(pattern, text):
            match = re.search(pattern, text, re.DOTALL)
            if match:
                return [line.strip('- ').strip() for line in match.group(1).strip().split('\n') if line.strip()]
            return []
        
        return {
            'general_standards': extract_standards(general_pattern, content),
            'cpp_standards': extract_standards(cpp_pattern, content),
            'python_standards': extract_standards(python_pattern, content)
        }
        
    except Exception as e:
        logger.error(f"Error parsing standards file: {e}")
        raise

def get_engine_version() -> str:
    """Get the current Hydragon Engine version"""
    try:
        # For now, return a default version
        # TODO: Implement proper version detection from Engine's version file
        return "0.1.0-alpha"
    except Exception as e:
        logger.warning(f"Could not determine engine version: {e}")
        return "development"

def generate_standards_doc():
    """Generate coding standards documentation"""
    try:
        # Setup logging first
        setup_logging()
        logger.info("Starting standards documentation generation...")
        
        # Ensure directories exist
        STANDARDS_DIR.mkdir(parents=True, exist_ok=True)
        BUILD_OUTPUT.mkdir(parents=True, exist_ok=True)

        # Parse standards
        standards_file = DOCS_ROOT / "DeveloperGuide" / "RoadmapChat" / "CodingStandards.h"
        if not standards_file.exists():
            raise FileNotFoundError(f"Standards file not found at: {standards_file}")
            
        standards_data = parse_standards(standards_file)
        
        # Generate markdown
        template_mgr = TemplateManager(TemplateConfig(
            template_dir=str(TEMPLATES_DIR),
            output_dir=str(STANDARDS_DIR)
        ))
        
        md_path = STANDARDS_DIR / "coding_standards.md"
        md_content = template_mgr.render_template(
            "coding_standards.j2",
            **standards_data
        )
        
        logger.info(f"Writing markdown to {md_path}")
        md_path.write_text(md_content)
        
        # Convert to HTML
        converter = FormatConverter(ConversionConfig(
            preserve_code_blocks=True,
            preserve_links=True,
            output_dir=str(STANDARDS_DIR),
            temp_dir=str(BUILD_OUTPUT / "temp")
        ))
        
        html_path = STANDARDS_DIR / "coding_standards.html"
        logger.info(f"Converting to HTML: {html_path}")
        
        converter.convert_file(
            md_path,
            html_path,
            from_format='markdown',
            to_format='html5',
            extra_args=[
                '--metadata', 'title="Hydragon Engine Coding Standards"',
                '--metadata', f'version="{get_engine_version()}"',
                '--metadata', 'language="C++"',
                '--css', str(TEMPLATES_DIR / 'style.css'),
                '--template', str(TEMPLATES_DIR / 'html5_template.html'),
                '--toc',
                '--toc-depth=3',
                '--section-divs',
                '--highlight-style=pygments',
                '--no-highlight',
                '--lua-filter', str(TOOL_ROOT / 'filters' / 'remove_first_heading.lua')
            ]
        )
        
        logger.info("\nDocumentation generated successfully!")
        logger.info(f"Generated files in {STANDARDS_DIR}:")
        logger.info(f"- Markdown: {md_path}")
        logger.info(f"- HTML: {html_path}")
        
    except Exception as e:
        logger.error(f"Error generating documentation: {e}")
        raise

if __name__ == "__main__":
    generate_standards_doc() 