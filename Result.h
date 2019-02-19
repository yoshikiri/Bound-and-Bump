#ifndef RESULT_H
#define RESULT_H

#include <memory>

#include "State.h"
#include "Figure.h"

class Result : public State{
public:
  Result(GLFWwindow *window, int score);

  ~Result();

  State *update() override;

  void drawScore();

private:
  const std::unique_ptr<Figure> background;
  const std::unique_ptr<Figure> number;

  const int score;
  unsigned int texture;
  std::unique_ptr<unsigned int[]> texture_numbers;
};

#endif