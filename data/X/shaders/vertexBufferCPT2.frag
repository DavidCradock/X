// vertexBufferCPT2.frag
// For use with CResourceVertexBufferCPT2
#version 330 core
out vec4 FragColour;  
in vec4 colour;
in vec2 texCoord;
in vec2 texCoord2;

uniform sampler2D texture0;
uniform sampler2D texture1;

// Amount of texture0 and texture1.
// If 0.0f, texture0 has full contribution and texture1, none.
// If 0.5f, texture0 and texture1 are added equally.
// If 1.0f, texture1 has full contribution and texture0, none.
uniform float fTextureContribution;   

void main()
{
    FragColour = texture(texture0, texCoord) * fTextureContribution;
    FragColour += texture(texture1, texCoord2) * (1.0 - fTextureContribution);
    FragColour *= colour;
}