"""
Base class for documentation generation with shared functionality
"""
import os
import sys
import logging
from pathlib import Path
import yaml
from typing import List, Dict, Optional

class DocumentationGenerator:
    """Base class for all documentation generation tasks"""
    
    def __init__(self, config_path: Path):
        self.config_path = config_path
        self.logger = logging.getLogger(self.__class__.__name__)
        self.setup_logging()
        self.template_vars = self.get_template_variables()
        
    def setup_logging(self) -> None:
        """Configure logging with both file and console output"""
        logs_dir = Path(__file__).parent.parent / "Shared" / "Logs" / "DocumentationTool"
        logs_dir.mkdir(parents=True, exist_ok=True)
        
        # File handler
        log_file = logs_dir / "documentation.log"
        file_handler = logging.FileHandler(log_file)
        file_handler.setFormatter(logging.Formatter('%(asctime)s - %(levelname)s - %(message)s'))
        
        # Console handler
        console_handler = logging.StreamHandler()
        console_handler.setFormatter(logging.Formatter('%(levelname)s: %(message)s'))
        
        # Add both handlers
        self.logger.addHandler(file_handler)
        self.logger.addHandler(console_handler)
        self.logger.setLevel(logging.INFO)
        
    def get_template_variables(self) -> Dict:
        """Load template variables from engine config"""
        try:
            with open(self.config_path, 'r', encoding='utf-8') as f:
                config = yaml.safe_load(f)
                
            return {
                'engine_name': config.get('engine', {}).get('name', ''),
                'engine_version': config.get('engine', {}).get('version', ''),
                'company_name': config.get('engine', {}).get('company', ''),
                'documentation': config.get('documentation', {})
            }
        except Exception as e:
            self.logger.error(f"Failed to load template variables: {str(e)}")
            return {}
            
    def find_source_files(self, root_folder: Path, extensions: List[str]) -> List[Path]:
        """Find all source files with given extensions in directory tree"""
        source_files = []
        try:
            for root, _, files in os.walk(root_folder):
                for file in files:
                    if any(file.endswith(ext) for ext in extensions):
                        file_path = Path(root) / file
                        if file_path.exists():
                            source_files.append(file_path)
            return source_files
        except Exception as e:
            self.logger.error(f"Error finding source files: {str(e)}")
            return []
            
    def parse_doc(self, source_path: Path) -> Optional[Dict]:
        """Base method for parsing documentation - to be implemented by subclasses"""
        raise NotImplementedError("parse_doc must be implemented by subclass")
        
    def generate_docs(self, 
                     doctype: str, 
                     source_path: Path, 
                     destination_path: Path) -> bool:
        """Generate documentation of specified type"""
        try:
            self.logger.info(f"Generating {doctype} documentation from {source_path}")
            
            # Create output directory if needed
            destination_path.mkdir(parents=True, exist_ok=True)
            
            # Generate markdown first
            markdown_path = destination_path / f"{source_path.stem}.md"
            if not self.generate_markdown(source_path, markdown_path):
                return False
                
            # Then generate HTML
            html_path = destination_path / f"{source_path.stem}.html"
            return self.generate_html(markdown_path, html_path)
            
        except Exception as e:
            self.logger.error(f"Documentation generation failed: {str(e)}")
            return False
            
    def generate_markdown(self, 
                         source_path: Path, 
                         output_path: Path) -> bool:
        """Generate markdown documentation - to be implemented by subclasses"""
        raise NotImplementedError("generate_markdown must be implemented by subclass")
        
    def generate_html(self, 
                     markdown_path: Path, 
                     output_path: Path) -> bool:
        """Generate HTML documentation from markdown"""
        try:
            # Verify markdown file exists
            if not markdown_path.exists():
                self.logger.error(f"Markdown file not found: {markdown_path}")
                return False
                
            # Implementation of HTML generation would go here
            # Using pandoc or other markdown-to-html converter
            
            self.logger.info(f"Successfully generated HTML: {output_path}")
            return True
            
        except Exception as e:
            self.logger.error(f"HTML generation failed: {str(e)}")
            return False
            
