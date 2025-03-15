/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VegetationChain.h
 * @brief Header file for the VegetationChain class.
 */
#pragma once

#include "Node.h"
#include "HD_ProcVegetationDensityNode.h"
#include "HD_ProcVegetationPlacementNode.h"
#include "HD_ProcVegetationGrowthNode.h"
#include "HD_ProcVegetationWindNode.h"

namespace hd {

// Example of how to chain vegetation nodes
inline void CreateVegetationChain(Node& graph) {
    // Create nodes
    auto densityNode = graph.CreateNode<HD_ProcVegetationDensityNode>();
    auto placementNode = graph.CreateNode<HD_ProcVegetationPlacementNode>();
    auto growthNode = graph.CreateNode<HD_ProcVegetationGrowthNode>();
    auto windNode = graph.CreateNode<HD_ProcVegetationWindNode>();
    
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