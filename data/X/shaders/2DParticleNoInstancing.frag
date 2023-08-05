// 2DParticleNoInstancing.frag
#version 330 core
out vec4 FragColour;  
in vec4 colour;
in vec2 textureCoordinate;

uniform sampler2D textureBirth;
uniform sampler2D textureDeath;

void main()
{
    FragColour = texture(textureBirth, textureCoordinate);
    FragColour *= colour;
}