"""
Copyright (c) 2024 Agua Games. All rights reserved.
Licensed under the Agua Games License 1.0

C++ source code parser for API documentation generation.
"""

import re
import logging
from pathlib import Path
from typing import Dict, List, Optional, Tuple, Set
from dataclasses import dataclass

logger = logging.getLogger(__name__)

@dataclass
class StandardsConfig:
    """Configuration for coding standards validation"""
    copyright_pattern: str
    include_guard_pattern: str
    namespace_pattern: str
    class_pattern: str
    method_pattern: str
    comment_pattern: str
    max_line_length: int
    required_sections: List[str]

@dataclass
class DocComment:
    """Represents a documentation comment block"""
    brief: str
    details: str
    params: Dict[str, str]
    returns: str
    see_also: List[str]
    notes: List[str]
    examples: List[str]

class CppParser:
    """Parses C++ source files for API documentation"""
    
    # Enhanced patterns for game engine context
    MODULE_PATTERNS = {
        'Core': {
            'desc': "Core engine systems and foundational functionality",
            'submodules': {
                'Memory': "Memory management and allocation systems",
                'Threading': "Multi-threading and concurrency management",
                'FileSystem': "File system operations and asset management",
                'Math': "Mathematical utilities and computations",
                'Debug': "Debugging and profiling tools",
                'Events': "Event system and message passing",
                'Config': "Configuration management system"
            }
        },
        'AI': {
            'desc': "Artificial Intelligence module for game behavior and decision making",
            'submodules': {
                'Behavior': "AI behavior tree and decision making system",
                'PathFinding': "Navigation and pathfinding algorithms",
                'Generation': "Procedural content generation using AI",
                'Training': "AI model training and optimization system",
                'Pipeline': "AI processing and inference pipeline",
                'Perception': "AI sensory and perception systems"
            }
        },
        'Physics': {
            'desc': "Physics simulation and collision detection system",
            'submodules': {
                'Collision': "Collision detection and response system",
                'Dynamics': "Physical dynamics simulation",
                'Constraints': "Physics constraint solver",
                'Forces': "Force and impulse management",
                'Rigging': "Character rigging and skeletal physics",
                'Particles': "Particle system physics"
            }
        },
        'Animation': {
            'desc': "Animation systems and skeletal manipulation",
            'submodules': {
                'Skeletal': "Skeletal animation system",
                'Blending': "Animation blending and transitions",
                'IK': "Inverse kinematics solver",
                'Rigging': "Character rigging and bone management",
                'States': "Animation state machine system"
            }
        }
    }

    def __init__(self, standards_config: Optional[StandardsConfig] = None):
        self.namespace_stack = []
        self.include_set: Set[str] = set()
        self.forward_declarations: List[str] = []
        self.standards = standards_config or self._load_default_standards()
        
    def _load_default_standards(self) -> StandardsConfig:
        """Load default standards configuration"""
        return StandardsConfig(
            copyright_pattern=r'/\*\s*Copyright \(c\) \d{4} Agua Games',
            include_guard_pattern=r'#pragma once',
            namespace_pattern=r'namespace \w+\s*{',
            class_pattern=r'class \w+(?:final)?\s*(?::\s*(?:public|protected|private)\s*\w+)?\s*{',
            method_pattern=r'(?:virtual\s+)?(?:explicit\s+)?(?:static\s+)?'
                         r'(?:const\s+)?(?:\w+::\w+|\w+)\s+\w+\s*\([^)]*\)',
            comment_pattern=r'/\*\*(?:[^*]|\*(?!/))*\*/',
            max_line_length=120,
            required_sections=['brief', 'details', 'params', 'returns']
        )

    def _verify_standards_compliance(self, content: str, file_path: Path) -> List[Dict]:
        """Verify compliance with coding standards"""
        violations = []
        
        def add_violation(rule: str, message: str, line: int = 0):
            violations.append({
                'file': str(file_path),
                'rule': rule,
                'message': message,
                'line': line
            })

        # Check copyright notice
        if not re.match(self.standards.copyright_pattern, content):
            add_violation('copyright', 'Missing or incorrect copyright notice')

        # Check include guards
        if file_path.suffix in ['.h', '.hpp'] and not re.search(self.standards.include_guard_pattern, content):
            add_violation('include_guard', 'Missing #pragma once include guard')

        # Check line length
        for i, line in enumerate(content.splitlines(), 1):
            if len(line) > self.standards.max_line_length:
                add_violation('line_length', 
                            f'Line exceeds {self.standards.max_line_length} characters',
                            i)

        # Check documentation comments
        doc_comments = re.finditer(self.standards.comment_pattern, content)
        for match in doc_comments:
            comment = match.group()
            missing_sections = []
            for section in self.standards.required_sections:
                if f'@{section}' not in comment:
                    missing_sections.append(section)
            if missing_sections:
                add_violation('documentation', 
                            f'Missing required sections: {", ".join(missing_sections)}',
                            content[:match.start()].count('\n') + 1)

        return violations

    def parse_file(self, file_path: Path) -> Dict:
        """Parse a C++ source file and extract API documentation"""
        try:
            content = file_path.read_text(encoding='utf-8')
            
            # Verify standards compliance
            violations = self._verify_standards_compliance(content, file_path)
            if violations:
                for violation in violations:
                    logger.warning(
                        f"{violation['file']}:{violation['line']} - "
                        f"{violation['rule']}: {violation['message']}"
                    )
            
            # Extract includes and forward declarations
            self._parse_includes(content)
            self._parse_forward_declarations(content)
            
            # Extract file-level documentation
            file_doc = self._extract_file_doc(content)
            
            # Parse declarations with proper scope handling
            scope_tree = self._build_scope_tree(content)
            
            # Generate intelligent descriptions where missing
            self._enhance_descriptions(file_path, scope_tree)
            
            return {
                'path': file_path,
                'doc': file_doc,
                'includes': list(self.include_set),
                'forward_declarations': self.forward_declarations,
                'scope_tree': scope_tree
            }
            
        except Exception as e:
            logger.error(f"Error parsing {file_path}: {e}")
            raise

    def _parse_includes(self, content: str) -> None:
        """Parse and validate include statements"""
        include_pattern = r'#include\s*[<"]([^>"]+)[>"]'
        self.include_set = set(re.findall(include_pattern, content))

    def _parse_forward_declarations(self, content: str) -> None:
        """Parse forward declarations"""
        forward_pattern = r'(?:class|struct|enum)\s+(\w+);'
        self.forward_declarations = re.findall(forward_pattern, content)

    def _build_scope_tree(self, content: str) -> Dict:
        """Build hierarchical scope tree of declarations"""
        scope_pattern = re.compile(
            r'namespace\s+(\w+)|'
            r'class\s+(\w+)(?:\s*:\s*(?:public|protected|private)\s+\w+)*\s*{|'
            r'struct\s+(\w+)(?:\s*:\s*(?:public|protected|private)\s+\w+)*\s*{|'
            r'enum(?:\s+class)?\s+(\w+)\s*{|'
            r'}\s*;?'
        )
        
        scope_stack = [{'type': 'global', 'name': '', 'children': []}]
        pos = 0
        
        while pos < len(content):
            match = scope_pattern.search(content, pos)
            if not match:
                break
                
            if match.group(0).startswith('}'):
                if len(scope_stack) > 1:
                    scope_stack.pop()
            else:
                scope_type = 'namespace' if match.group(1) else \
                            'class' if match.group(2) else \
                            'struct' if match.group(3) else 'enum'
                name = match.group(1) or match.group(2) or match.group(3) or match.group(4)
                
                new_scope = {
                    'type': scope_type,
                    'name': name,
                    'children': [],
                    'doc': self._extract_doc_comment(content[:match.start()])
                }
                
                scope_stack[-1]['children'].append(new_scope)
                scope_stack.append(new_scope)
            
            pos = match.end()
            
        return scope_stack[0]

    def _extract_doc_comment(self, text: str) -> Optional[DocComment]:
        """Extract documentation comment with enhanced tag parsing"""
        doc_pattern = r'/\*\*\s*(.*?)\s*\*/'
        match = re.search(doc_pattern, text, re.DOTALL)
        
        if not match:
            return None
            
        comment = match.group(1)
        
        # Parse documentation tags
        brief = self._extract_tag(comment, 'brief', single=True)
        details = self._extract_details(comment)
        params = self._extract_params(comment)
        returns = self._extract_tag(comment, 'returns?', single=True)
        see_also = self._extract_tag(comment, 'see')
        notes = self._extract_tag(comment, 'note')
        examples = self._extract_tag(comment, 'example')
        
        return DocComment(
            brief=brief or "",
            details=details,
            params=params,
            returns=returns or "",
            see_also=see_also,
            notes=notes,
            examples=examples
        )

    def _enhance_descriptions(self, file_path: Path, scope_tree: Dict):
        """Add intelligent descriptions based on context"""
        module_path = file_path.relative_to(Path('Engine/Source'))
        module_context = self._get_module_context(module_path)
        
        def enhance_scope(scope: Dict, context: str):
            if not scope['doc'] or not scope['doc'].brief:
                scope['doc'] = self._generate_description(
                    scope['type'],
                    scope['name'],
                    context
                )
            
            for child in scope['children']:
                enhance_scope(child, f"{context}/{child['name']}")
                
        enhance_scope(scope_tree, str(module_path))

    def _generate_description(self, scope_type: str, name: str, context: str) -> DocComment:
        """Generate intelligent description based on scope type and context"""
        # First check module-specific patterns
        for module, info in self.MODULE_PATTERNS.items():
            if module in context:
                for submodule, desc in info['submodules'].items():
                    if submodule in name:
                        return DocComment(
                            brief=desc,
                            details="",
                            params={},
                            returns="",
                            see_also=[],
                            notes=[],
                            examples=[]
                        )
        
        # Fall back to generic patterns
        return DocComment(
            brief=f"{scope_type.title()} for handling {name.lower()} operations",
            details="",
            params={},
            returns="",
            see_also=[],
            notes=[],
            examples=[]
        )

    def _get_module_context(self, file_path: Path) -> str:
        """Determine module context from file path"""
        parts = file_path.relative_to(Path('Engine/Source')).parts
        return ' '.join(parts[:-1]) if len(parts) > 1 else parts[0]

    def _extract_tag(self, comment: str, tag: str, single: bool = False) -> Optional[str]:
        """Extract a single tag from a documentation comment"""
        tag_pattern = r'@' + tag + r'\s+(.*?)(?=@|$)'
        match = re.search(tag_pattern, comment, re.DOTALL)
        
        if not match:
            return None
            
        if single:
            return match.group(1).strip()
        else:
            return [ref.strip() for ref in re.findall(tag_pattern, comment)]

    def _extract_details(self, comment: str) -> str:
        """Extract detailed description from a documentation comment"""
        details = ""
        if '@brief' in comment:
            next_tag = re.search(r'@\w+', comment[comment.find('@brief') + 5:])
            if next_tag:
                details = comment[comment.find('@brief') + 5:comment.find('@brief') + 5 + next_tag.start()].strip()
            else:
                details = comment[comment.find('@brief') + 5:].strip()
        
        return details

    def _extract_params(self, comment: str) -> Dict[str, str]:
        """Extract parameter information from a documentation comment"""
        param_pattern = r'@param\s+(\w+)\s+(.*?)(?=@|$)'
        return {name: desc.strip() for name, desc in re.findall(param_pattern, comment)}

    def _format_output(self, file_path: Path, file_doc: Dict, scope_tree: Dict) -> Dict:
        """Format the output for the API documentation"""
        # This method is not implemented in the original code block
        # It's left unchanged as it's not part of the code block
        pass

    # Additional parsing methods... 