"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Script to run the orphaned calls detector.
"""

import logging
from pathlib import Path
from orphaned_calls_detector import OrphanedCallsDetector

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

def main():
    try:
        detector = OrphanedCallsDetector()
        engine_root = Path(__file__).parent.parent.parent
        
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
            if elements:  # Only show category if it has elements
                logger.info(f"\n{display_name}:")
                for element in elements:
                    locations = element.locations
                    logger.info(f"- {element.name} ({len(locations)} locations)")
                    for loc in locations:
                        logger.info(f"  in file: {loc.file_path}")
                        logger.info(f"  line: {loc.line_number}")
                        if loc.scope:
                            logger.info(f"  scope: {loc.scope}")
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
            
    except Exception as e:
        logger.error(f"Error running detector: {e}")
        return 1
    return 0

if __name__ == "__main__":
    exit(main()) 