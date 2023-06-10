#version 330 core
out vec4 FragColour;  
in vec4 colour;
in vec2 textureCoordinate;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    FragColour = mix(texture(texture0, textureCoordinate), texture(texture1, textureCoordinate), 0.5);
    FragColour *= colour;
}