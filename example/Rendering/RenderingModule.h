#pragma once
#include "../../Core/IModule.h"

class IRenderingModule : public IModule
{
public:
    virtual void DrawText(const char* text, float x, float y) = 0;
    virtual void ClearScreen() = 0;
};

class RenderingModule : public IRenderingModule
{
public:
    const char* GetName() const override { return "Rendering"; }
    void Startup() override;
    void Shutdown() override;
    void DrawText(const char* text, float x, float y) override;
    void ClearScreen() override;
};