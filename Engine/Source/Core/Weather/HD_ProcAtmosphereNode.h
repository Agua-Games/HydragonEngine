// Info struct inherits from HD_NodeInfo - used for metadata/configuration
struct HD_AtmosphereInfo : public HD_NodeInfo {
    HD_AtmosphereInfo() {
        NodeType = "Weather/Atmosphere";
        
        Inputs = {
            "Temperature",       // Base temperature
            "Humidity",         // Base humidity
            "Pressure",         // Air pressure
            "TimeOfDay",        // Day/night cycle
            "Season",           // Seasonal influence
            "Location"          // Geographic position
        };
        
        Outputs = {
            "AtmosphereState",  // Current state
            "ScatteringParams", // Light scattering
            "DensityProfile",   // Atmospheric density
            "OpticalDepth"      // Light transmission
        };

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

// Actual node class inherits from HD_Node - provides functionality
class HD_ProcAtmosphereNode : public HD_Node<AtmosphereState, ScatteringParams, DensityProfile> {
public:
    explicit HD_ProcAtmosphereNode(const HD_AtmosphereInfo& info = HD_AtmosphereInfo())
        : HD_Node(info) {}

    void ProcessNodeGraph() override {
        // Process inputs
        auto temperature = GetInputValue<float>("Temperature");
        auto humidity = GetInputValue<float>("Humidity");
        auto pressure = GetInputValue<float>("Pressure");
        auto timeOfDay = GetInputValue<float>("TimeOfDay");
        auto season = GetInputValue<SeasonData>("Season");
        auto location = GetInputValue<glm::vec3>("Location");

        // Compute atmospheric state
        AtmosphereState state = ComputeAtmosphereState(
            temperature, humidity, pressure, timeOfDay, season, location
        );

        // Calculate derived parameters
        ScatteringParams scattering = ComputeScatteringParams(state);
        DensityProfile density = ComputeDensityProfile(state);
        float opticalDepth = ComputeOpticalDepth(density);

        // Set outputs
        SetOutputValue("AtmosphereState", state);
        SetOutputValue("ScatteringParams", scattering);
        SetOutputValue("DensityProfile", density);
        SetOutputValue("OpticalDepth", opticalDepth);
    }

private:
    // Implementation methods...
};
