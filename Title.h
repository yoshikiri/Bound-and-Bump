#ifndef TITLE_H
#define TITLE_H

#include <memory>

#include "State.h"

class Figure;
struct GLFWwindow;

class Title : public State{
public:
  Title(GLFWwindow *window);

  ~Title();

  State *update() override;

private:
  const std::unique_ptr<Figure> background;
  unsigned int texture;
};

#endif