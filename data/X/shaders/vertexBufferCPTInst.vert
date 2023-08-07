// vertexBufferCPTInst.vert
// For use with CResourceVertexBufferInstanced
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 matrixInstance;   // This will take up vertex atttribute locations 3, 4, 5 and 6 as it is 4 x vec4s

out vec4 colour;
out vec2 textureCoordinate;

uniform mat4 matrixView;
uniform mat4 matrixProjection;

void main()
{
    gl_Position = matrixProjection * matrixView * matrixInstance * vec4(aPos, 1.0f);
    colour = aColour;
    textureCoordinate = aTexCoord;
}   