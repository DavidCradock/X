#include "PCH.h"
#include "SMMaterial.h"

namespace X
{
	CSMMaterial::CSMMaterial()
	{
		_mstrTextureNameDiffuse = "X:default_diffuse";
		_mstrTextureNameEmission = "X:default_emission";
		_mstrTextureNameNormalmap = "X:default_normal";
		_mstrTextureNameRoughness = "X:default_roughness";
		_mfSpecularStrength = 1.0f;
		_mfAmbientStrength = 0.05f;
	}

	void CSMMaterial::setAmbientStrength(float fAmbientStrength)
	{
		_mfAmbientStrength = fAmbientStrength;
	}

	float CSMMaterial::getAmbientStrength(void) const
	{
		return _mfAmbientStrength;
	}

	void CSMMaterial::setSpecularStrength(float fSpecularStrength)
	{
		_mfSpecularStrength = fSpecularStrength;
	}

	float CSMMaterial::getSpecularStrength(void) const
	{
		return _mfSpecularStrength;
	}

	void CSMMaterial::setDiffuseTextureName(const std::string& strTextureName)
	{
		_mstrTextureNameDiffuse = strTextureName;
	}

	std::string CSMMaterial::getDiffuseTextureName(void) const
	{
		return _mstrTextureNameDiffuse;
	}

	void CSMMaterial::setEmissionTextureName(const std::string& strTextureName)
	{
		_mstrTextureNameEmission = strTextureName;
	}

	std::string CSMMaterial::getEmissionTextureName(void) const
	{
		return _mstrTextureNameEmission;
	}

	void CSMMaterial::setNormalmapTextureName(const std::string& strTextureName)
	{
		_mstrTextureNameNormalmap = strTextureName;
	}

	std::string CSMMaterial::getNormalmapTextureName(void) const
	{
		return _mstrTextureNameNormalmap;
	}

	void CSMMaterial::setRoughnessTextureName(const std::string& strTextureName)
	{
		_mstrTextureNameRoughness = strTextureName;
	}

	std::string CSMMaterial::getRoughnessTextureName(void) const
	{
		return _mstrTextureNameRoughness;
	}
}