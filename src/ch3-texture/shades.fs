#version 330 core
in vec3 ourColour;
in vec2 TexCoords;

out vec4 FragColour;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform int direction = 1;
uniform float mixer = 0.5;

void main()
{
    FragColour = mix(texture(texture0, vec2(direction * TexCoords.x, TexCoords.y)), texture(texture1, TexCoords), mixer);
}