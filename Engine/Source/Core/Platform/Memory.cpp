#include "Memory.h"
#include "Core/Common/Assert.h"

#if HD_PLATFORM_WINDOWS
    #include <windows.h>
#else
    #include <sys/mman.h>
    #include <unistd.h>
#endif

namespace Hydragon {
namespace Platform {

void* Memory::AlignedAlloc(size_t size, size_t alignment) {
    HD_ASSERT(alignment > 0 && (alignment & (alignment - 1)) == 0, 
              "Alignment must be a power of 2");

    #if HD_PLATFORM_WINDOWS
        return _aligned_malloc(size, alignment);
    #else
        void* ptr = nullptr;
        if (posix_memalign(&ptr, alignment, size) != 0) {
            return nullptr;
        }
        return ptr;
    #endif
}

void Memory::AlignedFree(void* ptr) {
    #if HD_PLATFORM_WINDOWS
        _aligned_free(ptr);
    #else
        free(ptr);
    #endif
}

size_t Memory::GetPageSize() {
    #if HD_PLATFORM_WINDOWS
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        return si.dwPageSize;
    #else
        return sysconf(_SC_PAGESIZE);
    #endif
}

bool Memory::IsPageAligned(void* ptr) {
    return (reinterpret_cast<uintptr_t>(ptr) & (GetPageSize() - 1)) == 0;
}

#if HD_PLATFORM_WINDOWS
void* Memory::VirtualAlloc(size_t size) {
    return ::VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

void Memory::VirtualFree(void* ptr) {
    ::VirtualFree(ptr, 0, MEM_RELEASE);
}
#else
void* Memory::MMap(size_t size) {
    return mmap(nullptr, size, PROT_READ | PROT_WRITE, 
               MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

void Memory::MUnmap(void* ptr, size_t size) {
    munmap(ptr, size);
}
#endif

}} // namespace Hydragon::Platform 