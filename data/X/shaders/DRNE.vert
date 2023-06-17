// DRNE: Diffuse Roughness Normal Emission
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColour;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBinormal;

//out vec3 vec3VertexNormal;    // Don't need this as we're using the TBN matrix
out vec4 vec4VertexColour;
out vec2 vec2TextureCoordinate;
out mat3 matTBN;
out vec3 vec3VertexPosWorld;

uniform mat4 matrixProjection;
uniform mat4 matrixView;
uniform mat4 matrixWorld;

void main()
{
    gl_Position = matrixProjection * matrixView * matrixWorld * vec4(aPos, 1.0f);

    // Compute TBN matrix and send to fragment shader
    vec3 T = normalize(vec3(matrixWorld * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(matrixWorld * vec4(aBinormal, 0.0)));
    vec3 N = normalize(vec3(matrixWorld * vec4(aNormal,    0.0)));
    matTBN = mat3(T, B, N);
    // NOTE: If we wanted to convert all the lighting vars from world space to tangent space instead,
    // We could simply take the transpose of the TBN matrix and use that to multiply all the world vectors into tangent space...
//    matTBN = transpose(mat3(T, B, N));
    // Note: We should really compute the inverse matrix, but because the transpose of orthogonal matrices (each axis is a perpendicular unit vector) equals its inverse,
    // we can use compute the transpose instead and as the transpose is less expensive to compute, woohoo!

    //vec3VertexNormal = aNormal;                                       // Vertex normal (OK to do this if not performing non-uniform scaling with the world matrix)
    // Compute the normal matrix, multiply the normal by that and we're all good.
    // We convert the world transform matrix to a 3x3 matrix, removing the translation part
    // NOTE: Expensive as this is per vertex, we should compute this on CPU and set as uniform
    // NOTE: (Don't need this as using TBN)
//    vec3VertexNormal = mat3(transpose(inverse(matrixWorld))) * aNormal;  

    vec4VertexColour = aColour;                                 // Vertex colour
    vec2TextureCoordinate = aTexCoord;                          // Vertex texture coordinates
    vec3VertexPosWorld = vec3(matrixWorld * vec4(aPos, 1.0));   // Vertex position in world space
}   