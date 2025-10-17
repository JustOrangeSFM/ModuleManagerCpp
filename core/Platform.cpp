#include "Platform.h"
#include <iostream>

DynamicLibrary::DynamicLibrary(const std::string& path)
{
#if defined(_WIN32)
    handle_ = LoadLibraryA(path.c_str());
#elif defined(__ANDROID__) || defined(__linux__)
    handle_ = dlopen(path.c_str(), RTLD_LAZY);
#elif defined(__APPLE__)
    handle_ = dlopen(path.c_str(), RTLD_LAZY);
#endif
    if (!handle_)
    {
        std::cerr << "Failed to load library: " << path << "\n";
#if defined(__ANDROID__) || defined(__linux__) || defined(__APPLE__)
        std::cerr << "dlerror: " << dlerror() << "\n";
#endif
    }
}

DynamicLibrary::~DynamicLibrary()
{
    if (handle_)
    {
#if defined(_WIN32)
        FreeLibrary(handle_);
#else
        dlclose(handle_);
#endif
    }
}