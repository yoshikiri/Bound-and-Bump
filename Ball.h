#ifndef BALL_H
#define BALL_H

#include <glm/glm.hpp>

#include "DrawFigure.h"
#include "Wall.h"

class Ball {
public:
  Ball(glm::vec3 location, glm::vec3 velocity, float size, glm::vec3 color)
      : location(location), velocity(velocity), size(size), color(color),
        currentTime(glfwGetTime()), previousTime(0), isAlive(true) {}

  void move() { location += velocity; }

  void collisionWall(Wall wall) {
    float left = wall.getCenter().x - wall.getWidth() / 2;
    float right = wall.getCenter().x + wall.getWidth() / 2;
    float top = wall.getCenter().y + wall.getHeight() / 2;
    float bottom = wall.getCenter().y - wall.getHeight() / 2;

    if (location.x - size / 2 <= left || location.x + size / 2 >= right)
      velocity.x *= -1;
    if (location.y - size / 2 <= bottom || location.y + size / 2 >= top)
      velocity.y *= -1;
  }

  bool collisionBall(Ball other) {
    if (glm::length(location - other.location) < (size + other.size)) {
      return true;
    }
    return false;
  }

  void setShouldDie() { isAlive = false; }

  bool shouldDie() const { return !isAlive; }

  bool canGetPoint() {
    currentTime = glfwGetTime();
    if (currentTime < previousTime + 1)
      return false;
    else {
      previousTime = glfwGetTime();
      return true;
    }
  }

  glm::vec3 getLocation() const { return location; }

  const float getSize() const { return size; }

  void draw() { DrawFigure::drawCircle(location, size, size * 200, color); }

private:
  glm::vec3 location;
  glm::vec3 velocity;
  float size;
  glm::vec3 color;
  float currentTime;
  float previousTime;
  bool isAlive;
};

#endif