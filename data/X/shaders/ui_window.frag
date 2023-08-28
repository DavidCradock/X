// ui_window.frag
#version 330 core
out vec4 FragColour;  
in vec4 colour;
in vec2 texCoord0_colour;
in vec2 texCoord1_normal;

uniform sampler2D texture0;

uniform float fNormalAmount;
uniform float fMouseCursorDistance;

uniform vec2 v2MousePos;    // Y axis should be inverted

void main()
{
    // Now get colour values from the textures
    vec4 v4Col = texture(texture0, texCoord0_colour);
    vec3 v3Normal = texture(texture0, texCoord1_normal).rgb;

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
    FragColour.rgb += fNormalAmount * fDot;                         // Normal
    FragColour *= colour;                                           // Vertex colour
}