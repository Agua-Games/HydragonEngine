#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include <shared_mutex>

namespace hd {

class StringID {
public:
    // Default constructor creates invalid ID
    constexpr StringID() : m_id(0) {}
    
    // Create from string - this is where interning happens
    explicit StringID(std::string_view str) : m_id(internString(str)) {}
    
    // Fast operations
    constexpr uint32_t id() const { return m_id; }
    bool isValid() const { return m_id != 0; }
    bool operator==(const StringID& other) const { return m_id == other.m_id; }
    
    // Get the actual string - slower operation
    std::string_view toString() const;
    
    // Hash support for containers
    struct Hash {
        size_t operator()(const StringID& sid) const {
            return std::hash<uint32_t>{}(sid.id());
        }
    };

private:
    uint32_t m_id;
    
    static uint32_t internString(std::string_view str);
};

// String table manager - handles the actual string storage
class StringTable {
public:
    static StringTable& get();
    
    uint32_t intern(std::string_view str);
    std::string_view lookup(uint32_t id) const;
    
private:
    StringTable() = default;
    
    mutable std::shared_mutex m_mutex;
    std::unordered_map<std::string, uint32_t> m_stringToId;
    std::unordered_map<uint32_t, std::string> m_idToString;
    uint32_t m_nextId = 1;
};

} // namespace hd