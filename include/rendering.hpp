#pragma once

#ifndef LULU_RENDERING_HPP
#define LULU_RENDERING_HPP
#include "assets.hpp"
#include "camera.hpp"

namespace Lulu::Rendering
{

    struct RenderData
    {
        Camera::OrthographicCamera2D camera;
        int transformCount;
        std::vector<Assets::Transform> transforms;
    };

    void drawSprite(RenderData *renderData, Assets::Sprite sprite, Assets::Vec2 position, Assets::Vec2 size);
    void drawSprite(RenderData *renderData, Assets::Sprite sprite, Assets::Vec2 position);
}

#endif // LULU_RENDERING_HPP
