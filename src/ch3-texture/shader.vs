#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

uniform float hOffset;
uniform float vOffset;
uniform int flip = 1;

out vec2 TexCoords;

void
main()
{
    gl_Position = vec4(aPos.x + hOffset, (aPos.y + vOffset) * flip, aPos.z, 1.0);
    TexCoords = aTexCoords;
}
