// ui.vert
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;
layout (location = 2) in vec2 aTexCoord0;
layout (location = 3) in vec2 aTexCoord1;

out vec4 colour;
out vec2 texCoord0_colour;
out vec2 texCoord1_normal;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    colour = aColour;
    texCoord0_colour = aTexCoord0;
	texCoord1_normal = aTexCoord1;
}   