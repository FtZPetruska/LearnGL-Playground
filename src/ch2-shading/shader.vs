#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColour;

uniform float hOffset;
uniform float vOffset;
uniform int flip = 1;

out vec3 ourColour;

void
main()
{
    gl_Position = vec4(aPos.x + hOffset, (aPos.y + vOffset) * flip, aPos.z, 1.0);
    ourColour = aColour;
}
