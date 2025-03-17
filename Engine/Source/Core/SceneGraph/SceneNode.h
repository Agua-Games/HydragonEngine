/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SceneNode.h
 * @brief Base class for all scenes in Hydragon
 * 
 * ARCHITECTURAL NOTES:
 * - Scene nodes are the primary scene representation entities in the system.
 * - By design, both levels, sub-levels, "game objects", attached components, etc. are all scene nodes - because we conceptually view scenes as visual hierarchical 
 * representation trees.
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
#include <future>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <any>
#include <glm/glm.hpp>

#include "Node.h"
#include "SceneAllocator.h"
#include "RuntimeVariants.h"

namespace hd {
    
/**
 * @brief Metadata and attributes container for scene nodes
 * 
 * Extends base node info with USD-specific attributes for scene composition
 * and layering capabilities.
 * A note on USD: as usd was designed for offline pipelines, we repurpose standard USD features for realtime use. Like: repurposing standard schemas "in-place", to trim
 * any unnecessary data, add other useful features, etc.
 */
struct SceneInfo : public NodeInfo {
    /** Path to the scene's USD layer file */
    std::string layerPath;
    /** Referenced USD files for composition */
    std::vector<std::string> references;
    /** Controls USD layer-based composition */
    bool isLayered = true;

    SceneInfo(const std::string& name = "", bool isSerializable = true,
                 bool isEditableInEditor = true, const std::string& nodeType = "",
                 const std::vector<std::string>& inputs = {},
                 const std::vector<std::string>& outputs = {},
                 bool isStreamable = true, bool isAsyncLoadable = true,
                 const std::string& layerPath = "",
                 const std::vector<std::string>& references = {},
                 bool isLayered = true)
        : NodeInfo(name, isSerializable, isEditableInEditor, nodeType,
                     inputs, outputs, isStreamable, isAsyncLoadable),
          layerPath(layerPath), references(references), isLayered(isLayered) {}
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
    // === Port Management ===
    class SceneNodePort {
    public:
        // Built-in primitive types.
        // TODO: Refactor after properly studying usd and beta testing SceneNode's use in Hydragon
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
        static TypeInfo createCustomType(const std::string& typeName) {
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

        bool isValid(const std::any& value) const {
            if (primitiveType == PrimitiveType::Custom && customTypeInfo) {
                return customTypeInfo->validator(value);
            }
            // Validate primitive types
            switch (primitiveType) {
                case PrimitiveType::Float: return validateType<float>(value);
                case PrimitiveType::Int: return validateType<int>(value);
                // ... add other primitive type validations
                default: return false;
            }
        }

        const std::string& getName() const { return name; }
        PrimitiveType getPrimitiveType() const { return primitiveType; }
        const TypeInfo* getCustomTypeInfo() const { return customTypeInfo.get(); }

    private:
        std::string name;
        PrimitiveType primitiveType;
        std::shared_ptr<TypeInfo> customTypeInfo;
        std::any value;

        template<typename T>
        static bool validateType(const std::any& value) {
            try {
                std::any_cast<T>(value);
                return true;
            } catch (const std::bad_any_cast&) {
                return false;
            }
        }
    };

    explicit SceneNode(const SceneInfo& info)
        : Node(info), SceneInfo(info) {
        initializePorts();
    }

    // === Core Scene Interface ===
    virtual ~SceneNode() = default;

    /** @return Current scene information and metadata */
    const SceneInfo& getSceneInfo() const { return sceneInfo; }

    // === Core Scene Implementation ===
    void onResume() override;
    void onPause() override;
    void onDirty() override { markChildrenDirty(); }

    // === Scene Graph Management ===
    /** @return Root node of this scene graph */
    std::shared_ptr<Node> getRoot() const;

    /** @brief Finds a node by name in this scene graph */
    std::shared_ptr<Node> findNode(const std::string& name) const;

    /** @brief Finds nodes by type in this scene graph */
    std::vector<std::shared_ptr<Node>> findNodesByType(const std::string& type) const;

    /** 
     * @brief Creates a node of specified type in this scene graph
     * @param type Type of node to create (e.g., "Transform", "Mesh", "Light")
     * @param name Optional name for the node
     * @return Newly created node
     */
    std::shared_ptr<Node> createNode(const std::string& type, const std::string& name = "");

    /**
     * @brief Adds a node to the scene hierarchy
     * @param node Node to add
     */
    void addNode(std::shared_ptr<Node> node);

    /**
     * @brief Removes a node from the scene hierarchy
     * @param nodeName Name of the node to remove
     */
    void removeNode(const std::string& nodeName);

    /**
     * @brief References another scene graph for composition
     * @param otherGraph Scene graph to reference
     */
    void referenceSceneGraph(const std::shared_ptr<SceneGraph>& otherGraph);

    /**
     * @brief Clones a subtree of nodes
     * @param sourceNode Root of subtree to clone
     * @param deep If true, performs deep copy of all children
     * @return Cloned subtree
     */
    std::shared_ptr<Node> cloneSubtree(const std::shared_ptr<Node>& sourceNode, bool deep = true);

    /**
     * @brief Creates an instance of a subtree
     * @param sourceNode Root of subtree to instance
     * @return Instanced subtree
     */
    std::shared_ptr<Node> createInstance(const std::shared_ptr<Node>& sourceNode);

    /** @brief Returns all instances of a given node */
    std::vector<std::shared_ptr<Node>> getInstances(const std::shared_ptr<Node>& sourceNode) const;

    #if EDITOR_MODE
    /**
     * @brief Creates a variant of a node or subtree (editor only)
     * @param node Node to create variant from
     * @param variantName Name of the variant
     */
    void createVariant(const std::shared_ptr<Node>& node, const std::string& variantName);

    /** @brief Switches to a specific variant (editor only) */
    void switchVariant(const std::string& variantName);

    /** @brief Returns available variants (editor only) */
    std::vector<std::string> getVariants() const;

    /**
     * @brief Compiles scene for runtime, baking selected variants
     * @param targetPath Output path for compiled scene
     * @param selectedVariants Map of nodes to their selected variant names
     */
    void compileForRuntime(const std::string& targetPath, 
                          const std::unordered_map<std::string, std::string>& selectedVariants);
#endif

    // === Scene Streaming ===
    /**
     * @brief Asynchronously loads scene content
     * @return Future that completes when loading finishes
     */
    std::future<void> loadAsync() override {
        return std::async(std::launch::async, [this]() {
            if (SceneInfo.isAsyncLoadable) {
                loadScene();
            }
        });
    }

    /**
     * @brief Streams scene content based on visibility and priority
     */
    void stream() override {
        if (SceneInfo.isStreamable) {
            streamScene();
        }
    }

    // === Scene Evaluation ===
    void processNodeGraph() override {
        // Process inputs
        auto worldTransform = getInputValue<glm::mat4>("WorldTransform");
        auto performanceMetric = getInputValue<float>("LODMetric");
        
        // Update internal state
        updateTransforms(worldTransform);
        updateLODSelection(performanceMetric);
        
        // Set outputs
        setOutputValue("MeshData", getCurrentMeshData());
        setOutputValue("BoundingBox", calculateBoundingBox());
    }

    /**
     * @brief Updates scene state and propagates transforms
     */
    void update() override {
        propagateTransformations();
        Node::update();
    }

    /**
     * @brief Evaluates the scene graph at a specific time
     * @param time Time point at which to evaluate
     */
    void evaluateAt(float time);

    /** @brief Returns current evaluation time */
    float getCurrentTime() const { return CurrentTime; }

    // === Scene I/O ===
    /**
     * @brief Loads scene from a file
     * @param filePath Path to the scene file
     */
    void loadFromFile(const std::string& filePath);

    /**
     * @brief Saves scene to a file
     * @param filePath Destination path for the scene file
     */
    void saveToFile(const std::string& filePath);

    /**
     * @brief Loads a compiled runtime scene
     * @param compiledScenePath Path to the compiled scene file
     */
    static std::shared_ptr<SceneNode> loadCompiledScene(const std::string& compiledScenePath);

    // === Visualization ===
    /**
     * @brief Renders scene properties in the Inspector panel
     */
    void drawInPropertyEditor() override;

    /**
     * @brief Renders scene node in the Node Graph Editor
     */
    void drawInNodeGraph() override;
   
    // === Scripting Support ===
    void generateLanguageSpecificCode(const std::string& language) override {
        // Implementation
    }
    
    void validateGeneratedCode(const std::string& code) override {
        // Implementation
    }
    
    void applyCodeChanges(const std::string& code) override {
        // Implementation
    }

    // === AI Support ===
    std::string getNodeSemantics() const override {
        return "Scene node managing hierarchical scene content with USD-like features";
    }
    
    std::vector<std::string> getSafetyConstraints() const override {
        return {
            "Transform hierarchy must remain valid",
            "Node names must be unique within scope",
            "Referenced scenes must exist"
        };
    }

protected:
    // === Core Scene Implementation ===
    /** Scene-specific metadata and attributes */
    SceneInfo sceneInfo;

    /** Current evaluation time */
    float currentTime = 0.0f;

    // === Scene Composition & Streaming ===
    /**
     * @brief Loads scene content from USD layer
     */
    virtual void loadScene() {
        if (!sceneInfo.layerPath.empty()) {
            // USD composition happens here
        }
    }

    /**
     * @brief Implements scene streaming logic
     */
    virtual void streamScene();

    /**
     * @brief Updates and propagates transformations through the scene hierarchy
     */
    virtual void propagateTransformations() {
        if (hasInstancedChildren()) {
        batchTransformUpdate();
        }
    }

#if EDITOR_MODE
    /** Editor-only variant storage */
    std::unordered_map<std::string, std::shared_ptr<Node>> variants;

    /** @brief serializes variants to USD */
    void serializeVariantsToUSD();

    /** @brief deserializes variants from USD */
    void deserializeVariantsFromUSD();
#endif

private:
    // === Runtime Optimization ===
    /** Runtime-optimized scene data */
    struct RuntimeBakedData {
        // Optimized flat arrays for cache-friendly access
        std::vector<glm::mat4> worldTransforms;
        std::vector<uint32_t> materialIndices;
        std::vector<uint32_t> meshIndices;
        
        // Minimal variant data for runtime LOD/switching
        struct RuntimeVariantSet {
            uint32_t baseIndex;
            uint32_t variantCount;
            float* switchThresholds;  // Performance/distance thresholds
        };
        std::vector<RuntimeVariantSet> variantSets;
        
        // Optimized hierarchy for traversal
        struct HierarchyNode {
            uint32_t parentIndex;
            uint32_t firstChildIndex;
            uint32_t siblingCount;
        };
        std::vector<HierarchyNode> hierarchy;
    } runtimeData;

    /** @brief Optimized transform update for instanced geometry */
    void batchTransformUpdate();

    /** @brief Checks if scene contains instanced children */
    bool hasInstancedChildren() const;

    /** @brief Updates instance references */
    void updateInstances();

    /** @brief Validates compiled data integrity */
    bool validateCompiledData() const;

    /** Instance tracking */
    std::unordered_map<std::shared_ptr<Node>, std::vector<std::weak_ptr<Node>>> instanceMap;

    // === Port Management ===
    // TODO: Almost certainly refactor to be based on the actual use of node graphs (the core usage being implementing them in code, inside classes, be it instancing and
    // connecting other nodes directly or assigning them as child nodes and them accessing their members), where the concise fluent style (it's an actual C++ coding style) 
    // syntax and workflow are paramount design features - we want users to feel good and highly productive using the engine in "barebones" mode, in code for most things.
    // So, the connection to and accessing of node graph members will possibly be simpler than currently we see below for port management. Or maybe those GetPort(), SetPort()
    // will still be useful for UI - visual node graph editor. For state, sync, etc.
    std::vector<SceneNodePort> inputPorts;
    std::vector<SceneNodePort> outputPorts;
    
    void initializePorts() {
        inputPorts = {
            {SceneNodePort::Type::Transform, "WorldTransform"},
            {SceneNodePort::Type::Performance, "LODMetric"},
            {SceneNodePort::Type::Material, "MaterialOverride"}
        };

        outputPorts = {
            {SceneNodePort::Type::Geometry, "MeshData"},
            {SceneNodePort::Type::Transform, "BoundingBox"}
        };
    }

    template<typename T>
    T getInputValue(const std::string& portName) {
        for (const auto& port : inputPorts) {
            if (port.name == portName) {
                return std::any_cast<T>(port.value);
            }
        }
        return T();
    }

    void setOutputValue(const std::string& portName, const std::any& value) {
        for (auto& port : outputPorts) {
            if (port.name == portName) {
                port.value = value;
                break;
            }
        }
    }

    // === Scene Evaluation & Caching ===
    void updateTransforms(const glm::mat4& worldTransform) {
        // Update transform hierarchy
        runtimeData.worldTransforms[0] = worldTransform;  // Root transform
        propagateTransformations();
    }

    void updateLODSelection(float performanceMetric) {
        for (auto& variantSet : runtimeData.variantSets) {
            selectAppropriateVariant(variantSet, performanceMetric);
        }
    }

    void selectAppropriateVariant(RuntimeBakedData::RuntimeVariantSet& variantSet, float metric) {
        for (uint32_t i = 0; i < variantSet.variantCount; ++i) {
            if (metric <= variantSet.switchThresholds[i]) {
                // Apply variant at index i
                break;
            }
        }
    }

    uint64_t computeCacheKey() const override {
        // Will combine: scene structure hash, transform states, active variants
        return 0;
    }

    // Cache system integration
    bool canCache() const override { return true; }
    
    void updateCache() override {
        if (!canCache() || !isDirty) return;
        
        // Will store:
        // - Flattened transform hierarchy
        // - Active variant states
        // - Computed bounds
        // - Visibility states
    }
    
    bool tryRestoreFromCache() {
        if (!canCache()) return false;
        // Will attempt to restore cached state
        return false;
    }

    // AI integration essentials
    AIInterface getAIInterface() const override {
        AIInterface interface;
        interface.taskDesc.intent = "Scene composition and management";
        interface.capabilities.canModifyProperties = true;
        interface.capabilities.canModifyLogic = false;
        interface.semantics.purpose = "Scene hierarchy management";
        interface.semantics.domain = "Scene Graph";
        return interface;
    }

    std::vector<std::string> getOptimizationSuggestions() const override {
        return {
            // Will include:
            // - Instance merging opportunities
            // - Transform hierarchy optimization
            // - Variant consolidation
        };
    }

    void markChildrenDirty() {
        for (auto& child : children) {
            if (auto sceneNode = std::dynamic_pointer_cast<SceneNode>(child)) {
                sceneNode->onDirty();
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
