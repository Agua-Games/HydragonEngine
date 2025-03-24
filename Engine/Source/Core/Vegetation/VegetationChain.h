/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VegetationChain.h
 * @brief Header file for the VegetationChain class.
 */
#pragma once

#include "Node.h"
#include "ProcVegetationDensity.h"
#include "ProcVegetationPlacement.h"
#include "ProcVegetationGrowth.h"
#include "ProcVegetationWind.h"

namespace hd {

// Example of how to chain vegetation nodes
inline void CreateVegetationChain(Node& graph) {
    // Create nodes
    auto densityNode = graph.createNode<ProcVegetationDensity>();
    auto placementNode = graph.createNode<ProcVegetationPlacement>();
    auto growthNode = graph.createNode<ProcVegetationGrowth>();
    auto windNode = graph.createNode<ProcVegetationWind>();
    
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