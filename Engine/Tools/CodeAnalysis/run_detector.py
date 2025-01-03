"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Script to run the orphaned calls detector.
"""

import logging
from pathlib import Path
from orphaned_calls_detector import OrphanedCallsDetector
from enum import Enum
import html

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class LogFormat(Enum):
    """Output format for logs"""
    TERMINAL = "terminal"
    HTML = "html"
    PLAIN = "plain"

class LoggingConfig:
    """Configuration for log output"""
    def __init__(self, format: LogFormat, output_file: Path, use_colors: bool):
        self.format = format
        self.output_file = output_file
        self.use_colors = use_colors

class ColorScheme:
    """ANSI and HTML color definitions"""
    # Terminal colors
    TERMINAL = {
        'high': '\033[91m',  # Red
        'medium': '\033[93m',  # Yellow
        'low': '\033[92m',  # Green
        'reset': '\033[0m',
        'bold': '\033[1m',
        'header': '\033[95m',  # Purple
        'info': '\033[94m',  # Blue
    }
    
    # HTML colors
    HTML = {
        'high': '#ff4444',
        'medium': '#ffbb33',
        'low': '#00C851',
        'header': '#9933CC',
        'info': '#33b5e5',
    }

def main():
    try:
        # Add logging configuration
        logging_config = LoggingConfig(
            format=LogFormat.TERMINAL,  # or HTML, or PLAIN
            output_file=Path("orphaned_calls_report.html") if LogFormat.HTML else None,
            use_colors=True
        )
        
        detector = OrphanedCallsDetector()
        engine_root = Path(__file__).parent.parent.parent
        
        if logging_config.format == LogFormat.HTML:
            # Start HTML document
            html_output = ["""
                <html>
                <head>
                    <style>
                        body { font-family: monospace; padding: 20px; }
                        .high { color: #ff4444; }
                        .medium { color: #ffbb33; }
                        .low { color: #00C851; }
                        .header { color: #9933CC; font-weight: bold; }
                        .info { color: #33b5e5; }
                        .element { margin: 10px 0; }
                        .location { margin-left: 20px; }
                    </style>
                </head>
                <body>
            """]
        
        # First analyze our codebase
        logger.info(f"Analyzing engine codebase in: {engine_root}")
        results = detector.analyze_codebase(engine_root)
        
        # Then analyze third-party packages
        logger.info("\nAnalyzing third-party packages...")
        third_party_stats = detector.analyze_third_party(engine_root)
        
        # Print results with updated terminology and improved formatting
        logger.info("\nAnalysis Results:")
        category_display_names = {
            'function_calls': 'Orphaned Function Calls',
            'variable_calls': 'Orphaned Variable References',
            'class_calls': 'Orphaned Class References'
        }
        
        for category, elements in results.orphaned_elements.items():
            display_name = category_display_names.get(category, category.title())
            if elements:
                if logging_config.format == LogFormat.HTML:
                    html_output.append(f'<h2 class="header">{html.escape(display_name)}</h2>')
                else:
                    header = f"\n{display_name}:"
                    if logging_config.use_colors:
                        header = f"{ColorScheme.TERMINAL['header']}{header}{ColorScheme.TERMINAL['reset']}"
                    logger.info(header)
                
                for element in elements:
                    # Calculate severity before using it
                    element.calculate_severity()
                    locations = element.locations
                    severity = element.severity.upper()
                    
                    # Format the element line with severity color
                    if logging_config.use_colors:
                        color = ColorScheme.TERMINAL.get(element.severity.lower(), ColorScheme.TERMINAL['reset'])
                        element_line = f"{color}- {element.name} ({len(locations)} locations) [{severity}]{ColorScheme.TERMINAL['reset']}"
                    else:
                        element_line = f"- {element.name} ({len(locations)} locations) [{severity}]"
                    
                    logger.info(element_line)
                    
                    # Show last usage time if available
                    if element.last_used:
                        from datetime import datetime
                        last_used = datetime.fromtimestamp(element.last_used)
                        logger.info(f"  Last used: {last_used.strftime('%Y-%m-%d')}")
                    
                    # Show locations
                    for loc in locations:
                        logger.info(f"  in file: {loc.file_path}")
                        logger.info(f"  line: {loc.line_number}")
                        if loc.scope:
                            logger.info(f"  scope: {loc.scope}")
                    
                    # Show suggested action
                    action = detector._suggest_action(element)
                    logger.info(f"  Suggested action: {action}")
                    logger.info("")  # Add blank line between elements
        
        # Print statistics with updated terminology
        logger.info("\nEngine Codebase Statistics:")
        stat_display_names = {
            'total_function_calls': 'Total Function Declarations',
            'orphaned_function_calls': 'Orphaned Function Calls',
            'total_variable_calls': 'Total Variable Declarations',
            'orphaned_variable_calls': 'Orphaned Variable References',
            'total_class_calls': 'Total Class Declarations',
            'orphaned_class_calls': 'Orphaned Class References'
        }
        
        for key, value in results.statistics.items():
            display_name = stat_display_names.get(key, key)
            logger.info(f"{display_name}: {value}")
            
        logger.info("\nThird-Party Package Statistics:")
        for category, count in third_party_stats.items():
            display_name = stat_display_names.get(f"total_{category}", category)
            logger.info(f"{display_name}: {count}")
            
        if logging_config.format == LogFormat.HTML and logging_config.output_file:
            html_output.append("</body></html>")
            logging_config.output_file.write_text("\n".join(html_output))
            logger.info(f"\nHTML report generated: {logging_config.output_file}")
            
    except Exception as e:
        logger.error(f"Error running detector: {e}")
        return 1
    return 0

if __name__ == "__main__":
    exit(main()) 