#pragma once
#include "PCH.h"

namespace X
{
	// This is an object which holds the position of the entity within the world is is assigned to, a rotation, non-uniform scale, the CResourceTexture2DAtlas used to 
	// render itself as well as it's current animation frame (if it is animated).
	// This holds multiple entities which have a parent/child relationship.
	// This is so that we can create for example, a spaceship parent object with the image of a spaceship and then we can add child entities
	// (for example, guns) with their position offset from the parent's origin so that when rotating the parent entity, the child entities
	// translate, rotate and scale along with their parent entity.
	// These also have layers, so we can control the order of rendering.
	class C2DEntityComplex
	{
	public:
		friend class SC2DRenderer;
		// strResourceTexture2DAtlasName is the name of the CResourceTexture2DAtlas added to SCResourceManager which contains this entity's image data.
		C2DEntityComplex(const std::string& strResourceTexture2DAtlasName);

		~C2DEntityComplex();

	private:
		std::string _mstrResourceTexture2DAtlasName;	// Name of the CResourceTexture2DAtlas added to SCResourceManager which holds this entity's image data
	};
}