#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aTexCoords;

uniform float offset;
uniform int flip = 1;

out vec3 ourColour;
out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x + offset, aPos.y * flip, aPos.z, 1.0);
    ourColour = aColour;
    TexCoords = aTexCoords;
}