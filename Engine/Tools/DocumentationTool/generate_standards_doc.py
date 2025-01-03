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
from datetime import datetime

# Use standard imports for local modules
import version
from template_manager import TemplateManager, TemplateConfig
from format_converter import FormatConverter, ConversionConfig
sys.path.append(str(Path(__file__).parent.parent))
from Common.config_manager import EngineConfig

# Setup paths relative to this script
SCRIPT_DIR = Path(__file__).parent
ENGINE_ROOT = SCRIPT_DIR.parent.parent
STANDARDS_FILE = ENGINE_ROOT / "Docs" / "DeveloperGuide" / "RoadmapChat" / "CodingStandards.h"
STANDARDS_DIR = ENGINE_ROOT / "Docs" / "DeveloperGuide" / "Standards"
BUILD_OUTPUT = ENGINE_ROOT / "BuildOutput" / "DocumentationTool"
LOGS_DIR = ENGINE_ROOT / "Shared" / "Logs" / "DocumentationTool"
TEMPLATES_DIR = SCRIPT_DIR / "templates"

# Setup logging
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

def get_template_variables():
    """Get variables for template rendering"""
    config = EngineConfig()
    return {
        'website_url': config.website_url,
        'website_name': config.website_name,
        'company_name': config.company_name,
        'engine_name': config.engine_name,
        'engine_version': config.engine_version,
        'tool_name': config.tool_name,
        'tool_version': version.__version__,
        'current_year': config.current_year
    }

def generate_standards_doc():
    """Generate coding standards documentation"""
    try:
        setup_logging()
        logger.info("Generating coding standards documentation...")
        
        # Verify standards file exists
        if not STANDARDS_FILE.exists():
            logger.error(f"Standards file not found at: {STANDARDS_FILE}")
            logger.info("Expected location: Engine/Docs/DeveloperGuide/RoadmapChat/CodingStandards.h")
            raise FileNotFoundError(f"Standards file not found at: {STANDARDS_FILE}")

        # Parse standards from header file
        standards = parse_standards(STANDARDS_FILE)
        
        # Generate markdown
        template_mgr = TemplateManager(TemplateConfig(
            template_dir=str(TEMPLATES_DIR),
            output_dir=str(STANDARDS_DIR)
        ))
        
        md_path = STANDARDS_DIR / "coding_standards.md"
        md_content = template_mgr.render_template(
            "coding_standards.j2",
            **standards
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
        
        # Get the title from the markdown file or use a default
        title = "Hydragon Engine Coding Standards"  # Default title
        
        # Get template variables
        template_vars = get_template_variables()
        
        # Add template variables to pandoc args
        extra_args = [
            '--metadata', f'title="{title}"',
            '--metadata', f'version="{get_engine_version()}"',
            '--metadata', f'language="C++"',
            '--css', str(TEMPLATES_DIR / 'style.css'),
            '--template', str(TEMPLATES_DIR / 'html5_template.html'),
            '--toc',
            '--toc-depth=3',
            '--section-divs',
            '--highlight-style=pygments',
            '--no-highlight',
            '--lua-filter', str(SCRIPT_DIR / 'filters' / 'remove_first_heading.lua')
        ]

        # Add all template variables as metadata
        for key, value in template_vars.items():
            extra_args.extend(['--metadata', f'{key}="{value}"'])

        converter.convert_file(
            md_path,
            html_path,
            from_format='markdown',
            to_format='html5',
            extra_args=extra_args
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