/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VegetationChain.h
 * @brief Header file for the VegetationChain class.
 */
#pragma once

#include "Node.h"
#include "ProcVegetationDensityNode.h"
#include "ProcVegetationPlacementNode.h"
#include "ProcVegetationGrowthNode.h"
#include "ProcVegetationWindNode.h"

namespace hd {

// Example of how to chain vegetation nodes
inline void CreateVegetationChain(Node& graph) {
    // Create nodes
    auto densityNode = graph.createNode<ProcVegetationDensityNode>();
    auto placementNode = graph.createNode<ProcVegetationPlacementNode>();
    auto growthNode = graph.createNode<ProcVegetationGrowthNode>();
    auto windNode = graph.createNode<ProcVegetationWindNode>();
    
    // Connect nodes
    graph.Connect(densityNode, "DensityMap", 
                 placementNode, "DensityMap");
    graph.Connect(densityNode, "ValidPlacementMap", 
                 placementNode, "PlacementMask");
    
    graph.Connect(placementNode, "PlacementPoints", 
                 growthNode, "PlacementPoints");
    graph.Connect(placementNode, "InstanceData", 
                 windNode, "InstanceData");
    
    graph.Connect(growthNode, "GrowthStates", 
                 windNode, "GrowthStates");
}

} // namespace hd