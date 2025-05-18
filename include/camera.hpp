#pragma once
#include "assets.hpp"
#ifndef LULU_CAMERA_HPP
#define LULU_CAMERA_HPP

namespace Lulu::Camera
{
    struct OrthographicCamera2D
    {
        float zoom = 1.0f;
        Assets::Vec2 dimensions;
        Assets::Vec2 position;
    };

    Assets::Mat4 translateScreenToWorldPosition(float left, float right, float top, float bottom);
    Assets::IVec2 translateScreenToWorldPosition(OrthographicCamera2D camera, Assets::IVec2 screenSize, Assets::IVec2 screenPosition);

}

#endif // LULU_CAMERA_HPP
