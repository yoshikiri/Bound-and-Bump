#include "Result.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include "Play.h"
#include "Title.h"

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

const float NUMBER_VERTICES[] = {
    // positions     // colors    // texture coords
    1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    1.0f,  -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
    -1.0f, 1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // top left
};
const unsigned int NUMBER_INDICES[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

const std::string NUMBER_FILENAMES[] = {
    "Image/Numbers_Result/zero.png",  "Image/Numbers_Result/one.png",
    "Image/Numbers_Result/two.png",   "Image/Numbers_Result/three.png",
    "Image/Numbers_Result/four.png",  "Image/Numbers_Result/five.png",
    "Image/Numbers_Result/six.png",   "Image/Numbers_Result/seven.png",
    "Image/Numbers_Result/eight.png", "Image/Numbers_Result/nine.png"};

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

Result::Result(GLFWwindow *window, int score)
    : State(window),
      background(std::make_unique<Figure>(GL_TRIANGLES, 4, BACKGROUND_VERTICIES,
                                          6, BACKGROUND_INDICES)),
      number(std::make_unique<Figure>(GL_TRIANGLES, 4, NUMBER_VERTICES, 6,
                                      NUMBER_INDICES)),
      score(score), texture_numbers(std::make_unique<unsigned int[]>(10)) {

  // load textures
  texture = loadTexture("Image/result.png", true);
  for (int i = 0; i < 10; i++) {
    texture_numbers[i] = loadTexture(NUMBER_FILENAMES[i], true);
  }
}

Result::~Result() {}

State *Result::update() {
  State *next = this;

  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
    next = new Play(window);
  }
  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
    next = new Title(window);
  }

  drawScore();

  return next;
}

// scoreとbackgroundの描画
void Result::drawScore() {
  std::string sscore = std::to_string(score);
  int len = sscore.length();
  glm::mat4 model(1.0);

  model = glm::translate(model, glm::vec3(-(len + 1.0f), 1.0f, 0));
  for (int i = 0; i < len; ++i) {
    model = glm::translate(model, glm::vec3(2.0, 0, 0));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glBindTexture(GL_TEXTURE_2D, texture_numbers[sscore[i] - '0']);
    number->draw();
  }

  model = glm::mat4(1.0);
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glBindTexture(GL_TEXTURE_2D, texture);
  background->draw();
}
