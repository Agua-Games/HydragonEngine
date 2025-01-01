/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Node-based terrain processing system
 */

#pragma once
#include "Core/Data/AttributeSystem.h"

namespace Hydragon {
namespace Terrain {

class TerrainNodeGraph {
public:
    // Node types using familiar terminology
    enum class NodeType {
        HeightGenerator,    // Instead of "TerrainElevationNode"
        Erosion,           // Instead of "HydraulicErosionProcessor"
        Mask,              // Instead of "TerrainMaskGenerator"
        Scatter,           // Instead of "InstanceDistributor"
        BlendLayers,       // Instead of "TerrainLayerCompositor"
        Paint             // Instead of "ManualHeightfieldEditor"
    };

    // Houdini-inspired attribute handling for terrain
    class TerrainAttributes : public Data::AttributeSystem {
    public:
        // Built-in attributes with familiar names
        static constexpr const char* HEIGHT = "height";
        static constexpr const char* SLOPE = "slope";
        static constexpr const char* MOISTURE = "moisture";
        static constexpr const char* EROSION = "erosion";
        
        // Custom attribute support
        template<typename T>
        void addCustomAttribute(const char* name) {
            createAttribute<T>(name, AttributeOwner::Terrain);
        }
    };

    // Node creation with intuitive naming
    NodeHandle addNode(const char* type) {
        // Example node types:
        // "height_noise"  (instead of "ProceduralElevationGenerator")
        // "erode"        (instead of "HydraulicErosionProcessor")
        // "scatter"      (instead of "VegetationDistributor")
        return createNode(getNodeTypeFromString(type));
    }
};

// Python usage example:
/*
    import hydragon as hd
    
    # Create terrain graph
    graph = hd.terrain.graph()
    
    # Add nodes with intuitive names
    height = graph.add_node("height_noise")
    height.noise_type = "perlin"  # Simple, clear parameters
    
    erode = graph.add_node("erode")
    erode.iterations = 1000
    
    # Custom attributes are easy to add
    terrain = graph.terrain
    terrain.add_attribute("vegetation_density", type="float")
    terrain.add_attribute("rock_type", type="string")
    
    # Familiar group operations like Houdini
    steep = terrain.create_group("steep_slopes")
    steep.add_by_slope(min=45)
    
    # Scatter objects on terrain
    scatter = graph.add_node("scatter")
    scatter.source = "trees.fbx"
    scatter.target_group = steep  # Use groups naturally
*/ 

/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Node-based terrain processing system
 */ 