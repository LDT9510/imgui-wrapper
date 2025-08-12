cmake_minimum_required(VERSION 3.23)
project(imgui_c-example
        VERSION 0.0.1
        DESCRIPTION "Example using imgui_c with Windows and OpenGL."
        LANGUAGES C
)

if (NOT WIN32)
    message(FATAL_ERROR "The example project is Windows only.")
endif ()

set(IMGUI_DEMO ON CACHE BOOL "" FORCE)
set(IMGUI_C_OPENGL3_BACKEND ON CACHE BOOL "" FORCE)
set(IMGUI_C_WIN32_BACKEND ON CACHE BOOL "" FORCE)

# I use a custom font in this example
set(IMGUI_C_DISABLE_DEFAULT_FONT ON CACHE BOOL "" FORCE)

message(STATUS "Compiling the imgui-c example project")
add_executable(${PROJECT_NAME} "main.c")
target_include_directories(${PROJECT_NAME} PRIVATE "../common")
target_link_libraries(${PROJECT_NAME} imgui_c opengl32)
