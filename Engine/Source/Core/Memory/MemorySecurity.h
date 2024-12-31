#pragma once
#include <random>
#include <array>

namespace Hydragon {

class MemorySecurity {
public:
    static constexpr size_t CANARY_SIZE = 8;
    using Canary = std::array<uint8_t, CANARY_SIZE>;
    
    static Canary GenerateCanary() {
        Canary canary;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        
        for(size_t i = 0; i < CANARY_SIZE; ++i) {
            canary[i] = static_cast<uint8_t>(dis(gen));
        }
        return canary;
    }
    
    static bool ValidateCanary(const void* ptr, const Canary& expected);
    
private:
    static std::array<uint8_t, 32> s_SecurityKey;  // Runtime-generated key
};

} // namespace Hydragon 