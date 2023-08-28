// ui.frag
#version 330 core
out vec4 FragColour;  
in vec4 colour;
in vec2 texCoordColour;
in vec2 texCoordNormal;

uniform sampler2D texture0_colourAndNormal;

uniform float fNormalAmount;
uniform float fMouseCursorDistance;

uniform vec2 v2MousePos;    // Y axis should be inverted

void main()
{
    // Now get colour values from the textures
    vec4 v4Col = texture(texture0_colourAndNormal, texCoordColour);
    vec3 v3Normal = texture(texture0_colourAndNormal, texCoordNormal).rgb;

    // Convert normal from 0.0f-1.0f to -1.0f to 1.0f
    v3Normal.x *= 2.0f;
    v3Normal.x -= 1.0f;
    v3Normal.y *= 2.0f;
    v3Normal.y -= 1.0f;

    // Compute direction from mouse cursor to fragment
    vec3 vDir = vec3(v2MousePos.x, v2MousePos.y, fMouseCursorDistance) - vec3(gl_FragCoord.x, gl_FragCoord.y, 0.0);
    vDir = normalize(vDir);
    float fDot = dot(vDir, v3Normal);

    // Add everything together to produce final fragment colour
    FragColour = v4Col;
//	FragColour.r = v3Normal.r;// Colour map
//	FragColour.a = 1;
//	FragColour = v4col;
    FragColour.rgb += fNormalAmount * fDot;                         // Normal
    FragColour *= colour;                                           // Vertex colour
}