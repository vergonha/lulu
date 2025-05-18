#include <GL/glew.h>
#include <GL/glx.h>
#include "scene.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "logger.hpp"
#include "platform.hpp"
#include "rendering.hpp"
#include "system.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

namespace Lulu::Program
{
  class PlatformLinux : public Platform
  {
    Display *display;
    Window window;
    GLXContext glx_context;

    void handleResize(XEvent *event, Lulu::Screen::Scene *scene)
    {
      XConfigureEvent *configure_event = &event->xconfigure;

      input->screenSize.x = configure_event->x;
      input->screenSize.y = configure_event->y;

      // should i?? >.<
      // for (const auto context : scene->contexts)
      // {
      // context->logic->state->camera.game.dimensions = input->screenSize.toVec2();
      // }
    }

    KeyCodeID convertKeySymToKeyCodeID(KeySym sym)
    {
      switch (sym)
      {
      case XK_a:
        return KEY_A;
      case XK_b:
        return KEY_B;
      case XK_c:
        return KEY_C;
      case XK_d:
        return KEY_D;
      case XK_e:
        return KEY_E;
      case XK_f:
        return KEY_F;
      case XK_g:
        return KEY_G;
      case XK_h:
        return KEY_H;
      case XK_i:
        return KEY_I;
      case XK_j:
        return KEY_J;
      case XK_k:
        return KEY_K;
      case XK_l:
        return KEY_L;
      case XK_m:
        return KEY_M;
      case XK_n:
        return KEY_N;
      case XK_o:
        return KEY_O;
      case XK_p:
        return KEY_P;
      case XK_q:
        return KEY_Q;
      case XK_r:
        return KEY_R;
      case XK_s:
        return KEY_S;
      case XK_t:
        return KEY_T;
      case XK_u:
        return KEY_U;
      case XK_v:
        return KEY_V;
      case XK_w:
        return KEY_W;
      case XK_x:
        return KEY_X;
      case XK_y:
        return KEY_Y;
      case XK_z:
        return KEY_Z;

      case XK_0:
        return KEY_0;
      case XK_1:
        return KEY_1;
      case XK_2:
        return KEY_2;
      case XK_3:
        return KEY_3;
      case XK_4:
        return KEY_4;
      case XK_5:
        return KEY_5;
      case XK_6:
        return KEY_6;
      case XK_7:
        return KEY_7;
      case XK_8:
        return KEY_8;
      case XK_9:
        return KEY_9;

      case XK_space:
        return KEY_SPACE;
      case XK_Escape:
        return KEY_ESCAPE;
      case XK_Return:
        return KEY_RETURN;
      case XK_BackSpace:
        return KEY_BACKSPACE;
      case XK_Tab:
        return KEY_TAB;
      case XK_Shift_L:
      case XK_Shift_R:
        return KEY_SHIFT;
      case XK_Control_L:
      case XK_Control_R:
        return KEY_CONTROL;
      case XK_Alt_L:
      case XK_Alt_R:
        return KEY_ALT;

      case XK_Left:
        return KEY_LEFT;
      case XK_Right:
        return KEY_RIGHT;
      case XK_Up:
        return KEY_UP;
      case XK_Down:
        return KEY_DOWN;

      case XK_F1:
        return KEY_F1;
      case XK_F2:
        return KEY_F2;
      case XK_F3:
        return KEY_F3;
      case XK_F4:
        return KEY_F4;
      case XK_F5:
        return KEY_F5;
      case XK_F6:
        return KEY_F6;
      case XK_F7:
        return KEY_F7;
      case XK_F8:
        return KEY_F8;
      case XK_F9:
        return KEY_F9;
      case XK_F10:
        return KEY_F10;
      case XK_F11:
        return KEY_F11;
      case XK_F12:
        return KEY_F12;

      default:
        return KEY_COUNT;
      }
    }

    void updateWindow(Lulu::Screen::Scene *scene)
    {
      XEvent event;
      // XNextEvent(display, &event);

      while (XCheckWindowEvent(display, window, ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask, &event))
        switch (event.type)
        {
        case (MotionNotify):
        {
          input->prevMousePos = input->mousePos;
          input->mousePos.x = event.xmotion.x;
          input->mousePos.y = event.xmotion.y;

          input->relMouse = input->mousePos - input->prevMousePos;

          input->prevMousePosWorld = input->mousePosWorld;
          // input->mousePosWorld = Camera::translate_screen_to_world_position(renderData->camera.game, input->screenSize, input->mousePos);
        }
        case (KeyPress):
        {
          KeySym sym = XLookupKeysym(&event.xkey, 0);
          KeyCodeID code = convertKeySymToKeyCodeID(sym);

          if (code != KEY_COUNT)
          {
            Key &key = input->keys[code];

            key.justPressed = !key.justPressed && !key.isDown;
            key.justReleased = !key.justReleased && !key.isDown;
            key.isDown = 1;
            key.halfTransitionCount++;
          }

          break;
        }
        case KeyRelease:
        {

          // Check if this is an auto-repeat event
          // if (XEventsQueued(display, QueuedAfterReading)) {
          //     XEvent nextEvent;
          //     XPeekEvent(display, &nextEvent);

          //     // If the next event is a KeyPress for the same key with a very close timestamp,
          //     // this KeyRelease is part of an auto-repeat sequence, so ignore it
          //     if (nextEvent.type == KeyPress &&
          //         nextEvent.xkey.time == event.xkey.time &&
          //         nextEvent.xkey.keycode == event.xkey.keycode) {
          //         // This is an auto-repeat event, skip it
          //         break;
          //     }
          // }

          KeySym sym = XLookupKeysym(&event.xkey, 0);
          KeyCodeID code = convertKeySymToKeyCodeID(sym);
          if (code != KEY_COUNT)
          {
            Key &key = input->keys[code];
            key.justPressed = !key.justPressed && !key.isDown;
            key.justReleased = !key.justReleased && !key.isDown;
            key.isDown = 0;
            key.halfTransitionCount++;
          }

          break;
        }
        case (ConfigureNotify):
          handleResize(&event, scene);
        case (Expose):
          handleResize(&event, scene);
        }
    }

    void resetInputStates()
    {
      for (int i = 0; i < KEY_COUNT; i++)
      {
        input->keys[i].justPressed = 0;
        input->keys[i].justReleased = 0;
        input->keys[i].halfTransitionCount = 0;
      }
    }

    void closeWindow()
    {
      glXDestroyContext(display, glx_context);
      XDestroyWindow(display, window);
      XCloseDisplay(display);
    }

    bool createWindow(int width, int height, char const *title,
                      System::Memory::LuluAllocator *allocator)
    {

      display = XOpenDisplay(NULL);

      if (!display)
      {
        LULU_ERROR("Failed to open X11 display");
        return false;
      }

      input = (Input *)System::Memory::alloc(allocator, sizeof(Input));
      input->screenSize.x = width;
      input->screenSize.y = height;

      int screen = DefaultScreen(display);
      Window root = RootWindow(display, screen);

      window = XCreateSimpleWindow(display, root, 100, 100, width, height, 1,
                                   BlackPixel(display, screen),
                                   WhitePixel(display, screen));

      XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask);
      XMapWindow(display, window);
      XStoreName(display, window, title);

      GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
      XVisualInfo *vi = glXChooseVisual(display, screen, att);
      if (!vi)
      {
        LULU_ERROR("No valid GLX configuration found");
        return false;
      }

      glx_context = glXCreateContext(display, vi, NULL, GL_TRUE);
      if (!glx_context)
      {
        LULU_ERROR("Failed to create OpenGL context with GLX");
        return false;
      }

      glXMakeCurrent(display, window, glx_context);

      glewExperimental = GL_TRUE;
      if (glewInit() != GLEW_OK)
      {
        LULU_ERROR("Failed to initialize GLEW");
        return false;
      }

      LULU_TRACE("OpenGL context successfully created!");
      return true;
    }

    bool createWindow(int width, int height, char const *title)
    {

      display = XOpenDisplay(NULL);

      if (!display)
      {
        LULU_ERROR("Failed to open X11 display");
        return false;
      }

      input = (Input *)calloc(1, sizeof(Input));
      input->screenSize.x = width;
      input->screenSize.y = height;

      int screen = DefaultScreen(display);
      Window root = RootWindow(display, screen);

      window = XCreateSimpleWindow(display, root, 100, 100, width, height, 1,
                                   BlackPixel(display, screen),
                                   WhitePixel(display, screen));

      XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask);
      XMapWindow(display, window);
      XStoreName(display, window, title);

      GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
      XVisualInfo *vi = glXChooseVisual(display, screen, att);
      if (!vi)
      {
        LULU_ERROR("No valid GLX configuration found");
        return false;
      }

      glx_context = glXCreateContext(display, vi, NULL, GL_TRUE);
      if (!glx_context)
      {
        LULU_ERROR("Failed to create OpenGL context with GLX");
        return false;
      }

      glXMakeCurrent(display, window, glx_context);

      glewExperimental = GL_TRUE;
      if (glewInit() != GLEW_OK)
      {
        LULU_ERROR("Failed to initialize GLEW");
        return false;
      }

      LULU_TRACE("OpenGL context successfully created!");
      return true;
    }

    void swapBuffers() { glXSwapBuffers(display, window); }
  };

}
