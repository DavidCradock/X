#include "PCH.h"
#include "SceneManagerEntityVertexbuffer.h"

namespace X
{
	SceneManagerEntityVertexbuffer::SceneManagerEntityVertexbuffer()
	{
		mstrTextureNameDiffuse = "X:default_diffuse";
		mstrTextureNameEmission = "X:default_emission";
		mstrTextureNameNormalmap = "X:default_normal";
		mstrTextureNameRoughness = "X:default_roughness";
		mfSpecularStrength = 0.5f;
	}
}