// vertexBufferInstanced.vert
// For use with CResourceVertexBufferInstanced
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;
layout (location = 2) in vec2 aTexCoord;

out vec4 colour;
out vec2 textureCoordinate;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * vec4(aPos, 1.0f);
    colour = aColour;
    textureCoordinate = aTexCoord;
}   