"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Manages and validates code examples in documentation.
"""

import os
import logging
import subprocess
from pathlib import Path
from typing import Dict, List, Optional
from dataclasses import dataclass
from concurrent.futures import ThreadPoolExecutor

from .template_manager import TemplateManager
from .format_converter import FormatConverter
from .style_enforcer import StyleEnforcer

logger = logging.getLogger(__name__)

@dataclass
class ExampleConfig:
    """Configuration for code examples"""
    compile_timeout: int = 30  # seconds
    max_memory: int = 512  # MB
    enable_gpu: bool = False
    sandbox_dir: str = "Examples/Sandbox"
    output_dir: str = "Examples/Output"
    template_dir: str = "Examples/Templates"
    parallel_execution: bool = True
    max_workers: int = 4

@dataclass
class ExampleResult:
    """Results from example validation"""
    success: bool
    output: str
    errors: List[str]
    duration: float
    resources_used: Dict[str, float]

class CodeExampleManager:
    def __init__(self, config: ExampleConfig = ExampleConfig()):
        """Initialize the example manager"""
        self.config = config
        self._setup_logging()
        self.template_manager = TemplateManager()
        self.format_converter = FormatConverter()
        self.style_enforcer = StyleEnforcer()
        self.examples: Dict[str, Path] = {}
        self._create_directories()

    def _setup_logging(self) -> None:
        """Configure logging for example management"""
        log_file = Path("Logs") / "example_management.log"
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
            os.makedirs(self.config.sandbox_dir, exist_ok=True)
            os.makedirs(self.config.output_dir, exist_ok=True)
            os.makedirs(self.config.template_dir, exist_ok=True)
            logger.info("Created example directories")
        except Exception as e:
            logger.error(f"Failed to create directories: {e}")
            raise

    def update_examples(self) -> None:
        """Updates outdated examples to match current API"""
        with ThreadPoolExecutor(max_workers=self.config.max_workers) as executor:
            futures = []
            for name, path in self.examples.items():
                future = executor.submit(self._update_example, path)
                futures.append((name, future))

            for name, future in futures:
                try:
                    future.result()
                    logger.info(f"Updated example: {name}")
                except Exception as e:
                    logger.error(f"Error updating example {name}: {str(e)}")

    def generate_output(self) -> Dict[str, str]:
        """Generates example outputs for documentation"""
        outputs = {}
        with ThreadPoolExecutor(max_workers=self.config.max_workers) as executor:
            futures = {}
            for name, path in self.examples.items():
                futures[executor.submit(self._generate_example_output, path)] = name

            for future in futures:
                name = futures[future]
                try:
                    output = future.result()
                    outputs[name] = output
                    logger.info(f"Generated output for example: {name}")
                except Exception as e:
                    logger.error(f"Error generating output for {name}: {str(e)}")
                    outputs[name] = f"Error: {str(e)}"

        return outputs

    def validate_examples(self) -> Dict[str, ExampleResult]:
        """Validates all examples"""
        results = {}
        with ThreadPoolExecutor(max_workers=self.config.max_workers) as executor:
            futures = {}
            for name, path in self.examples.items():
                futures[executor.submit(self._validate_example, path)] = name

            for future in futures:
                name = futures[future]
                try:
                    result = future.result()
                    results[name] = result
                    if not result.success:
                        logger.warning(f"Example validation failed: {name}")
                except Exception as e:
                    logger.error(f"Error validating example {name}: {str(e)}")
                    results[name] = ExampleResult(False, "", [str(e)], 0.0, {})

        return results

    def _validate_example(self, path: Path) -> ExampleResult:
        """Validate a single example"""
        try:
            # Check style compliance
            style_fixes = self.style_enforcer.enforce_docstring_format(path)
            if style_fixes:
                return ExampleResult(
                    False,
                    "",
                    [f"Style violation: {fix.rule}" for fix in style_fixes],
                    0.0,
                    {}
                )

            # Compile and run example
            compile_result = self._compile_example(path)
            if not compile_result.success:
                return compile_result

            run_result = self._run_example(path)
            return run_result

        except Exception as e:
            logger.error(f"Error validating example {path}: {e}")
            raise

    def _compile_example(self, path: Path) -> ExampleResult:
        """Compile a single example"""
        # Implementation depends on language and build system
        pass

    def _run_example(self, path: Path) -> ExampleResult:
        """Run a single example"""
        # Implementation depends on runtime environment
        pass

    def _update_example(self, path: Path) -> None:
        """Update a single example to match current API"""
        # Implementation depends on API version tracking
        pass

    def _generate_example_output(self, path: Path) -> str:
        """Generate output for a single example"""
        # Implementation depends on example type
        pass 