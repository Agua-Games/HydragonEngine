"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

API change tracking functionality.
"""

import logging
from dataclasses import dataclass
from pathlib import Path
import git

logger = logging.getLogger(__name__)

@dataclass
class APITrackerConfig:
    """Configuration for API change tracking"""
    repo_path: str
    output_path: str
    track_changes: bool = True
    track_history: bool = True
    track_authors: bool = True

class APIChangeTracker:
    """Tracks API changes in the codebase"""
    
    def __init__(self, config: APITrackerConfig):
        self.config = config
        try:
            self.repo = git.Repo(config.repo_path)
        except git.InvalidGitRepositoryError:
            logger.warning(f"Not a valid git repository: {config.repo_path}")
            self.repo = None 