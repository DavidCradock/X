#include "PCH.h"
#include "SMLightPoint.h"

namespace X
{
	CSMLightPoint::CSMLightPoint()
	{
		mvPosition.setZero();

		mvColour.set(1.0f, 1.0f, 1.0f, 1.0f);

		mfSpecularAttenuationLinear = 0.045f;
		mfSpecularAttenuationQuadratic = 0.0075f;
	}

	// Specular attenuation/falloff values taken from https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
	/*
	A point light's attenuation defines how bright it is with respect to its distance from objects.
	The shader takes four Real parameters: Range, Constant, Linear, and Quadratic.
	The value is computed in the fragment shader using...
		float attenuation = 1.0 / (1.0 + pointLights[iLightNumber].linear * distance + pointLights[iLightNumber].quadratic * (distance * distance));
	So we need to set linear and quadratic.
	By trial and error, the lovely people at Ogre3D.org came up with the following values that work...
	Range	Constant	Linear	Quadratic
	3250,	1.0,		0.0014, 0.000007
	600,	1.0,		0.007,	0.0002
	325,	1.0,		0.014,	0.0007
	200,	1.0,		0.022,	0.0019
	160,	1.0,		0.027,	0.0028
	100,	1.0,		0.045,	0.0075
	65,		1.0,		0.07,	0.017
	50,		1.0,		0.09,	0.032
	32,		1.0,		0.14,	0.07
	20,		1.0,		0.22,	0.20
	13,		1.0,		0.35,	0.44
	7,		1.0,		0.7,	1.8
	Range is the distance of the point light from the fragment's world position in which the light affects the specular attenuation.
	Whereby if the distance is greater to the range, it doesn't affect attenuation
	*/
}