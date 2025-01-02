/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime dialogue and conversation management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeDialogueConfig {
    bool enableBranching = true;
    bool enableLocalization = true;
    bool enableVariables = true;
    bool enableLipSync = true;
    uint32_t maxActiveDialogues = 4;
    uint32_t maxChoiceOptions = 8;
    float defaultTextSpeed = 30.0f;  // characters per second
    float defaultChoiceTimeout = 10.0f;
    std::string dialogueCachePath = "Cache/Dialogue";
};

class RuntimeDialogueSystem {
public:
    static RuntimeDialogueSystem& Get();
    
    void Initialize(const RuntimeDialogueConfig& config = {});
    void Shutdown();
    
    void Update();
    
    DialogueHandle StartDialogue(const std::string& dialogueId, const DialogueParams& params = {});
    void StopDialogue(DialogueHandle handle);
    void PauseDialogue(DialogueHandle handle);
    void ResumeDialogue(DialogueHandle handle);
    
    void AdvanceDialogue(DialogueHandle handle);
    void SelectChoice(DialogueHandle handle, uint32_t choiceIndex);
    void SetVariable(const std::string& name, const DialogueVariable& value);
    
    void SetSpeaker(DialogueHandle handle, EntityHandle speaker);
    void SetListener(DialogueHandle handle, EntityHandle listener);
    void SetTextSpeed(DialogueHandle handle, float charactersPerSecond);
    
    void LoadDialogueGraph(const std::string& path);
    void UnloadDialogueGraph(const std::string& path);
    
    void RegisterDialogueCallback(DialogueEventType type, const DialogueCallback& callback);
    void UnregisterDialogueCallback(DialogueEventType type);
    
    void AddCustomCondition(const std::string& name, const DialogueCondition& condition);
    void RemoveCustomCondition(const std::string& name);
    
    void SaveConversationState(DialogueHandle handle, const std::string& name);
    void LoadConversationState(DialogueHandle handle, const std::string& name);
    
    DialogueState GetDialogueState(DialogueHandle handle) const;
    std::vector<DialogueChoice> GetCurrentChoices(DialogueHandle handle) const;
    const RuntimeDialogueStats& GetStats() const { return m_Stats; }

private:
    RuntimeDialogueSystem() = default;
    
    RuntimeDialogueConfig m_Config;
    RuntimeDialogueStats m_Stats;
    std::unordered_map<DialogueHandle, DialogueInstance> m_Dialogues;
    std::unordered_map<std::string, DialogueGraph> m_LoadedGraphs;
    std::unordered_map<DialogueEventType, DialogueCallback> m_Callbacks;
    std::unordered_map<std::string, DialogueCondition> m_CustomConditions;
    std::unique_ptr<IDialogueProcessor> m_Processor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 