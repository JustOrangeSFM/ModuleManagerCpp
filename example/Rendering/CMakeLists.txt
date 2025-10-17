add_library(RenderingModule SHARED
    RenderingModule.cpp
)

target_link_libraries(RenderingModule PRIVATE EngineCore)

target_include_directories(RenderingModule PRIVATE
    ../../Core
)

set_target_properties(RenderingModule PROPERTIES
    PREFIX ""
    OUTPUT_NAME "Rendering"
)