"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Setup configuration for DocumentationTool package.
"""

from setuptools import setup, find_packages

setup(
    name="documentation-tool",
    version="1.0.0",
    packages=find_packages(),
    install_requires=[
        'gitpython>=3.1.0',
        'docker>=6.1.0',
        'requests>=2.31.0',
        'pyyaml>=6.0.1',
        'docutils>=0.20.1',
        'sphinx>=7.1.0',
        'mistune>=3.0.0'
    ],
    extras_require={
        'test': [
            'pytest>=7.4.0',
            'pytest-mock>=3.11.1',
            'pytest-cov>=4.1.0',
            'pytest-timeout>=2.1.0',
            'pycodestyle>=2.11.0',
            'pylint>=3.0.0',
            'mypy>=1.5.0'
        ]
    }
) 