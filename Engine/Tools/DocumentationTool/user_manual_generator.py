"""
User manual documentation generator implementation
"""
import re
from pathlib import Path
from typing import Dict, Optional
from .generate_documentation import DocumentationGenerator

class UserManualGenerator(DocumentationGenerator):
    """Generates user manual documentation from source files"""
    
    def __init__(self, config_path: Path):
        super().__init__(config_path)
        self.source_extensions = ['.md', '.txt']
        
    def parse_doc(self, source_path: Path) -> Optional[Dict]:
        """Parse user manual documentation from source file"""
        try:
            content = source_path.read_text(encoding='utf-8')
            self.logger.info(f"Parsing user manual documentation from: {source_path.name}")
            
            return {
                'title': self._extract_title(content),
                'chapters': self._parse_chapters(content),
                'content': content
            }
        except Exception as e:
            self.logger.error(f"Failed to parse {source_path}: {str(e)}")
            return None
            
    def _extract_title(self, content: str) -> str:
        """Extract title from markdown content"""
        title_match = re.search(r'^#\s+(.*)', content, re.MULTILINE)
        return title_match.group(1).strip() if title_match else "User Manual"
        
    def _parse_chapters(self, content: str) -> Dict[str, str]:
        """Parse markdown chapters into dictionary"""
        chapters = {}
        chapter_matches = re.finditer(r'^#\s+(.*?)\n(.*?)(?=^#|\Z)', content, re.DOTALL | re.MULTILINE)
        
        for match in chapter_matches:
            chapters[match.group(1).strip()] = match.group(2).strip()
            
        return chapters
        
    def generate_markdown(self, 
                         source_path: Path, 
                         output_path: Path) -> bool:
        """Generate user manual markdown documentation"""
        try:
            doc_data = self.parse_doc(source_path)
            if not doc_data:
                return False
                
            markdown = []
            
            # Add title
            markdown.append(f"# {doc_data['title']}\n")
            
            # Add chapters
            for chapter, content in doc_data['chapters'].items():
                markdown.append(f"# {chapter}\n")
                markdown.append(f"{content}\n")
                
            # Write markdown file
            output_path.write_text("\n".join(markdown))
            return True
            
        except Exception as e:
            self.logger.error(f"Failed to generate markdown: {str(e)}")
            return False
            
    def setup_doc_user_manual(self, **kwargs) -> None:
        """Setup user manual documentation generation"""
        self.logger.info("Setting up user manual documentation")
        # Additional user manual-specific setup can be added here
