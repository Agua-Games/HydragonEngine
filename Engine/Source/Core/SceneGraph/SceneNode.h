/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SceneNode.h
 * @brief Base class for all scenes in Hydragon
 * 
 * ARCHITECTURAL NOTES:
 * - Scene nodes are the primary entities in the system.
 * - By design, both levels, sub-levels, "game objects", attached components, etc. are all scene nodes.
 * - Scene nodes can be connected to form complex systems.
 * - As much as possible, scene nodes implement functionality akin to openUSD's UsdStage, though optimized for realtime.
 * 
 * TODO:
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 */
#pragma once

#include "Node.h"
#include "HD_SceneAllocator.h"
#include "HD_RuntimeVariants.h"
#include <future>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <any>
#include <glm/glm.hpp>

namespace hd {
    
/**
 * @brief Metadata and attributes container for scene nodes
 * 
 * Extends base node info with USD-specific attributes for scene composition
 * and layering capabilities.
 */
struct HD_SceneInfo : public NodeInfo {
    /** Path to the scene's USD layer file */
    std::string LayerPath;
    /** Referenced USD files for composition */
    std::vector<std::string> References;
    /** Controls USD layer-based composition */
    bool IsLayered = true;

    HD_SceneInfo(const std::string& name = "", bool isSerializable = true,
                 bool isEditableInEditor = true, const std::string& nodeType = "",
                 const std::vector<std::string>& inputs = {},
                 const std::vector<std::string>& outputs = {},
                 bool isStreamable = true, bool isAsyncLoadable = true,
                 const std::string& layerPath = "",
                 const std::vector<std::string>& references = {},
                 bool isLayered = true)
        : NodeInfo(name, isSerializable, isEditableInEditor, nodeType,
                     inputs, outputs, isStreamable, isAsyncLoadable),
          LayerPath(layerPath), References(references), IsLayered(isLayered) {}
};

/**
 * @brief Base class for all scenes in Hydragon
 * 
 * Provides scene management functionality including:
 * - USD-based scene composition
 * - Asynchronous loading and streaming
 * - Scene graph manipulation
 * - Transform propagation
 */
class SceneNode : public Node<> {  // Empty template params if no types needed
    // or specify needed types:
    // class SceneNode : public Node<Transform, Material, Geometry>
public:
    // Node Graph Integration
    class SceneNodePort {
    public:
        // Built-in primitive types
        enum class PrimitiveType {
            Transform,
            Material,
            Geometry,
            Variant,
            Performance,
            Float,
            Int,
            Bool,
            Vector2,
            Vector3,
            Vector4,
            Matrix4,
            String,
            Custom  // Marker for custom types
        };

        struct TypeInfo {
            std::string name;
            size_t hash;
            std::function<bool(const std::any&)> validator;
            
            bool operator==(const TypeInfo& other) const {
                return hash == other.hash;
            }
        };

        SceneNodePort(const std::string& name, PrimitiveType type)
            : name(name), primitiveType(type), customTypeInfo(nullptr) {}

        SceneNodePort(const std::string& name, const TypeInfo& customType)
            : name(name), primitiveType(PrimitiveType::Custom), 
              customTypeInfo(std::make_shared<TypeInfo>(customType)) {}

        template<typename T>
        static TypeInfo CreateCustomType(const std::string& typeName) {
            return TypeInfo{
                typeName,
                typeid(T).hash_code(),
                [](const std::any& value) -> bool {
                    try {
                        std::any_cast<T>(value);
                        return true;
                    } catch (const std::bad_any_cast&) {
                        return false;
                    }
                }
            };
        }

        bool IsValid(const std::any& value) const {
            if (primitiveType == PrimitiveType::Custom && customTypeInfo) {
                return customTypeInfo->validator(value);
            }
            // Validate primitive types
            switch (primitiveType) {
                case PrimitiveType::Float: return ValidateType<float>(value);
                case PrimitiveType::Int: return ValidateType<int>(value);
                // ... add other primitive type validations
                default: return false;
            }
        }

        const std::string& GetName() const { return name; }
        PrimitiveType GetPrimitiveType() const { return primitiveType; }
        const TypeInfo* GetCustomTypeInfo() const { return customTypeInfo.get(); }

    private:
        std::string name;
        PrimitiveType primitiveType;
        std::shared_ptr<TypeInfo> customTypeInfo;
        std::any value;

        template<typename T>
        static bool ValidateType(const std::any& value) {
            try {
                std::any_cast<T>(value);
                return true;
            } catch (const std::bad_any_cast&) {
                return false;
            }
        }
    };

    explicit SceneNode(const HD_SceneInfo& info)
        : Node(info), SceneInfo(info) {
        InitializePorts();
    }

    virtual ~SceneNode() = default;

    /** @return Current scene information and metadata */
    const HD_SceneInfo& GetSceneInfo() const { return SceneInfo; }

    /** @return Root node of this scene graph */
    std::shared_ptr<Node> GetRoot() const;

    /** @brief Finds a node by name in this scene graph */
    std::shared_ptr<Node> FindNode(const std::string& name) const;

    /** @brief Finds nodes by type in this scene graph */
    std::vector<std::shared_ptr<Node>> FindNodesByType(const std::string& type) const;

    /** 
     * @brief Creates a node of specified type in this scene graph
     * @param type Type of node to create (e.g., "Transform", "Mesh", "Light")
     * @param name Optional name for the node
     * @return Newly created node
     */
    std::shared_ptr<Node> CreateNode(const std::string& type, const std::string& name = "");

    /**
     * @brief Asynchronously loads scene content
     * @return Future that completes when loading finishes
     */
    std::future<void> LoadAsync() override {
        return std::async(std::launch::async, [this]() {
            if (SceneInfo.IsAsyncLoadable) {
                LoadScene();
            }
        });
    }

    /**
     * @brief Streams scene content based on visibility and priority
     */
    void Stream() override {
        if (SceneInfo.IsStreamable) {
            StreamScene();
        }
    }

    /**
     * @brief Updates scene state and propagates transforms
     */
    void Update() override {
        PropagateTransformations();
        Node::Update();
    }

    /**
     * @brief Adds a node to the scene hierarchy
     * @param node Node to add
     */
    void AddNode(std::shared_ptr<Node> node);

    /**
     * @brief Removes a node from the scene hierarchy
     * @param nodeName Name of the node to remove
     */
    void RemoveNode(const std::string& nodeName);

    /**
     * @brief References another scene graph for composition
     * @param otherGraph Scene graph to reference
     */
    void ReferenceSceneGraph(const std::shared_ptr<HD_SceneGraph>& otherGraph);

    /**
     * @brief Clones a subtree of nodes
     * @param sourceNode Root of subtree to clone
     * @param deep If true, performs deep copy of all children
     * @return Cloned subtree
     */
    std::shared_ptr<Node> CloneSubtree(const std::shared_ptr<Node>& sourceNode, bool deep = true);

    /**
     * @brief Creates an instance of a subtree
     * @param sourceNode Root of subtree to instance
     * @return Instanced subtree
     */
    std::shared_ptr<Node> CreateInstance(const std::shared_ptr<Node>& sourceNode);

    /** @brief Returns all instances of a given node */
    std::vector<std::shared_ptr<Node>> GetInstances(const std::shared_ptr<Node>& sourceNode) const;

    /**
     * @brief Evaluates the scene graph at a specific time
     * @param time Time point at which to evaluate
     */
    void EvaluateAt(float time);

    /** @brief Returns current evaluation time */
    float GetCurrentTime() const { return CurrentTime; }

    /**
     * @brief Loads scene from a file
     * @param filePath Path to the scene file
     */
    void LoadFromFile(const std::string& filePath);

    /**
     * @brief Saves scene to a file
     * @param filePath Destination path for the scene file
     */
    void SaveToFile(const std::string& filePath);

    /**
     * @brief Loads a compiled runtime scene
     * @param compiledScenePath Path to the compiled scene file
     */
    static std::shared_ptr<SceneNode> LoadCompiledScene(const std::string& compiledScenePath);

    /**
     * @brief Renders scene properties in the Inspector panel
     */
    void DrawInInspector() override;

    /**
     * @brief Renders scene node in the Node Graph Editor
     */
    void DrawInNodeGraph() override;

#if HD_EDITOR_MODE
    /**
     * @brief Creates a variant of a node or subtree (editor only)
     * @param node Node to create variant from
     * @param variantName Name of the variant
     */
    void CreateVariant(const std::shared_ptr<Node>& node, const std::string& variantName);

    /** @brief Switches to a specific variant (editor only) */
    void SwitchVariant(const std::string& variantName);

    /** @brief Returns available variants (editor only) */
    std::vector<std::string> GetVariants() const;

    /**
     * @brief Compiles scene for runtime, baking selected variants
     * @param targetPath Output path for compiled scene
     * @param selectedVariants Map of nodes to their selected variant names
     */
    void CompileForRuntime(const std::string& targetPath, 
                          const std::unordered_map<std::string, std::string>& selectedVariants);
#endif

    void ProcessNodeGraph() override {
        // Process inputs
        auto worldTransform = GetInputValue<glm::mat4>("WorldTransform");
        auto performanceMetric = GetInputValue<float>("LODMetric");
        
        // Update internal state
        UpdateTransforms(worldTransform);
        UpdateLODSelection(performanceMetric);
        
        // Set outputs
        SetOutputValue("MeshData", GetCurrentMeshData());
        SetOutputValue("BoundingBox", CalculateBoundingBox());
    }

    void OnResume() override;
    void OnPause() override;
    void OnDirty() override { MarkChildrenDirty(); }
    
    std::string GetNodeSemantics() const override {
        return "Scene node managing hierarchical scene content with USD-like features";
    }
    
    std::vector<std::string> GetSafetyConstraints() const override {
        return {
            "Transform hierarchy must remain valid",
            "Node names must be unique within scope",
            "Referenced scenes must exist"
        };
    }
    
    void GenerateLanguageSpecificCode(const std::string& language) override {
        // Implementation
    }
    
    void ValidateGeneratedCode(const std::string& code) override {
        // Implementation
    }
    
    void ApplyCodeChanges(const std::string& code) override {
        // Implementation
    }

protected:
    /** Scene-specific metadata and attributes */
    HD_SceneInfo SceneInfo;

    /** Current evaluation time */
    float CurrentTime = 0.0f;

    /**
     * @brief Loads scene content from USD layer
     */
    virtual void LoadScene() {
        if (!SceneInfo.LayerPath.empty()) {
            // USD composition happens here
        }
    }

    /**
     * @brief Implements scene streaming logic
     */
    virtual void StreamScene();

    /**
     * @brief Updates and propagates transformations through the scene hierarchy
     */
    virtual void PropagateTransformations() {
        if (HasInstancedChildren()) {
            BatchTransformUpdate();
        }
    }

#if HD_EDITOR_MODE
    /** Editor-only variant storage */
    std::unordered_map<std::string, std::shared_ptr<Node>> Variants;

    /** @brief Serializes variants to USD */
    void SerializeVariantsToUSD();

    /** @brief Deserializes variants from USD */
    void DeserializeVariantsFromUSD();
#endif

private:
    /** Runtime-optimized scene data */
    struct RuntimeBakedData {
        // Optimized flat arrays for cache-friendly access
        std::vector<glm::mat4> WorldTransforms;
        std::vector<uint32_t> MaterialIndices;
        std::vector<uint32_t> MeshIndices;
        
        // Minimal variant data for runtime LOD/switching
        struct RuntimeVariantSet {
            uint32_t baseIndex;
            uint32_t variantCount;
            float* switchThresholds;  // Performance/distance thresholds
        };
        std::vector<RuntimeVariantSet> VariantSets;
        
        // Optimized hierarchy for traversal
        struct HierarchyNode {
            uint32_t parentIndex;
            uint32_t firstChildIndex;
            uint32_t siblingCount;
        };
        std::vector<HierarchyNode> Hierarchy;
    } RuntimeData;

    /** @brief Optimized transform update for instanced geometry */
    void BatchTransformUpdate();

    /** @brief Checks if scene contains instanced children */
    bool HasInstancedChildren() const;

    /** @brief Updates instance references */
    void UpdateInstances();

    /** @brief Validates compiled data integrity */
    bool ValidateCompiledData() const;

    /** Instance tracking */
    std::unordered_map<std::shared_ptr<Node>, std::vector<std::weak_ptr<Node>>> InstanceMap;

    std::vector<SceneNodePort> InputPorts;
    std::vector<SceneNodePort> OutputPorts;
    
    void InitializePorts() {
        InputPorts = {
            {SceneNodePort::Type::Transform, "WorldTransform"},
            {SceneNodePort::Type::Performance, "LODMetric"},
            {SceneNodePort::Type::Material, "MaterialOverride"}
        };

        OutputPorts = {
            {SceneNodePort::Type::Geometry, "MeshData"},
            {SceneNodePort::Type::Transform, "BoundingBox"}
        };
    }

    template<typename T>
    T GetInputValue(const std::string& portName) {
        for (const auto& port : InputPorts) {
            if (port.name == portName) {
                return std::any_cast<T>(port.value);
            }
        }
        return T();
    }

    void SetOutputValue(const std::string& portName, const std::any& value) {
        for (auto& port : OutputPorts) {
            if (port.name == portName) {
                port.value = value;
                break;
            }
        }
    }

    void UpdateTransforms(const glm::mat4& worldTransform) {
        // Update transform hierarchy
        RuntimeData.WorldTransforms[0] = worldTransform;  // Root transform
        PropagateTransformations();
    }

    void UpdateLODSelection(float performanceMetric) {
        for (auto& variantSet : RuntimeData.VariantSets) {
            SelectAppropriateVariant(variantSet, performanceMetric);
        }
    }

    void SelectAppropriateVariant(RuntimeBakedData::RuntimeVariantSet& variantSet, float metric) {
        for (uint32_t i = 0; i < variantSet.variantCount; ++i) {
            if (metric <= variantSet.switchThresholds[i]) {
                // Apply variant at index i
                break;
            }
        }
    }

    uint64_t ComputeCacheKey() const override {
        // Will combine: scene structure hash, transform states, active variants
        return 0;
    }

    // Cache system integration
    bool CanCache() const override { return true; }
    
    void UpdateCache() override {
        if (!CanCache() || !isDirty) return;
        
        // Will store:
        // - Flattened transform hierarchy
        // - Active variant states
        // - Computed bounds
        // - Visibility states
    }
    
    bool TryRestoreFromCache() {
        if (!CanCache()) return false;
        // Will attempt to restore cached state
        return false;
    }

    // AI integration essentials
    AIInterface GetAIInterface() const override {
        AIInterface interface;
        interface.taskDesc.intent = "Scene composition and management";
        interface.capabilities.canModifyProperties = true;
        interface.capabilities.canModifyLogic = false;
        interface.semantics.purpose = "Scene hierarchy management";
        interface.semantics.domain = "Scene Graph";
        return interface;
    }

    std::vector<std::string> GetOptimizationSuggestions() const override {
        return {
            // Will include:
            // - Instance merging opportunities
            // - Transform hierarchy optimization
            // - Variant consolidation
        };
    }

    void MarkChildrenDirty() {
        for (auto& child : Children) {
            if (auto sceneNode = std::dynamic_pointer_cast<SceneNode>(child)) {
                sceneNode->OnDirty();
            }
        }
    }

    // Cache-related members
    struct SceneCache {
        std::vector<glm::mat4> flattenedTransforms;
        std::vector<uint32_t> activeVariants;
        BoundingBox cachedBounds;
        uint64_t lastCacheKey = 0;
    } cache;
};

} // namespace hd
