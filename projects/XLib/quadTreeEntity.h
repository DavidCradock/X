#pragma once
#include "PCH.h"
#include "vector2f.h"
#include "colour.h"

namespace X
{
	class CQuadTreeNode;

	// An entity which is assigned into a CQuadTreeNode
	// It contains it's unique name, it's position within the world and the node it belongs to.
	class CQuadTreeEntity
	{
		friend class CQuadTree;
		friend class CQuadTreeNode;
	public:
		// Constructor.
		// strName is the unique name given to this entity.
		// iPosX and iPosX are this entity's position within the world
		CQuadTreeEntity(const std::string& strName, int iPosX, int iPosY, CQuadTreeNode* pNodeOwner, int iUserData = 0, void *pUserData = 0);

		// Set the debug colour of the entity
		void debugSetColour(CColour& colour);

		// Returns position of entity
		CVector2f getPosition(void);

		// Returns name of the entity
		std::string getName(void);

		// Below are members which may be set to store various information.
		// They have nothing to do with the quad tree itself.
		int miUserData;
		void* mpUserData;

	private:
		std::string _mstrName;			// Unique name of this entity
		int _miPosX;					// Position of this entity along X axis
		int _miPosY;					// Position of this entity along Y axis
		CQuadTreeNode* _mpNodeOwner;	// The node this entity is in
		CColour _mColDebug;				// The colour used when debug rendering this entity
	};
}
