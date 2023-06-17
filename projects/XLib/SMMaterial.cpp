#include "PCH.h"
#include "SMMaterial.h"

namespace X
{
	SMMaterial::SMMaterial()
	{
		mstrTextureNameDiffuse = "X:default_diffuse";
		mstrTextureNameEmission = "X:default_emission";
		mstrTextureNameNormalmap = "X:default_normal";
		mstrTextureNameRoughness = "X:default_roughness";
		mfSpecularStrength = 0.5f;
		mfAmbientStrength = 0.05f;
	}
}