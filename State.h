#ifndef STATE_H
#define STATE_H

// #include "TextureManager.h"

struct GLFWwindow;

class State{
public:
  explicit State(GLFWwindow *window) : window(window){}

  virtual State *update() = 0;

  GLFWwindow *window;
};

#endif
