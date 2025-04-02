/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - Open world setup with streaming, biomes, and weather
 *  - Procedural vegetation and wildlife
 *  - Dynamic weather and time of day
 *  - Quest and POI system
 *  - Population system for NPCs
 */
#if 0
#include "Engine.h"
#include "OpenWorldScene.h"
#include "ChunkManager.h"
#include "BiomeSystem.h"
#include "EcosystemAI.h"
#include "Node.h"

namespace hd {

class OpenWorldSetupExample_01 : public Node {
public:
    void setup() {
        auto& engine = Engine::getInstance();
        auto& graph = engine.getNodeGraph();

        // Main world setup with streaming
        OpenWorldScene world("WildernessWorld");
        world.setWorldBounds(Vector3(-8000, -1000, -8000), Vector3(8000, 1000, 8000))
             .setLODLevels(5)
             .enableInfiniteHorizon(true)
             .setStreamingBudget(512); // MB

        // Chunk management for streaming
        ChunkManager chunks;
        chunks.setChunkSize(256) // meters
              .setLoadDistance(2000)
              .setUnloadDistance(2500)
              .enableAsyncLoading(true)
              .setPrioritization("Distance");

        // Biome system
        BiomeSystem biomes;
        biomes.addBiome("Forest")
              .addBiome("Mountains")
              .addBiome("Plains")
              .addBiome("Desert")
              .setTransitionWidth(100.0f)
              .enableDynamicBlending(true);

        // Procedural vegetation
        VegetationSystem vegetation;
        vegetation.density(1000) // per km²
                 .variation(150)   // species
                 .windEffect(true)
                 .LODBias(0.75f);

        // Weather and time
        DynamicEnvironment environment;
        environment.dayLength(45) // minutes
                  .seasonLength(28) // days
                  .weatherSystem(true)
                  .climateModel("Realistic");

        // Wildlife AI ecosystem
        EcosystemAI wildlife;
        wildlife.addSpecies("Deer", 100)
               .addSpecies("Wolf", 30)
               .addSpecies("Bird", 500)
               .setInteractionRadius(1000)
               .enableMigration(true);

        // Quest and POI system
        QuestSystem quests;
        quests.setMaxActiveQuests(10)
              .setDynamicGeneration(true)
              .setQuestDensity(5) // per km²
              .loadQuestTemplates("Data/Quests/OpenWorld");

        // Population system for NPCs
        PopulationSystem population;
        population.setDensity(50) // NPCs per km²
                 .setTypes("Villager", "Merchant", "Guard")
                 .enableDynamicSchedules(true)
                 .setActivityRadius(500);

        // Connect systems
        graph.connect(chunks, "LoadedChunk", vegetation, "PopulateChunk");
        graph.connect(biomes, "BiomeData", vegetation, "BiomeParameters");
        graph.connect(environment, "WeatherState", vegetation, "WindParameters");
        graph.connect(environment, "TimeOfDay", population, "ScheduleUpdate");
        
        // Wildlife interactions
        graph.connect(environment, "SeasonData", wildlife, "MigrationTrigger");
        graph.connect(biomes, "BiomeData", wildlife, "HabitatParameters");
        
        // Quest generation
        graph.connect(population, "NPCData", quests, "QuestGivers");
        graph.connect(wildlife, "PopulationData", quests, "HuntingQuests");

        // Setup resource management
        setupResourceNodes(graph);
        setupStreamingPriorities(chunks);
    }

private:
    void setupResourceNodes(NodeGraph& graph) {
        ResourceManager resources;
        resources.setMemoryBudget(4096) // MB
                .setPriorityScheme("Distance")
                .enableBackgroundLoading(true);

        // Connect to streaming systems
        graph.connect(resources, "MemoryStatus", "ChunkManager", "LoadBudget");
        graph.connect(resources, "LoadProgress", "UI", "LoadingIndicator");
    }

    void setupStreamingPriorities(ChunkManager& chunks) {
        StreamingPriority priorities;
        priorities.addRule("PlayerDistance", 1.0f)
                 .addRule("ViewDirection", 0.7f)
                 .addRule("QuestRelevance", 0.5f);

        chunks.setPrioritySystem(priorities);
    }
};

} // namespace hd

#endif