cmake_minimum_required(VERSION 3.23)
project(imgui_c
        VERSION 1.92.2
        DESCRIPTION "Custom Dear ImGui C wrapper"
        LANGUAGES C CXX
)

option(IMGUI_C_EXAMPLES "Compile example projects" ON)
option(IMGUI_C_DEMO "Compile the ImGui demo project (this is the regular C++ project)" ON)
option(IMGUI_C_DISABLE_DEFAULT_FONT "Disable the default font, user should provide one or crash" OFF)

# NOTE: user code is responsible for providing externally linked libraries required by the backends

# Backend Platforms
if (WIN32)
    option(IMGUI_C_WIN32_BACKEND "Compile the ImGui Windows platform backend" OFF)
endif ()
option(IMGUI_C_SDL3_BACKEND "Compile the ImGui SDL3 platform backend" OFF)

# Backend Renderers
option(IMGUI_C_OPENGL3_BACKEND "Compile the ImGui OpenGL 3 renderer backend" OFF)

set(SOURCES_DIR src)

set(IMGUI_SOURCES
        # ========== Dear ImGui ==========
        "${SOURCES_DIR}/imgui.cpp"
        "${SOURCES_DIR}/imgui_draw.cpp"
        "${SOURCES_DIR}/imgui_tables.cpp"
        "${SOURCES_DIR}/imgui_widgets.cpp"
        $<$<BOOL:${IMGUI_C_DEMO}>:${SOURCES_DIR}/imgui_demo.cpp>
        # ========== Dear Bindings ==========
        "${SOURCES_DIR}/dcimgui.cpp"
        "${SOURCES_DIR}/dcimgui_internal.cpp")

set(IMGUI_BACKEND_SOURCES
        # ========== original sources ==========
        $<$<BOOL:${IMGUI_C_WIN32_BACKEND}>:${SOURCES_DIR}/original_backend/imgui_impl_win32.cpp>
        $<$<BOOL:${IMGUI_C_SDL3_BACKEND}>:${SOURCES_DIR}/original_backend/imgui_impl_sdl3.cpp>
        $<$<BOOL:${IMGUI_C_OPENGL3_BACKEND}>:${SOURCES_DIR}/original_backend/imgui_impl_opengl3.cpp>
        # ========== binding sources ==========
        $<$<BOOL:${IMGUI_C_WIN32_BACKEND}>:${SOURCES_DIR}/backend_wrapper/win32.cpp>
        $<$<BOOL:${IMGUI_C_SDL3_BACKEND}>:${SOURCES_DIR}/backend_wrapper/sdl3.cpp>
        $<$<BOOL:${IMGUI_C_OPENGL3_BACKEND}>:${SOURCES_DIR}/backend_wrapper/opengl3.cpp>)

add_library(${PROJECT_NAME} STATIC ${IMGUI_SOURCES} ${IMGUI_BACKEND_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC "include" PRIVATE "include/imgui_c" ${SOURCES_DIR})

target_compile_definitions(${PROJECT_NAME} PUBLIC
        # disable obsolete API
        IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        IMGUI_DISABLE_OBSOLETE_KEYIO
        
        # makes excluding te default font easier
        $<$<BOOL:${IMGUI_C_DISABLE_DEFAULT_FONT}>:IMGUI_DISABLE_DEFAULT_FONT>)

if (IMGUI_C_SDL3_BACKEND)
    # declare linking here, but SDL3 should be provided by the user code
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
endif ()

if ("${IMGUI_C_EXAMPLES}")
    if (WIN32)
        add_subdirectory(examples/win32_opengl3)
    endif ()
    
    add_subdirectory(examples/sdl3_opengl3)
endif ()

message(STATUS "${PROJECT_NAME} configured correctly")