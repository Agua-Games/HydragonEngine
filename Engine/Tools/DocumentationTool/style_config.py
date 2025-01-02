"""
Documentation style configuration and validation
"""

style_rules = {
    'max_line_length': 100,
    'section_order': [
        'brief',
        'detailed',
        'parameters',
        'returns',
        'exceptions',
        'see_also',
        'notes',
        'examples'
    ],
    'required_sections': ['brief', 'parameters', 'returns'],
    'code_block_style': 'fenced',  # or 'indented'
    'link_style': 'inline'  # or 'reference'
} 