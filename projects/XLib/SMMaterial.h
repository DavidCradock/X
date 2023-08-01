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
	// If roughness texture name is not set, we use the default tiny black texture, which equates to no roughness
	class CSMMaterial
	{
	public:
		CSMMaterial();

		// Sets the ambient strength of this material.
		void setAmbientStrength(float fAmbientStrength = 0.05f);
		
		// Gets the ambient strength of this material.
		float getAmbientStrength(void) const;

		// Sets the specular strength of this material.
		void setSpecularStrength(float fSpecularStrength = 1.0f);

		// Gets the specular strength of this material.
		float getSpecularStrength(void) const;

		// Sets the name of the texture in the resource manager used by this material for diffuse colour.
		void setDiffuseTextureName(const std::string& strTextureName = "X:default_diffuse");

		// Gets the name of the texture in the resource manager used by this material for diffuse colour.
		std::string getDiffuseTextureName(void) const;

		// Sets the name of the texture in the resource manager used by this material for emission colour.
		void setEmissionTextureName(const std::string& strTextureName = "X:default_emission");

		// Gets the name of the texture in the resource manager used by this material for emission colour.
		std::string getEmissionTextureName(void) const;

		// Sets the name of the texture in the resource manager used by this material for normal map.
		void setNormalmapTextureName(const std::string& strTextureName = "X:default_normal");

		// Gets the name of the texture in the resource manager used by this material for normal map.
		std::string getNormalmapTextureName(void) const;

		// Sets the name of the texture in the resource manager used by this material for roughness.
		void setRoughnessTextureName(const std::string& strTextureName = "X:default_roughness");

		// Gets the name of the texture in the resource manager used by this material for roughness.
		std::string getRoughnessTextureName(void) const;

	private:
		// Texture names used for this material
		std::string _mstrTextureNameDiffuse;	// Name of the texture in the resource manager used for this material. Diffuse colour
		std::string _mstrTextureNameEmission;	// Name of the texture in the resource manager used for this material. Emission
		std::string _mstrTextureNameNormalmap;	// Name of the texture in the resource manager used for this material. Normal mapping
		std::string _mstrTextureNameRoughness;	// Name of the texture in the resource manager used for this material. Roughness

		float _mfSpecularStrength;
		float _mfAmbientStrength;

	};
}
