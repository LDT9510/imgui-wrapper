cmake_minimum_required(VERSION 3.23)
project(imgui_c-win32-opengl3
        VERSION 0.0.1
        DESCRIPTION "Example using imgui_c with Windows and OpenGL."
        LANGUAGES C
)

set(IMGUI_C_DEMO ON CACHE BOOL "" FORCE)
set(IMGUI_C_WIN32_BACKEND ON CACHE BOOL "" FORCE)
set(IMGUI_C_OPENGL3_BACKEND ON CACHE BOOL "" FORCE)

# Use a custom font in this example
set(IMGUI_C_DISABLE_DEFAULT_FONT ON CACHE BOOL "" FORCE)

add_executable(${PROJECT_NAME} "main.c")
target_include_directories(${PROJECT_NAME} PRIVATE "../common")
target_link_libraries(${PROJECT_NAME} imgui_c opengl32)

message(STATUS "Configured '${PROJECT_NAME}' example project")
