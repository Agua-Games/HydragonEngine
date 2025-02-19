/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <string>
#include <vector>
#include <algorithm>

#include <imgui.h>
#include "IconsMaterialSymbols.h"

#include "MonetizationEditor.h"
#include "hdImgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace hdImgui {

struct MonetizationProduct {
    std::string id;
    std::string name;
    std::string type;  // "consumable", "non_consumable", "subscription"
    float price = 0.0f;
    std::string currency = "USD";
    bool isActive = true;
};

struct MonetizationMetrics {
    float revenue = 0.0f;
    int activeSubscriptions = 0;
    int totalTransactions = 0;
    float conversionRate = 0.0f;
    std::vector<std::pair<std::string, float>> topProducts;
};

struct MonetizationState {
    bool isConfigured = false;
    std::string platformName;
    std::vector<MonetizationProduct> products;
    MonetizationMetrics metrics;
    bool autoSync = true;
    bool testMode = true;
    
    enum class ViewMode {
        Dashboard,
        Products,
        Analytics,
        Integration
    } currentView = ViewMode::Dashboard;
};

void ShowMonetizationEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    static MonetizationState state{};
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Monetization", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Import Configuration")) {}
            if (ImGui::MenuItem("Export Configuration")) {}
            if (ImGui::MenuItem("Reset Settings")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Platform Settings")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Dashboard", nullptr, state.currentView == MonetizationState::ViewMode::Dashboard)) 
                state.currentView = MonetizationState::ViewMode::Dashboard;
            if (ImGui::MenuItem("Products", nullptr, state.currentView == MonetizationState::ViewMode::Products)) 
                state.currentView = MonetizationState::ViewMode::Products;
            if (ImGui::MenuItem("Analytics", nullptr, state.currentView == MonetizationState::ViewMode::Analytics)) 
                state.currentView = MonetizationState::ViewMode::Analytics;
            if (ImGui::MenuItem("Integration", nullptr, state.currentView == MonetizationState::ViewMode::Integration)) 
                state.currentView = MonetizationState::ViewMode::Integration;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Quick Actions Toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    if (ImGui::Button(ICON_MS_SHOPPING_CART "##Products", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Manage Products");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_ANALYTICS "##Analytics", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("View Analytics");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_SETTINGS "##Settings", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Integration Settings");
    ImGui::PopStyleVar();
    
    ImGui::Separator();

    // Main Layout - Left Panel and Right Panel
    static float leftPanelWidth = 250.0f;
    const float minLeftPanelWidth = 200.0f;
    const float maxLeftPanelWidth = 400.0f;

    // Left Panel - Quick Stats & Products List
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), true);
    
    if (ImGui::CollapsingHeader("Quick Stats", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Revenue: $%.2f", state.metrics.revenue);
        ImGui::Text("Active Subs: %d", state.metrics.activeSubscriptions);
        ImGui::Text("Conversion: %.1f%%", state.metrics.conversionRate);
    }

    if (ImGui::CollapsingHeader("Active Products", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginChild("ProductsList", ImVec2(0, 200), true);
        for (const auto& product : state.products)
        {
            ImGui::TextColored(
                product.isActive ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImVec4(0.7f, 0.7f, 0.7f, 1.0f),
                "%s", product.name.c_str()
            );
            ImGui::SameLine(ImGui::GetWindowWidth() - 70);
            ImGui::Text("%.2f %s", product.price, product.currency.c_str());
        }
        ImGui::EndChild();
    }
    
    ImGui::EndChild();

    ImGui::SameLine();

    // Resizer
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
    ImGui::Button("##Resizer", ImVec2(4.0f, -1));
    if (ImGui::IsItemActive())
    {
        leftPanelWidth += ImGui::GetIO().MouseDelta.x;
        leftPanelWidth = std::clamp(leftPanelWidth, minLeftPanelWidth, maxLeftPanelWidth);
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    // Right Panel - Main Content Area
    ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
    
    // Status Bar
    ImGui::BeginChild("StatusBar", ImVec2(0, 30), true);
    ImGui::Text("Status: %s", state.isConfigured ? "Configured" : "Not Configured");
    ImGui::SameLine();
    ImGui::TextColored(state.testMode ? ImVec4(1.0f, 0.8f, 0.0f, 1.0f) : ImVec4(0.0f, 0.8f, 0.0f, 1.0f),
        "(%s)", state.testMode ? "Test Mode" : "Live Mode");
    ImGui::SameLine(ImGui::GetWindowWidth() - 150);
    ImGui::Text("Platform: %s", state.platformName.c_str());
    ImGui::EndChild();

    // Main Content based on current view
    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);
    switch (state.currentView)
    {
        case MonetizationState::ViewMode::Dashboard:
            ImGui::Text("Monetization Dashboard");
            ImGui::TextWrapped("Connect to NodeGraph Editor for monetization workflow automation");
            
            ImGui::Spacing();
            ImGui::Checkbox("Test Mode", &state.testMode);
            ImGui::SameLine();
            ImGui::Checkbox("Auto-Sync", &state.autoSync);
            break;
            
        case MonetizationState::ViewMode::Products:
            if (ImGui::CollapsingHeader("Top Products", ImGuiTreeNodeFlags_DefaultOpen))
            {
                for (const auto& [product, revenue] : state.metrics.topProducts)
                {
                    ImGui::Text("%s: $%.2f", product.c_str(), revenue);
                }
            }
            break;
            
        case MonetizationState::ViewMode::Analytics:
            ImGui::Text("Revenue Analytics");
            ImGui::TextWrapped("Use NodeGraph Editor to create custom analytics workflows");
            break;
            
        case MonetizationState::ViewMode::Integration:
            ImGui::Text("Platform Integration");
            ImGui::TextWrapped("Configure platform-specific settings and NodeGraph connections");
            break;
    }
    ImGui::EndChild();

    ImGui::EndChild(); // End RightPanel
    ImGui::End();
}

} // namespace hdImgui
