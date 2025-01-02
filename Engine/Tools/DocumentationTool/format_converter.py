"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Converts documentation between different formats while maintaining structure and style.
"""

import os
import re
import logging
from pathlib import Path
from typing import Dict, List, Optional, Set, Union
from dataclasses import dataclass

logger = logging.getLogger(__name__)

# Get Engine root directory relative to this script
ENGINE_ROOT = Path(__file__).parent.parent.parent

@dataclass
class ConversionConfig:
    """Configuration for format conversion"""
    preserve_links: bool = True
    preserve_tables: bool = True
    preserve_images: bool = True
    preserve_code_blocks: bool = True
    preserve_interactive: bool = True
    parallel_conversion: bool = True
    max_workers: int = 4
    output_dir: str = str(ENGINE_ROOT / "BuildOutput/DocumentationTool/Converted")
    temp_dir: str = str(ENGINE_ROOT / "BuildOutput/DocumentationTool/temp")
    fallback_to_markdown: bool = True

class FormatConverter:
    """Handles conversion between documentation formats"""
    def __init__(self, config: ConversionConfig):
        self.config = config
        self._ensure_pandoc()
        
    def _ensure_pandoc(self):
        """Ensure pandoc is available"""
        try:
            import pypandoc
            self.pandoc = pypandoc
            try:
                pypandoc.get_pandoc_version()
            except OSError:
                logger.info("Pandoc not found. Attempting to download...")
                pypandoc.download_pandoc(delete_installer=True)
        except ImportError:
            logger.error("pypandoc not installed. Please install: pip install pypandoc")
            raise

    def convert_file(self, input_path: Union[str, Path], 
                    output_path: Union[str, Path],
                    from_format: str = 'markdown',
                    to_format: str = 'html5',
                    extra_args: List[str] = None) -> bool:
        """Convert a single file between formats"""
        try:
            input_path = Path(input_path)
            output_path = Path(output_path)
            
            if not input_path.exists():
                raise FileNotFoundError(f"Input file not found: {input_path}")
                
            # Create output directory if needed
            output_path.parent.mkdir(parents=True, exist_ok=True)
            
            # Ensure temp directory exists
            temp_dir = Path(self.config.temp_dir)
            temp_dir.mkdir(parents=True, exist_ok=True)
            
            # Convert file
            self.pandoc.convert_file(
                str(input_path),
                to_format,
                outputfile=str(output_path),
                format=from_format,
                extra_args=extra_args or []
            )
            
            logger.info(f"Converted {input_path} to {output_path}")
            return True
            
        except Exception as e:
            logger.error(f"Error converting {input_path}: {e}")
            if self.config.fallback_to_markdown and to_format != 'markdown':
                logger.info("Attempting fallback to markdown...")
                return self.convert_file(input_path, output_path, 
                                      from_format=from_format,
                                      to_format='markdown')
            raise 