// DRNE: Diffuse Roughness Normal Emission
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColour;
layout (location = 3) in vec2 aTexCoord;

out vec3 normal;
out vec4 colour;
out vec2 textureCoordinate;

// We're doing all the lighting calculations in world space
// So we need to pass the vertex position in world space
out vec3 vertexPosWorld;

uniform mat4 matrixProjection;
uniform mat4 matrixView;
uniform mat4 matrixWorld;

void main()
{
    gl_Position = matrixProjection * matrixView * matrixWorld * vec4(aPos, 1.0f);
    normal = aNormal;                                       // Vertex normal
    colour = aColour;                                       // Vertex colour
    textureCoordinate = aTexCoord;                          // Vertex texture coordinates
    vertexPosWorld = vec3(matrixWorld * vec4(aPos, 1.0));   // Vertex position in world space
}   