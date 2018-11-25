#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <irrKlang/irrKlang.h>

#include "Play.h"
#include "State.h"
#include "Title.h"
#include "Window.h"
#include "shaderManager.h"

//----------------------------------------------------------------------------//

int main() {
  if (glfwInit() == GL_FALSE) {
    std::cerr << "Cannot init GLFW" << std::endl;
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  Window window;
  const float WIDTH = 8.0f;
  const float HEIGHT = 6.0f;

  atexit(glfwTerminate);

  // 背景色設定
  glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

  // デプスバッファを有効にする
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  // textureを上下逆に読み込む
  stbi_set_flip_vertically_on_load(true);

  // shader proramを作成する
  const GLuint program(loadProgram("vertexshader.vert", "fragmentshader.frag"));

  //--------------------------------------------------------------------------//

  glUseProgram(program);
  glUniform1i(glGetUniformLocation(program, "myTexture1"), 0);

  // uniform変数の場所を得る
  const unsigned int modelLoc = glGetUniformLocation(program, "model");
  const unsigned int viewLoc = glGetUniformLocation(program, "view");
  const unsigned int projectionLoc =
      glGetUniformLocation(program, "projection");

  glm::mat4 model(1.0);
  glm::mat4 view(1.0);
  view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 projection(1.0);
  projection = glm::ortho(-WIDTH, WIDTH, -HEIGHT, HEIGHT, 0.0f, 10.0f);

  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

  // BGMの再生
  // std::unique_ptr<irrklang::ISoundEngine *> soundEngine =
  //     std::make_unique<irrklang::ISoundEngine *>(
  //         irrklang::createIrrKlangDevice());
  // (*soundEngine)->play2D("Music/PerituneMaterial_Machinery_loop.ogg", GL_TRUE);
  // (*soundEngine)->play2D("Music/Galaxy.ogg", GL_TRUE);

  // ゲームのシーン状態
  State *state = new Title(window.getWindow());
  State *next = 0;

  //--------------------------------------------------------------------------//
  // メインループ
  while (window.shouldClose() == GL_FALSE) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    next = state->update();
    if (next != state)
      state = next;

    window.swapBuffers();
    glfwPollEvents();
  }

  return 0;
}
