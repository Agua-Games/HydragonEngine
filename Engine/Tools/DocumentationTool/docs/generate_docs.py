"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Documentation generation script for DocumentationTool package.
"""

from pathlib import Path
import logging
import sys
from Engine.Tools.DocumentationTool.doc_generator import DocumentationGenerator, GeneratorConfig

logger = logging.getLogger(__name__)

def setup_directories():
    """Create all necessary directories"""
    base_dir = Path("Engine/Tools/DocumentationTool")
    dirs = {
        "logs": base_dir / "logs",
        "docs": base_dir / "docs",
        "generated": base_dir / "docs/generated",
        "temp": base_dir / "docs/temp"
    }
    
    for name, path in dirs.items():
        try:
            path.mkdir(parents=True, exist_ok=True)
            logger.info(f"Created directory: {path}")
        except Exception as e:
            logger.error(f"Failed to create {name} directory at {path}: {e}")
            raise

def main():
    """Generate documentation for DocumentationTool"""
    try:
        # Setup logging first
        setup_directories()
        logging.basicConfig(
            level=logging.DEBUG,  # Changed to DEBUG for more detailed logging
            format='%(asctime)s - %(levelname)s - %(message)s',
            handlers=[
                logging.FileHandler("Engine/Tools/DocumentationTool/logs/doc_generation.log"),
                logging.StreamHandler(sys.stdout)  # Added console output
            ]
        )
        
        logger.info("Starting documentation generation process")
        
        config = GeneratorConfig(
            source_dir="Engine/Tools/DocumentationTool",
            docs_dir="Engine/Tools/DocumentationTool/docs",
            output_dir="Engine/Tools/DocumentationTool/docs/generated",
            temp_dir="Engine/Tools/DocumentationTool/docs/temp",
            enable_validation=True,
            enable_examples=True,
            enable_interactive=True,
            enable_api_tracking=True,
            clean_output=True
        )
        
        logger.info(f"Configuration initialized: {vars(config)}")
        
        generator = DocumentationGenerator(config)
        logger.info("Documentation generator initialized")
        
        success = generator.generate_documentation()
        
        if success:
            logger.info("Documentation generated successfully")
            return 0
        else:
            logger.error("Documentation generation failed")
            return 1
            
    except Exception as e:
        logger.error(f"Documentation generation failed: {e}", exc_info=True)  # Added exc_info
        return 1

if __name__ == "__main__":
    sys.exit(main()) 