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
        
        logger.info(f"Analyzing codebase in: {engine_root}")
        results = detector.analyze_codebase(engine_root)
        
        # Print results
        logger.info("\nAnalysis Results:")
        for category, elements in results.orphaned_elements.items():
            logger.info(f"\n{category.title()}:")
            for element in elements:
                locations = [f"{loc.file_path}:{loc.line_number}" for loc in element.locations]
                logger.info(f"- {element.name} ({len(locations)} locations)")
                for loc in locations:
                    logger.info(f"  • {loc}")
        
        # Print statistics
        logger.info("\nStatistics:")
        for key, value in results.statistics.items():
            logger.info(f"{key}: {value}")
            
    except Exception as e:
        logger.error(f"Error running detector: {e}")
        return 1
    return 0

if __name__ == "__main__":
    exit(main()) 