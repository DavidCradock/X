// ui_window.frag
#version 330 core
out vec4 FragColour;  
in vec4 colour;
in vec2 texCoord0;
in vec2 texCoord1;

uniform sampler2D texture0_colour;
uniform sampler2D texture1_normal;
uniform sampler2D texture2_background;

uniform float fNormalAmount;
uniform float fMouseCursorDistance;
uniform vec2 v2MousePos;    // Y axis should be inverted

void main()
{
    // Convert background texture coordinate from fragment position to 0.0f to 1.0f range
	vec2 v2BGDims = textureSize(texture2_background, 0);
    vec2 v2BGCoords = 1.0 / v2BGDims;

	v2BGCoords.x *= gl_FragCoord.x;
	v2BGCoords.y *= gl_FragCoord.y;

    // Now get colour values from the textures
    vec4 v4Col = texture(texture0_colour, texCoord0);
	vec3 v3Normal = texture(texture1_normal, texCoord1).rgb;
	vec3 v3ColBG = texture(texture2_background, v2BGCoords).rgb;

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
    FragColour = v4Col;                                             // Colour map
    FragColour.rgb += fNormalAmount * fDot;                         // Normal
//	FragColour.rgb += 1.0 - v3ColBG;								// Background
    FragColour *= colour;                                           // Vertex colour
}