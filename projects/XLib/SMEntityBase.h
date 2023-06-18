#pragma once
#include "PCH.h"

namespace X
{
	// This is an object which is inserted into the various scene managers
	// This is a base class which we derive other classes from, we don't use these directly.
	// This is because all entitys have their position, rotation and scale, but differ by what they render (meshes, particles etc)
	class SMEntityBase
	{
	public:
		SMEntityBase();
		
		// Returns the world matrix and updates it if needed
		glm::mat4 getWorldMatrix(void);

		// Sets this entities position in world space
		void setWorldPosition(glm::vec3 vNewPosition);

		// Moves the entity by the specified amounts in world space from it's current position
		void translateWorld(glm::vec3 vTranslation);

		// Gets currently set position in world space
		glm::vec3 getWorldPosition(void);

		// Set scale of this entity
		void setScale(glm::vec3 vScale);

		// Returns entity's currently set scale
		glm::vec3 getScale(void);

		// Temp rotation
		void setRotation(float fX, float fY, float fZ);
	private:
		// Updates the world matrix ragardless of _mbWorldNeedsUpdate value
		void _updateWorldMatrix(void);

		bool _mbWorldNeedsUpdate;	// Whether the world matrix of this entity needs updating
		glm::mat4 matrixWorld;		// Holds the world transform of this entity
		glm::vec3 _mvPosition;		// Position of this entity in worldspace
		glm::vec3 _mvScale;			// Scale of this entity
		glm::vec3 _mvRotTEMP;	// temporary rotation
	};
}