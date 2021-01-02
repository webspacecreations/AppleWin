#pragma once

#include <SDL.h>
#include <memory>
#include <iosfwd>
#include <string>

void printRendererInfo(std::ostream & os,
		       const std::shared_ptr<SDL_Renderer> & ren,
		       const Uint32 pixelFormat,
		       const int selectedDriver);

bool show_yes_no_dialog(const std::shared_ptr<SDL_Window> & win,
			const std::string & title,
			const std::string & text);

int getRefreshRate();
void updateWindowTitle(const std::shared_ptr<SDL_Window> & win);
void cycleVideoType(const std::shared_ptr<SDL_Window> & win);
void cycle50ScanLines(const std::shared_ptr<SDL_Window> & win);
void setApplicationIcon(const std::shared_ptr<SDL_Window> & win);
