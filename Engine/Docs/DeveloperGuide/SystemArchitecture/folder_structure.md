# Engine Folder Structure Documentation
(as this list is not yet automated, please update it as needed)

## Main Directory Structure

### Root Directories
- `Engine/` - Root engine directory
  - `Build/` - Build system files
    - `CI/` - Continuous Integration configuration
  - `Source/` - Source code
  - `Tools/` - Compiled tools and utilities
    - `BuildTools/` - Build utilities
    - `Documentation/` - Documentation tools
  - `Docs/` - Documentation
    - `DeveloperGuide/` - Developer documentation
      - `SystemArchitecture/` - System architecture docs
    - `UserGuide/` - User documentation

### Core Systems (`/Engine/Source/Core/`)

#### AI
- `/Engine/Source/Core/AI/Behavior/` - AI behavior tree and decision making systems
- `/Engine/Source/Core/AI/Generation/` - AI-driven content generation tools
- `/Engine/Source/Core/AI/PathFinding/` - Navigation and pathfinding algorithms
- `/Engine/Source/Core/AI/Pipeline/` - AI processing and inference pipeline
- `/Engine/Source/Core/AI/Training/` - AI model training infrastructure

#### Animation & Physics
- `/Engine/Source/Core/Animation/Rigging/` - Character rigging and skeletal animation
- `/Engine/Source/Core/Physics/Collision/` - Physics collision detection and response

#### Asset Management
- `/Engine/Source/Core/Assets/HotReload/` - Hot reload functionality for assets
- `/Engine/Source/Core/Resources/Cache/` - Resource caching system
- `/Engine/Source/Core/Resources/Compression/` - Asset compression algorithms
- `/Engine/Source/Core/Resources/Packaging/` - Content packaging tools
- `/Engine/Source/Core/Resources/Streaming/` - Asset streaming system
- `/Engine/Source/Core/Resources/Version/` - Resource version control

#### Audio
- `/Engine/Source/Core/Audio/Effects/` - Audio effects processing
- `/Engine/Source/Core/Audio/Spatializer/` - 3D audio spatialization
- `/Engine/Source/Core/Audio/Streaming/` - Audio streaming system

#### Core Infrastructure
- `/Engine/Source/Core/Collaboration/` - Multi-user collaboration features
- `/Engine/Source/Core/Config/` - Engine configuration system
- `/Engine/Source/Core/Data/` - Core data structures and management
- `/Engine/Source/Core/Plugin/` - Plugin system infrastructure
- `/Engine/Source/Core/Tasks/` - Task and job management
- `/Engine/Source/Core/Utilities/` - Common utility functions

#### Debug & Profiling
- `/Engine/Source/Core/Debug/Visualization/` - Debug visualization tools

#### Graphics
- `/Engine/Source/Core/Graphics/Lighting/` - Lighting and shadow systems
- `/Engine/Source/Core/Graphics/Materials/` - Material system
- `/Engine/Source/Core/Graphics/Meshlet/` - Meshlet processing
- `/Engine/Source/Core/Graphics/Pipeline/` - Rendering pipeline
- `/Engine/Source/Core/Graphics/PostProcess/` - Post-processing effects
- `/Engine/Source/Core/Graphics/RenderAPI/` - Rendering API abstraction

#### Hardware & Platform
- `/Engine/Source/Core/Hardware/CPU/` - CPU-specific optimizations
- `/Engine/Source/Core/Hardware/GPU/` - GPU capabilities and management
- `/Engine/Source/Core/Platform/Linux/` - Linux platform support
- `/Engine/Source/Core/Platform/Mobile/` - Mobile platform support
- `/Engine/Source/Core/Platform/Windows/` - Windows platform support

#### Input
- `/Engine/Source/Core/Input/Devices/` - Input device support
- `/Engine/Source/Core/Input/Mapping/` - Input mapping system

#### Integration
- `/Engine/Source/Core/Integration/AI/` - AI service integration
- `/Engine/Source/Core/Integration/DCC/` - Digital content creation tool integration

#### Memory Management
- `/Engine/Source/Core/Memory/Debug/` - Memory debugging tools
- `/Engine/Source/Core/Memory/GPU/` - GPU memory management
- `/Engine/Source/Core/Memory/Management/Strategies/` - Memory allocation strategies
- `/Engine/Source/Core/Memory/NUMA/` - NUMA-aware memory management
- `/Engine/Source/Core/Memory/Profiling/` - Memory profiling tools
- `/Engine/Source/Core/Memory/Threading/` - Thread-safe memory operations
- `/Engine/Source/Core/Memory/Types/` - Memory type definitions
- `/Engine/Source/Core/Memory/Virtualization/` - Virtual memory management

#### Navigation & Networking
- `/Engine/Source/Core/Navigation/Streaming/` - Navigation data streaming
- `/Engine/Source/Core/Network/Prediction/` - Network prediction systems
- `/Engine/Source/Core/Network/Replication/` - Network state replication
- `/Engine/Source/Core/Network/Security/` - Network security features
- `/Engine/Source/Core/Network/Transport/` - Network transport layer

#### Node System
- `/Engine/Source/Core/Nodes/Execution/` - Node execution engine
- `/Engine/Source/Core/Nodes/Graph/` - Node graph system
- `/Engine/Source/Core/Nodes/Types/` - Node type definitions

#### Output System
- `/Engine/Source/Core/Output/Encoders/` - Output encoding systems
- `/Engine/Source/Core/Output/Platforms/` - Platform-specific output handling
- `/Engine/Source/Core/Output/RenderTargets/` - Render target management

#### Procedural Generation
- `/Engine/Source/Core/Procedural/Algorithms/` - Procedural generation algorithms

#### Scene Management
- `/Engine/Source/Core/Scene/Culling/` - Scene culling systems
- `/Engine/Source/Core/Scene/Description/` - Scene description framework
- `/Engine/Source/Core/Scene/Graph/` - Scene graph management
- `/Engine/Source/Core/Scene/Serialization/` - Scene serialization

#### Scripting
- `/Engine/Source/Core/Scripting/Bindings/` - Script language bindings
- `/Engine/Source/Core/Scripting/Debug/` - Script debugging tools
- `/Engine/Source/Core/Scripting/HotReload/` - Script hot reload system
- `/Engine/Source/Core/Scripting/Nodes/` - Script node system
- `/Engine/Source/Core/Scripting/VM/` - Script virtual machine

#### Terrain
- `/Engine/Source/Core/Terrain/Rendering/` - Terrain rendering system
- `/Engine/Source/Core/Terrain/Streaming/` - Terrain data streaming

#### UI System
- `/Engine/Source/Core/UI/Animation/` - UI animation system
- `/Engine/Source/Core/UI/Framework/` - Core UI framework
- `/Engine/Source/Core/UI/Widgets/` - UI widget library

### Development Tools (`/Engine/Source/DevTools/`)
- `/Engine/Source/DevTools/Build/Dependency/` - Build dependency management
- `/Engine/Source/DevTools/Debug/Console/` - Debug console implementation
- `/Engine/Source/DevTools/Experimental/` - Experimental features
- `/Engine/Source/DevTools/TestFrameworks/` - Testing infrastructure

### Editor (`/Engine/Source/Editor/`)
- `/Engine/Source/Editor/Core/` - Core editor functionality
- `/Engine/Source/Editor/Tools/AssetManager/` - Asset management tools
- `/Engine/Source/Editor/Tools/Plugins/` - Editor plugin system
- `/Engine/Source/Editor/Tools/UI/` - Editor UI tools

### Tools (`/Engine/Source/Tools/`)
- Standalone tools and utilities

Note: This documentation reflects only the folders that currently exist in the codebase. Additional folders will be documented as they are implemented.
