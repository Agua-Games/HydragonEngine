// Example of how to chain vegetation nodes
void CreateVegetationChain(HD_NodeGraph& graph) {
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
    
    // The chain provides similar functionality to HD_VegetationNode
    // but with more granular control and flexibility
}