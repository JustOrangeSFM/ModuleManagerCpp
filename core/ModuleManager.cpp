#include "ModuleManager.h"
#include "Platform.h"
#include <filesystem>
#include <iostream>
#include <set>
#include <queue>

ModuleManager& ModuleManager::Get()
{
    static ModuleManager inst;
    return inst;
}

void ModuleManager::LoadModulesFromDirectory(const std::string& dirPath)
{
    namespace fs = std::filesystem;
    if (!fs::exists(dirPath)) return;

    for (const auto& entry : fs::directory_iterator(dirPath))
    {
        const auto& path = entry.path();
        std::string ext = path.extension().string();

#if defined(_WIN32)
        if (ext != ".dll") continue;
#elif defined(__ANDROID__) || defined(__linux__)
        if (ext != ".so") continue;
#elif defined(__APPLE__)
        if (ext != ".dylib") continue;
#endif

        auto lib = std::make_unique<DynamicLibrary>(path.string());
        if (!lib->IsValid()) continue;

        auto createFunc = lib->GetFunction<CreateModuleFunc>("CreateModule");
        if (!createFunc) continue;

        IModule* module = createFunc();
        if (!module) continue;

        std::cout << "Loaded module: " << module->GetName() << "\n";
        modules_.push_back({ std::move(lib), module });
        moduleMap_[module->GetName()] = module;
    }
}


void ModuleManager::TopologicalStartup()
{
    std::unordered_map<std::string, int> inDegree;
    std::unordered_map<std::string, std::vector<std::string>> graph;

    // Инициализация
    for (auto& m : modules_)
    {
        std::string name = m.instance->GetName();
        inDegree[name] = 0;
        graph[name] = {};
    }

    // Построение графа зависимостей
    for (auto& m : modules_)
    {
        std::string name = m.instance->GetName();
        auto deps = m.instance->GetDependencies();
        for (const char* dep : deps)
        {
            if (moduleMap_.count(dep) == 0)
            {
                std::cerr << "ERROR: Module '" << name << "' depends on missing module '" << dep << "'\n";
                continue;
            }
            graph[dep].push_back(name);
            inDegree[name]++;
        }
    }

    std::queue<std::string> q;
    for (auto& [name, deg] : inDegree)
    {
        if (deg == 0) q.push(name);
    }

    while (!q.empty())
    {
        std::string name = q.front(); q.pop();
        moduleMap_[name]->Startup();

        for (const std::string& neighbor : graph[name])
        {
            if (--inDegree[neighbor] == 0)
                q.push(neighbor);
        }
    }

    for (auto& [name, deg] : inDegree)
    {
        if (deg > 0)
        {
            std::cerr << "ERROR: Circular dependency involving module '" << name << "'\n";
        }
    }
}

void ModuleManager::StartupAll()
{
    TopologicalStartup();
}

void ModuleManager::ShutdownAll()
{
    // Обратный порядок (можно улучшить)
    for (auto it = modules_.rbegin(); it != modules_.rend(); ++it)
    {
        it->instance->Shutdown();
    }
    modules_.clear();
    moduleMap_.clear();
}