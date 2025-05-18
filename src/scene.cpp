#include <GL/glew.h>
#include <iostream>
#include <algorithm>
#include "scene.hpp"
#include "scripts.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "input.hpp"
#include "rendering.hpp"
#include "logger.hpp"
#include "system.hpp"
#include <GL/glcorearb.h>
#include <cstring>

Lulu::Screen::Scene::Scene(Lulu::System::Memory::LuluAllocator *transientStorage)
{
    this->transientStorage = transientStorage;
    this->compileShaders();

    glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(GL_MULTISAMPLE);
    glUseProgram(this->programID);
};

Lulu::Screen::Scene::Scene()
{
    this->compileShaders();

    glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(GL_MULTISAMPLE);
    glUseProgram(this->programID);
}

void Lulu::Screen::Scene::registerScript(Lulu::UserScripts::CustomLogic *script, int priority)
{

    GLContext *context = new GLContext;
    context->logic = script;

    Renderable item{
        .context = context,
        .priority = priority,
        .insertOrder = this->insertOrder++,
    };

    this->renderQueue.push_back(item);
    std::sort(this->renderQueue.begin(), this->renderQueue.end());
}

GLuint Lulu::Screen::Scene::compileShaderFromFile(GLenum shaderType, const char *path)
{
    void *shader_content = Lulu::System::IO::getFileContent(path);
    LULU_ASSERT(shader_content, "Error while loading shader.");

    GLuint shader_id = glCreateShader(shaderType);
    glShaderSource(shader_id, 1, (const GLchar **)(&shader_content), nullptr);
    glCompileShader(shader_id);

    GLint success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[2048];
        glGetShaderInfoLog(shader_id, 2048, NULL, infoLog);
        LULU_ERROR(infoLog);
    }

    return shader_id;
}

void Lulu::Screen::Scene::compileShaders()
{

    LULU_TRACE("Compiling shaders");
    std::unique_ptr<GLuint> vertexShader = std::make_unique<GLuint>(this->compileShaderFromFile(GL_VERTEX_SHADER, "../assets/shaders/quad.vert"));
    std::unique_ptr<GLuint> fragmentShader = std::make_unique<GLuint>(this->compileShaderFromFile(GL_FRAGMENT_SHADER, "../assets/shaders/quad.frag"));

    this->programID = glCreateProgram();
    glAttachShader(this->programID, *vertexShader);
    glAttachShader(this->programID, *fragmentShader);
    glLinkProgram(this->programID);

    GLint success;
    glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[2048];
        glGetProgramInfoLog(this->programID, 2048, NULL, infoLog);
        LULU_ERROR(infoLog);
    }

    LULU_TRACE("Shaders attached");
};

void Lulu::Screen::Scene::start()
{
    this->loadTextures();

    for (auto &item : this->renderQueue)
    {
        GLContext *context = item.context;

        context->logic->start();
        context->screenSizeID = glGetUniformLocation(programID, "screenSize");
        context->projectionID = glGetUniformLocation(programID, "orthographicProjection");

        glGenBuffers(1, &context->transformSBOID);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, context->transformSBOID);

        // Allocate buffer size
        glBufferData(GL_SHADER_STORAGE_BUFFER,
                     sizeof(Lulu::Assets::Transform) * INITIAL_BUFFER_SIZE,
                     context->logic->state->transforms.data(),
                     GL_DYNAMIC_DRAW);
    }
}

void Lulu::Screen::Scene::loadTextures()
{
    for (auto &item : this->renderQueue)
    {
        GLContext *context = item.context;

        int width, height, channels;
        char *data = (char *)stbi_load(context->logic->getTexturePath(), &width, &height, &channels, 4);
        LULU_ASSERT(data, "Failed to load texture");

        glGenTextures(1, &context->textureID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, context->textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }
}

void Lulu::Screen::Scene::render(Input *input)
{
    glClearColor(199.0f, 199.0f, 199.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, input->screenSize.x, input->screenSize.y);
    GLfloat screen[2] = {
        static_cast<float>(input->screenSize.x),
        static_cast<float>(input->screenSize.y)};

    // calculate and resize buffer if needed
    int totalTransformsCount = calculateTotalTransforms(input);

    for (size_t i = 0; i < this->renderQueue.size(); i++)
    {

        auto &item = this->renderQueue[i];
        GLContext *context = item.context;

        // updated before render to allow for dynamic resizings
        // context->logic->update(input);

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, context->textureID);

        GLint textureSamplerLocation = glGetUniformLocation(programID, "textureAtlas");
        glUniform1i(textureSamplerLocation, i);

        glUniform2fv(context->screenSizeID, 1, screen);

        Lulu::Camera::OrthographicCamera2D camera = context->logic->state->camera;
        Lulu::Assets::Mat4 projection = Lulu::Camera::translateScreenToWorldPosition(
            camera.position.x - camera.dimensions.x / 2.0f,
            camera.position.x + camera.dimensions.x / 2.0f,
            camera.position.y - camera.dimensions.y / 2.0f,
            camera.position.y + camera.dimensions.y / 2.0f);

        glUniformMatrix4fv(context->projectionID, 1, GL_FALSE, &projection.ax);

        if (totalTransformsCount > INITIAL_BUFFER_SIZE)
        {
            glBufferData(GL_SHADER_STORAGE_BUFFER,
                         sizeof(Lulu::Assets::Transform) * totalTransformsCount,
                         context->logic->state->transforms.data(),
                         GL_DYNAMIC_DRAW);
        }

        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
                        sizeof(Lulu::Assets::Transform) * context->logic->state->transformCount,
                        context->logic->state->transforms.data());

        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, context->logic->state->transformCount);

        // Reset transform count
        context->logic->state->transformCount = 0;
        context->logic->state->transforms.clear();
    }
}

void Lulu::Screen::Scene::resizeBuffer(int requiredSize)
{
    static size_t currentBufferSize = INITIAL_BUFFER_SIZE;

    if (requiredSize > currentBufferSize)
    {
        currentBufferSize = static_cast<size_t>(currentBufferSize * BUFFER_GROWTH_FACTOR);
        for (auto &item : renderQueue)
        {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, item.context->transformSBOID);
            glBufferData(GL_SHADER_STORAGE_BUFFER,
                         sizeof(Assets::Transform) * currentBufferSize,
                         item.context->logic->state->transforms.data(),
                         GL_DYNAMIC_DRAW);
        }
    }
}

int Lulu::Screen::Scene::calculateTotalTransforms(Input *input)
{
    int totalTransformsCount = 0;
    for (auto &item : this->renderQueue)
    {
        GLContext *context = item.context;
        context->logic->update(input);

        totalTransformsCount += context->logic->state->transformCount;
    }

    return totalTransformsCount;
}