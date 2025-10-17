#include "GameplayModule.h"
#include "../../Core/ModuleManager.h"
#include "../Rendering/RenderingModule.h"
#include <iostream>

void GameplayModule::Startup()
{
    std::cout << "[Gameplay] Starting...\n";

    // Получаем модуль Rendering
    auto* rendering = ModuleManager::Get().GetModule<IRenderingModule>("Rendering");
    if (rendering)
    {
        rendering->ClearScreen();
        rendering->DrawText("Hello from Gameplay!", 100, 100);
    }
}

void GameplayModule::Shutdown()
{
    std::cout << "[Gameplay] Shutdown\n";
}

extern "C" IModule* CreateModule()
{
    return new GameplayModule();
}