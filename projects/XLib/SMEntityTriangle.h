#pragma once
#include "PCH.h"
#include "SMEntityBase.h"

namespace X
{
	// This is an entity which is inserted into the various scene managers.
	// It's derived from the SceneManagerEntityBase class which holds
	// an entity's position, rotation and scale within the scene manager. Along with various helper methods to move and rotate the thing.
	// This entity holds the name of a resource triangle located inside the SCResourceManager class.
	// It also holds the material name (which is added via the scene manager) used to render this entity
	class CSMEntityTriangle : public CSMEntityBase
	{
	public:
		CSMEntityTriangle();

		std::string mstrTriangleName;		// Name of the triangle resource in the resource manager used to render this entity
		std::string mstrMaterialName;		// Name of the material added to the scene manager which this entity uses
		
	};
}