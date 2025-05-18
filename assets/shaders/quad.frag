
#version 430 core

layout(location = 0) out vec4 fragColor;

layout(location = 0) uniform sampler2D textureAtlas;

layout(location = 0) in vec2 textureCoordsIn;
void main()
{
    vec4 textureColor = texelFetch(textureAtlas, ivec2(textureCoordsIn), 0);

    if (textureColor.a == 0)
        discard;

    fragColor = textureColor;
}
