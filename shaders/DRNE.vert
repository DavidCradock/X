// DRNE: Diffuse Roughness Normal Emission
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColour;
layout (location = 3) in vec2 aTexCoord;

out vec3 vec3VertexNormal;
out vec4 vec4VertexColour;
out vec2 vec2TextureCoordinate;

// We're doing all the lighting calculations in world space
// So we need to pass the vertex position in world space
out vec3 vec3VertexPosWorld;

uniform mat4 matrixProjection;
uniform mat4 matrixView;
uniform mat4 matrixWorld;

void main()
{
    gl_Position = matrixProjection * matrixView * matrixWorld * vec4(aPos, 1.0f);
    //vec3VertexNormal = aNormal;                                       // Vertex normal (OK to do this if not performing non-uniform scaling with the world matrix)
    // Compute the normal matrix, multiply the normal by that and we're all good.
    // We convert the world transform matrix to a 3x3 matrix, removing the translation part
    // NOTE: Expensive as this is per vertex, we should compute this on CPU and set as uniform
    vec3VertexNormal = mat3(transpose(inverse(matrixWorld))) * aNormal;  

    vec4VertexColour = aColour;                                 // Vertex colour
    vec2TextureCoordinate = aTexCoord;                          // Vertex texture coordinates
    vec3VertexPosWorld = vec3(matrixWorld * vec4(aPos, 1.0));   // Vertex position in world space
}   