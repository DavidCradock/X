#pragma once
#include "PCH.h"
#include "SceneManagerEntityBase.h"

namespace X
{
	// This is an entity which is inserted into the various scene managers.
	// It's derived from the SceneManagerEntityBase class which holds
	// an entity's position, rotation and scale within the scene manager. Along with various helper methods to move and rotate the thing.
	// This entity holds the name of a vertex buffer, the name of the shader and texture names, each located inside the ResourceManager class.
	// We have default texture maps stored in the resource manager which are used if these texture strings are not set.
	// They are added from the ApplicationManager::addDefaultResources() method.
	// They are named "X:default_diffuse", "X:default_emission", "X:default_roughness" and "X:default_normal"
	// This is usefull if we do not need to use them.
	// Examples...
	// if diffuse texture name is not set, we use the default tiny greyscale texture.
	// If emission texture name is not set, we use the default tiny black texture, which equates to zero emission or turning it off.
	// If normal texture name is not set, we use the default tiny flat normal map, which equates to turning off bump mapping
	// If roughness texture name is not set, we use the default tiny black texture, which equates to medium (0.5f) roughness
	class SceneManagerEntityVertexbuffer : public SceneManagerEntityBase
	{
	public:
		SceneManagerEntityVertexbuffer();

		std::string mstrVertexbufferName;	// Name of the vertex buffer in the resource manager used to render this entity

		// Texture names used whilst rendering this entity
		std::string mstrTextureNameDiffuse;		// Name of the texture in the resource manager used to render this entity for diffuse colour
		std::string mstrTextureNameEmission;	// Name of the texture in the resource manager used to render this entity for emission
		std::string mstrTextureNameNormalmap;	// Name of the texture in the resource manager used to render this entity for normal mapping
		std::string mstrTextureNameRoughness;	// Name of the texture in the resource manager used to render this entity for roughness
		
	};
}