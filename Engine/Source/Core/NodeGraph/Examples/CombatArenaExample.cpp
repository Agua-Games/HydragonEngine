#include "Core/Engine.h"
#include "Core/Scene/SceneNode.h"
#include "Core/Effects/PostProcess.h"
#include "Core/Audio/AudioNode.h"
#include "Core/Particles/ParticleSystem.h"
#include "Core/Physics/PhysicsNode.h"
#include "Core/Gameplay/TriggerNode.h"

namespace hd {

class CombatArenaSetup {
public:
    void setupLevel() {
        auto& engine = Engine::GetInstance();
        auto& graph = engine.getNodeGraph();

        // Main scene setup
        auto scene = std::make_shared<SceneNode>("CombatArena");
        scene->loadGeometry("Levels/Arena/MainGeometry.usd");
        scene->setLightingSetup("DynamicBattleField");

        // Combat area trigger zones
        auto combatTrigger = std::make_shared<TriggerNode>("CombatZone");
        combatTrigger->setVolume(Box(Vector3(-50, 0, -50), Vector3(50, 20, 50)));
        
        // Dynamic particle effects for combat
        auto combatEffects = std::make_shared<ParticleSystem>("CombatEffects");
        combatEffects->addEmitter<SphereEmitter>()
                     ->setProperty("radius", 5.0f)
                     ->setProperty("particleLife", 2.0f);

        // Combat music system
        auto musicSystem = std::make_shared<AudioNode>("CombatMusic");
        musicSystem->loadBank("Music/Combat/Intensity.bank");
        musicSystem->setProperty("crossfadeTime", 2.0f);

        // Physics setup for destructible elements
        auto physics = std::make_shared<PhysicsNode>("ArenaPhysics");
        physics->setGravity(Vector3(0, -9.81f, 0));
        physics->enableDebris(true);

        // Post-process chain
        auto postProcess = std::make_shared<PostProcess>("CombatPostProcess");
        postProcess->addEffect<BloomEffect>()
                   ->addEffect<MotionBlurEffect>()
                   ->addEffect<ColorGradingEffect>("LUTs/Combat.cube");

        // Connect the nodes
        graph.connect(combatTrigger, "OnEnter", musicSystem, "IncreaseCombatIntensity");
        graph.connect(combatTrigger, "OnEnter", combatEffects, "Activate");
        graph.connect(physics, "DebrisEmitted", combatEffects, "EmitLocation");
        graph.connect(combatEffects, "ParticleData", postProcess, "ParticleBloom");

        // Setup feedback loops
        graph.connect(musicSystem, "IntensityLevel", postProcess, "BloomIntensity");
        graph.connect(physics, "DestructionAmount", musicSystem, "CombatIntensity");
    }
};

} // namespace hd