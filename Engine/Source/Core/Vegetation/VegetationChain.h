#pragma once

#include "Core/NodeGraph/HD_Node.h"
#include "Core/Vegetation/HD_ProcVegetationDensityNode.h"
#include "Core/Vegetation/HD_ProcVegetationPlacementNode.h"
#include "Core/Vegetation/HD_ProcVegetationGrowthNode.h"
#include "Core/Vegetation/HD_ProcVegetationWindNode.h"

namespace hd {

// Example of how to chain vegetation nodes
inline void CreateVegetationChain(HD_Node& graph) {
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
```

I added `inline` to the function since it's now in a header file, and `#pragma once` to prevent multiple inclusions.