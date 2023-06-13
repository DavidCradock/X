#version 330 core
out vec4 FragColour;  
in vec4 colour;
in vec2 textureCoordinate;

uniform sampler2D texture0;

void main()
{
    FragColour = texture(texture0, textureCoordinate);
    FragColour *= colour;
}