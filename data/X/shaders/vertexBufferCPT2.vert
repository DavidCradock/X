// vertexBufferCPT2.vert
// For use with CResourceVertexBufferCPT2
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec2 aTexCoord2;

out vec4 colour;
out vec2 texCoord;
out vec2 texCoord2;

uniform mat4 matrixWorld;
uniform mat4 matrixView;
uniform mat4 matrixProjection;

void main()
{
    gl_Position = matrixProjection * matrixView * matrixWorld * vec4(aPos, 1.0f);
    colour = aColour;
    texCoord = aTexCoord;
    texCoord2 = aTexCoord2;
}   