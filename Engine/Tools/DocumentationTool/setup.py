"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Setup script for DocumentationTool package.
"""

from setuptools import setup, find_packages

setup(
    name="documentation_tool",
    version="1.0.0",
    packages=find_packages(),
    python_requires=">=3.8",
    install_requires=[
        "pytest>=7.0.0",
        "requests>=2.25.0",
        "psutil>=5.8.0",
        "pytest-timeout>=2.1.0",
        "pytest-mock>=3.10.0",
    ],
) 