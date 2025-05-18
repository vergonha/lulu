#include "scene.hpp"
#include "input.hpp"
#include "rendering.hpp"
#include "system.hpp"
#include <memory>

#ifndef LULU_PLATFORM_HPP
#define LULU_PLATFORM_HPP

#if defined(_WIN32) || defined(_WIN64)
#define LULU_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#define LULU_MACOS
#elif defined(__linux__)
#define LULU_LINUX
#else
#error "Unsupported platform!"
#endif

namespace Lulu::Program
{

    class Platform
    {
    public:
        Input *input;
        virtual ~Platform() = default;

        virtual bool createWindow(int width, int height, const char *title, Lulu::System::Memory::LuluAllocator *allocator) = 0;
        virtual bool createWindow(int width, int height, const char *title) = 0;
        virtual void updateWindow(Lulu::Screen::Scene *) = 0;
        virtual void swapBuffers() = 0;
        virtual void closeWindow() = 0;
        virtual void resetInputStates() = 0;

        static std::unique_ptr<Platform> GetPlatform();
    };
}

#endif // LULU_PLATFORM_HPP
