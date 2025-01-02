"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Core documentation generation system that integrates all documentation components.
"""

import os
import sys
import logging
import yaml
from pathlib import Path
from typing import Dict, List, Optional, Set
from dataclasses import dataclass
from concurrent.futures import ThreadPoolExecutor

from Documentation.doc_tester import DocumentationTester, DocTestConfig
from Documentation.example_manager import CodeExampleManager, ExampleConfig
from Documentation.interactive_examples import InteractiveExampleGenerator, DOCKER_AVAILABLE
from Documentation.api_tracker import APIChangeTracker, APITrackerConfig

logger = logging.getLogger(__name__)

@dataclass
class GeneratorConfig:
    """Configuration for documentation generation"""
    source_dir: str = "Engine/Source"
    docs_dir: str = "Engine/Docs"
    output_dir: str = "Engine/Docs/Generated"
    temp_dir: str = "Engine/Docs/Temp"
    parallel_generation: bool = True
    max_workers: int = 4
    enable_validation: bool = True
    enable_examples: bool = True
    enable_interactive: bool = True
    enable_api_tracking: bool = True
    clean_output: bool = True

class DocumentationGenerator:
    def __init__(self, config: GeneratorConfig):
        self.config = config
        self._setup_components()
        
    def _setup_components(self):
        """Initialize documentation components"""
        try:
            if self.config.enable_interactive:
                self.interactive_generator = InteractiveExampleGenerator()
                if not DOCKER_AVAILABLE:
                    logger.warning("Docker not available - falling back to static examples")
                    self.config.enable_interactive = False
        except Exception as e:
            logger.warning(f"Failed to initialize interactive examples: {e}")
            self.config.enable_interactive = False

        if self.config.enable_api_tracking:
            try:
                api_config = APITrackerConfig(
                    repo_path=str(Path().absolute()),
                    track_changes=True,
                    generate_changelog=True
                )
                self.api_tracker = APIChangeTracker(api_config)
            except Exception as e:
                logger.warning(f"Failed to initialize API tracker: {e}")
                self.config.enable_api_tracking = False

    def _setup_logging(self) -> None:
        """Configure logging for documentation generation"""
        log_file = Path("Logs") / "doc_generation.log"
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
            os.makedirs(self.config.output_dir, exist_ok=True)
            os.makedirs(self.config.temp_dir, exist_ok=True)
            logger.info("Created necessary directories")
        except Exception as e:
            logger.error(f"Failed to create directories: {e}")
            raise

    def generate_documentation(self) -> bool:
        """Generate complete documentation"""
        try:
            logger.info("Starting documentation generation")

            # Validate documentation if enabled
            if self.config.enable_validation:
                self._validate_documentation()

            # Track API changes if enabled
            if self.config.enable_api_tracking:
                self._track_api_changes()

            # Generate core documentation
            self._generate_core_docs()

            # Generate examples if enabled
            if self.config.enable_examples:
                self._generate_examples()

            # Generate interactive content if enabled
            if self.config.enable_interactive:
                self._generate_interactive_content()

            # Finalize documentation
            self._finalize_documentation()

            logger.info("Documentation generation completed successfully")
            return True

        except Exception as e:
            logger.error(f"Documentation generation failed: {e}")
            return False

    def _validate_documentation(self) -> None:
        """Validate existing documentation"""
        logger.info("Validating documentation")
        
        # Check documentation coverage
        coverage = self.doc_tester.check_coverage(self.config.source_dir)
        if coverage.coverage_ratio < self.doc_tester.config.min_coverage:
            logger.warning(
                f"Documentation coverage ({coverage.coverage_ratio:.2%}) "
                f"below minimum threshold ({self.doc_tester.config.min_coverage:.2%})"
            )

        # Validate links
        self.doc_tester.validate_links(self.config.docs_dir)

        # Check spelling
        self.doc_tester.spell_check(self.config.docs_dir)

        # Verify style compliance
        self.doc_tester.style_check(self.config.docs_dir)

    def _track_api_changes(self) -> None:
        """Track and document API changes"""
        logger.info("Tracking API changes")
        
        # Detect changes
        changes = self.api_tracker.detect_changes()
        
        # Generate changelog
        if changes:
            changelog = self.api_tracker.generate_changelog()
            self._write_changelog(changelog)
        
        # Handle breaking changes
        breaking_changes = self.api_tracker.notify_breaking()
        if breaking_changes:
            logger.warning(f"Detected {len(breaking_changes)} breaking changes")

    def _generate_core_docs(self) -> None:
        """Generate core documentation"""
        logger.info("Generating core documentation")
        
        # Generate API documentation
        self._generate_api_docs()
        
        # Generate architecture documentation
        self._generate_architecture_docs()
        
        # Generate user guides
        self._generate_user_guides()

    def _generate_examples(self) -> None:
        """Generate code examples"""
        logger.info("Generating code examples")
        
        # Validate examples
        results = self.example_manager.validate_examples()
        
        # Update outdated examples
        self.example_manager.update_examples()
        
        # Generate example outputs
        outputs = self.example_manager.generate_output()
        
        # Include examples in documentation
        self._integrate_examples(outputs)

    def _generate_interactive_content(self) -> None:
        """Generate interactive documentation content"""
        logger.info("Generating interactive content")
        
        # Create interactive playgrounds
        self._create_playgrounds()
        
        # Generate visualization content
        self._create_visualizations()

    def _finalize_documentation(self) -> None:
        """Finalize documentation generation"""
        logger.info("Finalizing documentation")
        
        # Clean up temporary files
        if self.config.clean_output:
            self._cleanup_temp_files()
        
        # Generate search index
        self._generate_search_index()
        
        # Create documentation manifest
        self._create_manifest()

    def _write_changelog(self, changelog: str) -> None:
        """Write changelog to file"""
        try:
            changelog_path = Path(self.config.docs_dir) / "CHANGELOG.md"
            changelog_path.write_text(changelog)
            logger.info("Changelog updated successfully")
        except Exception as e:
            logger.error(f"Failed to write changelog: {e}")
            raise

    def _generate_api_docs(self) -> None:
        """Generate API documentation"""
        # Implementation for API documentation generation
        pass

    def _generate_architecture_docs(self) -> None:
        """Generate architecture documentation"""
        # Implementation for architecture documentation generation
        pass

    def _generate_user_guides(self) -> None:
        """Generate user guides"""
        # Implementation for user guide generation
        pass

    def _integrate_examples(self, outputs: Dict[str, str]) -> None:
        """Integrate examples into documentation"""
        # Implementation for example integration
        pass

    def _create_playgrounds(self) -> None:
        """Create interactive code playgrounds"""
        # Implementation for playground creation
        pass

    def _create_visualizations(self) -> None:
        """Create interactive visualizations"""
        # Implementation for visualization creation
        pass

    def _cleanup_temp_files(self) -> None:
        """Clean up temporary files"""
        # Implementation for cleanup
        pass

    def _generate_search_index(self) -> None:
        """Generate documentation search index"""
        # Implementation for search index generation
        pass

    def _create_manifest(self) -> None:
        """Create documentation manifest"""
        # Implementation for manifest creation
        pass

def main():
    """Main entry point for documentation generation"""
    try:
        generator = DocumentationGenerator()
        success = generator.generate_documentation()
        sys.exit(0 if success else 1)
    except Exception as e:
        logger.error(f"Documentation generation failed: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main() 