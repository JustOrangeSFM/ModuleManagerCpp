#include "RenderingModule.h"
#include <iostream>


void RenderingModule::Startup() { std::cout << "[Rendering] Started\n"; }
void RenderingModule::Shutdown() { std::cout << "[Rendering] Shutdown\n"; }
void RenderingModule::DrawText(const char* text, float x, float y)
{
    std::cout << "[Rendering] DrawText: " << text << " at (" << x << ", " << y << ")\n";
}
void RenderingModule::ClearScreen()
{
    std::cout << "[Rendering] Screen cleared\n";
}

extern "C" IModule* CreateModule()
{
    return new RenderingModule();
}