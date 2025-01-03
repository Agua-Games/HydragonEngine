# Hydragon Engine Coding Standards

This document outlines the coding standards for all development in the Hydragon Engine.

## General Standards

- Always include at the top of the file, inside proper comment brackets
- for each language and standardized, the copyright notice below:
- Copyright (c) 2024 Agua Games. All rights reserved.
- Licensed under the Agua Games License 1.0
- Adopt the standard for the coding language.
- standardize code comments.
- standardize code documentation.
- standardize code documentation blocks.
- Always implement error catching and detection.
- Always implement logging.
- Always implement documentation, but strive to be concise.
- Always implement code formatting, following each language's standard.
- Always use unified namespace style.
- Avoid absolute paths as much as possible.
- Avoid hardcoded values, paths and any value which should be procedural,
- configurable or dynamic.
- Always have in mind the existing engine's codebase and its structure, its
- existing API and documentation.
- C++ SPECIFIC:
- Adopt the C++ Core Guidelines standard for C++ code.
- standardize C++ code guards. Ex: #pragma once
- always check includes for correct usage, consistency, and correct order.

## Language-Specific Standards

### C++ Standards
- Adopt the C++ Core Guidelines standard for C++ code.
- standardize C++ code guards. Ex: #pragma once
- always check includes for correct usage, consistency, and correct order.

### Python Standards
- Adopt the Python standard for Python code. Prefer PEP 8.

## Implementation Notes

- These standards are enforced through automated tooling in the CI/CD pipeline
- Documentation coverage is automatically checked and must maintain >80% coverage
- Style violations will trigger build warnings
- Breaking changes to these standards require team review 