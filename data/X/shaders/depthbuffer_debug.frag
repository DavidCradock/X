// depthbuffer_debug.frag
#version 330 core
out vec4 FragColour;  
in vec4 colour;
in vec2 textureCoordinate;

uniform sampler2D texture0;

void main()
{
    float depthValue = texture(texture0, textureCoordinate).r;
    FragColour = vec4(vec3(depthValue), 1.0);
    FragColour *= colour;
}