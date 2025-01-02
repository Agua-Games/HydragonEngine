"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Manages templates for documentation generation.
"""

import logging
from pathlib import Path
from dataclasses import dataclass
from jinja2 import Environment, FileSystemLoader, select_autoescape

logger = logging.getLogger(__name__)

@dataclass
class TemplateConfig:
    """Configuration for template rendering"""
    template_dir: str
    output_dir: str
    auto_escape: bool = True
    trim_blocks: bool = True
    lstrip_blocks: bool = True

class TemplateManager:
    """Handles template loading and rendering"""
    def __init__(self, config: TemplateConfig):
        self.config = config
        self.env = Environment(
            loader=FileSystemLoader(config.template_dir),
            autoescape=select_autoescape(['html', 'xml']) if config.auto_escape else [],
            trim_blocks=config.trim_blocks,
            lstrip_blocks=config.lstrip_blocks
        )
        
    def render_template(self, template_name: str, **kwargs) -> str:
        """Render a template with given context"""
        try:
            template = self.env.get_template(template_name)
            return template.render(**kwargs)
        except Exception as e:
            logger.error(f"Error rendering template {template_name}: {e}")
            raise 