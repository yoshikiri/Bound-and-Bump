#include "Title.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <irrKlang/irrKlang.h>

#include "Play.h"
#include "Figure.h"

namespace {

const float WIDTH = 8.0f;
const float HEIGHT = 6.0f;
const unsigned int modelLoc = 0;

const float BACKGROUND_VERTICIES[] = {
    // positions     // colors       // texture coords
    WIDTH,  HEIGHT,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    WIDTH,  -HEIGHT, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -WIDTH, -HEIGHT, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
    -WIDTH, HEIGHT,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // top left
};
const unsigned int BACKGROUND_INDICES[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

// textureを読み込んでtextureIDを返す
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

} // namespace

Title::Title(GLFWwindow *window)
    : State(window), background(std::make_unique<Figure>(
                         GL_TRIANGLES, 4, BACKGROUND_VERTICIES, 6, BACKGROUND_INDICES)) {
  texture = loadTexture("Image/title.png", true);
}

Title::~Title() {}

State *Title::update() {
  State *next = this;

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    next = new Play(window);
  }

  glm::mat4 model(1.0);
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glBindTexture(GL_TEXTURE_2D, texture);
  background->draw();

  return next;
}
