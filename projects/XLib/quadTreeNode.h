#pragma once
#include "PCH.h"
#include "rect.h"
#include "quadTreeEntity.h"
#include "resourceManager.h"

namespace X
{
	class CQuadTree;

	// A node used by the CQuadTree class
	class CQuadTreeNode
	{
		friend class CQuadTree;
	public:
		// Used for accessing child nodes based upon their location
		enum ChildNode
		{
			BL,	// Bottom left child node
			BR,	// Bottom right child node
			TL,	// Top left child node
			TR,	// Top right child node
			NONE
		};

		// Constructor.
		// Sets up the node to represent the given region within the 2D world, with no child nodes.
		// pParentNode is a pointer to this node's parent node. However, if this node is to represent
		// the root node, this will be 0.
		CQuadTreeNode(const CRect& rectRegion, CQuadTreeNode* pParentNode, CQuadTree* pQuadTree);

		// Destructor
		// Removes all child nodes
		// Although this obviously removes the entities from the nodes, because the nodes themselves
		// no longer exist, this does NOT delete the entity pointers. They are stored in the CQuadTree's
		// _mmapEntities hashmap.
		~CQuadTreeNode();

		// Debug renders this node and it's child nodes', node boundaries
		// pLine is the CResourceLine object which is being used to add vertices to be rendered
		// vertex is the vertex object we're using to add vertices using the pLine object.
		void debugRenderNodes(CResourceVertexBufferLine* pLine, CResourceVertexBufferLine::Vertex& vertex, CColour colour) const;

		// Debug renders this node and it's child nodes' entities
		// pLine is the CResourceLine object which is being used to add vertices to be rendered
		void debugRenderEntities(CResourceVertexBufferLine* pLine, int iCircleRadius, unsigned int uiCircleNumSegments) const;

		// Returns true if this node has the specified child node
		bool hasChildNode(ChildNode childNode) const;

		// Returns whether this node has any child nodes
		bool hasAnyChildNodes(void) const;

		// Returns whether this node and all it's children have any entities within
		bool hasEntitiesInThisAndAllChildren(void) const;

		// Creates the specified child node
		// If the node already exists, this does nothing.
		void createChildNode(ChildNode childNode);

		// Calculates and returns a child's rect region.
		CRect computeChildNodeRegion(ChildNode childNode) const;

		// Adds an entity into this node, or it's children
		void addEntity(CQuadTreeEntity* pEntity);

		// Removes an entity from this node
		// If the entity couldn't be found, an exception occurs
		void removeEntity(CQuadTreeEntity* pEntity);

		// Adds nodes to a vector of CQuadTreeNodes which have entities in them
		void getNodesWithEntities(std::vector<CQuadTreeNode*>& vecNodes);

		// Adds nodes to a vector of CQuadTreeNodes which intersect with the given rect and have entities
		void getNodesWithEntitiesWhichIntersect(std::vector<CQuadTreeNode*>& vecNodes, const CRect& rect);

		// Go through all children and if their depth is greater, increases given uiMaxNodeDepth
		void getMaxNodeDepth(unsigned int& uiMaxNodeDepth);
	private:
		// Holds the rectangular region which this node covers
		// Must be a multiple of 2, otherwise child nodes' regions will not cover all space.
		CRect _mRectRegion;

		// Pointer to the parent of this node. May be 0 if this is the root node
		CQuadTreeNode* _mpParentNode;

		// Pointer to the four possible child nodes.
		// A pointer may be 0 for no child node allocated yet.
		// Use the ChildNode enum with this array to access the correct child node.
		CQuadTreeNode* _mpChildNode[4];

		// The quad tree which owns this node, this is passed to the constructor
		CQuadTree* _mpQuadTree;

		// Depth of this node.
		// How many nodes there are above this node.
		unsigned int _muiNodeDepth;

		// Hashmap holding pointers to each of the added entities, until
		// this node has children, in which case this would be empty as 
		// the child nodes now own the entities (or their siblings)
		mutable std::map<std::string, CQuadTreeEntity*> _mmapEntities;
	};
}
