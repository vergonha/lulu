#include "assets.hpp"
#include "input.hpp"
#include <math.h>
#include "platform_specific/linux.cpp"
#include "rendering.hpp"
#include "scene.hpp"
#include "logger.hpp"
#include "platform.hpp"
#include "scripts.hpp"
#include "system.hpp"
#include <GLFW/glfw3.h>
#include <X11/Xlib.h>
#include <cstdlib>
#include <iostream>

using namespace Lulu;
using namespace Lulu::System;
using namespace Lulu::Assets;

class WarriorLogic : public UserScripts::CustomLogic
{
public:
  Vec2 warriorPos = {0, 0};

  WarriorLogic()
  {
    setTexturePath("../assets/textures/TEXTURE_ATLAS.png");
    registerDrawable({{0, 0}, {16, 16}}, {16, 16}, "warrior");
    createCamera({1280, 720}, {0, 0});
  }

  void start() override
  {
    // Initialization code here
  }

  void update(Input *input) override
  {
    draw("warrior", warriorPos, {32, 32});
    customHandler(input);
  }

private:
  void customHandler(Input *input)
  {
    if (Keyboard::keyIsDown(input, KEY_W))
      warriorPos.y -= 10;
    if (Keyboard::keyIsDown(input, KEY_S))
      warriorPos.y += 10;
    if (Keyboard::keyIsDown(input, KEY_A))
      warriorPos.x -= 10;
    if (Keyboard::keyIsDown(input, KEY_D))
      warriorPos.x += 10;
  }
};

std::unique_ptr<Program::Platform> initPlatform()
{
  auto platform = Program::Platform::GetPlatform();
  platform->createWindow(1280, 720, "Warrior Example");
  return platform;
}

Lulu::Screen::Scene *initScene()
{
  auto *scene = new Lulu::Screen::Scene();
  scene->registerScript(new WarriorLogic());
  scene->start();
  return scene;
}

void gameLoop(Program::Platform *platform, Lulu::Screen::Scene *scene)
{
  while (true)
  {
    platform->updateWindow(scene);
    scene->render(platform->input);
    platform->swapBuffers();
    platform->resetInputStates();

    if (Keyboard::keyIsDown(platform->input, KEY_ESCAPE))
      break;
  }
}

int main()
{
  auto platform = initPlatform();
  auto *scene = initScene();

  gameLoop(platform.get(), scene);

  platform->closeWindow();
  return 0;
}
