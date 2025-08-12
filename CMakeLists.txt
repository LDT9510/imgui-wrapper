cmake_minimum_required(VERSION 3.23)
project(imgui_c
        VERSION 1.92.2
        DESCRIPTION "Custom Dear ImGui C wrapper"
        LANGUAGES C CXX
)

option(IMGUI_C_EXAMPLE "Compiles an example project using Win32 and Opengl backends" ON)
option(IMGUI_DEMO "Compiles the ImGui demo project" ON)
option(IMGUI_C_DISABLE_DEFAULT_FONT "Disables the default font, user should provide one or crash" OFF)

# NOTE: user code is responsible for linking libraries required by the backend

# Backend Platforms
if (WIN32)
    option(IMGUI_C_WIN32_BACKEND "Compiles the ImGui Windows platform backend" ON)
endif ()

# Backend Renderers
option(IMGUI_C_OPENGL3_BACKEND "Compiles the ImGui OpenGL 3 renderer backend" ON)

set(IMGUI_SOURCES
        "src/imgui.cpp"
        "src/imgui_draw.cpp"
        "src/imgui_tables.cpp"
        "src/imgui_widgets.cpp")

set(IMGUI_BINDING_SOURCES
        "src/dcimgui.cpp"
        "src/dcimgui_internal.cpp")

set(IMGUI_BACKEND_SOURCES
        $<$<BOOL:${IMGUI_WIN32_BACKEND}>:src/original_backend/imgui_impl_win32.cpp>
        $<$<BOOL:${IMGUI_OPENGL3_BACKEND}>:src/original_backend/imgui_impl_opengl3.cpp>)

set(IMGUI_BINDING_BACKEND_SOURCES
        $<$<BOOL:${IMGUI_WIN32_BACKEND}>:src/backend_wrapper/win32.cpp>
        $<$<BOOL:${IMGUI_OPENGL3_BACKEND}>:src/backend_wrapper/opengl3.cpp>)

set(IMGUI_DEMO_SOURCES $<$<BOOL:${IMGUI_DEMO}>:src/imgui_demo.cpp>)

add_library(${PROJECT_NAME} STATIC
        ${IMGUI_SOURCES}
        ${IMGUI_BINDING_SOURCES}
        ${IMGUI_BACKEND_SOURCES}
        ${IMGUI_BINDING_BACKEND_SOURCES}
        ${IMGUI_DEMO_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC "include" PRIVATE "include/imgui_c" "src")

target_compile_definitions(${PROJECT_NAME} PUBLIC
        IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        IMGUI_DISABLE_OBSOLETE_KEYIO
        $<$<BOOL:${IMGUI_C_DISABLE_DEFAULT_FONT}>:IMGUI_DISABLE_DEFAULT_FONT>)

if ("${IMGUI_C_EXAMPLE}")
    add_subdirectory(examples/win32_opengl3)
endif ()

message(STATUS "${PROJECT_NAME} configured correctly.")