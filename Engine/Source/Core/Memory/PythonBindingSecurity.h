/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Python binding security for Hydragon
 */

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