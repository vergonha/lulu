<h1 align="center">
  <img src="https://cdn-icons-png.flaticon.com/512/6132/6132222.png" width="30px">
  Lulu - Game Engine 
  <img src="https://i.pinimg.com/736x/d0/ca/e3/d0cae31793946506809acee48eeceab5.jpg" width="30px">
</h1>
<p align="center">
A lightweight cross-platform C++ game engine from scratch with a memory management system and OpenGL rendering</p>
</div>

**LuluProject** is my personal exploration into the world of low-level graphics programming, memory management, and game engine architecture. This is **not** intended to be a production-ready engine, but rather a hands-on educational project where I'm learning:

- Low-level graphics programming with OpenGL
- Memory management system design and tracking logic
- Game engine architecture design
- Cross-platform development concepts
- Performance optimization techniques

> **Note**: This project is in early experimental stages and is constantly evolving as I learn new concepts.

---

## 🧩 Features

| Feature Description 🚀 | Details                                                    |
| ---------------------- | ---------------------------------------------------------- |
| **C++ Core**           | Built entirely in C++ 🔺                                   |
| **OpenGL Rendering**   | Fundamental rendering pipeline using OpenGL 🖼️             |
| **Image Loading**      | Texture loading via `stb_image` library                    |
| **Input System**       | Basic keyboard input handling 🕹️                           |
| **Scene System**       | Simple scene management with entity logic scripting        |
| **Memory Management**  | Optional custom memory management system implementation 🧠 |
| **Platform Layer**     | Foundation for cross-platform support (Linux-first) 🧰     |

---

## 🧪 Example

Your custom logic. You can create as many as you want and assign them different rendering priorities.

```cpp
class WarriorLogic : public UserScripts::CustomLogic {
public:
    Vec2 warriorPos = {0, 0};

    WarriorLogic() {
        /*
        * Register a drawable sprite:
        * - Sprite rectangle within the atlas: position {0,0}, size {16,16}
        * - On-screen size for rendering: {16,16}
        * - Identifier name: "warrior"
        */
        setTexturePath("../assets/textures/TEXTURE_ATLAS.png");
        registerDrawable({{0, 0}, {16, 16}}, {16, 16}, "warrior");

        // Create a camera with viewport size 1280x720 and initial position {0,0}
        createCamera({1280, 720}, {0, 0});
    }

    /*
    * - The `start()` method runs only once at the beginning, ideal for initialization logic.
    * - The `update()` method runs every frame and contains the custom logic that updates the game state.
    */
    void start() override {}

    void update(Input* input) override {
        /*
        * - The `draw()` function draws the sprite by its registered name at a given position and size.
        * - You can call `draw()` multiple times per frame, and you can override the position and size each time,
        *   allowing dynamic resizing or repositioning of the sprite during rendering.
        */
        draw("warrior", warriorPos, {32, 32});
        handleInput(input);
    }



private:
    void handleInput(Input* input) {
        if (Keyboard::keyIsDown(input, KEY_W)) warriorPos.y -= 10;
        if (Keyboard::keyIsDown(input, KEY_S)) warriorPos.y += 10;
        if (Keyboard::keyIsDown(input, KEY_A)) warriorPos.x -= 10;
        if (Keyboard::keyIsDown(input, KEY_D)) warriorPos.x += 10;
    }

    /*
    -   The `handleCamera` function updates the camera position independently from individual sprite positions.
    -   Moving the camera dynamically shifts the view of all elements rendered within this scene or logic,
    -   effectively changing how everything inside this logic is displayed on screen without altering their actual positions.
    */

    void  handleCamera(Input  *input)
    {
        if  (Keyboard::keyIsDown(input, KEY_UP))  this->state->camera.position.y  -=  1;
        if  (Keyboard::keyIsDown(input, KEY_DOWN))  this->state->camera.position.y  +=  1;
        if  (Keyboard::keyIsDown(input, KEY_LEFT))  this->state->camera.position.x  -=  1;
        if  (Keyboard::keyIsDown(input, KEY_RIGHT))  this->state->camera.position.x  +=  1;
    }
};
```

The minimal boilerplate to initialize the platform, create the scene, and start the main loop:

```cpp
std::unique_ptr<Program::Platform> initPlatform() {
    auto platform = Program::Platform::GetPlatform();
    platform->createWindow(1280, 720, "Warrior Example");
    return platform;
}

Lulu::Screen::Scene* initScene() {
    auto* scene = new Lulu::Screen::Scene();

    /* All custom scripts are added to a rendering priority queue. Each script is assigned a priority level (lower values render first).
    If two scripts share the same priority, they are rendered in the order they were registered.
    This allows layered rendering logic — for example, you can draw backgrounds first, then UI on top.
    */

    scene->registerScript(new WarriorLogic(), 10);
    scene->start();
    return scene;
}

void gameLoop(Program::Platform* platform, Lulu::Screen::Scene* scene) {
    while (true) {
        platform->updateWindow(scene);
        scene->render(platform->input);
        platform->swapBuffers();
        platform->resetInputStates();

        if (Keyboard::keyIsDown(platform->input, KEY_ESCAPE))
            break;
    }
}

int main() {
    auto platform = initPlatform();
    auto* scene = initScene();

    gameLoop(platform.get(), scene);

    platform->closeWindow();
    return 0;
}
```

---

## 🛠️ Build Instructions

### ✅ Dependencies

Install the required development packages:

```
sudo apt install libglfw3-dev libglew-dev libgl1-mesa-dev libx11-dev cmake
```

### 🔧 Build and Run

```
mkdir build
cd build
cmake ..
cmake --build .
./LuluProject
```

---

## 🗺️ Roadmap

### ✅ Done

- [x] Initial scene system with script attachment
- [x] Input handling (keyboard)
- [x] Basic rendering using OpenGL
- [x] Texture loading via `stb_image`
- [x] Custom memory allocator (early prototype)
- [x] Linux platform layer and window creation

### 🚧 In Progress / Planned

- [ ] Improve memory tracking (fragmentation handling, stats, reuse logic)
- [ ] Platform API abstractions for Windows 🪟
- [ ] Platform API abstractions for macOS 🍎
- [ ] Component system (transform, sprite, etc.)
- [ ] Audio system (basic sound effects)
- [ ] Sprite animations (using delta time and FPS capping for smooth timing)

## 🤝 Contributions

While this is primarily a personal learning project, I welcome:

- Suggestions for learning resources
- Code reviews and feedback
- Discussions about game engine architecture
- Ideas for interesting features to implement as learning exercises
