#include "texture/image.hpp"

#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

void BuildGlTexture(Texture* texture) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  texture->id = textureID;
  GLenum format;
  switch (texture->num_components) {
    case 1:
      format = GL_RED;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      std::cerr << "Unexpected number of components " << texture->num_components
                << " in texture from " << texture->path << std::endl;
      exit(-1);
      break;
  }

  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height, 0,
               format, GL_UNSIGNED_BYTE, texture->data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture TextureFromFile(const std::string& filename,
                        const std::string& typeName, bool gamma) {
  std::string clean_filename = filename;
#ifdef _WIN32
  std::replace(clean_filename.begin(), clean_filename.end(), '/', '\\');
#else
  std::replace(clean_filename.begin(), clean_filename.end(), '\\', '/');
#endif

  Texture texture;
  texture.type = typeName;
  texture.path = clean_filename;

  unsigned char* data = stbi_load(clean_filename.c_str(), &texture.width,
                                  &texture.height, &texture.num_components, 0);
  texture.row_alignment = texture.width * texture.num_components;
  if (data) {
    texture.data = data;
    BuildGlTexture(&texture);
  } else {
    std::cerr << "Texture failed to load at path: " << clean_filename
              << std::endl;
    stbi_image_free(data);
    exit(-1);
  }

  return texture;
}

Texture TextureFromFile(const std::string& path, const std::string& directory,
                        const std::string& typeName, bool gamma) {
  const char kPathSeparator =
#ifdef _WIN32
      '\\';
#else
      '/';
#endif
  std::string filename = std::string(path);
  filename = directory + kPathSeparator + filename;
  return TextureFromFile(filename, typeName, gamma);
}

void TextureToFile(const std::string& filename, const Texture& tex) {
  std::string clean_filename = filename;
#ifdef _WIN32
  std::replace(clean_filename.begin(), clean_filename.end(), '/', '\\');
#else
  std::replace(clean_filename.begin(), clean_filename.end(), '\\', '/');
#endif
  if (!stbi_write_png(clean_filename.c_str(), tex.width, tex.height,
                      tex.num_components, tex.data, tex.row_alignment)) {
    std::cerr << "Texture failed to write to path: " << clean_filename
              << std::endl;
    exit(-1);
  }
}
