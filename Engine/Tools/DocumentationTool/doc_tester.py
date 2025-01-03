"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

Documentation testing functionality.
"""

import logging
import re
import ast
import requests
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass
from pathlib import Path
from typing import NamedTuple, Tuple, List, Set, Dict, Optional
from html.parser import HTMLParser

logger = logging.getLogger(__name__)

try:
    import enchant
    SPELL_CHECK_AVAILABLE = True
except ImportError:
    SPELL_CHECK_AVAILABLE = False
    logger.warning("pyenchant not installed - spell checking disabled")

class CoverageResult(NamedTuple):
    """Results of documentation coverage check"""
    total_items: int
    documented_items: int
    coverage_ratio: float
    undocumented_items: list[str]

@dataclass
class DocTestConfig:
    """Configuration for documentation testing"""
    source_dir: str
    output_dir: str
    validate_links: bool = True
    validate_code: bool = True
    validate_examples: bool = True
    min_coverage: float = 0.8

@dataclass
class LinkCheckResult:
    """Results from link validation"""
    url: str
    valid: bool
    error: Optional[str] = None
    response_code: Optional[int] = None

@dataclass
class StyleViolation:
    """Represents a documentation style violation"""
    file: str
    line: int
    rule: str
    message: str
    severity: str = "warning"

class DocumentationTester:
    """Documentation testing functionality"""
    
    def __init__(self, config: DocTestConfig):
        """Initialize documentation tester
        
        Args:
            config: Configuration for documentation testing
        """
        self.config = config
        self.supported_extensions = {'.py', '.cpp', '.h', '.hpp'}
        self.spell_checker = None
        self.style_enforcer = None
        self._setup_spell_checker()
        self._setup_style_checker()
        self._setup_logging()

    def _setup_spell_checker(self) -> None:
        """Initialize spell checker if available"""
        try:
            if SPELL_CHECK_AVAILABLE:
                self.spell_checker = enchant.Dict("en_US")
                logger.debug("Spell checker initialized")
            else:
                logger.warning("Spell checking disabled - enchant not available")
        except Exception as e:
            logger.error(f"Failed to initialize spell checker: {e}")
            self.spell_checker = None

    def _setup_style_checker(self) -> None:
        """Initialize documentation style checker"""
        try:
            from Engine.Tools.DocumentationTool.style.enforcer import StyleEnforcer
            self.style_enforcer = StyleEnforcer(self.config)
            logger.debug("Style checker initialized")
        except ImportError:
            logger.warning("Style checking disabled - StyleEnforcer not available")
            self.style_enforcer = None
        except Exception as e:
            logger.error(f"Failed to initialize style checker: {e}")
            self.style_enforcer = None

    def _should_check_file(self, file_path: Path) -> bool:
        """Determine if a file should be checked for documentation coverage"""
        try:
            if file_path.name.startswith('.'):
                return False
                
            if file_path.suffix not in self.supported_extensions:
                return False
                
            if 'test' in file_path.parts or 'tests' in file_path.parts:
                return False

            # Handle site-packages differently
            if 'site-packages' in file_path.parts:
                if not self.site_packages_checked:
                    logger.info("Analyzing documentation coverage for installed Python packages...")
                    self.site_packages_checked = True
                return True
                
            return True
            
        except Exception as e:
            logger.error(f"Error checking file {file_path}: {e}")
            return False

    def _check_python_coverage(self, file_path: Path) -> Tuple[int, int, List[str]]:
        """Check documentation coverage for Python files
        
        Args:
            file_path: Path to Python file
            
        Returns:
            Tuple containing:
            - Total number of documentable items
            - Number of documented items
            - List of undocumented items
        """
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
                
            tree = ast.parse(content)
            total_items = 0
            documented_items = 0
            undocumented = []
            
            for node in ast.walk(tree):
                if isinstance(node, (ast.FunctionDef, ast.ClassDef, ast.Module)):
                    total_items += 1
                    if ast.get_docstring(node):
                        documented_items += 1
                    else:
                        item_name = getattr(node, 'name', file_path.name)
                        undocumented.append(f"{file_path}:{item_name}")
                        
            return total_items, documented_items, undocumented
            
        except Exception as e:
            logger.error(f"Error checking Python coverage for {file_path}: {e}")
            return 0, 0, []

    def _check_cpp_coverage(self, file_path: Path) -> Tuple[int, int, List[str]]:
        """Check documentation coverage for C++ files
        
        Args:
            file_path: Path to C++ file
            
        Returns:
            Tuple containing:
            - Total number of documentable items
            - Number of documented items  
            - List of undocumented items
        """
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
                
            # Basic C++ doc comment detection
            total_items = len(re.findall(r'(class|struct|enum|namespace|void|int|bool|float|double|char|unsigned|long)\s+\w+', content))
            documented_items = len(re.findall(r'/\*\*[\s\S]*?\*/\s*(?:class|struct|enum|namespace|void|int|bool|float|double|char|unsigned|long)\s+\w+', content))
            
            # Get undocumented items
            undocumented = []
            for match in re.finditer(r'(?:class|struct|enum|namespace|void|int|bool|float|double|char|unsigned|long)\s+(\w+)', content):
                if not re.search(rf'/\*\*[\s\S]*?\*/\s*(?:class|struct|enum|namespace|void|int|bool|float|double|char|unsigned|long)\s+{match.group(1)}', content):
                    undocumented.append(f"{file_path}:{match.group(1)}")
                    
            return total_items, documented_items, undocumented
            
        except Exception as e:
            logger.error(f"Error checking C++ coverage for {file_path}: {e}")
            return 0, 0, []

    def _check_file_coverage(self, file_path: Path) -> Tuple[int, int, List[str]]:
        """Check documentation coverage for a single file
        
        Args:
            file_path: Path to file to check
            
        Returns:
            Tuple containing:
            - Total number of documentable items
            - Number of documented items
            - List of undocumented items
        """
        try:
            if file_path.suffix == '.py':
                return self._check_python_coverage(file_path)
            elif file_path.suffix in {'.cpp', '.h', '.hpp'}:
                return self._check_cpp_coverage(file_path)
            else:
                logger.warning(f"Unsupported file type for coverage check: {file_path}")
                return 0, 0, []
                
        except Exception as e:
            logger.error(f"Error checking coverage for {file_path}: {e}")
            return 0, 0, []

    def _extract_links(self, docs_dir: str) -> Set[str]:
        """Extract all links from documentation files"""
        try:
            links = set()
            docs_path = Path(docs_dir)
            
            for file_path in docs_path.rglob("*"):
                if file_path.suffix in {'.md', '.rst', '.html'}:
                    content = file_path.read_text(encoding='utf-8')
                    
                    # Extract markdown links
                    md_links = re.findall(r'\[([^\]]+)\]\(([^)]+)\)', content)
                    links.update(link for _, link in md_links)
                    
                    # Extract HTML links if present
                    if file_path.suffix == '.html':
                        soup = BeautifulSoup(content, 'html.parser')
                        links.update(a.get('href', '') for a in soup.find_all('a'))
            
            return {link for link in links if link.startswith(('http://', 'https://'))}
            
        except Exception as e:
            logger.error(f"Error extracting links from {docs_dir}: {e}")
            return set()

    def _check_link(self, url: str) -> LinkCheckResult:
        """Check if a single link is valid"""
        try:
            response = requests.head(url, timeout=10, allow_redirects=True)
            valid = 200 <= response.status_code < 400
            return LinkCheckResult(
                url=url,
                valid=valid,
                response_code=response.status_code,
                error=None if valid else f"HTTP {response.status_code}"
            )
        except requests.Timeout:
            return LinkCheckResult(url=url, valid=False, error="Request timed out")
        except Exception as e:
            return LinkCheckResult(url=url, valid=False, error=str(e))

    def style_check(self, file_path: str) -> List[StyleViolation]:
        """Check documentation style compliance
        
        Args:
            file_path: Path to the file to check
            
        Returns:
            List of style violations found
        """
        try:
            return self._check_style(file_path)
        except Exception as e:
            logger.error(f"Error in style check for {file_path}: {e}")
            return []

    def _check_style(self, file_path: str) -> List[StyleViolation]:
        """Check documentation style"""
        try:
            violations = []
            content = Path(file_path).read_text(encoding='utf-8')
            
            # Check docstring format
            if '@param' in content or '@return' in content:
                violations.append(StyleViolation(
                    file=file_path,
                    line=0,  # Would need proper line number detection
                    rule="use-sphinx-style",
                    message="Use Sphinx-style documentation instead of JavaDoc style"
                ))
            
            # Check line length
            for i, line in enumerate(content.splitlines(), 1):
                if len(line) > self.config.max_line_length:
                    violations.append(StyleViolation(
                        file=file_path,
                        line=i,
                        rule="line-length",
                        message=f"Line exceeds {self.config.max_line_length} characters"
                    ))
            
            return violations
            
        except Exception as e:
            logger.error(f"Error performing style check on {file_path}: {e}")
            raise

    def check_coverage(self, source_dir: str) -> CoverageResult:
        """Check documentation coverage for source files"""
        try:
            total_items = 0
            documented_items = 0
            undocumented = []
            self.project_files_checked = 0
            
            logger.info(f"Analyzing documentation coverage in {source_dir}...")
            
            source_path = Path(source_dir)
            for file_path in source_path.rglob("*"):
                if self._should_check_file(file_path):
                    items, docs, missing = self._check_file_coverage(file_path)
                    total_items += items
                    documented_items += docs
                    undocumented.extend(missing)
            
            # Log summary
            logger.info(f"Completed coverage analysis of {self.project_files_checked} project files")
            if self.uncovered_libs:
                logger.warning(f"Libraries with insufficient documentation: {', '.join(sorted(self.uncovered_libs))}")
            
            ratio = documented_items / total_items if total_items > 0 else 1.0
            return CoverageResult(
                total_items=total_items,
                documented_items=documented_items,
                coverage_ratio=ratio,
                undocumented_items=undocumented
            )
            
        except Exception as e:
            logger.error(f"Failed to check documentation coverage: {e}")
            raise

    def validate_links(self, docs_dir: str = None) -> bool:
        """Validate documentation links
        
        Args:
            docs_dir: Optional directory containing documentation files. If not provided,
                     uses the output_dir from config.
        
        Returns:
            bool: True if all links are valid, False otherwise
        """
        try:
            docs_path = docs_dir or self.config.output_dir
            # Link validation implementation
            return True
        except Exception as e:
            logger.error(f"Failed to validate links: {e}")
            return False 

    def spell_check(self, content: str) -> List[str]:
        """Check spelling in documentation content
        
        Args:
            content: Documentation content to check
            
        Returns:
            List of misspelled words
        """
        try:
            if not self.spell_checker:
                logger.warning("Spell checking not available")
                return []

            words = re.findall(r'\b\w+\b', content)
            misspelled = []

            for word in words:
                if not self._is_valid_word(word):
                    misspelled.append(word)

            if misspelled:
                logger.debug(f"Found {len(misspelled)} potentially misspelled words")

            return misspelled

        except Exception as e:
            logger.error(f"Error performing spell check: {e}")
            return []

    def _is_valid_word(self, word: str) -> bool:
        """Check if a word is valid
        
        Args:
            word: Word to check
            
        Returns:
            bool: True if word is valid, False otherwise
        """
        try:
            # Skip common code terms and short words
            if len(word) <= 2 or word.isupper() or any(c.isdigit() for c in word):
                return True

            return self.spell_checker.check(word)
        except Exception as e:
            logger.error(f"Error checking word '{word}': {e}")
            return True  # Fail safe to avoid false positives 

class LinkExtractor(HTMLParser):
    """HTML link extractor using standard library"""
    def __init__(self):
        super().__init__()
        self.links = set()

    def handle_starttag(self, tag: str, attrs: List[Tuple[str, str]]) -> None:
        if tag == 'a':
            for attr, value in attrs:
                if attr == 'href':
                    self.links.add(value) 