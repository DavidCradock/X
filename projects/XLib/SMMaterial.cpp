#include "PCH.h"
#include "SMMaterial.h"

namespace X
{
	SMMaterial::SMMaterial()
	{
		mstrTextureNameDiffuse = "X:texture_default_diffuse";
		mstrTextureNameEmission = "X:texture_default_emission";
		mstrTextureNameNormalmap = "X:texture_default_normal";
		mstrTextureNameRoughness = "X:texture_default_roughness";
		mfSpecularStrength = 1.0f;
		mfAmbientStrength = 0.05f;
	}
}