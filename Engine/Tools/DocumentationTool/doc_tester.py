"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Documentation testing functionality.
"""

import os
import re
import logging
from pathlib import Path
from typing import List, Optional, Dict, Set
from dataclasses import dataclass

logger = logging.getLogger(__name__)

try:
    import enchant
    SPELL_CHECK_AVAILABLE = True
except ImportError:
    SPELL_CHECK_AVAILABLE = False
    logger.warning("pyenchant not installed - spell checking disabled")

@dataclass
class DocTestConfig:
    """Configuration for documentation testing"""
    enable_spell_check: bool = False  # Changed to False by default
    min_coverage: float = 0.8  # 80% minimum coverage
    spell_check_lang: str = "en_US"
    max_line_length: int = 100
    parallel_checks: bool = True
    max_workers: int = 4
    ignore_patterns: List[str] = None
    style_guide_path: str = "Tools/Documentation/style_guide.md"

@dataclass
class CoverageResult:
    """Results from documentation coverage analysis"""
    total_elements: int
    documented_elements: int
    coverage_ratio: float
    missing_docs: List[str]

@dataclass
class LinkCheckResult:
    """Results from link validation"""
    url: str
    valid: bool
    error: Optional[str] = None
    response_code: Optional[int] = None

@dataclass
class StyleViolation:
    """Style guide violation"""
    file: str
    line: int
    rule: str
    message: str
    severity: str

class DocumentationTester:
    def __init__(self, config: Optional[DocTestConfig] = None):
        self.config = config or DocTestConfig()
        self.spell_checker = None
        if self.config.enable_spell_check and SPELL_CHECK_AVAILABLE:
            self.spell_checker = enchant.Dict("en_US")

    def _setup_logging(self) -> None:
        """Configure logging for documentation testing"""
        log_file = Path("Logs") / "doc_testing.log"
        os.makedirs(log_file.parent, exist_ok=True)
        
        file_handler = logging.FileHandler(log_file)
        file_handler.setFormatter(
            logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
        )
        logger.addHandler(file_handler)
        logger.setLevel(logging.INFO)

    def check_coverage(self, source_dir: str) -> CoverageResult:
        """Tests documentation coverage across the codebase"""
        try:
            total = 0
            documented = 0
            missing = []

            for path in Path(source_dir).rglob("*"):
                if not self._should_check_file(path):
                    continue

                file_total, file_documented, file_missing = self._check_file_coverage(path)
                total += file_total
                documented += file_documented
                missing.extend(file_missing)

            coverage_ratio = documented / total if total > 0 else 0
            
            if coverage_ratio < self.config.min_coverage:
                logger.warning(
                    f"Documentation coverage ({coverage_ratio:.2%}) below minimum "
                    f"threshold ({self.config.min_coverage:.2%})"
                )

            return CoverageResult(total, documented, coverage_ratio, missing)

        except Exception as e:
            logger.error(f"Error checking documentation coverage: {str(e)}")
            raise

    def validate_links(self, docs_dir: str) -> List[LinkCheckResult]:
        """Verifies all documentation links are valid"""
        try:
            links = self._extract_links(docs_dir)
            results = []

            with ThreadPoolExecutor(max_workers=self.config.max_workers) as executor:
                future_to_url = {
                    executor.submit(self._check_link, url): url 
                    for url in links
                }
                
                for future in future_to_url:
                    url = future_to_url[future]
                    try:
                        result = future.result()
                        results.append(result)
                        if not result.valid:
                            logger.warning(f"Invalid link found: {url} - {result.error}")
                    except Exception as e:
                        logger.error(f"Error checking link {url}: {str(e)}")
                        results.append(LinkCheckResult(url, False, str(e)))

            return results

        except Exception as e:
            logger.error(f"Error validating links: {str(e)}")
            raise

    def spell_check(self, content: str) -> List[str]:
        """Checks spelling in documentation content"""
        try:
            words = re.findall(r'\b\w+\b', content)
            misspelled = []

            for word in words:
                if not self._is_valid_word(word):
                    misspelled.append(word)
                    logger.debug(f"Potentially misspelled word: {word}")

            return misspelled

        except Exception as e:
            logger.error(f"Error performing spell check: {str(e)}")
            raise

    def style_check(self, file_path: str) -> List[StyleViolation]:
        """Validates documentation style guide compliance"""
        try:
            violations = []
            content = Path(file_path).read_text()
            
            # Check line length
            for i, line in enumerate(content.splitlines(), 1):
                if len(line) > self.config.max_line_length:
                    violations.append(
                        StyleViolation(
                            file_path, i, "line_length",
                            f"Line exceeds {self.config.max_line_length} characters",
                            "warning"
                        )
                    )

            # Check section order
            if not self._check_section_order(content):
                violations.append(
                    StyleViolation(
                        file_path, 0, "section_order",
                        "Sections are not in the recommended order",
                        "warning"
                    )
                )

            # Check formatting consistency
            violations.extend(self._check_formatting(file_path, content))

            return violations

        except Exception as e:
            logger.error(f"Error performing style check on {file_path}: {str(e)}")
            raise

    def _should_check_file(self, path: Path) -> bool:
        """Determine if a file should be checked"""
        if self.config.ignore_patterns:
            for pattern in self.config.ignore_patterns:
                if path.match(pattern):
                    return False
        return path.suffix in {'.py', '.cpp', '.h', '.md', '.rst'}

    def _check_file_coverage(self, path: Path) -> tuple[int, int, List[str]]:
        """Check documentation coverage for a single file"""
        # Implementation for checking individual file coverage
        pass

    def _extract_links(self, docs_dir: str) -> Set[str]:
        """Extract all links from documentation files"""
        # Implementation for link extraction
        pass

    def _check_link(self, url: str) -> LinkCheckResult:
        """Check if a single link is valid"""
        # Implementation for link checking
        pass

    def _is_valid_word(self, word: str) -> bool:
        """Check if a word is valid"""
        # Implementation for word validation
        pass

    def _check_style(self, file_path: str) -> List[StyleViolation]:
        """Check documentation style"""
        try:
            # Delegate style checking to StyleEnforcer
            fixes = self.style_enforcer.enforce_docstring_format(Path(file_path))
            
            # Convert StyleFix objects to StyleViolation objects
            violations = [
                StyleViolation(
                    file=fix.file,
                    line=fix.line,
                    rule=fix.rule,
                    message=f"Style violation: {fix.rule}",
                    severity="warning"
                )
                for fix in fixes
            ]
            
            return violations

        except Exception as e:
            logger.error(f"Error performing style check on {file_path}: {str(e)}")
            raise

    def _check_formatting(self, file_path: str, content: str) -> List[StyleViolation]:
        """Check formatting consistency"""
        # Delegate to StyleEnforcer's formatting checks
        fixes = self.style_enforcer.enforce_file_headers(Path(file_path))
        fixes.extend(self.style_enforcer.enforce_comment_style(Path(file_path)))
        
        return [
            StyleViolation(
                file=fix.file,
                line=fix.line,
                rule=fix.rule,
                message=f"Formatting violation: {fix.rule}",
                severity="error"
            )
            for fix in fixes
        ]

    def _check_section_order(self, content: str) -> bool:
        """Check if sections are in the correct order"""
        # Delegate to StyleEnforcer's section order checking
        fixes = self.style_enforcer._fix_section_order(content, Path("temp"))
        return len(fixes) == 0 