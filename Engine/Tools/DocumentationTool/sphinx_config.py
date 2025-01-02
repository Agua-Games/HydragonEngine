"""
Sphinx-specific configuration for API documentation
"""

project = 'Hydragon Engine'
copyright = '2024 Agua Games'
author = 'Agua Games Engine Team'

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.napoleon',  # Google-style docstrings
    'sphinx.ext.viewcode',  # Link to source code
    'sphinx.ext.graphviz',  # Diagrams
    'sphinx_rtd_theme',     # Theme
    'sphinx.ext.intersphinx',  # Link to other projects' documentation
    'sphinx_markdown_builder'  # Markdown output
]

# Custom features
enable_cpp_support = True
enable_inheritance_diagrams = True
enable_collaboration_diagrams = True 