#pragma once
#include "PCH.h"

namespace X
{
	// This is a material which entities use whilst rendering and are added into the various scene managers.
	// This holds the texture names used by this material, each located inside the SCResourceManager class.
	// It also holds some other variables such as specular and ambient strength.
	// We have default texture maps stored in the resource manager which are used if these texture strings are not set.
	// They are added from the SCApplicationManager::addDefaultResources() method.
	// They are named "X:default_diffuse", "X:default_emission", "X:default_roughness" and "X:default_normal"
	// This is usefull if we do not need to use them.
	// Examples...
	// if diffuse texture name is not set, we use the default tiny greyscale texture.
	// If emission texture name is not set, we use the default tiny black texture, which equates to zero emission or turning it off.
	// If normal texture name is not set, we use the default tiny flat normal map, which equates to turning off bump mapping
	// If roughness texture name is not set, we use the default tiny black texture, which equates to medium (0.5f) roughness
	class CSMMaterial
	{
	public:
		CSMMaterial();

		// Texture names used for this material
		std::string mstrTextureNameDiffuse;		// Name of the texture in the resource manager used for this material. Diffuse colour
		std::string mstrTextureNameEmission;	// Name of the texture in the resource manager used for this material. Emission
		std::string mstrTextureNameNormalmap;	// Name of the texture in the resource manager used for this material. Normal mapping
		std::string mstrTextureNameRoughness;	// Name of the texture in the resource manager used for this material. Roughness

		float mfSpecularStrength;
		float mfAmbientStrength;

	};
}
