"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Tracks API changes and maintains changelog.
"""

import os
import re
import logging
import git
from pathlib import Path
from typing import Dict, List, Set, Optional, Tuple
from dataclasses import dataclass
from datetime import datetime
from concurrent.futures import ThreadPoolExecutor

from .template_manager import TemplateManager
from .format_converter import FormatConverter

logger = logging.getLogger(__name__)

@dataclass
class APIChange:
    """Represents a change in the API"""
    type: str  # 'added', 'removed', 'modified'
    component: str
    description: str
    breaking: bool
    commit_hash: str
    author: str
    date: datetime
    affected_files: List[str]
    version_impact: str  # 'major', 'minor', 'patch'

@dataclass
class APITrackerConfig:
    """Configuration for API tracking"""
    repo_path: str = "."
    changelog_path: str = "Docs/Changelog.md"
    api_cache_dir: str = ".api_cache"
    parallel_analysis: bool = True
    max_workers: int = 4
    notify_on_breaking: bool = True
    version_file: str = "version.txt"

class APIChangeTracker:
    def __init__(self, config: APITrackerConfig = APITrackerConfig()):
        """Initialize the API change tracker"""
        self.config = config
        self._setup_logging()
        self.template_manager = TemplateManager()
        self.format_converter = FormatConverter()
        self.repo = git.Repo(self.config.repo_path)
        self.changes: List[APIChange] = []
        self._create_directories()

    def _setup_logging(self) -> None:
        """Configure logging for API tracking"""
        log_file = Path("Logs") / "api_changes.log"
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
            os.makedirs(Path(self.config.api_cache_dir), exist_ok=True)
            logger.info("Created API tracking directories")
        except Exception as e:
            logger.error(f"Failed to create directories: {e}")
            raise

    def track_changes(self) -> List[APIChange]:
        """Track API changes between versions"""
        try:
            current_api = self._analyze_current_api()
            previous_api = self._load_previous_api()
            
            changes = self._compare_apis(previous_api, current_api)
            self.changes.extend(changes)
            
            if changes:
                self._update_changelog(changes)
                if any(change.breaking for change in changes):
                    self._send_notifications(changes)
                
                new_version = self._calculate_new_version(self._get_current_version())
                self._update_version(new_version)
            
            return changes

        except Exception as e:
            logger.error(f"Error tracking API changes: {e}")
            raise

    def _analyze_current_api(self) -> Dict:
        """Analyze current API state"""
        try:
            api_state = {}
            source_files = list(Path(self.config.repo_path).rglob("*.h"))
            
            with ThreadPoolExecutor(max_workers=self.config.max_workers) as executor:
                futures = {executor.submit(self._analyze_file, file): file 
                          for file in source_files}
                
                for future in futures:
                    file = futures[future]
                    try:
                        api_state[str(file)] = future.result()
                    except Exception as e:
                        logger.error(f"Error analyzing {file}: {e}")
            
            return api_state

        except Exception as e:
            logger.error(f"Error analyzing current API: {e}")
            raise

    def _update_changelog(self, changes: List[APIChange]) -> None:
        """Update changelog with new changes"""
        try:
            changelog_path = Path(self.config.changelog_path)
            
            # Generate changelog entry using template
            entry = self.template_manager.generate_from_template(
                "changelog_entry.j2",
                {
                    "changes": changes,
                    "date": datetime.now().strftime("%Y-%m-%d"),
                    "version": self._calculate_new_version(self._get_current_version())
                }
            )
            
            # Prepend new entry to changelog
            current_content = changelog_path.read_text() if changelog_path.exists() else ""
            changelog_path.write_text(entry + "\n" + current_content)
            
            logger.info("Updated changelog")

        except Exception as e:
            logger.error(f"Error updating changelog: {e}")
            raise

    def _update_version(self, new_version: str) -> None:
        """Update version number"""
        try:
            version_path = Path(self.config.version_file)
            version_path.write_text(new_version)
            logger.info(f"Updated version to {new_version}")
        except Exception as e:
            logger.error(f"Error updating version: {e}")
            raise 