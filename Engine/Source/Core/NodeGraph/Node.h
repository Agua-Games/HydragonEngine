/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * Node is the fundamental building block in the engine's
 * node-graph centric architecture. All functionality is
 * represented as nodes or connections between nodes.
 * 
 * ARCHITECTURAL NOTE:
 * - Nodes are the primary entities in the system.
 * - Nodes can be visual, logical, or data-oriented.
 * - Nodes can be connected to form complex systems.
 * - A Node is a Node Graph - they're one and the same. As soon as a node contains other nodes, it becomes a node graph - but it's still a node.
 * - Inputs and outputs are defined using the property system. Input and output ports are the UI representation of input and output properties.
 * - Node inherits Object's features and also supports custom nodes, scripting interchange (code/UI), streaming, AI tasks.
 * 
 * TODO:
 * - Fix capital letters for variables - should start with lowercase.
 * - Fix capital letters for functions - should start with lowercase.
 * - Move implementation (of functions) to .cpp file.
 * - Move Debugging features (DebugInfo, functions) to Object. Override as needed, here.
 * - Move Streaming features to object? *Consult assistant.
 * - Move AI features to Object?
 * - Refactor caching to be an override from Object (moved from Node to Object).
 * - Study this initial design sketch thoroughly and eliminate redundant code.
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

struct NodeInfo : public ObjectInfo {
    std::string NodeType;               
    std::vector<std::string> Inputs;   
    std::vector<std::string> Outputs;  
    
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
          NodeType(nodeType), 
          Inputs(inputs), 
          Outputs(outputs) {
        streamingConfig.enabled = enableStreaming;
    }
};

/**@brief Base class for all nodes in the engine.
 * Nodes are the primary entities in the system and
 * represent all functionality, whether visual, logical,
 * or data-oriented.
 */
class Node : public Object {
public:
    // === Structure definitions ===
    // For properties
    struct PropertyDefinition {
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
    std::vector<PropertyDefinition> properties;

    // For ports (input/output)
    struct PortValidationState {
        bool isValid = true;
        std::string lastError;
    };
    std::unordered_map<std::string, PortValidationState> portValidationState;

    // === Core Lifecycle Methods ===
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

    void update(float deltaTime) override {
        if (!IsInitialized()) return;

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

    void Cleanup() override {
        if (!IsInitialized()) return;

        // Call onPause before cleanup
        onPause();

        // Cleanup node-specific resources
        cleanupNodeResources();

        // Clear cache
        resultCache = CacheEntry{};

        // Base class cleanup
        Object::Cleanup();
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

    // === Property & Reflection Management ===
    // TODO: This may be refactored after we properly design and test the Reflection subsystem

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
        
        // Auto-register if not defined
        if (propertyDefs.find(name) == propertyDefs.end()) {
            PropertyDefinition def;
            def.name = name;
            def.type = typeid(T).name();
            def.isRuntime = runtime;
            propertyDefs[name] = def;
        }
        
        return *this;
    }

    // Define property with full metadata
    void defineProperty(const std::string& name, const PropertyDefinition& def) {
        propertyDefs[name] = def;
    }

    // Enhanced connection method that works with property system
    Node& connect(const std::string& sourceProperty, const std::string& targetPath) {
        // Verify source property exists and is connectable
        auto srcMetadataIt = propertyDefs.find(sourceProperty);
        if (srcMetadataIt == propertyDefs.end() || !srcMetadataIt->second.isConnectable) {
            throw std::runtime_error("Source property not found or not connectable");
        }

        auto [targetNode, targetProperty] = NodeManager::get().parsePortPath(targetPath);
        
        // Verify target property exists and is connectable
        auto targetMetadataIt = targetNode->propertyDefs.find(targetProperty);
        if (targetMetadataIt == targetMetadataIt->end() || !targetMetadataIt->second.isConnectable) {
            throw std::runtime_error("Target property not found or not connectable");
        }

        // Use existing connection system
        NodeManager::get().connect(shared_from_this(), targetNode, sourceProperty, targetProperty);
        return *this;
    }

    // === Validation Support ===
    std::shared_ptr<CommandValidator> validator;

    void setValidator(std::shared_ptr<CommandValidator> v) { validator = std::move(v); }
    const std::shared_ptr<CommandValidator>& getValidator() const { return validator; }

    template<typename T>
    bool setInputValue(const std::string& name, const T& value);

    bool isPortValid(const std::string& name) const;

    std::string getPortError(const std::string& name) const;

    template<typename T>
    void addPortValidationRule(const std::string& portName,
                             std::function<bool(const T&)> rule,
                             const std::string& errorMsg);

    // === Streaming Support ===
    struct StreamingState {
        bool isStreaming = false;
        size_t currentChunk = 0;
        size_t totalChunks = 0;
        float priority = 0.0f;
    };

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
    
    // === Execution Support ===
    struct ExecutionState {
        size_t currentStep = 0;
        bool isComplete = false;
        std::any intermediateResult;
        std::chrono::steady_clock::time_point lastYield;
        virtual ~ExecutionState() = default;
    };

    bool isPaused = false;
    bool autoResumeAfterModification = true;
    mutable std::mutex modificationMutex;

    /**
     * @brief Mark this node as dirty, indicating that its state has changed.
     */
    virtual void markDirty();
    
    /**
     * @brief Modify this node in play mode.
     * @param modifier Function to apply modifications.
     */
    virtual void modifyInPlayMode(const std::function<void(Node*)>& modifier);
    virtual void pauseExecution();
    virtual void resumeExecution();

    // === Compilation Support ===
    virtual bool canCompile() const { return false; }
    virtual void contributeToCompilation(CompilationContext& context);

    // Optimization hints
    virtual bool hasConstantOutput() const { return false; }
    virtual bool canInline() const { return false; }
    virtual bool requiresFullPrecision() const { return true; }

    // === Node Graph Interface, Event Handlers ===
    // Core virtual methods - to be implemented only in derived classes
    virtual void process() = 0;
    virtual void onResume() = 0;
    virtual void onPause() = 0;
    virtual void onDirty() = 0;
    virtual bool canCache() const { return true; }
    virtual uint64_t computeCacheKey() const = 0;
    virtual void generateRuntimeCode(CodeGenContext& context);

    // === Procedural Generation ===
    // Every node can:
    virtual void evolve();        // Change over time
    virtual void adapt();         // Respond to context
    virtual void selfModify();    // Modify its own behavior
    virtual void harmonize();     // Maintain system balance

    // === Custom Node Support ===
    struct CustomizableElements {
        std::string processFunction;      // Main processing logic
        std::string initializeFunction;   // Setup/initialization code
        std::string cleanupFunction;      // Cleanup/destruction code
        std::string validateFunction;     // Input validation logic
        std::string cacheFunction;        // Custom caching logic
        std::string optimizeFunction;     // Custom optimization hints
    };
    virtual CustomizableElements exportToCustomNode() const;            // Convert built-in node to custom
    virtual void importCustomImplementation(const CustomizableElements& elements);

    // === Port Management ===
    // Ports are used for UI - visual node graph editor, state, sync, etc.
    // Input and output ports correspond to input and output properties, which are the actual connections between nodes.
    std::unordered_map<std::string, std::any> runtimeProperties;
    std::vector<PropertyDefinition> inputs;
    std::vector<PropertyDefinition> outputs;

    /**
     * @brief Get the input ports for this node. 
     * This method should be implemented in derived classes.
     */
    virtual std::vector<std::string> getInputPorts() const override {
        std::vector<std::string> ports;
        for (const auto& [name, metadata] : propertyDefs) {
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
        for (const auto& [name, metadata] : propertyDefs) {
            if (metadata.isOutput) ports.push_back(name);
        }
        return ports;
    }

    /**
     * @brief Set the value of a port. Type-safe port system.
     * @param portName Name of the port.
     * @param value Value to set.
     */
    template<typename T>
    void setPortValue(const std::string& portName, T&& value);

    template<typename T>
    T getPortValue(const std::string& portName) const;

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

    // === Scripting Support ===        // Including support for Scripting Editor and external IDEs
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

    // Scripting Editor methods
    virtual ScriptingInterface getScriptingInterface() const;
    virtual void exportToScriptingEditor();
    virtual void importFromScriptingEditor(const std::string& code, const std::string& language);
    
    // External IDE methods
    virtual std::string generateExternalEditorFile() const;
    virtual void importFromExternalEditor(const std::string& sourceCode);

    // === AI Agent Support ===
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

    virtual AIInterface getAIInterface() const {
        AIInterface interface;
        // Populate interface based on node type
        return interface;
    }

    virtual bool validateAITask(const std::string& taskIntent) {
        // Validate if the task is applicable to this node
        return true;
    }

    virtual bool applyAITask(const std::string& taskIntent,
                            const std::map<std::string, std::any>& parameters) {
        // Apply AI-suggested changes
        return true;
    }

    // AI-assisted optimization
    virtual std::vector<std::string> getOptimizationSuggestions() const {
        return {}; // Return potential optimization opportunities
    }

    // AI safety and validation
    virtual bool validateAIChanges(const std::vector<std::string>& changes) {
        return true; // Validate proposed changes
    }

    // === Debug/Development Support ===
    struct DebugInfo {
        std::vector<std::string> watchedVariables;
        std::vector<std::string> breakpoints;
        std::string profileHints;             // Performance profiling hints
    };
    DebugInfo debugInfo;

protected:
    // === Core Node Implementation ===
    NodeInfo nodeInfo;                           // Metadata and attributes for the node
    std::vector<std::shared_ptr<Node>> children; // Child nodes (making this a node graph)
    virtual void Load();                         // Load node-specific data

    mutable std::mutex childrenMutex;         // Mutex for thread-safe access to children
    mutable std::mutex serializationMutex;    // Mutex for thread-safe serialization

    // === Property Management ===
    std::unordered_map<std::string, std::any> properties;
    std::unordered_map<std::string, std::any> runtimeProperties;
    std::unordered_map<std::string, PropertyDefinition> propertyDefs;

    // === Node Resource Management ===
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

    // === Caching & Optimization Support ===
    // Support for caching intermediate results
    struct CacheEntry {
        bool isValid = false;
        uint64_t inputHash = 0;
        std::vector<std::any> outputs;
        // Derived classes can add their specific output cache here
    };
    
    mutable CacheEntry resultCache;
    bool isDirty;

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

private:
    // === Caching & Optimization Support ===
    uint64_t calculateInputHash() const;
    void storeOutputsToCache();
    void restoreOutputsFromCache();

    // === Custom Node Support ===
    virtual std::string getProcessFunctionImpl() const = 0;
    virtual std::string getInitializeFunctionImpl() const = 0;
    virtual std::string getCleanupFunctionImpl() const = 0;
    
    // === Port Management ===
    virtual std::string getPortType(const std::string& portName) const = 0;
    virtual std::string getPortDefaultValue(const std::string& portName) const = 0;
    virtual std::string getPortValidation(const std::string& portName) const;
    virtual bool validatePort(const std::string& portName, const std::any& value) const;
    virtual std::string getPortDescription(const std::string& portName) const = 0;
    virtual bool isPortRequired(const std::string& portName) const = 0;

    // === Custom Implementation Support ===
    bool validateCustomImplementation(const CustomizableElements& elements);
    void applyCustomImplementation(const CustomizableElements& elements);
    std::string generateNodeSourceFile(const CustomizableElements& elements) const;
    CustomizableElements parseNodeSourceFile(const std::string& sourceCode);

    // === Scripting Support ===
    struct ScriptingState{                            // Scripting state tracking
        std::string currentLanguage;
        bool isDirty = false;
        std::string lastValidCode;
        std::vector<std::string> errorLog;
    } scriptingState;

    virtual void generateLanguageSpecificCode(const std::string& language) = 0;
    virtual void validateGeneratedCode(const std::string& code) = 0;
    virtual void applyCodeChanges(const std::string& code) = 0;

    // === AI Support ===
    struct AIState{                                    // AI agent state tracking
        std::vector<std::string> appliedTasks;
        std::map<std::string, float> taskSuccess;
        std::vector<std::string> rejectedTasks;
        bool isUnderAIModification = false;
    } aiState;

    virtual bool isAIManipulationAllowed() const { return true; }
    virtual std::string getNodeSemantics() const = 0;
    virtual std::vector<std::string> getSafetyConstraints() const = 0; 
};
    
} // namespace hd
