#version 330 core
in vec2 TexCoords;

out vec4 FragColour;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec2 flips;
uniform float mixer = 0.5;

void
main()
{
    FragColour = mix(texture(texture0, TexCoords * flips), texture(texture1, TexCoords * flips), mixer);
}
