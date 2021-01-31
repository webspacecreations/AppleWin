#pragma once

#include <memory>
#include "frontends/imgui/gles.h"

#include "frontends/common2/speed.h"
#include "imgui.h"

class Emulator
{
public:
  Emulator(
    const std::shared_ptr<SDL_Window> & window,
    const GLuint texture,
    const bool fixedSpeed
    );

  void execute(const size_t milliseconds);

  void updateTexture();
  void drawImage();

  void processEvents(bool & quit);

private:
  void processKeyDown(const SDL_KeyboardEvent & key, bool & quit);
  void processKeyUp(const SDL_KeyboardEvent & key);
  void processText(const SDL_TextInputEvent & text);

  const std::shared_ptr<SDL_Window> myWindow;
  const GLuint myTexture;

  int myMultiplier;
  bool myFullscreen;

  Speed mySpeed;

  size_t myWidth;
  size_t myHeight;
  uint8_t* myFrameBuffer;
};
