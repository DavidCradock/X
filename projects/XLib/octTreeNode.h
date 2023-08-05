#pragma once
#include "PCH.h"
#include "AABB.h"
#include "octTreeEntity.h"
#include "resourceManager.h"
#include "frustum.h"

namespace X
{
	class COctTree;

	// A node used by the COctTree class
	class COctTreeNode
	{
		friend class COctTree;
	public:
		// Used for accessing child nodes based upon their location
		enum ChildNode
		{
			NEGX_NEGY_NEGZ,
			NEGX_POSY_NEGZ,
			NEGX_NEGY_POSZ,
			NEGX_POSY_POSZ,
			POSX_NEGY_NEGZ,
			POSX_POSY_NEGZ,
			POSX_NEGY_POSZ,
			POSX_POSY_POSZ,
			NONE
		};

		// Constructor.
		// Sets up the node to represent the given region within the 3D world, with no child nodes.
		// pParentNode is a pointer to this node's parent node. However, if this node is to represent
		// the root node, this will be 0.
		COctTreeNode(const CAABB& region, COctTreeNode* pParentNode, COctTree* pQuadTree);

		// Destructor
		// Removes all child nodes
		// Although this obviously removes the entities from the nodes, because the nodes themselves
		// no longer exist, this does NOT delete the entity pointers. They are stored in the COctTree's
		// _mmapEntities hashmap.
		~COctTreeNode();

		
		// Debug renders this node and it's child nodes', node boundaries
		// pLine is the CResourceLine object which is being used to add vertices to be rendered
		// vertex is the vertex object we're using to add vertices using the pLine object.
		void debugRenderNodes(CResourceVertexBufferLine* pLine, CColour colour) const;

		/*
		// Debug renders this node and it's child nodes' entities
		// pLine is the CResourceLine object which is being used to add vertices to be rendered
		void debugRenderEntities(CResourceLine* pLine, int iCircleRadius, unsigned int uiCircleNumSegments) const;
		*/

		// Returns true if this node has the specified child node
		bool hasChildNode(ChildNode childNode) const;

		// Returns whether this node has any child nodes
		bool hasAnyChildNodes(void) const;

		// Returns whether this node and all it's children have any entities within
		bool hasEntitiesInThisAndAllChildren(void) const;

		// Creates the specified child node
		// If the node already exists, this does nothing.
		void createChildNode(ChildNode childNode);

		// Calculates and returns a child's region.
		CAABB computeChildNodeRegion(ChildNode childNode) const;

		// Adds an entity into this node, or it's children
		void addEntity(COctTreeEntity* pEntity);

		// Removes an entity from this node
		// If the entity couldn't be found, an exception occurs
		void removeEntity(COctTreeEntity* pEntity);

		// Adds nodes to a vector of COctTreeNodes which have entities in them
		void getNodesWithEntities(std::vector<COctTreeNode*>& vecNodes);

		// Adds nodes to a vector of COctTreeNodes which intersect with the given AABB and have entities
		void getNodesWithEntitiesWhichIntersect(std::vector<COctTreeNode*>& vecNodes, const CAABB& aabb);

		// Adds nodes to a vector of COctTreeNodes which intersect with the given CFrustum and have entities
		void getNodesWithEntitiesWhichIntersect(std::vector<COctTreeNode*>& vecNodes, const CFrustum& frustum);

		// Go through all children and if their depth is greater, increases given uiMaxNodeDepth
		void getMaxNodeDepth(unsigned int& uiMaxNodeDepth);
	private:
		// Holds the region which this node covers
		// Must be a multiple of 2, otherwise child nodes' regions will not cover all space.
		CAABB _mRegion;

		// Pointer to the parent of this node. May be 0 if this is the root node
		COctTreeNode* _mpParentNode;

		// Pointer to the eight possible child nodes.
		// A pointer may be 0 for no child node allocated yet.
		// Use the ChildNode enum with this array to access the correct child node.
		COctTreeNode* _mpChildNode[8];

		// The quad tree which owns this node, this is passed to the constructor
		COctTree* _mpOctTree;

		// Depth of this node.
		// How many nodes there are above this node.
		unsigned int _muiNodeDepth;

		// Hashmap holding pointers to each of the added entities, until
		// this node has children, in which case this would be empty as 
		// the child nodes now own the entities (or their siblings)
		mutable std::map<std::string, COctTreeEntity*> _mmapEntities;
	};
}
