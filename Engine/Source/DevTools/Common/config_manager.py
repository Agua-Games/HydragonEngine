"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Manages engine-wide configuration settings.
"""

import yaml
from pathlib import Path
from datetime import datetime

ENGINE_ROOT = Path(__file__).parent.parent.parent
CONFIG_PATH = ENGINE_ROOT / "Config" / "engine_config.yaml"

class EngineConfig:
    """Singleton class to manage engine configuration"""
    _instance = None
    
    def __new__(cls):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
            cls._instance._load_config()
        return cls._instance
    
    def _load_config(self):
        """Load configuration from yaml file"""
        try:
            with open(CONFIG_PATH, 'r') as f:
                self._config = yaml.safe_load(f)
        except Exception as e:
            raise RuntimeError(f"Failed to load engine configuration: {e}")
    
    @property
    def engine_name(self) -> str:
        return self._config['engine']['name']
    
    @property
    def engine_version(self) -> str:
        return self._config['engine']['version']
    
    @property
    def company_name(self) -> str:
        return self._config['engine']['company']
    
    @property
    def website_url(self) -> str:
        return self._config['documentation']['website_url']
    
    @property
    def website_name(self) -> str:
        return self._config['documentation']['website_name']
    
    @property
    def tool_name(self) -> str:
        return self._config['documentation']['tool_name']
    
    @property
    def current_year(self) -> int:
        return datetime.now().year 