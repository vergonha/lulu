#include <vector>
#include "rendering.hpp"
#include "assets.hpp"
#include "logger.hpp"
#include "scene.hpp"
#include "stb_image.h"
#include <GL/glx.h>

namespace Lulu::Rendering
{

    void drawSprite(RenderData *renderData, Assets::Sprite sprite, Assets::Vec2 position, Assets::Vec2 size)
    {
        Assets::Transform transform{.offset = sprite.offset, .size = sprite.size, .position = position - size / 2.0f, .area = size};

        renderData->transforms.push_back(transform);
        renderData->transformCount++;
    };

    void drawSprite(RenderData *renderData, Assets::Sprite sprite, Assets::Vec2 position)
    {
        Assets::Transform transform{.offset = sprite.offset, .size = sprite.size, .position = position - sprite.size.toVec2() / 2.0f, .area = sprite.size.toVec2()};

        renderData->transforms.push_back(transform);
        renderData->transformCount++;
    };

}
