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
 * 
 * TODO:
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 *   (Remember to not implement in Node what is supposed to be in Object).
 * - Flesh out the class and its methods, structs, enums, etc.
 */
#pragma once

#include <string>
#include <unordered_map>
#include <any>
#include <memory>
#include <vector>
#include <future>
#include <mutex>

#include "Object.h"
#include "NodeCompilationContext.h"
#include "CodeGenContext.h"
#include "DependencyGraph.h"
#include "CommandValidator.h"

namespace hd {

struct NodeInfo : public ObjectInfo {
    std::string NodeType;               // Type of the node (e.g., "Transform", "Physics")
    std::vector<std::string> Inputs;   // List of input ports for the node
    std::vector<std::string> Outputs;  // List of output ports for the node
    
    struct StreamingConfig {
        bool enabled = true;
        bool forceSync = false;
        size_t chunkSize = 1024;        // In bytes
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
template<typename... Types>
class Node : public Object {
public:
    // === Core Node Interface ===
    explicit Node(const NodeInfo& info);
    virtual ~Node() = default;               // Virtual destructor for proper cleanup of derived classes

    // Getters for attributes & metadata
    const NodeInfo& GetNodeInfo() const { return NodeInfo; }

    // === Procedural Generation ===
    // Every node can:
    virtual void Evolve();        // Change over time
    virtual void Adapt();         // Respond to context
    virtual void SelfModify();    // Modify its own behavior
    virtual void Harmonize();     // Maintain system balance

    // === Node Graph Interface ===

    // Core virtual methods - to be implemented only in derived classes
    virtual void OnResume() = 0;
    virtual void OnPause() = 0;
    virtual void OnDirty() = 0;
    virtual bool CanCache() const { return true; }
    virtual uint64_t ComputeCacheKey() const = 0;
    virtual void GenerateRuntimeCode(CodeGenContext& context);

    // === Node Graph Management ===
    /**
     * @brief Add a child node to this node.
     * @param child The child node to add.
     */
    void AddChild(const std::shared_ptr<Node>& child);

    /**
     * @brief Remove a child node by name.
     * TODO: if we're going to allow instances with the same name, we need to use a different approach, like remove a child by unique ID (probably automatically generated)
     * or remove a child by pointer.
     * @param childName The name of the child node to remove.
     * @return True if the child was found and removed, false otherwise.
     */
    bool RemoveChild(const std::string& childName);

    /**
     * @brief Get all child nodes of this node.
     * @return A vector of shared pointers to child nodes.
     */
    const std::vector<std::shared_ptr<Node>>& GetChildren() const;

    // === Reflection Support ===
    // *This probably will be removed, responsibility transferred to the Reflection system, or even a simpler system, like we are planning to use, where in the
    // node graphs implemented in code we simply pass a "runtime" argument to the property definition function.
    struct PropertyDefinition {
        std::string name;
        std::string type;
        std::string defaultValue;
        std::string uiHints;         // How to display in property panel
        std::string validation;
        bool isSerializable;
        bool isExposed;              // Available to external scripts
    };
    std::vector<PropertyDefinition> properties;

    // === Validation Support ===
    std::shared_ptr<CommandValidator> validator;

    void SetValidator(std::shared_ptr<CommandValidator> v) { validator = std::move(v); }
    const std::shared_ptr<CommandValidator>& GetValidator() const { return validator; }

    template<typename T>
    bool SetInputValue(const std::string& name, const T& value);

    bool IsPortValid(const std::string& name) const;

    std::string GetPortError(const std::string& name) const;

    template<typename T>
    void AddPortValidationRule(const std::string& portName,
                             std::function<bool(const T&)> rule,
                             const std::string& errorMsg);
    
    struct PortValidationState {
        bool isValid = true;
        std::string lastError;
    };
    std::unordered_map<std::string, PortValidationState> portValidationState;

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
    virtual std::future<void> LoadAsync();
    virtual void Stream();
    virtual bool ShouldStream() const;
    virtual void UpdateStreamingPriority();

    /**
     * @brief Synchronously stream this node's data. This is the default implementation, but derived classes can override it.
     */
    virtual void StreamSync();
    virtual void StreamAsync();
    
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
    virtual void MarkDirty();
    
    /**
     * @brief Modify this node in play mode.
     * @param modifier Function to apply modifications.
     */
    virtual void ModifyInPlayMode(const std::function<void(Node*)>& modifier);
    virtual void PauseExecution();
    virtual void ResumeExecution();

    // Update logic for this node (can be overridden by derived classes)
    virtual void Update();

    // === Compilation Support ===
    virtual bool CanCompile() const { return false; }
    virtual void ContributeToCompilation(CompilationContext& context);

    // Optimization hints
    virtual bool HasConstantOutput() const { return false; }
    virtual bool CanInline() const { return false; }
    virtual bool RequiresFullPrecision() const { return true; }

    // === Custom Node Support ===
    struct CustomizableElements {
        std::string processFunction;      // Main processing logic
        std::string initializeFunction;   // Setup/initialization code
        std::string cleanupFunction;      // Cleanup/destruction code
        std::string validateFunction;     // Input validation logic
        std::string cacheFunction;        // Custom caching logic
        std::string optimizeFunction;     // Custom optimization hints
    };
    virtual CustomizableElements ExportToCustomNode() const;            // Convert built-in node to custom
    virtual void ImportCustomImplementation(const CustomizableElements& elements);

    // === Port Management ===
    // TODO: Almost certainly refactor to be based on the actual use of node graphs (the core usage being implementing them in code, inside classes, be it instancing and
    // connecting other nodes directly or assigning them as child nodes and them accessing their members), where the concise fluent style (it's an actual C++ coding style) 
    // syntax and workflow are paramount design features - we want users to feel good and highly productive using the engine in "barebones" mode, in code for most things.
    // So, the connection to and accessing of node graph members will possibly be simpler than currently we see below for port management. Or maybe those GetPort(), SetPort()
    // will still be useful for UI - visual node graph editor. For state, sync, etc.
    struct PortDefinition {             // Port definitions and metadata
        std::string name;
        std::string type;
        std::string defaultValue;
        std::string validation;         // Optional validation expression
        std::string description;        // Documentation
        bool isRequired;
        bool isAdvanced;                // Hidden by default in UI
    };
    std::vector<PortDefinition> inputs;
    std::vector<PortDefinition> outputs;

    /**
     * @brief Get the input ports for this node. 
     * This method should be implemented in derived classes.
     */
    virtual std::vector<std::string> GetInputPorts() const = 0;  // Inputs for connections

    /**
     * @brief Get the output ports for this node. 
     * This method should be implemented in derived classes.
     */
    virtual std::vector<std::string> GetOutputPorts() const = 0; // Outputs for connections

    /**
     * @brief Set the value of a port. Type-safe port system.
     * @param portName Name of the port.
     * @param value Value to set.
     */
    template<typename T>
    void SetPortValue(const std::string& portName, T&& value);

    template<typename T>
    T GetPortValue(const std::string& portName) const;

    // === Serialization ===
    /**
     * @brief Serialize this node and its children.
     * @param stream Output stream for serialization.
     */
    void Serialize(std::ostream& stream) const override;

    /**
     * @brief Deserialize this node and its children.
     * @param stream Input stream for deserialization.
     */
    void Deserialize(std::istream& stream) override;

    // === Visualization ===
    /**
     * @brief Draw this node in the Properties panel.
     * Ports will be automatically drawn based on their data types, etc. But for custom visualization logic, override this method in derived classes.
     */
    virtual void DrawInPropertyEditor();

    /**
     * @brief Draw this node in the Node Graph Editor.
     * Nodes will be automatically drawn based on their data types, structure, etc. But for custom visualization logic, override this method in derived classes.
     */
    virtual void DrawInNodeGraph();

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
    virtual ScriptingInterface GetScriptingInterface() const;
    virtual void ExportToScriptingEditor();
    virtual void ImportFromScriptingEditor(const std::string& code, const std::string& language);
    
    // External IDE methods
    virtual std::string GenerateExternalEditorFile() const;
    virtual void ImportFromExternalEditor(const std::string& sourceCode);

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

    virtual AIInterface GetAIInterface() const {
        AIInterface interface;
        // Populate interface based on node type
        return interface;
    }

    virtual bool ValidateAITask(const std::string& taskIntent) {
        // Validate if the task is applicable to this node
        return true;
    }

    virtual bool ApplyAITask(const std::string& taskIntent,
                            const std::map<std::string, std::any>& parameters) {
        // Apply AI-suggested changes
        return true;
    }

    // AI-assisted optimization
    virtual std::vector<std::string> GetOptimizationSuggestions() const {
        return {}; // Return potential optimization opportunities
    }

    // AI safety and validation
    virtual bool ValidateAIChanges(const std::vector<std::string>& changes) {
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
    NodeInfo NodeInfo;                           // Metadata and attributes for the node
    std::vector<std::shared_ptr<Node>> Children; // Child nodes (making this a node graph)
    virtual void Load();                         // Load node-specific data

    mutable std::mutex ChildrenMutex;         // Mutex for thread-safe access to children
    mutable std::mutex SerializationMutex;    // Mutex for thread-safe serialization

    // === Caching & Optimization Support ===
    // Support for caching intermediate results
    struct CacheEntry {
        uint64_t inputHash;
        std::vector<std::any> outputs;
        bool isValid = false;
    };
    
    mutable CacheEntry resultCache;
    bool isDirty = true;

    bool TryUseCache() const;
    void UpdateCache();

private:
    // === Caching & Optimization Support ===
    uint64_t CalculateInputHash() const;
    void StoreOutputsToCache();
    void RestoreOutputsFromCache();

    // === Custom Node Support ===
    virtual std::string GetProcessFunctionImpl() const = 0;
    virtual std::string GetInitializeFunctionImpl() const = 0;
    virtual std::string GetCleanupFunctionImpl() const = 0;
    
    // === Port Management ===
    virtual std::string GetPortType(const std::string& portName) const = 0;
    virtual std::string GetPortDefaultValue(const std::string& portName) const = 0;
    virtual std::string GetPortValidation(const std::string& portName) const;
    virtual bool ValidatePort(const std::string& portName, const std::any& value) const;
    virtual std::string GetPortDescription(const std::string& portName) const = 0;
    virtual bool IsPortRequired(const std::string& portName) const = 0;

    // === Custom Implementation Support ===
    bool ValidateCustomImplementation(const CustomizableElements& elements);
    void ApplyCustomImplementation(const CustomizableElements& elements);
    std::string GenerateNodeSourceFile(const CustomizableElements& elements) const;
    CustomizableElements ParseNodeSourceFile(const std::string& sourceCode);

    // === Scripting Support ===
    struct {                            // Scripting state tracking
        std::string currentLanguage;
        bool isDirty = false;
        std::string lastValidCode;
        std::vector<std::string> errorLog;
    } scriptingState;

    virtual void GenerateLanguageSpecificCode(const std::string& language) = 0;
    virtual void ValidateGeneratedCode(const std::string& code) = 0;
    virtual void ApplyCodeChanges(const std::string& code) = 0;

    // === AI Support ===
    struct {                                    // AI agent state tracking
        std::vector<std::string> appliedTasks;
        std::map<std::string, float> taskSuccess;
        std::vector<std::string> rejectedTasks;
        bool isUnderAIModification = false;
    } aiState;

    virtual bool IsAIManipulationAllowed() const { return true; }
    virtual std::string GetNodeSemantics() const = 0;
    virtual std::vector<std::string> GetSafetyConstraints() const = 0; 
};
    
} // namespace hd



