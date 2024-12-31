#pragma once

namespace Hydragon {

class PythonBindingSecurity {
public:
    // Validate Python callbacks
    static bool ValidateCallback(PyObject* callback);
    
    // Secure memory access from Python
    static void* SecureAccessFromPython(void* ptr, size_t size);
    
    // Prevent Python memory scanning
    static void ObfuscateMemoryLayout();
};

} // namespace Hydragon 