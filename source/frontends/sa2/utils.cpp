#include "frontends/sa2/utils.h"
#include <ostream>
#include <iostream>
#include <gtk/gtk.h>

void printRendererInfo(std::ostream & os, std::shared_ptr<SDL_Renderer> & ren, const Uint32 pixelFormat, const int selectedDriver)
{
  SDL_RendererInfo info;
  SDL_GetRendererInfo(ren.get(), &info);

  const size_t n = SDL_GetNumRenderDrivers();
  os << "SDL: " << n << " drivers" << std::endl;
  for(size_t i = 0; i < n; ++i)
  {
      SDL_RendererInfo info;
      SDL_GetRenderDriverInfo(i, &info);
      os << " " << i << ": " << info.name << std::endl;
  }

  if (SDL_GetRendererInfo(ren.get(), &info) == 0)
  {
    os << "Active driver (" << selectedDriver << "): " << info.name << std::endl;
    os << " SDL_RENDERER_SOFTWARE: " << ((info.flags & SDL_RENDERER_SOFTWARE) > 0) << std::endl;
    os << " SDL_RENDERER_ACCELERATED: " << ((info.flags & SDL_RENDERER_ACCELERATED) > 0) << std::endl;
    os << " SDL_RENDERER_PRESENTVSYNC: " << ((info.flags & SDL_RENDERER_PRESENTVSYNC) > 0) << std::endl;
    os << " SDL_RENDERER_TARGETTEXTURE: " << ((info.flags & SDL_RENDERER_TARGETTEXTURE) > 0) << std::endl;

    os << "Supported pixel formats:" << std::endl;
    for (size_t i = 0; i < info.num_texture_formats; ++i)
    {
      os << " " << SDL_GetPixelFormatName(info.texture_formats[i]) << std::endl;
    }
    os << "Selected format: " << SDL_GetPixelFormatName(pixelFormat) << std::endl;
  }
  else
  {
    os << "No Renderinfo" << std::endl;
  }
}

void doit()
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Open File",
					NULL,
					action,
					"_Cancel",
					GTK_RESPONSE_CANCEL,
				        "_Open",
					GTK_RESPONSE_ACCEPT,
					NULL);
  std::cout << "x" << std::endl;
  res = gtk_dialog_run (GTK_DIALOG (dialog));
  std::cout << "x" << std::endl;
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    std::cout << "x" << std::endl;
    filename = gtk_file_chooser_get_filename (chooser);
    std::cout << filename << std::endl;
    std::cout << "x" << std::endl;
    g_free (filename);
    std::cout << "x" << std::endl;
  }

  gtk_widget_destroy (dialog);
}
