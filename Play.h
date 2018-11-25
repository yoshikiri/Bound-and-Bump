#ifndef PLAY_H
#define PLAY_H

#include <memory>
// #include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include "State.h"

// 前方宣言
class Figure;
struct GLFWwindow;
class Ball;
class Wall;

namespace std {
template <class T> struct less;
template <class T1, class T2> struct pair;
template <class T> class allocator;
template <class T, class Allocator> class vector;
}

namespace irrklang{
  class ISoundEngine;
}


class Play : public State {
public:
  explicit Play(GLFWwindow *window);

  ~Play();

  State *update() override;

  void drawScore();

  void makeRandomVelocity();

  void makeRandomeSize();

  void makeRandomColor();

private:
  Play(const Play &p);
  Play& operator=(const Play &);

  const std::unique_ptr<Figure> background;
  const std::unique_ptr<Figure> number;
  const std::unique_ptr<Wall> wall;
  std::unique_ptr<glm::vec3> velocity;
  std::unique_ptr<glm::vec3> color;
  std::unique_ptr<std::vector<Ball, std::allocator<Ball>>> balls;
  std::unique_ptr<irrklang::ISoundEngine*> soundEngine;

  unsigned int texture;
  std::unique_ptr<unsigned int[]> texture_numbers;
  const int INTERVAL;
  const int PLAYTIME;
  float leftTime;
  float startTime;
  float preTime;
  float curTime;
  int score;
  float size;
};

#endif
