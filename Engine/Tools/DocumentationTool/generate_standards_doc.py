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
    """Get variables for template substitution"""
    try:
        # Load engine config
        config_path = ENGINE_ROOT / "Config" / "engine_config.yaml"
        with open(config_path, 'r', encoding='utf-8') as f:
            config = yaml.safe_load(f)
            
        template_vars = {
            'current_year': str(datetime.now().year),
            'company_name': config["engine"]["company"],
            'engine_name': config["engine"]["name"],
            'engine_version': config["engine"]["version"],
            'tool_name': config["documentation"]["tool_name"],
            'tool_version': config["documentation"]["tool_version"],
            'website_name': config["documentation"]["website_name"],
            'website_url': config["documentation"]["website_url"]
        }
        
        logger.debug(f"Template variables: {template_vars}")
        return template_vars
        
    except Exception as e:
        logger.error(f"Error loading template variables: {e}")
        raise

def generate_markdown(standards: dict) -> str:
    """Generate markdown content from parsed standards"""
    try:
        markdown = []
        
        # Add title
        markdown.append("# Hydragon Engine Coding Standards\n")
        markdown.append("This document outlines the coding standards for all development in the Hydragon Engine.\n")
        
        # Add general standards
        markdown.append("## General Standards\n")
        if 'general' in standards:
            for item in standards['general'].split('\n'):
                if item.strip():
                    markdown.append(f"- {item.strip()}")
            markdown.append("\n")
            
        # Add language-specific standards
        markdown.append("## Language-Specific Standards\n")
        
        # C++ Standards
        if 'cpp' in standards:
            markdown.append("### C++ Standards\n")
            for item in standards['cpp'].split('\n'):
                if item.strip():
                    markdown.append(f"- {item.strip()}")
            markdown.append("\n")
            
        # Python Standards
        if 'python' in standards:
            markdown.append("### Python Standards\n")
            for item in standards['python'].split('\n'):
                if item.strip():
                    markdown.append(f"- {item.strip()}")
            markdown.append("\n")
            
        # Add implementation notes
        markdown.append("## Implementation Notes\n")
        markdown.extend([
            "- These standards are enforced through automated tooling in the CI/CD pipeline",
            "- Documentation coverage is automatically checked and must maintain >80% coverage",
            "- Style violations will trigger build warnings",
            "- Breaking changes to these standards require team review\n"
        ])
        
        logger.debug("Generated markdown content successfully")
        return "\n".join(markdown)
        
    except Exception as e:
        logger.error(f"Error generating markdown: {e}")
        raise

def generate_standards_doc():
    """Generate coding standards documentation"""
    try:
        setup_logging()
        logger.info("Generating coding standards documentation...")
        
        # Create output directories
        STANDARDS_DIR.mkdir(parents=True, exist_ok=True)
        BUILD_OUTPUT.mkdir(parents=True, exist_ok=True)
        
        # Initialize the converter
        converter = FormatConverter(ConversionConfig(
            preserve_links=True,
            output_dir=str(STANDARDS_DIR),
            temp_dir=str(BUILD_OUTPUT / "temp")
        ))
        
        # Parse standards from header file
        standards = parse_standards(STANDARDS_FILE)
        
        # Generate markdown
        md_path = STANDARDS_DIR / "coding_standards.md"
        logger.info(f"Generating markdown: {md_path}")
        
        with open(md_path, 'w', encoding='utf-8') as f:
            f.write(generate_markdown(standards))
            
        # Load config for template variables
        config_path = ENGINE_ROOT / "Config" / "engine_config.yaml"
        with open(config_path, 'r', encoding='utf-8') as f:
            config = yaml.safe_load(f)
            
        # Convert to HTML using pandoc
        css_path = TEMPLATES_DIR / "style.css"
        html_path = STANDARDS_DIR / "coding_standards.html"
        logger.info(f"Converting to HTML: {html_path}")
        
        # Add all required metadata
        extra_args = [
            '--metadata', 'title=Hydragon Engine Coding Standards',
            '--metadata', f'current_year={datetime.now().year}',
            '--metadata', f'company_name={config["engine"]["company"]}',
            '--metadata', f'engine_name={config["engine"]["name"]}',
            '--metadata', f'engine_version={config["engine"]["version"]}',
            '--metadata', f'tool_name={config["documentation"]["tool_name"]}',
            '--metadata', f'tool_version={config["documentation"]["tool_version"]}',
            '--metadata', f'website_name={config["documentation"]["website_name"]}',
            '--metadata', f'website_url={config["documentation"]["website_url"]}',
            '--css', str(css_path),
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
        
        logger.info("\nDocumentation generated successfully!")
        logger.info(f"Generated files in {STANDARDS_DIR}:")
        logger.info(f"- Markdown: {md_path}")
        logger.info(f"- HTML: {html_path}")
        
    except Exception as e:
        logger.error(f"Error generating documentation: {e}")
        raise

if __name__ == "__main__":
    generate_standards_doc() 