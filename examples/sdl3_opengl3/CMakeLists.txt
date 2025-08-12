cmake_minimum_required(VERSION 3.23)
project(imgui_c-sdl3-opengl3
        VERSION 0.0.1
        DESCRIPTION "Example using imgui_c with SDL3 and OpenGL."
        LANGUAGES C
)

set(IMGUI_C_DEMO ON CACHE BOOL "" FORCE)
set(IMGUI_C_SDL3_BACKEND ON CACHE BOOL "" FORCE)
set(IMGUI_C_OPENGL3_BACKEND ON CACHE BOOL "" FORCE)

# Use a custom font in this example
set(IMGUI_C_DISABLE_DEFAULT_FONT ON CACHE BOOL "" FORCE)

add_subdirectory(external/SDL3)

add_executable(${PROJECT_NAME} "main.c")
target_include_directories(${PROJECT_NAME} PRIVATE "../common")
target_link_libraries(${PROJECT_NAME} imgui_c SDL3::SDL3 opengl32)

if (WIN32)
    add_custom_command (
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            "$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>" 
            USES_TERMINAL 
            COMMAND_EXPAND_LISTS
    )
endif ()

message(STATUS "Configured '${PROJECT_NAME}' example project")
