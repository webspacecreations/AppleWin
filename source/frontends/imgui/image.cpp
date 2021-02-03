#include "frontends/imgui/image.h"

#ifdef GL_UNPACK_ROW_LENGTH
#define UGL_UNPACK_LENGTH GL_UNPACK_ROW_LENGTH
#else
#define UGL_UNPACK_LENGTH GL_UNPACK_ROW_LENGTH_EXT
#endif

void LoadTextureFromData(GLuint texture, const uint8_t * data, size_t width, size_t height, size_t pitch)
{
  glBindTexture(GL_TEXTURE_2D, texture);
  glPixelStorei(UGL_UNPACK_LENGTH, pitch); // in pixels
  GLint data2;
  glGetIntegerv(UGL_UNPACK_LENGTH, &data2);
  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

  // I have no idea how this works, but it works
  const GLenum format = GL_RGBA; // without _EXT colors are all over the place
  const GLenum type = GL_UNSIGNED_BYTE;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, type, data);
  glPixelStorei(UGL_UNPACK_LENGTH, 0);
}
