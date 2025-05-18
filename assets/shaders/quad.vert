#version 430 core

struct Transform {
    ivec2 offset;
    ivec2 size;
    vec2 position;
    vec2 area;
};

layout(std430, binding = 0) buffer TransformSBO
{
    Transform transforms[];
};

uniform vec2 screenSize;
uniform mat4 orthographicProjection;

layout(location = 0) out vec2 textureCoordsOut;

void main()
{
    Transform transform = transforms[gl_InstanceID];

    // OpenGL Coordinates
    // -1/+1            +1/+1
    // -1/-1            +1/-1

    vec2 vertices[6] =
        {
            transform.position, // Top Left
            vec2(transform.position + vec2(0.0, transform.area.y)), // Bottom Left
            vec2(transform.position + vec2(transform.area.x, 0.0)), // Top Right
            vec2(transform.position + vec2(transform.area.x, 0.0)), // Top Right
            vec2(transform.position + vec2(0.0, transform.area.y)), // Bottom Left
            transform.position + transform.area, // Bottom Right
        };

    float left = transform.offset.x;
    float top = transform.offset.y;
    float right = left + transform.size.x;
    float bottom = top + transform.size.y;

    // Texture coords INSIDE atlas!
    vec2 textureCoords[6] = {
            vec2(left, top),
            vec2(left, bottom),
            vec2(right, top),
            vec2(right, top),
            vec2(left, bottom),
            vec2(right, bottom),
        };

    {
        vec2 vertexPos = vertices[gl_VertexID];
        // vertexPos.y = (screenSize.y - vertexPos.y) / screenSize.y;
        // vertexPos = 2.0 * (vertexPos / screenSize) - 1.0;
        gl_Position = orthographicProjection * vec4(vertexPos, 0.0, 1.0);
    }

    textureCoordsOut = textureCoords[gl_VertexID];
}
