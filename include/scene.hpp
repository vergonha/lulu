#pragma once
#ifndef LULU_SCENE_HPP
#define LULU_SCENE_HPP
#include <queue>
#include "scripts.hpp"
#include <GL/glew.h>
#include "system.hpp"
#include <vector>

namespace Lulu::Screen
{
    struct GLContext
    {
        GLuint programID;
        GLuint textureID;
        GLuint screenSizeID;
        GLuint transformSBOID;
        GLuint projectionID;

        UserScripts::CustomLogic *logic;
    };

    class Scene
    {
    private:
        static constexpr size_t INITIAL_BUFFER_SIZE = 512;
        static constexpr float BUFFER_GROWTH_FACTOR = 2.0f;

        static const int MAX_TEXTURES = 32;
        Lulu::System::Memory::LuluAllocator *transientStorage;
        struct Renderable
        {
            GLContext *context;
            int priority;

            size_t insertOrder;

            bool operator<(const Renderable &other) const
            {
                if (priority != other.priority)
                    return priority > other.priority;

                return priority < other.priority;
            }
        };

        std::vector<Renderable> renderQueue;
        size_t insertOrder = 0;

    public:
        GLuint programID;
        Scene(Lulu::System::Memory::LuluAllocator *transientStorage);
        Scene();

        void loadTextures();
        void registerScript(UserScripts::CustomLogic *script, int priority = 10);
        void compileShaders();
        GLuint compileShaderFromFile(GLenum shaderType, const char *path);
        int calculateTotalTransforms(Input *input);
        void resizeBuffer(int newSize);
        void start();
        void render(Input *input);
    };
}

#endif // LULU_SCENE_HPP
