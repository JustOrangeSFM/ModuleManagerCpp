add_library(GameplayModule SHARED
    GameplayModule.cpp
)

target_link_libraries(GameplayModule PRIVATE EngineCore)

target_include_directories(GameplayModule PRIVATE
    ../../Core
    ../Rendering
)

set_target_properties(GameplayModule PROPERTIES
    PREFIX ""
    OUTPUT_NAME "Gameplay"
)