#pragma once
#ifndef LULU_SCRIPTS_HPP
#define LULU_SCRIPTS_HPP
#include <iostream>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <memory>
#include <utility>
#include <map>
#include <string>
#include "system.hpp"
#include "input.hpp"
#include "rendering.hpp"
#include "logger.hpp"
#include "assets.hpp"

namespace Lulu::UserScripts
{
    class DrawableComponent
    {
    public:
        Lulu::Assets::Sprite sprite;
        Lulu::Assets::Vec2 pos;

        DrawableComponent(Lulu::Assets::Sprite sprite, Lulu::Assets::Vec2 pos)
        {
            this->sprite = sprite;
            this->pos = pos;
        }
    };

    class CustomLogic
    {
    private:
        const char *texturePath;

    protected:
        CustomLogic() : state(std::make_unique<Rendering::RenderData>()) {};

    public:
        std::map<std::string, DrawableComponent *> drawables;
        std::unique_ptr<Rendering::RenderData> state;

        virtual ~CustomLogic() = default;
        virtual void start() = 0;
        virtual void update(Input *input) = 0;

        void createCamera(Lulu::Assets::Vec2 dimensions, Lulu::Assets::Vec2 position)
        {
            this->state->camera.dimensions = dimensions;
            this->state->camera.position = position;
        }

        void registerDrawable(Lulu::Assets::Sprite sprite, Lulu::Assets::Vec2 pos, const char *name)
        {
            LULU_ASSERT(name != nullptr, "registerDrawable: Name is null");
            LULU_ASSERT(this->drawables.find(name) == this->drawables.end(), "registerDrawable: Name already exists");
            this->drawables[name] = new DrawableComponent(sprite, pos);
        }

        void setTexturePath(const char *path)
        {
            LULU_ASSERT(path != nullptr, "setTexturePath: Path is null");
            LULU_ASSERT(Lulu::System::IO::fileExists(path), "setTexturePath: File does not exist");
            this->texturePath = path;
        }

        const char *getTexturePath()
        {
            return this->texturePath;
        }

        void draw(const char *name)
        {
            LULU_ASSERT(name != nullptr, "draw: Name is null");
            LULU_ASSERT(this->drawables.find(name) != this->drawables.end(), "draw: Name does not exist");
            DrawableComponent *drawable = this->drawables[name];
            Rendering::drawSprite(state.get(), drawable->sprite, drawable->pos);
        }

        void draw(const char *name, Lulu::Assets::Vec2 pos)
        {
            LULU_ASSERT(name != nullptr, "draw: Name is null");
            LULU_ASSERT(this->drawables.find(name) != this->drawables.end(), "draw: Name does not exist");
            DrawableComponent *drawable = this->drawables[name];
            Rendering::drawSprite(state.get(), drawable->sprite, pos);
        }

        void draw(const char *name, Lulu::Assets::Vec2 pos, Lulu::Assets::Vec2 size)
        {
            LULU_ASSERT(name != nullptr, "draw: Name is null");
            LULU_ASSERT(this->drawables.find(name) != this->drawables.end(), "draw: Name does not exist");
            DrawableComponent *drawable = this->drawables[name];
            Rendering::drawSprite(state.get(), drawable->sprite, pos, size);
        }
    };

}

#endif // LULU_SCRIPTS_HPP
