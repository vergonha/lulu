#include "camera.hpp"
#include "assets.hpp"

namespace Lulu::Camera
{
    // https://en.wikipedia.org/wiki/Orthographic_projection
    Assets::Mat4 translateScreenToWorldPosition(float left, float right, float top, float bottom)
    {
        Assets::Mat4 ortho = {0};
        float far = 1, near = 0;

        ortho.ax = 2.0f / (right - left);
        ortho.aw = -(right + left) / (right - left);

        ortho.by = 2.0f / (top - bottom);
        ortho.bw = (top + bottom) / (top - bottom);

        ortho.cz = -2.0f / (far - near);
        ortho.cw = 0.0f; // Near Plane

        ortho.dw = 1.0f;

        return ortho;
    };

    Assets::IVec2 translate_screen_to_world_position(OrthographicCamera2D camera, Assets::IVec2 screenSize, Assets::IVec2 screenPosition)
    {
        Assets::IVec2 relativePosition;

        relativePosition.x = static_cast<float>(screenPosition.x) / screenSize.x * camera.dimensions.x;
        relativePosition.x += -camera.dimensions.x / 2.00f + camera.position.x;

        relativePosition.y = static_cast<float>(screenPosition.y) / screenSize.y * camera.dimensions.y;
        relativePosition.y += -camera.dimensions.y / 2.00f + camera.position.y;

        return relativePosition;
    }
}
