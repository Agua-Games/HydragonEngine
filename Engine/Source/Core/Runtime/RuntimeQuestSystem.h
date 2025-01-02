/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime quest and objective management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeQuestConfig {
    bool enableBranching = true;
    bool enableParallelQuests = true;
    bool enableQuestChains = true;
    bool enableTimeTracking = true;
    uint32_t maxActiveQuests = 50;
    uint32_t maxQuestSteps = 20;
    uint32_t maxQuestConditions = 10;
    float questUpdateInterval = 0.5f;
    std::string questDatabasePath = "Config/Quests";
};

class RuntimeQuestSystem {
public:
    static RuntimeQuestSystem& Get();
    
    void Initialize(const RuntimeQuestConfig& config = {});
    void Shutdown();
    
    void Update();
    
    QuestHandle StartQuest(const std::string& questId, const QuestParams& params = {});
    void CompleteQuest(QuestHandle handle);
    void FailQuest(QuestHandle handle);
    void AbandonQuest(QuestHandle handle);
    
    void UpdateObjective(QuestHandle handle, const std::string& objectiveId, uint32_t progress);
    void CompleteObjective(QuestHandle handle, const std::string& objectiveId);
    void FailObjective(QuestHandle handle, const std::string& objectiveId);
    
    void AddQuestCondition(QuestHandle handle, const QuestCondition& condition);
    void RemoveQuestCondition(QuestHandle handle, const std::string& conditionId);
    
    void RegisterQuestCallback(QuestEventType type, const QuestCallback& callback);
    void UnregisterQuestCallback(QuestEventType type);
    
    void LoadQuestDatabase(const std::string& path);
    void ReloadQuestDatabase();
    
    void SaveQuestState(const std::string& name);
    void LoadQuestState(const std::string& name);
    
    bool IsQuestAvailable(const std::string& questId) const;
    bool IsQuestComplete(QuestHandle handle) const;
    QuestState GetQuestState(QuestHandle handle) const;
    float GetObjectiveProgress(QuestHandle handle, const std::string& objectiveId) const;
    std::vector<QuestHandle> GetActiveQuests() const;
    const RuntimeQuestStats& GetStats() const { return m_Stats; }

private:
    RuntimeQuestSystem() = default;
    
    RuntimeQuestConfig m_Config;
    RuntimeQuestStats m_Stats;
    std::unordered_map<QuestHandle, QuestInstance> m_Quests;
    std::unordered_map<std::string, QuestTemplate> m_QuestTemplates;
    std::unordered_map<QuestEventType, QuestCallback> m_Callbacks;
    std::unique_ptr<IQuestProcessor> m_Processor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 