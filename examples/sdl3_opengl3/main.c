// Original C++ example ported to C by XReaper95, using Dear Bindings instead of
// ImGui directly

// Dear ImGui: standalone example application for SDL3 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows,
// inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/
// folder).
// - Introduction, links and more at the top of imgui.cpp

#include "embedded_font/font_roboto_medium.h"
#include "imgui_c/backend/opengl3.h"
#include "imgui_c/backend/sdl3.h"
#include "imgui_c/dcimgui.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

// Maybe look into porting this later
// #ifdef __EMSCRIPTEN__
// #include "libs/emscripten/emscripten_mainloop_stub.h"
// #endif

// Main code
int main(void) {
  // Setup SDL
  // [If using SDL_MAIN_USE_CALLBACKS: all code below until the main loop starts
  // would likely be your SDL_AppInit() function]
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
    printf("Error: SDL_Init(): %s\n", SDL_GetError());
    return -1;
  }

  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100 (WebGL 1.0)
  const char *glsl_version = "#version 100";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
  // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
  const char *glsl_version = "#version 300 es";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
  // GL 3.2 Core + GLSL 150
  const char *glsl_version = "#version 150";
  SDL_GL_SetAttribute(
      SDL_GL_CONTEXT_FLAGS,
      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                                 SDL_WINDOW_HIDDEN |
                                 SDL_WINDOW_HIGH_PIXEL_DENSITY;
  SDL_Window *window = SDL_CreateWindow("Dear ImGui SDL3+OpenGL3 example",
                                        (int)(1280 * main_scale),
                                        (int)(720 * main_scale), window_flags);
  if (window == NULL) {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    return -1;
  }
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  if (gl_context == NULL) {
    printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
    return -1;
  }

  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  SDL_ShowWindow(window);

  // Setup Dear ImGui context
  CIMGUI_CHECKVERSION();
  ImGuiContext *ctx = ImGui_CreateContext(NULL);
  ImGuiIO *io = ImGui_GetIO();
  io->ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io->ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // load a custom font
  ImFontAtlas_AddFontFromMemoryCompressedTTF(
      io->Fonts, &RobotoMedium_compressed_data, RobotoMedium_compressed_size,
      20.0f, NULL, NULL);

  // Setup Dear ImGui style
  ImGui_StyleColorsDark(NULL);
  // ImGui::StyleColorsLight();

  // Setup scaling
  ImGuiStyle *style = ImGui_GetStyle();

  ImGuiStyle_ScaleAllSizes(
      style, main_scale); // Bake a fixed style scale. (until we have a solution
                          // for dynamic style scaling, changing this requires
                          // resetting Style + calling this again)
  style->FontScaleDpi =
      main_scale; // Set initial font scale. (using io.ConfigDpiScaleFonts=true
                  // makes this unnecessary. We leave both here for
                  // documentation purpose)

  // Setup Platform/Renderer backends
  ImGui_C_ImplSDL3_InitForOpenGL(window, gl_context);
  ImGui_C_ImplOpenGL3_Init(glsl_version);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can
  // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
  // them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
  // need to select the font among multiple.
  // - If the file cannot be loaded, the function will return a nullptr. Please
  // handle those errors in your application (e.g. use an assertion, or display
  // an error and quit).
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype
  // for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // - Our Emscripten build process allows embedding fonts to be accessible at
  // runtime from the "fonts/" folder. See Makefile.emscripten for details.
  // style.FontSizeBase = 20.0f;
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
  // IM_ASSERT(font != nullptr);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = {0.45f, 0.55f, 0.60f, 1.00f};

  // Main loop
  bool done = false;
  // #ifdef __EMSCRIPTEN__
  //     // For an Emscripten build we are disabling file-system access, so
  //     let's not attempt to do a fopen() of the imgui.ini file.
  //     // You may manually call LoadIniSettingsFromMemory() to load settings
  //     from your own storage. io.IniFilename = nullptr;
  //     EMSCRIPTEN_MAINLOOP_BEGIN
  // #else
  while (!done)
  // #endif
  {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    // your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    // data to your main application, or clear/overwrite your copy of the
    // keyboard data. Generally you may always pass all inputs to dear imgui,
    // and hide them from your application based on those two flags. [If using
    // SDL_MAIN_USE_CALLBACKS: call ImGui_ImplSDL3_ProcessEvent() from your
    // SDL_AppEvent() function]
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_C_ImplSDL3_ProcessEvent(&event);
      if (event.type == SDL_EVENT_QUIT)
        done = true;
      if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
          event.window.windowID == SDL_GetWindowID(window))
        done = true;
    }

    // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your
    // SDL_AppIterate() function]
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
      SDL_Delay(10);
      continue;
    }

    // Start the Dear ImGui frame
    ImGui_C_ImplOpenGL3_NewFrame();
    ImGui_C_ImplSDL3_NewFrame();
    ImGui_NewFrame();

    // 1. Show the big demo window (Most of the sample code is in
    // ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
    // ImGui!).
    if (show_demo_window)
      ImGui_ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
    // to create a named window.
    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui_Begin(
          "Hello, world!", NULL,
          0); // Create a window called "Hello, world!" and append into it.

      ImGui_Text("This is some useful text."); // Display some text (you can use
                                               // a format strings too)
      ImGui_Checkbox(
          "Demo Window",
          &show_demo_window); // Edit bools storing our window open/close state
      ImGui_Checkbox("Another Window", &show_another_window);

      ImGui_SliderFloat("float", &f, 0.0f,
                        1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui_ColorEdit3("clear color", (float *)&clear_color,
                       0); // Edit 3 floats representing a color

      if (ImGui_Button("Button")) // Buttons return true when clicked (most
                                  // widgets return true when edited/activated)
        counter++;
      ImGui_SameLine();
      ImGui_Text("counter = %d", counter);

      ImGui_Text("Application average %.3f ms/frame (%.1f FPS)",
                 1000.0f / io->Framerate, io->Framerate);
      ImGui_End();
    }

    // 3. Show another simple window.
    if (show_another_window) {
      ImGui_Begin(
          "Another Window", &show_another_window,
          0); // Pass a pointer to our bool variable (the window will have a
              // closing button that will clear the bool when clicked)
      ImGui_Text("Hello from another window!");
      if (ImGui_Button("Close Me"))
        show_another_window = false;
      ImGui_End();
    }

    // Rendering
    ImGui_Render();
    glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_C_ImplOpenGL3_RenderDrawData(ImGui_GetDrawData());
    SDL_GL_SwapWindow(window);
  }
#ifdef __EMSCRIPTEN__
  EMSCRIPTEN_MAINLOOP_END;
#endif

  // Cleanup
  // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your
  // SDL_AppQuit() function]
  ImGui_C_ImplOpenGL3_Shutdown();
  ImGui_C_ImplSDL3_Shutdown();
  ImGui_DestroyContext(ctx);

  SDL_GL_DestroyContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
