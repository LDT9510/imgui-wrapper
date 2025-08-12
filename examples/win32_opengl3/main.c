// Dear ImGui: standalone example application for Win32 + OpenGL 3

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/
// folder).
// - Introduction, links and more at the top of imgui.cpp

// This is provided for completeness, however it is strongly recommended you use
// OpenGL with SDL or GLFW.

// Original C++ example ported to C by XReaper, using Dear Bindings instead of
// ImGui directly

#include "embedded_font/font_roboto_medium.h"

#include "imgui_c/backend/opengl3.h"
#include "imgui_c/backend/win32.h"
#include "imgui_c/dcimgui.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <tchar.h>
#include <windows.h>
#include <GL/GL.h>

#include <stdbool.h>

// Data stored per platform window
typedef struct wgl_window_data {
  HDC hdc;
} wgl_window_data_t;

// Data
static HGLRC g_hrc;
static wgl_window_data_t g_main_window;
static int g_width;
static int g_height;

// Forward declarations of helper functions
bool create_device_wgl(HWND window_handler, wgl_window_data_t *data);
void cleanup_device_wgl(HWND window_handler, const wgl_window_data_t *data);
void reset_device_wgl();
LRESULT WINAPI wnd_proc(HWND window_handler, UINT msg, WPARAM w_param,
                        LPARAM l_param);

// Main code
int main(void) {
  SetProcessDPIAware();
  
  // Create application window
  //ImGui_C_ImplWin32_EnableDpiAwareness();  // this gives wrong result
  WNDCLASSEXW wc = {sizeof(wc),
                    CS_OWNDC,
                    wnd_proc,
                    0L,
                    0L,
                    GetModuleHandle(NULL),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    L"imgui-c Example",
                    NULL};
  RegisterClassExW(&wc);
  HWND hwnd = CreateWindowW(wc.lpszClassName, L"imgui-c Win32+OpenGL3 Example",
                            WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL,
                            NULL, wc.hInstance, NULL);

  // Initialize OpenGL
  if (!create_device_wgl(hwnd, &g_main_window)) {
    cleanup_device_wgl(hwnd, &g_main_window);
    DestroyWindow(hwnd);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
    return 1;
  }
  wglMakeCurrent(g_main_window.hdc, g_hrc);

  // Show the window
  ShowWindow(hwnd, SW_SHOWDEFAULT);
  UpdateWindow(hwnd);

  // Setup Dear ImGui context
  CIMGUI_CHECKVERSION();

  ImGuiContext *ctx = ImGui_CreateContext(NULL);
  ImGuiIO *io = ImGui_GetIO();

  // load a custom font
  ImFontAtlas_AddFontFromMemoryCompressedTTF(
     io->Fonts, &RobotoMedium_compressed_data, RobotoMedium_compressed_size,
     20.0f, NULL, NULL);

  (void)io;
  io->ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io->ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui_StyleColorsDark(NULL);
  // ImGui_StyleColorsClassic(NULL);

  // Setup Platform/Renderer backends
  ImGui_C_ImplWin32_InitForOpenGL(hwnd);
  ImGui_C_ImplOpenGL3_Init(NULL);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can
  // also load multiple fonts and use ImGui_PushFont()/PopFont() to select
  // them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
  // need to select the font among multiple.
  // - If the file cannot be loaded, the function will return a NULL. Please
  // handle those errors in your application (e.g. use an assertion, or display
  // an error and quit).
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype
  // for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // style.FontSizeBase = 20.0f;
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
  // IM_ASSERT(font != NULL);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = {0.45f, 0.55f, 0.60f, 1.00f};

  // Main loop
  bool done = false;
  while (!done) {
    // Poll and handle messages (inputs, window resize, etc.)
    // See the WndProc() function below for our to dispatch events to the Win32
    // backend.
    MSG msg;
    while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      if (msg.message == WM_QUIT)
        done = true;
    }

    if (IsIconic(hwnd)) {
      Sleep(10);
      continue;
    }

    // Start the Dear ImGui frame
    ImGui_C_ImplOpenGL3_NewFrame();
    ImGui_C_ImplWin32_NewFrame();
    ImGui_NewFrame();

    // 1. Show the big demo window (Most of the sample code is in
    // ImGui_ShowDemoWindow(NULL)! You can browse its code to learn more about
    // Dear ImGui!).
    if (show_demo_window)
      ImGui_ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
    // to create a named window.
    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui_Begin("Hello, world!", NULL, 0); // Create a window called "Hello,
                                             // world!" and append into it.

      ImGui_Text("This is some useful text."); // Display some text (you can
                                               // use a format strings too)
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
      ImGui_Begin("Another Window", &show_another_window,
                  0); // Pass a pointer to our bool variable (the
                      // window will have a closing button that will
                      // clear the bool when clicked)
      ImGui_Text("Hello from another window!");
      if (ImGui_Button("Close Me"))
        show_another_window = false;
      ImGui_End();
    }

    // Rendering
    ImGui_Render();
    glViewport(0, 0, g_width, g_height);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_C_ImplOpenGL3_RenderDrawData(ImGui_GetDrawData());

    // Present
    SwapBuffers(g_main_window.hdc);
  }

  ImGui_C_ImplOpenGL3_Shutdown();
  ImGui_C_ImplWin32_Shutdown();
  ImGui_DestroyContext(ctx);

  cleanup_device_wgl(hwnd, &g_main_window);
  wglDeleteContext(g_hrc);
  DestroyWindow(hwnd);
  UnregisterClassW(wc.lpszClassName, wc.hInstance);

  return 0;
}

// Helper functions
bool create_device_wgl(HWND window_handler, wgl_window_data_t *data) {
  HDC hDc = GetDC(window_handler);
  PIXELFORMATDESCRIPTOR pfd = {0};
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;

  const int pf = ChoosePixelFormat(hDc, &pfd);
  if (pf == 0)
    return false;
  if (SetPixelFormat(hDc, pf, &pfd) == FALSE)
    return false;
  ReleaseDC(window_handler, hDc);

  data->hdc = GetDC(window_handler);
  if (!g_hrc)
    g_hrc = wglCreateContext(data->hdc);
  return true;
}

void cleanup_device_wgl(HWND window_handler, const wgl_window_data_t *data) {
  wglMakeCurrent(NULL, NULL);
  ReleaseDC(window_handler, data->hdc);
}

// Forward declare message handler from wrapper
extern CIMGUI_IMPL_API LRESULT ImGui_C_ImplWin32_WndProcHandler(HWND hWnd,
                                                                UINT msg,
                                                                WPARAM wParam,
                                                                LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if
// dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your
// main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to
// your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from
// your application based on those two flags.
LRESULT WINAPI wnd_proc(HWND window_handler, UINT msg, WPARAM w_param,
                        LPARAM l_param) {
  if (ImGui_C_ImplWin32_WndProcHandler(window_handler, msg, w_param, l_param))
    return true;

  switch (msg) {
  case WM_SIZE:
    if (w_param != SIZE_MINIMIZED) {
      g_width = LOWORD(l_param);
      g_height = HIWORD(l_param);
    }
    return 0;
  case WM_SYSCOMMAND:
    if ((w_param & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
      return 0;
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  default:
    break;
  }
  return DefWindowProcW(window_handler, msg, w_param, l_param);
}
