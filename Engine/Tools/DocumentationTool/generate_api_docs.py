"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Generates API Reference documentation by scanning Engine/Source/ directory.
"""
import os
import sys
import logging
from pathlib import Path
import yaml
from typing import List

# Local imports
from api_reference_generator import APIReferenceGenerator
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
    """Configure logging for API docs with both file and console output"""
    LOGS_DIR.mkdir(parents=True, exist_ok=True)
    log_file = LOGS_DIR / "api_docs.log"
    
    # Clear existing log file
    if log_file.exists():
        log_file.unlink()
    
    # File handler
    file_handler = logging.FileHandler(log_file)
    file_handler.setFormatter(logging.Formatter('%(asctime)s - %(levelname)s - %(message)s'))
    
    # Console handler
    console_handler = logging.StreamHandler()
    console_handler.setFormatter(logging.Formatter('%(levelname)s: %(message)s'))
    
    # Add both handlers
    logger.addHandler(file_handler)
    logger.addHandler(console_handler)
    
    logger.setLevel(logging.INFO)

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
        
        # Initialize API reference generator
        generator = APIReferenceGenerator(config_path)
        
        # Process source files
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
                    
                # Generate documentation
                rel_path = source_file.relative_to(SOURCE_DIR)
                output_dir = API_DOCS_DIR / rel_path.parent
                
                try:
                    output_dir.mkdir(parents=True, exist_ok=True)
                    if generator.generate_documentation(source_file, output_dir):
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
