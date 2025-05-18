#include "assets.hpp"

namespace Lulu::Assets
{
    Sprite createSprite(Assets::IVec2 offset, Assets::IVec2 size)
    {
        Sprite sprite = {.offset = offset, .size = size};
        return sprite;
    }
}
