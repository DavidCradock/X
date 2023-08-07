// vertexBufferCPT.vert
// For use with CResourceVertexBufferCPT
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;
layout (location = 2) in vec2 aTexCoord;

out vec4 colour;
out vec2 texCoord;

uniform mat4 matrixWorld;
uniform mat4 matrixView;
uniform mat4 matrixProjection;

void main()
{
    gl_Position = matrixProjection * matrixView * matrixWorld * vec4(aPos, 1.0f);
    colour = aColour;
    texCoord = aTexCoord;
}   