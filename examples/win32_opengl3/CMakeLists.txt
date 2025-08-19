cmake_minimum_required(VERSION 3.23)
project(imgui-wrapper-win32-opengl3
        VERSION 0.0.1
        DESCRIPTION "Example using imgui-wrapper with Windows and OpenGL."
        LANGUAGES C
)

set(IMWRAPPER_DEMO ON CACHE BOOL "" FORCE)
set(IMWRAPPER_WIN32_BACKEND ON CACHE BOOL "" FORCE)
set(IMWRAPPER_OPENGL3_BACKEND ON CACHE BOOL "" FORCE)

# Use a custom font in this example
set(IMWRAPPER_DISABLE_DEFAULT_FONT ON CACHE BOOL "" FORCE)

add_executable(${PROJECT_NAME} "main.c")
target_include_directories(${PROJECT_NAME} PRIVATE "../common")
target_link_libraries(${PROJECT_NAME} imgui_wrapper opengl32)

message(STATUS "Configured '${PROJECT_NAME}' example project")
