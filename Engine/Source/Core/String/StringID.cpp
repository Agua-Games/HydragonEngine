#include "StringID.h"

namespace hd {

StringTable& StringTable::get() {
    static StringTable instance;
    return instance;
}

uint32_t StringTable::intern(std::string_view str) {
    std::unique_lock lock(m_mutex);
    
    // Check if string already exists
    auto it = m_stringToId.find(std::string(str));
    if (it != m_stringToId.end()) {
        return it->second;
    }
    
    // Create new entry
    uint32_t newId = m_nextId++;
    std::string stored_str(str);
    m_stringToId[stored_str] = newId;
    m_idToString[newId] = std::move(stored_str);
    return newId;
}

std::string_view StringTable::lookup(uint32_t id) const {
    std::shared_lock lock(m_mutex);
    auto it = m_idToString.find(id);
    return it != m_idToString.end() ? 
        std::string_view(it->second) : std::string_view();
}

uint32_t StringID::internString(std::string_view str) {
    return StringTable::get().intern(str);
}

std::string_view StringID::toString() const {
    return StringTable::get().lookup(m_id);
}

} // namespace hd