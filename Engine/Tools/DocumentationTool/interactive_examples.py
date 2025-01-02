"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Generates interactive code examples and documentation playgrounds.
"""

import os
import sys
import json
import logging
from pathlib import Path
from typing import Dict, List, Optional, Union
from dataclasses import dataclass
from concurrent.futures import ThreadPoolExecutor

logger = logging.getLogger(__name__)

try:
    from docker import DockerClient
    try:
        docker_client = DockerClient()
        DOCKER_AVAILABLE = True
    except Exception as e:
        logger.warning(f"Docker not available: {e}")
        DOCKER_AVAILABLE = False
except ImportError:
    DOCKER_AVAILABLE = False
    logger.warning("Docker SDK not available - interactive examples disabled")

from .template_manager import TemplateManager
from .example_manager import CodeExampleManager, ExampleConfig
from .style_enforcer import StyleEnforcer

@dataclass
class PlaygroundConfig:
    """Configuration for interactive playground"""
    max_memory: str = "512m"
    max_cpu: float = 1.0
    timeout: int = 30  # seconds
    enable_networking: bool = False
    allowed_imports: List[str] = None
    template_dir: str = "Templates/Playground"
    output_dir: str = "Generated/Interactive"
    parallel_execution: bool = True
    max_workers: int = 4
    fallback_to_static: bool = True  # New option

@dataclass
class InteractiveResult:
    """Results from interactive example generation"""
    success: bool
    output_url: Optional[str]
    container_id: Optional[str]
    errors: List[str]
    resources: Dict[str, float]

class InteractiveExampleGenerator:
    def __init__(self, config: PlaygroundConfig = None):
        self.config = config or PlaygroundConfig()
        self.docker_client = docker_client if DOCKER_AVAILABLE else None
        self._setup_logging()
        self.template_manager = TemplateManager()
        self.example_manager = CodeExampleManager(ExampleConfig())
        self.style_enforcer = StyleEnforcer()
        self._create_directories()

    def _setup_logging(self) -> None:
        """Configure logging for interactive examples"""
        log_file = Path("Logs") / "interactive_examples.log"
        os.makedirs(log_file.parent, exist_ok=True)
        
        file_handler = logging.FileHandler(log_file)
        file_handler.setFormatter(
            logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
        )
        logger.addHandler(file_handler)
        logger.setLevel(logging.INFO)

    def _create_directories(self) -> None:
        """Create necessary directories"""
        try:
            os.makedirs(self.config.template_dir, exist_ok=True)
            os.makedirs(self.config.output_dir, exist_ok=True)
            logger.info("Created interactive example directories")
        except Exception as e:
            logger.error(f"Failed to create directories: {e}")
            raise

    def create_playground(
        self,
        example_name: str,
        code: str,
        language: str
    ) -> InteractiveResult:
        """Creates an interactive playground for code example"""
        try:
            # Validate and format code
            style_fixes = self.style_enforcer.enforce_docstring_format(
                Path(f"temp_{example_name}.{language}")
            )
            if style_fixes:
                logger.warning(f"Style issues found in {example_name}")

            # Setup playground environment
            playground_path = self._setup_playground_files(example_name, code)
            
            # Create container configuration
            container_config = {
                "memory": self.config.max_memory,
                "cpu_quota": int(self.config.max_cpu * 100000),
                "network_disabled": not self.config.enable_networking
            }

            # Create and start container
            container_id = self._create_container(playground_path, container_config)
            
            # Generate interface
            interface_url = self._generate_interface(example_name, container_id)
            
            logger.info(f"Created playground for {example_name}")
            return InteractiveResult(
                True,
                interface_url,
                container_id,
                [],
                {"memory": 0, "cpu": 0}  # Updated by monitoring
            )

        except Exception as e:
            logger.error(f"Failed to create playground for {example_name}: {e}")
            return InteractiveResult(False, None, None, [str(e)], {})

    def cleanup_resources(self, container_id: Optional[str] = None) -> None:
        """Cleanup playground resources"""
        try:
            if container_id:
                container = self.docker_client.containers.get(container_id)
                container.stop()
                container.remove()
                logger.info(f"Cleaned up container: {container_id}")
            else:
                # Cleanup all stopped containers
                self.docker_client.containers.prune()
                logger.info("Cleaned up all stopped containers")

        except Exception as e:
            logger.error(f"Error cleaning up resources: {e}")
            raise

    def _setup_playground_files(self, example_name: str, code: str) -> Path:
        """Set up files needed for playground"""
        try:
            playground_path = Path(tempfile.mkdtemp(dir=self.config.output_dir))
            
            # Write code file
            code_file = playground_path / f"{example_name}.py"
            code_file.write_text(code)
            
            # Copy necessary templates
            for template in Path(self.config.template_dir).glob("*"):
                if template.is_file():
                    dest = playground_path / template.name
                    dest.write_bytes(template.read_bytes())
            
            return playground_path

        except Exception as e:
            logger.error(f"Error setting up playground files: {e}")
            raise

    def _create_container(self, playground_path: Path, config: Dict) -> Optional[str]:
        """Create Docker container for playground"""
        if not DOCKER_AVAILABLE:
            if self.config.fallback_to_static:
                logger.info("Docker not available - generating static example")
                return None
            else:
                raise RuntimeError("Docker required but not available")
                
        try:
            container = self.docker_client.containers.create(
                image="python:3.8-slim",
                command=f"python {playground_path.name}",
                mem_limit=self.config.max_memory,
                cpu_quota=int(self.config.max_cpu * 100000),
                network_disabled=not self.config.enable_networking,
                volumes={
                    str(playground_path.parent): {
                        'bind': '/playground',
                        'mode': 'ro'
                    }
                },
                working_dir='/playground'
            )
            return container.id
        except Exception as e:
            logger.error(f"Error creating container: {e}")
            return None

    def _generate_interface(self, example_name: str, container_id: str) -> str:
        """Generate web interface for playground"""
        try:
            template = self.template_manager.templates.get("playground_interface.j2")
            if not template:
                raise ValueError("Playground interface template not found")

            interface_path = Path(self.config.output_dir) / f"{example_name}.html"
            
            data = {
                "example_name": example_name,
                "container_id": container_id,
                "config": self.config.__dict__
            }
            
            self.template_manager.generate_from_template(
                "playground_interface.j2",
                data,
                interface_path
            )
            
            return f"/interactive/{example_name}.html"

        except Exception as e:
            logger.error(f"Error generating interface: {e}")
            raise 