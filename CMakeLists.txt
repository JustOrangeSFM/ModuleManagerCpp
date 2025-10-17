# MyEngine/CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(MyEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# === 1. Статическая библиотека с ядром ===
add_library(EngineCore STATIC
    Core/ModuleManager.cpp
    Core/Platform.cpp
)
target_include_directories(EngineCore PUBLIC Core)

# === 2. Основное приложение (.exe) ===
add_executable(MyEngine ModuleManagerCpp.cpp)
target_link_libraries(MyEngine PRIVATE EngineCore)

# === 3. Модули ===
add_subdirectory(example/Rendering)
add_subdirectory(example/Gameplay)

# === 4. Копирование DLL в ./modules/ ===
if(NOT ANDROID)
    set(MODULES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/modules")
    add_custom_command(TARGET MyEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${MODULES_OUTPUT_DIR}"
    )
    foreach(module RenderingModule GameplayModule)
        add_custom_command(TARGET MyEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_FILE:${module}>
                "${MODULES_OUTPUT_DIR}/"
        )
    endforeach()
endif()