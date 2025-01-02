/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Editor dialog and modal management system
 */

#pragma once
#include "UITypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Editor {

struct DialogManagerConfig {
    bool enableCustomDialogs = true;
    bool enableModalStack = true;
    bool enableAnimations = true;
    bool enableBackdrop = true;
    uint32_t maxModalDepth = 5;
    float fadeTime = 0.2f;
    std::string defaultStyle = "Modern";
};

class DialogManagerSystem {
public:
    static DialogManagerSystem& Get();
    
    void Initialize(const DialogManagerConfig& config = {});
    void Shutdown();
    
    void Update();
    
    DialogHandle ShowDialog(const std::string& type, const DialogDesc& desc);
    void CloseDialog(DialogHandle handle);
    void CloseAllDialogs();
    
    DialogHandle ShowModal(const std::string& type, const DialogDesc& desc);
    void CloseModal(DialogHandle handle);
    
    void RegisterDialogType(const std::string& type, std::unique_ptr<IDialog> dialog);
    void UnregisterDialogType(const std::string& type);
    
    bool IsDialogOpen(DialogHandle handle) const;
    bool HasOpenModals() const;
    
    const DialogManagerStats& GetStats() const { return m_Stats; }

private:
    DialogManagerSystem() = default;
    
    DialogManagerConfig m_Config;
    DialogManagerStats m_Stats;
    std::vector<DialogInstance> m_DialogStack;
    std::unordered_map<std::string, std::unique_ptr<IDialog>> m_DialogTypes;
    std::unique_ptr<IDialogFactory> m_Factory;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 