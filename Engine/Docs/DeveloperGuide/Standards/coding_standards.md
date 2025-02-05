# Hydragon Engine Coding Standards

This document outlines the coding standards for all development in the Hydragon Engine.

## General Standards

- Include copyright notice in file headers
- Standardize comments, docs, and doc blocks
- Implement error catching and logging
- Follow language-specific formatting
- Use unified namespace style
- Avoid absolute paths and hardcoded values
- Maintain consistency with existing codebase
- Never call undeclared functions/variables
- Acknowledge uncertainty, avoid speculation
- Balance speed and accuracy in analysis
- Maintain holistic codebase understanding
- Implement comprehensive logging
- Be explicit about uncertainties
- Focus on core tasks, avoid peripheral issues
- Consider full context before new implementations
- State task understanding before implementation
- Strive for concise, clear, maintainable code
- Propose minimal necessary solutions
- Confirm before expanding scope or major changes
- Minimize third-party library usage
- Follow CI/CD requirements:
- >80% doc coverage
- Resolve style violations
- Team review for breaking changes


## Language-Specific Standards

### C++ Standards

- Follow C++ Core Guidelines
- Use #pragma once for include guards
- Verify includes for correct usage and order


### Python Standards

- Follow PEP 8 guidelines
- Use type hints where appropriate
- Maintain clean imports organization


## Implementation Notes

- These standards are enforced through automated tooling in the CI/CD pipeline
- Documentation coverage is automatically checked and must maintain >80% coverage
- Style violations will trigger build warnings
- Breaking changes to these standards require team review
