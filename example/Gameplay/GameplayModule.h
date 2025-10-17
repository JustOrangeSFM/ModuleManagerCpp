#pragma once
#include "../../Core/IModule.h"

class GameplayModule : public IModule
{
public:
    const char* GetName() const override { return "Gameplay"; }
    std::vector<const char*> GetDependencies() const override
    {
        return { "Rendering" }; //deps
    }
    void Startup() override;
    void Shutdown() override;
};