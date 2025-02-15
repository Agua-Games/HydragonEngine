/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include <string>
#include <unordered_map>
#include <any>
#include <memory>
#include <vector>
#include <future>
#include <mutex>

#include "HD_Object.h"
#include "HD_CompilationContext.h"
#include "HD_CodeGenContext.h"
#include "HD_DependencyGraph.h"


namespace hd {

// Struct to hold metadata and attributes for HD_Node
struct HD_NodeInfo : public HD_ObjectInfo {
    std::string NodeType;               // Type of the node (e.g., "Transform", "Physics")
    //std::vector<std::string> validInputs;  // Allowed input types. Need to refactor this to be a list/check for each input port below.
    //std::vector<std::string> validOutputs; // Allowed output types. Need to refactor this to be a list/check for each output port below.
    std::vector<std::string> Inputs;   // List of input ports for the node
    std::vector<std::string> Outputs;  // List of output ports for the node
    bool IsStreamable = true;          // Whether the node supports streaming
    bool IsAsyncLoadable = true;       // Whether the node supports async loading

    // Constructor for convenience
    HD_NodeInfo(const std::string& name = "", bool isSerializable = true, bool isEditableInEditor = true,
                const std::string& nodeType = "", const std::vector<std::string>& inputs = {},
                const std::vector<std::string>& outputs = {}, bool isStreamable = true, bool isAsyncLoadable = true)
        : HD_ObjectInfo(name, isSerializable, isEditableInEditor),
            NodeType(nodeType), Inputs(inputs), Outputs(outputs),
            IsStreamable(isStreamable), IsAsyncLoadable(isAsyncLoadable) {}
};

// Base class for all nodes in Hydragon
template<typename... Types>
class HD_Node : public HD_Object {
public:
    // Constructor with HD_NodeInfo for initialization
    explicit HD_Node(const HD_NodeInfo& info) : HD_Object(info), NodeInfo(info) {}

    // Virtual destructor for proper cleanup of derived classes
    virtual ~HD_Node() = default;

    // Getters for metadata
    const HD_NodeInfo& GetNodeInfo() const { return NodeInfo; }

    // Add a child node to this node (making it a node graph)
    void AddChild(const std::shared_ptr<HD_Node>& child) {
        std::lock_guard<std::mutex> lock(ChildrenMutex);
        Children.push_back(child);
    }

    // Remove a child node by name
    void RemoveChild(const std::string& childName) {
        std::lock_guard<std::mutex> lock(ChildrenMutex);
        Children.erase(std::remove_if(Children.begin(), Children.end(),
                                        [&childName](const std::shared_ptr<HD_Node>& child) {
                                            return child->GetInfo().Name == childName;
                                        }),
                        Children.end());
    }

    // Get all child nodes
    const std::vector<std::shared_ptr<HD_Node>>& GetChildren() const {
        return Children;
    }

    // Async load this node's data
    virtual std::future<void> LoadAsync() {
        return std::async(std::launch::async, [this]() {
            if (NodeInfo.IsAsyncLoadable) {
                Load();
            }
        });
    }

    // Stream this node's data
    virtual void Stream() {
        if (NodeInfo.IsStreamable) {
            // Implement streaming logic here
        }
    }

    // Update logic for this node (can be overridden by derived classes)
    virtual void Update() {
        std::lock_guard<std::mutex> lock(ChildrenMutex);
        for (const auto& child : Children) {
            child->Update();
        }
    }
    // Get the imputs and outputs for this node, to use in Node Graph Editor, process input data and output it.
    // These methods should be implemented in derived classes.
    virtual std::vector<std::string> GetInputPorts() const = 0;  // Inputs for connections
    virtual std::vector<std::string> GetOutputPorts() const = 0; // Outputs for connections

    // Type-safe port system
    template<typename T>
    void SetPortValue(const std::string& portName, T&& value) {
        static_assert((std::is_same_v<T, Types> || ...), 
                     "Type not supported by this node");
        // Implementation
    }

    template<typename T>
    T GetPortValue(const std::string& portName) const {
        static_assert((std::is_same_v<T, Types> || ...), 
                     "Type not supported by this node");
        // Implementation
    }

    // Draw this node in the Inspector (can be overridden by derived classes)
    virtual void DrawInInspector() {
        ImGui::Text("Node Name: %s", NodeInfo.Name.c_str());
        ImGui::Text("Node Type: %s", NodeInfo.NodeType.c_str());
    }

    // Draw this node in the Node Graph Editor (can be overridden by derived classes)
    virtual void DrawInNodeGraph() {
        ImGui::BeginGroup();
        ImGui::Text("Node: %s", NodeInfo.Name.c_str());
        ImGui::Text("Inputs:");
        for (const auto& input : NodeInfo.Inputs) {
            ImGui::BulletText("%s", input.c_str());
        }
        ImGui::Text("Outputs:");
        for (const auto& output : NodeInfo.Outputs) {
            ImGui::BulletText("%s", output.c_str());
        }
        ImGui::EndGroup();
    }

    // Serialization (overrides base class implementation)
    void Serialize(std::ostream& stream) const override {
        std::lock_guard<std::mutex> lock(SerializationMutex);
        stream << "Node Name: " << NodeInfo.Name << "\n";
        stream << "Node Type: " << NodeInfo.NodeType << "\n";
        for (const auto& child : Children) {
            child->Serialize(stream);
        }
    }

    // Deserialization (overrides base class implementation)
    void Deserialize(std::istream& stream) override {
        std::lock_guard<std::mutex> lock(SerializationMutex);
        std::string line;
        while (std::getline(stream, line)) {
            if (line.find("Node Name:") != std::string::npos) {
                NodeInfo.Name = line.substr(line.find(":") + 1);
            } else if (line.find("Node Type:") != std::string::npos) {
                NodeInfo.NodeType = line.substr(line.find(":") + 1);
            }
        }
        for (auto& child : Children) {
            child->Deserialize(stream);
        }
    }

    // Support for compilation
    virtual bool CanCompile() const { return false; }
    
    virtual void ContributeToCompilation(CompilationContext& context) {
        // Default implementation for nodes that don't need special compilation handling
        context.AddDefaultNodeCompilation(this);
    }

    // Optimization hints
    virtual bool HasConstantOutput() const { return false; }
    virtual bool CanInline() const { return false; }
    virtual bool RequiresFullPrecision() const { return true; }

    // Runtime execution
    virtual void GenerateRuntimeCode(CodeGenContext& context) {
        // Default implementation generates standard Process() call
        context.AddProcessCall(this);
    }

    // Custom node support
    struct CustomizableElements {
        std::string processFunction;      // Main processing logic
        std::string initializeFunction;   // Setup/initialization code
        std::string cleanupFunction;      // Cleanup/destruction code
        std::string validateFunction;     // Input validation logic
        std::string cacheFunction;        // Custom caching logic
        std::string optimizeFunction;     // Custom optimization hints
        
        // Port definitions and metadata
        struct PortDefinition {
            std::string name;
            std::string type;
            std::string defaultValue;
            std::string validation;       // Optional validation expression
            std::string description;      // Documentation
            bool isRequired;
            bool isAdvanced;             // Hidden by default in UI
        };
        std::vector<PortDefinition> inputs;
        std::vector<PortDefinition> outputs;

        // Node-specific properties
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

        // Custom UI elements
        struct UIElement {
            std::string type;            // Button, Slider, etc.
            std::string label;
            std::string callback;        // Function to call
            std::string layout;          // UI layout hints
        };
        std::vector<UIElement> customUI;

        // Debug/Development helpers
        struct DebugInfo {
            std::vector<std::string> watchedVariables;
            std::vector<std::string> breakpoints;
            std::string profileHints;    // Performance profiling hints
        };
        DebugInfo debugInfo;
    };

    // Convert built-in node to custom
    virtual CustomizableElements ExportToCustomNode() const {
        CustomizableElements elements;
        // Export current node's implementation
        elements.processFunction = GetProcessFunctionImpl();
        elements.initializeFunction = GetInitializeFunctionImpl();
        elements.cleanupFunction = GetCleanupFunctionImpl();
        
        // Export port definitions
        for (const auto& port : GetInputPorts()) {
            elements.inputs.push_back({
                .name = port,
                .type = GetPortType(port),
                .defaultValue = GetPortDefaultValue(port),
                .validation = GetPortValidation(port),
                .description = GetPortDescription(port),
                .isRequired = IsPortRequired(port)
            });
        }
        // Similar for outputs...

        return elements;
    }

    // Import custom implementation
    virtual void ImportCustomImplementation(const CustomizableElements& elements) {
        // Validate and apply custom implementation
        if (ValidateCustomImplementation(elements)) {
            ApplyCustomImplementation(elements);
        }
    }

    // Support for external editing
    virtual std::string GenerateExternalEditorFile() const {
        return GenerateNodeSourceFile(ExportToCustomNode());
    }

    virtual void ImportFromExternalEditor(const std::string& sourceCode) {
        auto elements = ParseNodeSourceFile(sourceCode);
        ImportCustomImplementation(elements);
    }

    // Scripting Editor Integration
    struct ScriptingInterface {
        // Code templates for different languages
        struct CodeTemplates {
            std::string cpp;
            std::string python;
            std::string lua;
            std::string blueprint; // Visual scripting
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

    // AI Agent Integration
    struct AIInterface {
        // Task descriptions for AI manipulation
        struct TaskDescription {
            std::string intent;           // What the task aims to achieve
            std::string constraints;      // Limitations and requirements
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
            std::string purpose;          // Node's primary function
            std::string domain;           // Domain-specific context
            std::vector<std::string> tags;// Semantic tags
            std::string relationships;    // Relationship with other nodes
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

    // Scripting Editor methods
    virtual ScriptingInterface GetScriptingInterface() const {
        ScriptingInterface interface;
        // Populate interface based on node type
        return interface;
    }

    virtual void ExportToScriptingEditor() {
        auto interface = GetScriptingInterface();
        // Generate editor-specific code and metadata
    }

    virtual void ImportFromScriptingEditor(const std::string& code, 
                                         const std::string& language) {
        // Parse and validate code
        // Update node implementation
    }

    // AI Agent methods
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

protected:
    HD_NodeInfo NodeInfo;                      // Metadata and attributes for the node
    std::vector<std::shared_ptr<HD_Node>> Children; // Child nodes (making this a node graph)

    // Protected methods for derived classes
    virtual void Load() {
        // Default implementation for loading node data
    }

    mutable std::mutex ChildrenMutex;         // Mutex for thread-safe access to children
    mutable std::mutex SerializationMutex;    // Mutex for thread-safe serialization

    // Support for caching intermediate results
    struct CacheEntry {
        uint64_t inputHash;
        std::vector<std::any> outputs;
        bool isValid = false;
    };
    
    mutable CacheEntry resultCache;

    bool TryUseCache() const {
        if (resultCache.isValid && resultCache.inputHash == CalculateInputHash()) {
            RestoreOutputsFromCache();
            return true;
        }
        return false;
    }

    void UpdateCache() {
        resultCache.inputHash = CalculateInputHash();
        StoreOutputsToCache();
        resultCache.isValid = true;
    }

private:
    uint64_t CalculateInputHash() const;
    void StoreOutputsToCache();
    void RestoreOutputsFromCache();

    // Implementation extraction helpers
    virtual std::string GetProcessFunctionImpl() const = 0;
    virtual std::string GetInitializeFunctionImpl() const = 0;
    virtual std::string GetCleanupFunctionImpl() const = 0;
    
    // Port information helpers
    virtual std::string GetPortType(const std::string& portName) const = 0;
    virtual std::string GetPortDefaultValue(const std::string& portName) const = 0;
    virtual std::string GetPortValidation(const std::string& portName) const = 0;
    virtual std::string GetPortDescription(const std::string& portName) const = 0;
    virtual bool IsPortRequired(const std::string& portName) const = 0;

    bool ValidateCustomImplementation(const CustomizableElements& elements);
    void ApplyCustomImplementation(const CustomizableElements& elements);
    std::string GenerateNodeSourceFile(const CustomizableElements& elements) const;
    CustomizableElements ParseNodeSourceFile(const std::string& sourceCode);

    // Scripting support
    virtual void GenerateLanguageSpecificCode(const std::string& language) = 0;
    virtual void ValidateGeneratedCode(const std::string& code) = 0;
    virtual void ApplyCodeChanges(const std::string& code) = 0;

    // AI support
    virtual bool IsAIManipulationAllowed() const { return true; }
    virtual std::string GetNodeSemantics() const = 0;
    virtual std::vector<std::string> GetSafetyConstraints() const = 0;

    // Scripting editor state
    struct {
        std::string currentLanguage;
        bool isDirty = false;
        std::string lastValidCode;
        std::vector<std::string> errorLog;
    } scriptingState;

    // AI agent state
    struct {
        std::vector<std::string> appliedTasks;
        std::map<std::string, float> taskSuccess;
        std::vector<std::string> rejectedTasks;
        bool isUnderAIModification = false;
    } aiState;
};

} // namespace hd

