#include <memory>
#include <iomanip>
#include <algorithm>
#include <iostream>

#include "linux/interface.h"
#include "linux/benchmark.h"

#include "frontends/common2/fileregistry.h"
#include "frontends/common2/utils.h"
#include "frontends/common2/programoptions.h"
#include "frontends/common2/timer.h"
#include "frontends/common2/resources.h"
#include "frontends/imgui/emulator.h"
#include "frontends/sdl/sdirectsound.h"
#include "frontends/sdl/gamepad.h"
#include "frontends/sdl/utils.h"

#include "StdAfx.h"
#include "Core.h"
#include "Log.h"
#include "CPU.h"
#include "NTSC.h"
#include "SaveState.h"
#include "Interface.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "frontends/imgui/gles.h"

// comment out to test / debug init / shutdown only
#define EMULATOR_RUN

int MessageBox(HWND, const char * text, const char * caption, UINT type)
{
  // tabs do not render properly
  std::string s(text);
  std::replace(s.begin(), s.end(), '\t', ' ');
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, caption, s.c_str(), nullptr);
  return IDOK;
}

void run_sdl(int argc, const char * argv [], std::shared_ptr<SDL_Window> & win, ImGuiIO& io)
{
  EmulatorOptions options;
  options.memclear = g_nMemoryClearType;
  const bool run = getEmulatorOptions(argc, argv, "SDL2", options);

  if (!run)
    return;

  if (options.log)
  {
    LogInit();
  }

  InitializeFileRegistry(options);

  Paddle::instance.reset(new Gamepad(0));

  g_nMemoryClearType = options.memclear;

  initialiseEmulator();
  applyOptions(options);

  setApplicationIcon(win);

  Video & video = GetVideo();

  const int width = video.GetFrameBufferWidth();
  const int height = video.GetFrameBufferHeight();
  const int sw = video.GetFrameBufferBorderlessWidth();
  const int sh = video.GetFrameBufferBorderlessHeight();

  std::cerr << std::fixed << std::setprecision(2);

  updateWindowTitle(win);

  const int fps = getRefreshRate();
  std::cerr << "Video refresh rate: " << fps << " Hz, " << 1000.0 / fps << " ms" << std::endl;

  const ImVec4 background(0.45f, 0.55f, 0.60f, 1.00f);

  GLuint texture;
  glGenTextures(1, &texture);

  Emulator emulator(win, texture, options.fixedSpeed);

  Timer global;
  Timer updateTextureTimer;
  Timer refreshScreenTimer;
  Timer cpuTimer;
  Timer eventTimer;

  const std::string globalTag = ". .";
  std::string updateTextureTimerTag, refreshScreenTimerTag, cpuTimerTag, eventTimerTag;

  refreshScreenTimerTag = "0 .";
  cpuTimerTag           = "0 .";
  eventTimerTag         = "0 .";
  updateTextureTimerTag = "0 .";

  bool quit = false;

  // it does not need to be exact
  const size_t oneFrame = 1000 / fps;

  do
  {
    eventTimer.tic();
    SDirectSound::writeAudio();
    emulator.processEvents(quit);
    eventTimer.toc();

    cpuTimer.tic();
    emulator.execute(oneFrame);
    cpuTimer.toc();

    updateTextureTimer.tic();
    emulator.updateTexture();
    updateTextureTimer.toc();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(win.get());
    ImGui::NewFrame();

    ImGui::Begin("Apple ][");
    emulator.drawImage();
    ImGui::End();

    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(background.x, background.y, background.z, background.w);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(win.get());
  } while (!quit);

  global.toc();

  glDeleteTextures(1, &texture);

  const char sep[] = "], ";
  std::cerr << "Global:  [" << globalTag << sep << global << std::endl;
  std::cerr << "Events:  [" << eventTimerTag << sep << eventTimer << std::endl;
  std::cerr << "Texture: [" << updateTextureTimerTag << sep << updateTextureTimer << std::endl;
  std::cerr << "Screen:  [" << refreshScreenTimerTag << sep << refreshScreenTimer << std::endl;
  std::cerr << "CPU:     [" << cpuTimerTag << sep << cpuTimer << std::endl;

  const double timeInSeconds = global.getTimeInSeconds();
  const double actualClock = g_nCumulativeCycles / timeInSeconds;
  std::cerr << "Expected clock: " << g_fCurrentCLK6502 << " Hz, " << g_nCumulativeCycles / g_fCurrentCLK6502 << " s" << std::endl;
  std::cerr << "Actual clock:   " << actualClock << " Hz, " << timeInSeconds << " s" << std::endl;
  SDirectSound::stop();

  uninitialiseEmulator();
}

int main(int argc, const char * argv [])
{
  //First we need to start up SDL, and make sure it went ok
  const Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO | SDL_INIT_TIMER;
  if (SDL_Init(flags) != 0)
  {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  int exit = 0;

  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 100";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  std::shared_ptr<SDL_Window> window(SDL_CreateWindow("Dear ImGui SDL2+OpenGL example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags), SDL_DestroyWindow);
  SDL_GLContext gl_context = SDL_GL_CreateContext(window.get());
  SDL_GL_MakeCurrent(window.get(), gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(window.get(), gl_context);

  // Setup Platform/Renderer backends
  ImGui_ImplOpenGL3_Init(glsl_version);

  try
  {
    run_sdl(argc, argv, window, io);
  }
  catch (const std::exception & e)
  {
    exit = 2;
    std::cerr << e.what() << std::endl;
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context);

  // this must happen BEFORE the SDL_Quit() as otherwise we have a double free (of the game controller).
  Paddle::instance.reset();
  window.reset();
  SDL_Quit();

  return exit;
}
