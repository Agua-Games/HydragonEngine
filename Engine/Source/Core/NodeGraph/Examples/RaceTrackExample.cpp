#include "Core/Engine.h"
#include "Core/Scene/SceneNode.h"
#include "Core/Physics/VehiclePhysics.h"
#include "Core/Effects/WeatherSystem.h"
#include "Core/Audio/AudioNode.h"
#include "Core/Gameplay/RaceManager.h"

namespace hd {

class RaceTrackSetup {
public:
    void setupLevel() {
        auto& graph = Engine::GetInstance().getNodeGraph();

        // Track and environment
        auto scene = std::make_shared<SceneNode>("MountainCircuit");
        scene->loadGeometry("Tracks/Mountain/Track.usd")
             ->setProperty("materialQuality", 0.9f);

        // Weather and time system
        auto weather = std::make_shared<WeatherSystem>("DynamicWeather");
        weather->setTimeProgression(0.5f) // Day/night cycle speed
               ->enableDynamicWeather(true)
               ->setWeatherTransitionTime(300.0f); // 5 minutes

        // Vehicle physics
        auto vehiclePhysics = std::make_shared<VehiclePhysics>("PlayerVehicle");
        vehiclePhysics->loadVehicleData("Vehicles/SportsCar.json")
                      ->setTireFriction(0.8f)
                      ->enableAerodynamics(true);

        // Surface system for different materials
        auto surfaceSystem = std::make_shared<SurfaceNode>("TrackSurface");
        surfaceSystem->addSurface("Asphalt", 1.0f)
                     ->addSurface("Gravel", 0.7f)
                     ->addSurface("Grass", 0.3f);

        // Audio system with dynamic mixing
        auto audioSystem = std::make_shared<AudioNode>("RaceAudio");
        audioSystem->loadBank("Vehicles/Engine.bank")
                   ->loadBank("Environment/Track.bank")
                   ->setDoppler(true);

        // Race management
        auto raceManager = std::make_shared<RaceManager>("RaceControl");
        raceManager->setLaps(3)
                   ->setCheckpoints("Tracks/Mountain/Checkpoints.json")
                   ->enableGhostCar(true);

        // Post-processing for speed effects
        auto speedEffects = std::make_shared<PostProcess>("SpeedEffects");
        speedEffects->addEffect<MotionBlurEffect>()
                    ->addEffect<LensDistortionEffect>()
                    ->addEffect<ColorGradingEffect>("LUTs/Racing.cube");

        // Connect systems
        graph.connect(weather, "SurfaceWetness", surfaceSystem, "WetnessFactor");
        graph.connect(surfaceSystem, "SurfaceData", vehiclePhysics, "SurfaceResponse");
        graph.connect(vehiclePhysics, "Speed", speedEffects, "MotionBlurAmount");
        graph.connect(vehiclePhysics, "EngineLoad", audioSystem, "EngineSound");
        
        // Weather affects vehicle physics
        graph.connect(weather, "WindData", vehiclePhysics, "AerodynamicsInput");
        graph.connect(weather, "RainAmount", surfaceSystem, "WetnessFactor");

        // Race events
        graph.connect(raceManager, "RaceStart", audioSystem, "PlayStartSequence");
        graph.connect(raceManager, "LapComplete", audioSystem, "PlayLapSound");
        
        // Setup dynamic difficulty
        setupDynamicDifficulty(graph, raceManager, vehiclePhysics);
    }

private:
    void setupDynamicDifficulty(NodeGraph& graph, std::shared_ptr<RaceManager> race,
                               std::shared_ptr<VehiclePhysics> vehicle) {
        auto difficulty = std::make_shared<DifficultyNode>("DynamicDifficulty");
        difficulty->setAdaptationRate(0.3f)
                  ->setMinDifficulty(0.4f)
                  ->setMaxDifficulty(0.9f);

        graph.connect(race, "PlayerPerformance", difficulty, "AdaptationInput");
        graph.connect(difficulty, "DifficultyLevel", vehicle, "AIDifficulty");
    }
};

} // namespace hd