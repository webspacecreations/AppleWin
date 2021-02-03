#include "frontends/imgui/image.h"
#include <GLES2/gl2ext.h>

#ifdef GL_UNPACK_ROW_LENGTH
// GLES3: defined in gl3.h
#define UGL_UNPACK_LENGTH GL_UNPACK_ROW_LENGTH
#else
// GLES2: defined in gl2ext.h as an extension GL_EXT_unpack_subimage
#define UGL_UNPACK_LENGTH GL_UNPACK_ROW_LENGTH_EXT
#endif

void LoadTextureFromData(GLuint texture, const uint8_t * data, size_t width, size_t height, size_t pitch)
{
  glBindTexture(GL_TEXTURE_2D, texture);
  glPixelStorei(UGL_UNPACK_LENGTH, pitch); // in pixels

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  const GLenum format = GL_BGRA_EXT; // this is defined in gl2ext.h and nowhere in gl3.h
  const GLenum type = GL_UNSIGNED_BYTE;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
  // reset to default state
  glPixelStorei(UGL_UNPACK_LENGTH, 0);
}
