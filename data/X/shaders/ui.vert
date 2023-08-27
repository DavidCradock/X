// ui.vert
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;
layout (location = 2) in vec2 aTexCoord0;
layout (location = 3) in vec2 aTexCoord1;

out vec4 colour;
out vec2 texCoord0;
out vec2 texCoord1;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    colour = aColour;
    texCoord0 = aTexCoord0;
	texCoord1 = aTexCoord1;
}   