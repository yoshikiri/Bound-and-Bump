#include "Play.h"

#include <iostream>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <irrKlang/irrKlang.h>
#include <stb_image.h>

#include "Ball.h"
#include "DrawFigure.h"
#include "Figure.h"
#include "Result.h"
#include "Wall.h"

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
    "Image/Numbers/zero.png",  "Image/Numbers/one.png",
    "Image/Numbers/two.png",   "Image/Numbers/three.png",
    "Image/Numbers/four.png",  "Image/Numbers/five.png",
    "Image/Numbers/six.png",   "Image/Numbers/seven.png",
    "Image/Numbers/eight.png", "Image/Numbers/nine.png"};

// textureを読み込んでtextureIDを返す
unsigned int loadTexture(std::string filename, bool rgbaFlag = false) {
  int WIDTH, HEIGHT, nrChannels;
  unsigned char *data =
      stbi_load(filename.c_str(), &WIDTH, &HEIGHT, &nrChannels, 0);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
  if (rgbaFlag == true)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  return texture;
}

} // namespace

//----------------------------------------------------------------------------//

Play::Play(GLFWwindow *window)
    : State(window), background(std::make_unique<Figure>(
                         GL_TRIANGLES, 4, BACKGROUND_VERTICIES, 6, BACKGROUND_INDICES)),
      number(std::make_unique<Figure>(GL_TRIANGLES, 4, NUMBER_VERTICES, 6,
                                      NUMBER_INDICES)),
      wall(std::make_unique<Wall>(glm::vec3(0, 1.0f, 0), 2 * WIDTH,
                                  HEIGHT * 2 - 2.0f)),
      velocity(std::make_unique<glm::vec3>(glm::vec3(0.0f, 0.0f, 0.0f))),
      color(std::make_unique<glm::vec3>(glm::vec3(0.0f, 0.0f, 0.0f))),
      balls(std::make_unique<std::vector<Ball, std::allocator<Ball>>>()),
      soundEngine(std::make_unique<irrklang::ISoundEngine *>(
          irrklang::createIrrKlangDevice())),
      texture_numbers(std::make_unique<unsigned int[]>(10)), INTERVAL(1),
      PLAYTIME(20), leftTime(PLAYTIME), startTime(glfwGetTime()),
      preTime(-INTERVAL), curTime(glfwGetTime()), score(0) {

  // velocity, size, colorに値を代入
  makeRandomVelocity();
  makeRandomeSize();
  makeRandomColor();

  // texturesのロード
  texture = loadTexture("Image/background.png", true);
  for (int i = 0; i < 10; i++) {
    texture_numbers[i] = loadTexture(NUMBER_FILENAMES[i], true);
  }
}

Play::~Play() { (*soundEngine)->drop(); }

State *Play::update() {
  State *next = this;

  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
    next = new Result(window, score);
  }
  if (leftTime <= 0) {
    next = new Result(window, score);
  }

  // マウスカーソルの位置に次に追加するボールを描画
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  x = (x / (WIDTH * 50) - 1) * WIDTH;
  y = (y / (HEIGHT * 50) - 1) * -HEIGHT;
  DrawFigure::drawCircle(glm::vec3(x, y, 0.0f), size, 20, *color);

  // ボールをマウスカーソルの位置に追加する
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
    // マウスカーソルがwallの内側にあるときのみ追加可能
    if (x < wall->getCenter().x - wall->getWidth() / 2 ||
        x > wall->getCenter().x + wall->getWidth() / 2 ||
        y < wall->getCenter().y - wall->getHeight() / 2 ||
        y > wall->getCenter().y + wall->getHeight() / 2) {
    } else {
      curTime = glfwGetTime();

      // INTERVAL秒経過後追加可能
      if (curTime > preTime + INTERVAL) {
        preTime = glfwGetTime();
        balls->emplace_back(
            Ball(glm::vec3(x, y, 0.0f), glm::vec3(*velocity), size, *color));

        makeRandomVelocity();
        makeRandomeSize();
        makeRandomColor();
        (*soundEngine)->play2D("Music/se_maoudamashii_system40.ogg", GL_FALSE);
      }
    }
  }

  DrawFigure::drawArrow(glm::vec3(x, y, 0.0f), glm::vec3(*velocity) * 10.0f);

  // 衝突判定
  for (auto ball = balls->end() - 1; ball != balls->begin() - 1; --ball) {
    for (auto other = balls->end() - 1; other != balls->begin() - 1; --other) {
      if (ball == other)
        continue;

      if (ball->collisionBall(*other)) {
        ball->setShouldDie();
        other->setShouldDie();
        (*soundEngine)->play2D("Music/se_maoudamashii_system42.ogg", GL_FALSE);
        break;
      }
    }

    // shouldDieがtrueになっているものを消す
    if (ball->shouldDie())
      balls->erase(ball);
    ball->collisionWall(*wall);
    ball->move();
    ball->draw();

    // score加算
    if (ball->canGetPoint()) {
      score += ball->getSize() * 20;
    }
  }

  wall->draw();

  drawScore();

  return next;
}

// scoreとbackgroundの描画
void Play::drawScore() {
  std::string sscore = std::to_string(score);
  int len = sscore.length();
  glm::mat4 model(1.0);

  model = glm::translate(model, glm::vec3(-WIDTH, -HEIGHT + 1.2f, 0));
  model = glm::scale(model, glm::vec3(0.5, 0.5, 1));
  for (int i = 0; i < len; ++i) {
    model = glm::translate(model, glm::vec3(1.8, 0, 0));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glBindTexture(GL_TEXTURE_2D, texture_numbers[sscore[i] - '0']);
    number->draw();
  }

  leftTime = 20 + 1 - (glfwGetTime() - startTime);
  if (leftTime <= 0) {
    leftTime = 0;
  }
  std::string sCurrentTime = std::to_string((int)leftTime);
  int timeLen = sCurrentTime.length();
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(WIDTH - 4.0f, -HEIGHT + 1.2f, 0));
  model = glm::scale(model, glm::vec3(0.5, 0.5, 1));
  for (int i = 0; i < timeLen; ++i) {
    model = glm::translate(model, glm::vec3(1.8, 0, 0));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glBindTexture(GL_TEXTURE_2D, texture_numbers[sCurrentTime[i] - '0']);
    number->draw();
  }

  model = glm::mat4(1.0);
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glBindTexture(GL_TEXTURE_2D, texture);
  background->draw();
}

void Play::makeRandomVelocity() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> rand_dist(-10, 10);

  velocity->x = rand_dist(mt);
  velocity->y = rand_dist(mt);
  velocity->z = 0;
  *velocity = glm::normalize(*velocity);
  *velocity /= 10.0f;
}

void Play::makeRandomeSize() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> rand_dist(1, 10);

  size = rand_dist(mt) / 20.0;
}

void Play::makeRandomColor() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> rand_dist(0, 10);

  color->x = rand_dist(mt) / 10.0;
  color->y = rand_dist(mt) / 10.0;
  color->z = rand_dist(mt) / 10.0;
}
