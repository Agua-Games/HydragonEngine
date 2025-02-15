namespace hd {

struct HD_FogNodeInfo : public HD_NodeInfo {
    bool useVolumetrics = true;
    bool supportsDynamicPatterns = true;

    HD_FogNodeInfo() {
        NodeType = "Atmosphere/Fog";
        IsStreamable = true;
        IsAsyncLoadable = true;
        
        // Define default ports
        Inputs = {
            "WindIntensity",
            "Humidity",
            "Temperature",
            "DensityPattern",
            "TurbulencePattern",
            "GlobalAtmosphere"
        };
        
        Outputs = {
            "VolumetricTexture",
            "DensityField",
            "ScatteringParams"
        };
    }
};

class HD_FogNode : public HD_Node {
public:
    explicit HD_FogNode(const HD_FogNodeInfo& info = HD_FogNodeInfo())
        : HD_Node(info), FogInfo(info) {
        InitializeProceduralPorts();
    }

    std::vector<std::string> GetInputPorts() const override {
        return FogInfo.Inputs;
    }

    std::vector<std::string> GetOutputPorts() const override {
        return FogInfo.Outputs;
    }

    void ProcessNodeGraph() override {
        // Get input values from connected nodes
        float windIntensity = GetInputValue<float>("WindIntensity");
        float humidity = GetInputValue<float>("Humidity");
        float temperature = GetInputValue<float>("Temperature");
        
        auto densityPattern = GetInputValue<ProceduralPatternData>("DensityPattern");
        auto turbulencePattern = GetInputValue<ProceduralPatternData>("TurbulencePattern");
        
        // If no pattern is connected, create procedural ones
        if (!densityPattern.IsValid()) {
            ProceduralStructureParams fogParams;
            fogParams.type = ProceduralStructureType::Organic;
            fogParams.regularity = windIntensity * 0.5f;
            fogParams.complexity = humidity;
            
            densityPattern = CreateProceduralPattern(fogParams);
        }

        // Process the patterns
        auto volumetricTexture = GenerateVolumetricTexture(densityPattern, turbulencePattern);
        auto densityField = ComputeDensityField(densityPattern, temperature);
        auto scatteringParams = CalculateScatteringParams(humidity, temperature);

        // Set output values for connected nodes
        SetOutputValue("VolumetricTexture", volumetricTexture);
        SetOutputValue("DensityField", densityField);
        SetOutputValue("ScatteringParams", scatteringParams);
    }

    void DrawInNodeGraph() override {
        ImGui::BeginGroup();
        ImGui::Text("Fog Node");
        
        // Draw input ports
        DrawInputPort("WindIntensity", "Wind");
        DrawInputPort("Humidity", "Humidity");
        DrawInputPort("Temperature", "Temp");
        DrawInputPort("DensityPattern", "Density");
        DrawInputPort("TurbulencePattern", "Turbulence");
        
        // Draw output ports
        DrawOutputPort("VolumetricTexture", "Volume");
        DrawOutputPort("DensityField", "Density");
        DrawOutputPort("ScatteringParams", "Scatter");
        
        ImGui::EndGroup();
    }

private:
    HD_FogNodeInfo FogInfo;

    void InitializeProceduralPorts() {
        // Set up additional procedural-specific ports
        if (FogInfo.supportsDynamicPatterns) {
            FogInfo.Inputs.push_back("TimeScale");
            FogInfo.Inputs.push_back("Evolution");
        }
    }

    ProceduralPatternData CreateProceduralPattern(const ProceduralStructureParams& params) {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        std::string patternId = orchestrator.CreateVolumeTexturePattern(params);
        return orchestrator.GetProceduralPattern(patternId);
    }
};

} // namespace hd
```

This revised approach:
1. Properly integrates with the node graph system
2. Allows visual composition in the editor
3. Supports connection with other nodes (materials, atmosphere, weather, etc.)
4. Maintains the procedural capabilities while being node-based
5. Can be part of larger node graphs for complex atmospheric effects

Should I revise the other systems similarly to make them node-based? This would allow users to create complex weather systems, vegetation systems, and particle effects through visual node composition while still leveraging the procedural orchestrator.