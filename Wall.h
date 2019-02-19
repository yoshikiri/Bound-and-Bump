#ifndef WALL_H
#define WALL_H

#include <glm/glm.hpp>
// #include <glm/fwd.hpp>

#include "DrawFigure.h"

class Wall {
public:
  Wall(glm::vec3 center, float width, float height)
      : center(center), WIDTH(width), HEIGHT(height) {}

  void draw() { DrawFigure::drawRectangle(center, WIDTH, HEIGHT); }

  glm::vec3 getCenter() const { return center; }
  float getWidth() const { return WIDTH; }
  float getHeight() const { return HEIGHT; }

private:
  const glm::vec3 center;
  const float WIDTH;
  const float HEIGHT;
};

#endif
