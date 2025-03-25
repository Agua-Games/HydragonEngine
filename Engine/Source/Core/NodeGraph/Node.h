/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Node.h
 * @brief Node is the fundamental building block in the engine's
 * node-graph centric architecture. All functionality is
 * represented as nodes or connections between nodes.
 * 
 * ARCHITECTURAL NOTES:
 * - Node inherits from Object, which provides the core features of all objects in the system. 
 * See Object.h to understand the core architecture of Hydragon's data objects, and the Object interface.
 * - Node's interface is designed around the core phases of a Node's lifecycle in hardware, in proper order: initialization, 
 * update, and cleanup. They're further broken down into sub-phases, like:
 * 
 *      - Memory Allocation, Initialization, Loading
 *      - Resource Allocation, Initialization, Loading
 *      - Validation: Ensure correctness before use (e.g. input/output connections)
 *      - Caching: Store and retrieve data efficiently
 *      - Streaming & Data Handling: Load from disk/network, dynamic data, etc.
 *      - Processing (CPU/GPU Workload): Transform, updates, physics, logic
 *      - Execution (Final Computation & Rendering) Submitting GPU/CPU workloads
 *      - Finalization & Cleanup: Deallocation, Destruction
 * 
 * - Nodes are the primary entities in the system, and represent all functionality, whether visual, logical, or data-oriented.
 * - It provides enhanced support for Object's features, and also:
 *      - Property management
 *      - Port management
 *      - Node graph management
 *      - Procedural generation
 *      - Caching
 *      - Streaming
 *      - Processing, Execution
 *      - Versioning
 *      - Dependency management
 *      - Compilation
 *      - Custom Node Implementation
 *      - Custom visualization
 *      - Scripting
 *      - AI Agent Integration
 * 
 * - Nodes can be connected to form complex systems.
 * - A Node is a Node Graph - they're one and the same. As soon as a Node contains other Nodes, it becomes a Node Graph - but it's still a Node.
 * - Inputs and outputs are defined using the Property system. Input and output Ports are the UI representation of input and output Properties.
 * 
 * TODO:
 * - Move implementation (of functions) to .cpp file.
 * - Questions to ask assistant:
 *          - Modify struct PortValidationState to hold a more comprehensive set of Port features, and call it PortInfo. Unify all port info into it.
			- Why put properties, runtimeProperties, inputs and outputs in "protected:"?
            - Clarify commandValidator: related to portsm or any command?
            - Caching interface (vars, methods) in public or private?
			- Why have a separate variable for propertyInfos? Isn't it redundant with properties?
		- Eliminate all redundant code
            - Redundancy between setInputValue() and setPortValue()?
			- struct Property, in Object.h; struct PropertyInfo; classes PropertyBase and Property, in Property.h
			Keep only the best and most fitting to the engine architecture. Move useful parts from eliminated ones to the remaining one.
		- Eliminate unused and obsolete structs, variables and functions.
		- Organize code into logical sections and functions, proper order of declaration, etc.

		- Ask assistant to explain "class Node : public Object, public std::enable_shared_from_this<Node>" 
		(shared_from_this).
		- Document Object, Node, Scene and Command so that they are self-documented, clear, easy to make sense of.
		- Separar claramente funcionalidade Core de Avançada e Secundária.
 * - Move Debugging features (DebugInfo, functions) to Object. Override as needed, here.
 * - Move Streaming features to object. *Consult assistant.
 * - Move Caching features to object. Node should override to allow caching for properties, inputs, outputs.
 * - Move AI features to Object?
 * - Refactor NodeInfo to be a template parameter of Node.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 *   (Remember to not implement in Node what is supposed to be in Object).
 * - Flesh out the class and its methods, structs, enums, etc.
 */

 #pragma once
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <any>
#include <future>
#include <chrono>

#include "Object.h"
#include "NodeManager.h"
#include "NodeCompilationContext.h"
#include "CodeGenContext.h"
#include "DependencyGraph.h"
#include "CommandValidator.h"

namespace hd {

/*
 * @brief Metadata and attributes for the node
 */
struct NodeInfo : public ObjectInfo {
    std::string nodeType;               
    std::vector<std::string> inputs;   
    std::vector<std::string> outputs;  
    
    struct StreamingConfig {
        bool enabled = true;
        bool forceSync = false;
        size_t chunkSize = 1024;        
        float priorityThreshold = 0.5f;
    } streamingConfig;

    NodeInfo(const std::string& name = "", 
             bool isSerializable = true,
             bool isEditableInEditor = true,
             const std::string& nodeType = "",
             const std::vector<std::string>& inputs = {},
             const std::vector<std::string>& outputs = {},
             bool enableStreaming = true)
        : ObjectInfo(name, isSerializable, isEditableInEditor),
          nodeType(nodeType), 
          inputs(inputs), 
          outputs(outputs) {
        streamingConfig.enabled = enableStreaming;
    }
};

/**@brief Base class for all nodes in the engine.
 * Nodes are the primary entities in the system and
 * represent all functionality, whether visual, logical,
 * or data-oriented.
 */
class Node : public Object, public std::enable_shared_from_this<Node> {
public:
    // === Structure Definitions ===
    // For properties (input/output)
    struct PropertyInfo {
        std::string name;
        std::string type;
        std::string defaultValue;
        std::string description;
        std::string validation;
        std::string uiHints;        // How to display in UI/property panel
        
        // Core flags
        bool isInput = false;       // Can receive values from other nodes
        bool isOutput = false;      // Can send values to other nodes
        bool isRuntime = false;     // Can change during runtime
        
        // UI/Editor flags
        bool isRequired = false;    // Must have a value/connection
        bool isAdvanced = false;    // Hidden by default in UI
        bool isExposed = false;     // Available to external scripts
        
        // System flags
        bool isSerializable = true; // Should be saved/loaded
        bool isConnectable = false; // Can be connected in node graph
    };

    // For ports (input/output)
    struct PortValidationState {
        bool isValid = true;
        std::string lastError;
    };

    struct StreamingState {
        bool isStreaming = false;
        size_t currentChunk = 0;
        size_t totalChunks = 0;
        float priority = 0.0f;
    };

    struct ExecutionState {
        size_t currentStep = 0;
        bool isComplete = false;
        std::any intermediateResult;
        std::chrono::steady_clock::time_point lastYield;
        virtual ~ExecutionState() = default;
    };

    // For custom implementation - implementation which can override the original implementation "in-place" and externally (out of the class definition), 
    // for example right after a node is created, in a node graph creation code file.
    struct CustomizableElements {
        std::string processFunction;
        std::string initializeFunction;
        std::string cleanupFunction;
        std::string validateFunction;     // Input validation logic
        std::string cacheFunction;
        std::string optimizeFunction;
    };

    struct ScriptingInterface {
        // Code templates for different languages
        struct CodeTemplates {
            std::string cpp;
            std::string python;
            std::string lua;
            std::string graph; // Visual scripting
        };
        
        // Language-specific metadata
        struct LanguageMetadata {
            std::string imports;          // Required imports/includes
            std::string baseClass;        // Language-specific base class
            std::string interfaceClass;   // Interface to implement
            std::vector<std::string> attributes; // Language-specific attributes
        };

        // Debugging interface
        struct DebugInterface {
            std::vector<std::string> breakpointLocations;
            std::vector<std::string> watchExpressions;
            std::map<std::string, std::string> stateInspection;
        };

        CodeTemplates templates;
        LanguageMetadata metadata;
        DebugInterface debugInfo;
    };

    struct ScriptingState{                            // Scripting state tracking
        std::string currentLanguage;
        bool isDirty = false;
        std::string lastValidCode;
        std::vector<std::string> errorLog;
    };

    struct AIInterface {
        // Task descriptions for AI manipulation
        struct TaskDescription {
            std::string intent;                     // What the task aims to achieve
            std::string constraints;                // Limitations and requirements
            std::vector<std::string> prerequisites; // Required conditions
            std::vector<std::string> effects;       // Expected outcomes
        };

        // Node manipulation capabilities
        struct ManipulationCapabilities {
            bool canModifyPorts;
            bool canModifyLogic;
            bool canModifyProperties;
            bool canCreateConnections;
            bool canOptimize;
        };

        // Semantic information for AI understanding
        struct SemanticInfo {
            std::string purpose;             // Node's primary function
            std::string domain;              // Domain-specific context
            std::vector<std::string> tags;   // Semantic tags
            std::string relationships;       // Relationship with other nodes
        };

        // Performance metrics for AI optimization
        struct PerformanceMetrics {
            float computationalCost;
            float memoryUsage;
            float latency;
            std::vector<std::string> bottlenecks;
        };

        TaskDescription taskDesc;
        ManipulationCapabilities capabilities;
        SemanticInfo semantics;
        PerformanceMetrics metrics;
    };

    struct AIState{                                    // AI agent state tracking
        std::vector<std::string> appliedTasks;
        std::map<std::string, float> taskSuccess;
        std::vector<std::string> rejectedTasks;
        bool isUnderAIModification = false;
    };
    
    struct DebugInfo {
        std::vector<std::string> watchedVariables;
        std::vector<std::string> breakpoints;
        std::string profileHints;                   // Performance profiling hints
    };

    struct CacheEntry {
        bool isValid = false;
        uint64_t inputHash = 0;
        std::vector<std::any> outputs;
        // Derived classes can add their specific output cache here
    };

    // === Allocation, Initialization, Loading ===
    // Constructor and destructor
    explicit Node(const std::string& name = "");
    virtual ~Node() = default;

    /**
     * @brief Create a node of the specified type.
     * It's basically a wrapper around NodeManager::createNode. Used for setting up node graphs in code, in fluent style, among other things.
     * @param name The name of the node.
     */
    static std::shared_ptr<Node> create(const std::string& name) {
        return NodeManager::get().createNode(name);
    }
    
    /**
     * @brief Create a node of the specified type.
     * It's basically a wrapper around NodeManager::createNode. Used for setting up node graphs in code, in fluent style, among other things.
     * @param name The name of the node.
     */
    template<typename T>
    static std::shared_ptr<T> Create(const std::string& name = "") {
        return NodeManager::get().createNode<T>(name);
    }

    bool initialize() override {
        if (!Object::initialize()) return false;

        // Initialize node-specific resources
        if (!initializeNodeResources()) return false;

        // Initialize cache if supported
        if (canCache()) {
            resultCache = CacheEntry{};
            resultCache.isValid = false;
        }

        // Call onResume as part of initialization
        onResume();

        return true;
    }

    // === Property Management ===
    /**
     * @brief Set a property value.
     * Fluent property interface - for setting up node graphs in code, using fluent style.
     */
    template<typename T>
    Node& property(const std::string& name, const T& value, bool runtime = false) {
        if (runtime) {
            runtimeProperties[name] = value;
        } else {
            properties[name] = value;
        }
        onDirty();

        // Auto-register if not defined
        if (propertyInfos.find(name) == propertyInfos.end()) {
            PropertyInfo info;
            info.name = name;
            info.type = typeid(T).name();
            info.isRuntime = runtime;
            propertyInfos[name] = info;
        }
        
        return *this;
    }

    // Define property with full metadata
    void defineProperty(const std::string& name, const PropertyInfo& def) {
        propertyInfos[name] = def;
    }

    // Enhanced connection method that works with property system
    Node& connect(const std::string& sourceProperty, const std::string& targetPath) {
        // Verify source property exists and is connectable
        auto srcMetadataIt = propertyInfos.find(sourceProperty);
        if (srcMetadataIt == propertyInfos.end() || !srcMetadataIt->second.isConnectable) {
            throw std::runtime_error("Source property not found or not connectable");
        }

        auto [targetNode, targetProperty] = NodeManager::get().parsePortPath(targetPath);
        
        // Verify target property exists and is connectable
        auto targetMetadataIt = targetNode->propertyInfos.find(targetProperty);
        if (targetMetadataIt == targetMetadataIt->end() || !targetMetadataIt->second.isConnectable) {
            throw std::runtime_error("Target property not found or not connectable");
        }

        // Use existing connection system
        NodeManager::get().connect(shared_from_this(), targetNode, sourceProperty, targetProperty);
        return *this;
    }


    // === Port Management ===
    // Ports are used for UI - visual node graph editor, state, sync, etc.
    // Input and output ports correspond to input and output properties, which are the actual connections between nodes.
    std::unordered_map<std::string, PortValidationState> portValidationState;

    /**
     * @brief Get the input ports for this node. 
     * This method should be implemented in derived classes.
     */
    virtual std::vector<std::string> getInputPorts() const override {
        std::vector<std::string> ports;
        for (const auto& [name, metadata] : propertyInfos) {
            if (metadata.isInput) ports.push_back(name);
        }
        return ports;
    }

    /**
     * @brief Get the output ports for this node. 
     * This method should be implemented in derived classes.
     */
    virtual std::vector<std::string> getOutputPorts() const override {
        std::vector<std::string> ports;
        for (const auto& [name, metadata] : propertyInfos) {
            if (metadata.isOutput) ports.push_back(name);
        }
        return ports;
    }

    template<typename T>
    bool setInputValue(const std::string& name, const T& value);

    /**
     * @brief Set the value of a port. Type-safe port system.
     * @param portName Name of the port.
     * @param value Value to set.
     */
    template<typename T>
    void setPortValue(const std::string& portName, T&& value);

    template<typename T>
    T getPortValue(const std::string& portName) const;

    // === Validation ===
    std::shared_ptr<CommandValidator> validator;

    void setValidator(std::shared_ptr<CommandValidator> v) { validator = std::move(v); }
    const std::shared_ptr<CommandValidator>& getValidator() const { return validator; }

    template<typename T>
    void addPortValidationRule(const std::string& portName,
                             std::function<bool(const T&)> rule,
                             const std::string& errorMsg);

    bool isPortValid(const std::string& name) const;
    std::string getPortError(const std::string& name) const;

    /**
     * @brief Validates object state and connections
     */
    virtual bool validate() const {
        // Check property validity
        for (const auto& [id, prop] : properties) {
            if (!validateProperty(id, prop)) return false;
        }

        // Check dependency validity
        for (const auto& dep : dependencies) {
            if (auto ptr = dep.lock()) {
                if (!ptr->validate()) return false;
            } else {
                return false; // Invalid weak_ptr
            }
        }

        return true;
    }

    // === Node Graph Management ===
    const NodeInfo& getNodeInfo() const { return nodeInfo; }

    template<typename T>
    std::shared_ptr<T> add(const std::string& name = "") {
        auto node = NodeManager::get().createNode<T>(name);
        children.push_back(node);
        return node;
    }

    /**
     * @brief Add a child node to this node.
     * @param child The child node to add.
     */
    void addChild(const std::shared_ptr<Node>& child);

    /**
     * @brief Remove a child node by name.
     * TODO: if we're going to allow instances with the same name, we need to use a different approach, like remove a child by unique ID (probably automatically generated)
     * or remove a child by pointer.
     * @param childName The name of the child node to remove.
     * @return True if the child was found and removed, false otherwise.
     */
    bool removeChild(const std::string& childName);

    /**
     * @brief Get all child nodes of this node.
     * @return A vector of shared pointers to child nodes.
     */
    const std::vector<std::shared_ptr<Node>>& getChildren() const;

    // === Procedural Generation ===
    // Every node can:
    virtual void evolve();        // Change over time
    virtual void adapt();         // Respond to context
    virtual void selfModify();    // Modify its own behavior
    virtual void harmonize();     // Maintain system balance
    
    // === Caching & Optimization ===
    mutable CacheEntry resultCache;
    bool isDirty;

    /**
     * @brief Mark this node as dirty, indicating that its state has changed.
     */
    virtual void markDirty();

    virtual bool canCache() const { return true; }
    virtual uint64_t computeCacheKey() const = 0;
    virtual void generateRuntimeCode(CodeGenContext& context);

    bool tryUseCache() const {
        if (!resultCache.isValid) return false;
        
        uint64_t currentHash = computeCacheKey();
        return currentHash == resultCache.inputHash;
    }

    void updateCache() {
        resultCache.inputHash = computeCacheKey();
        resultCache.isValid = true;
        // Derived classes should update resultCache.outputs
    }

    // === Streaming ===
    // Support for asynchronous, multi-threaded loading and streaming
    StreamingState streamingState;

    /**
     * @brief Asynchronously load this node's data.
     */
    virtual std::future<void> loadAsync();
    virtual void stream();
    virtual bool shouldStream() const;
    virtual void updateStreamingPriority();

    /**
     * @brief Synchronously stream this node's data. This is the default implementation, but derived classes can override it.
     */
    virtual void streamSync();
    virtual void streamAsync();

    // === Processing ===
    virtual void process() = 0;

    void update(float deltaTime) override {
        if (!isInitialized()) return;

        // Base class update
        Object::update(deltaTime);

        // update node state
        if (isDirty) {
            onDirty();
            isDirty = false;
        }

        // Try using cache if available
        if (canCache() && tryUseCache()) {
            return;
        }

        // Process node
        process();

        // Update cache if needed
        if (canCache()) {
            updateCache();
        }
    }
    
    // === Execution ===
    // Support for execution in play mode, pausing, resuming, etc.
    bool isPaused = false;
    bool autoResumeAfterModification = true;
    mutable std::mutex modificationMutex;
    
    /**
     * @brief Modify this node in play mode.
     * @param modifier Function to apply modifications.
     */
    virtual void modifyInPlayMode(const std::function<void(Node*)>& modifier);
    virtual void pauseExecution();
    virtual void resumeExecution();

    // Event Handlers
    virtual void onResume() = 0;
    virtual void onPause() = 0;
    virtual void onDirty() = 0;

    // === Compilation ===
    virtual bool canCompile() const { return false; }
    virtual void contributeToCompilation(CompilationContext& context);

    // Optimization hints
    virtual bool hasConstantOutput() const { return false; }
    virtual bool canInline() const { return false; }
    virtual bool requiresFullPrecision() const { return true; }

    // === Custom Node Implementation ===
    virtual CustomizableElements exportToCustomNode() const;            // Convert built-in node to custom
    virtual void importCustomImplementation(const CustomizableElements& elements);

    // === Serialization ===
    /**
     * @brief serialize this node and its children.
     * @param stream Output stream for serialization.
     */
    void serialize(std::ostream& stream) const override;

    /**
     * @brief deserialize this node and its children.
     * @param stream Input stream for deserialization.
     */
    void deserialize(std::istream& stream) override;

    // === Visualization ===
    /**
     * @brief Draw this node in the Properties panel.
     * Ports will be automatically drawn based on their data types, etc. But for custom visualization logic, override this method in derived classes.
     */
    virtual void drawInPropertyEditor();

    /**
     * @brief Draw this node in the Node Graph Editor.
     * Nodes will be automatically drawn based on their data types, structure, etc. But for custom visualization logic, override this method in derived classes.
     */
    virtual void drawInNodeGraph();

    // === Debug/Development ===
    DebugInfo debugInfo;
    virtual void debugDraw() const;
    virtual void debugLog() const;

    // === Scripting ===        // Including support for Scripting Editor and external IDEs
    // Scripting Editor methods
    virtual ScriptingInterface getScriptingInterface() const;
    virtual void exportToScriptingEditor();
    virtual void importFromScriptingEditor(const std::string& code, const std::string& language);
    
    // External IDE methods
    virtual std::string generateExternalEditorFile() const;
    virtual void importFromExternalEditor(const std::string& sourceCode);

    // === AI Agent Integration ===
    virtual AIInterface getAIInterface() const {
        AIInterface interface;
        // Populate interface based on node type
        return interface;
    }

    // AI-assisted optimization
    virtual std::vector<std::string> getOptimizationSuggestions() const {
        return {}; // Return potential optimization opportunities
    }

    // === Cleanup ===
    void cleanup() override {
        if (!isInitialized()) return;

        // Call onPause before cleanup
        onPause();

        // Cleanup node-specific resources
        cleanupNodeResources();

        // Clear cache
        resultCache = CacheEntry{};

        // Base class cleanup
        Object::cleanup();
    }


protected:
    // === Resource Management ===
    virtual bool initializeNodeResources() {
        // Initialize any node-specific resources
        return true;
    }

    virtual void cleanupNodeResources() {
        // Cleanup any node-specific resources
        children.clear();
        properties.clear();
        runtimeProperties.clear();
    }

    // === Core Interface, Lifecycle ===
    NodeInfo nodeInfo;                           // Metadata and attributes for the node (name, description, category, etc.)
    virtual void Load();                         // Load node-specific data

    // === Property Management ===
    // TODO: Cleanup these variables below, simplify, remove unused
    // TODO: This may be refactored after we properly design and test the Reflection subsystem
    std::vector<PropertyInfo> properties;
    std::unordered_map<std::string, std::any> runtimeProperties;    // Runtime properties (can change during runtime)
    std::vector<PropertyInfo> inputs;
    std::vector<PropertyInfo> outputs;

    // === Node Graph Management ===
    std::vector<std::shared_ptr<Node>> children; // Child nodes (making this a node graph)
    mutable std::mutex childrenMutex;            // Mutex for thread-safe access to children

    // === Serialization ===
    mutable std::mutex serializationMutex;       // Mutex for thread-safe serialization

private:
    // === Port Management ===
    virtual std::string getPortType(const std::string& portName) const = 0;
    virtual std::string getPortDefaultValue(const std::string& portName) const = 0;
    virtual std::string getPortValidation(const std::string& portName) const;
    virtual bool validatePort(const std::string& portName, const std::any& value) const;
    virtual std::string getPortDescription(const std::string& portName) const = 0;
    virtual bool isPortRequired(const std::string& portName) const = 0;

    // === Caching & Optimization ===
    uint64_t calculateInputHash() const;
    void storeOutputsToCache();
    void restoreOutputsFromCache();

    // === Custom Node Implementation ===
    virtual std::string getProcessFunctionImpl() const = 0;
    virtual std::string getInitializeFunctionImpl() const = 0;
    virtual std::string getCleanupFunctionImpl() const = 0;
    bool validateCustomImplementation(const CustomizableElements& elements);
    void applyCustomImplementation(const CustomizableElements& elements);
    std::string generateNodeSourceFile(const CustomizableElements& elements) const;
    CustomizableElements parseNodeSourceFile(const std::string& sourceCode);

    // === Scripting ===
    ScriptingState scriptingState;
    virtual void generateLanguageSpecificCode(const std::string& language) = 0;
    virtual void validateGeneratedCode(const std::string& code) = 0;
    virtual void applyCodeChanges(const std::string& code) = 0;

    // === AI Agent Integration ===
    // AI agent state tracking
    AIState aiState;

    // AI safety and validation
    virtual bool isAIManipulationAllowed() const { return true; }
    virtual bool validateAIChanges(const std::vector<std::string>& changes) = 0;
    virtual bool validateAITask(const std::string& taskIntent, const std::map<std::string, std::any>& parameters) = 0;
    virtual bool applyAITask(const std::string& taskIntent, const std::map<std::string, std::any>& parameters) = 0;
    virtual std::string getNodeSemantics() const = 0;
    virtual std::vector<std::string> getSafetyConstraints() const = 0; 
};
    
} // namespace hd

#endif
