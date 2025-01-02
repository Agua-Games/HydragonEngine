"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Automatic documentation update system.
"""

import os
import sys
import yaml
import time
import logging
import schedule
from pathlib import Path
from typing import Dict, List, Optional
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

from .doc_generator import DocumentationGenerator, GeneratorConfig
from .api_tracker import APIChangeTracker

logger = logging.getLogger(__name__)

class DocUpdateConfig:
    """Configuration for documentation updates"""
    def __init__(self, config_path: str = "Tools/Documentation/auto_update_config.yml"):
        with open(config_path, 'r') as f:
            config = yaml.safe_load(f)
            
        self.watch_paths = config.get('watch_paths', [])
        self.triggers = config.get('triggers', {})
        self.notifications = config.get('notifications', {})

class DocUpdateHandler(FileSystemEventHandler):
    """Handles file system events for documentation updates"""
    def __init__(self, updater: 'DocumentationUpdater'):
        self.updater = updater
        self.last_update = 0
        self.update_cooldown = 5  # seconds

    def on_modified(self, event):
        """Handle file modification events"""
        if time.time() - self.last_update < self.update_cooldown:
            return

        if any(Path(event.src_path).match(pattern) 
              for pattern in self.updater.config.watch_paths):
            logger.info(f"Change detected in {event.src_path}")
            self.updater.update_documentation()
            self.last_update = time.time()

class DocumentationUpdater:
    """Manages automatic documentation updates"""
    def __init__(
        self,
        config_path: str = "Tools/Documentation/auto_update_config.yml"
    ):
        self.config = DocUpdateConfig(config_path)
        self._setup_logging()
        self.generator = DocumentationGenerator()
        self.api_tracker = APIChangeTracker(".")
        self.observer = Observer()

    def _setup_logging(self) -> None:
        """Configure logging for documentation updates"""
        log_file = Path("Logs") / "doc_updates.log"
        os.makedirs(log_file.parent, exist_ok=True)
        
        file_handler = logging.FileHandler(log_file)
        file_handler.setFormatter(
            logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
        )
        logger.addHandler(file_handler)
        logger.setLevel(logging.INFO)

    def start_watching(self) -> None:
        """Start watching for documentation changes"""
        try:
            handler = DocUpdateHandler(self)
            for path in self.config.watch_paths:
                self.observer.schedule(handler, path, recursive=True)
            
            self.observer.start()
            logger.info("Started watching for documentation changes")
            
            if self.config.triggers.get('schedule'):
                self._setup_scheduled_updates()
            
            try:
                while True:
                    time.sleep(1)
            except KeyboardInterrupt:
                self.observer.stop()
                
            self.observer.join()
            
        except Exception as e:
            logger.error(f"Error watching for changes: {e}")
            raise

    def update_documentation(self) -> bool:
        """Update documentation based on changes"""
        try:
            logger.info("Starting documentation update")
            
            # Check for API changes
            changes = self.api_tracker.detect_changes()
            if changes:
                logger.info(f"Detected {len(changes)} API changes")
                self._handle_api_changes(changes)
            
            # Generate documentation
            success = self.generator.generate_documentation()
            
            if success:
                logger.info("Documentation updated successfully")
                self._send_notifications("Documentation updated successfully")
            else:
                logger.error("Documentation update failed")
                self._send_notifications("Documentation update failed", error=True)
            
            return success
            
        except Exception as e:
            logger.error(f"Error updating documentation: {e}")
            self._send_notifications(f"Documentation update error: {e}", error=True)
            return False

    def _setup_scheduled_updates(self) -> None:
        """Setup scheduled documentation updates"""
        schedule_str = self.config.triggers.get('schedule')
        if schedule_str:
            schedule.every().day.at(schedule_str).do(self.update_documentation)
            logger.info(f"Scheduled updates configured for {schedule_str}")

    def _handle_api_changes(self, changes: List[dict]) -> None:
        """Handle detected API changes"""
        # Generate changelog
        changelog = self.api_tracker.generate_changelog()
        
        # Check for breaking changes
        breaking_changes = self.api_tracker.notify_breaking()
        if breaking_changes:
            logger.warning(f"Detected {len(breaking_changes)} breaking changes")
            self._send_notifications(
                "Breaking API changes detected",
                error=True
            )

    def _send_notifications(self, message: str, error: bool = False) -> None:
        """Send update notifications"""
        if error:
            logger.error(message)
        else:
            logger.info(message)

        # Send notifications based on configuration
        if self.config.notifications.get('slack'):
            self._send_slack_notification(message, error)
        
        if self.config.notifications.get('email'):
            self._send_email_notification(message, error)
        
        if self.config.notifications.get('github'):
            self._send_github_notification(message, error)

    def _send_slack_notification(self, message: str, error: bool) -> None:
        """Send Slack notification"""
        # Implementation for Slack notifications
        pass

    def _send_email_notification(self, message: str, error: bool) -> None:
        """Send email notification"""
        # Implementation for email notifications
        pass

    def _send_github_notification(self, message: str, error: bool) -> None:
        """Send GitHub notification"""
        # Implementation for GitHub notifications
        pass

def main():
    """Main entry point for documentation updates"""
    try:
        updater = DocumentationUpdater()
        updater.start_watching()
    except Exception as e:
        logger.error(f"Documentation update system failed: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main() 