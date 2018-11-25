#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

unsigned int loadTexture(std::string filename, bool rgbaFlag = false) {
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
  if (data == 0) {
    std::cout << "cannot load " << filename << '\n';
  }

  unsigned int texture = 0;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (rgbaFlag == false)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
  if (rgbaFlag == true)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return texture;
}

unsigned int *loadTextures(unsigned int size, std::string filenames[],
                           bool rgbaFlag = false) {
  unsigned int *textures = new unsigned int[size];
  glGenTextures(size, textures);

  for (unsigned int i = 0; i < size; ++i) {
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(filenames[i].c_str(), &width, &height, &nrChannels, 0);
    if (data == 0) {
      std::cout << "cannot load " << filenames[i] << '\n';
    }

    glBindTexture(GL_TEXTURE_2D, textures[i]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (rgbaFlag == false)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
    if (rgbaFlag == true)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
  }
  return textures;
}

#endif
