/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Book.h
 * @brief Header file for the Book class.
 * 
 * ARCHITECTURAL NOTES:
 * - Book is a class that represents a book in Hydragon.
 * - It is used to represent any book in the game world, such as books, scrolls, etc.
 * - It supports interactive features, such as reading, turning pages, and bookmarking. Also supports two-way messaging with other books, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "Prop.h"
#include "DataTable.h"
#include "MemoryBank.h"

namespace hd {

struct BookInfo : public PropInfo {
    BookInfo() {
        nodeType = "Gameplay/Book";
        
        inputs = {
            "bookData",        // Book data
            "environment",     // Environment data
            "characterData",   // Character data
            "bookState"        // Book state
        };
        
        outputs = {
            "bookStatus",      // Book status
            "bookMetrics"      // Book performance metrics
        };
    }
};

class Book : public Prop {
public:
    // === Allocation, Initialization, Loading ===
    explicit Book(const BookInfo& info = BookInfo())
        : Prop(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void read();
    void turnPage();
    void bookmark();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Book() = default;     // Default destructor
};

} // namespace hd
