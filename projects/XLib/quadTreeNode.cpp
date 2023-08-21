#include "PCH.h"
#include "quadTreeNode.h"
#include "logging.h"
#include "quadTree.h"

namespace X
{
	CQuadTreeNode::CQuadTreeNode(const CRect& rectRegion, CQuadTreeNode* pParentNode, CQuadTree* pQuadTree)
	{
		_mRectRegion = rectRegion;
		_mpParentNode = pParentNode;
		_mpQuadTree = pQuadTree;
		// Is this the root node?
		if (!pParentNode)
			_muiNodeDepth = 0;
		else
			_muiNodeDepth = pParentNode->_muiNodeDepth + 1;

		if (_muiNodeDepth > pQuadTree->_muiCurrentMaxNodeDepth)
			pQuadTree->_muiCurrentMaxNodeDepth = _muiNodeDepth;

		// No children yet
		for (int i = 0; i < 4; i++)
			_mpChildNode[i] = 0;
	}

	CQuadTreeNode::~CQuadTreeNode()
	{
		// Destroy each child node
		// This will recursively call each child node's destructor
		for (int i = 0; i < 4; i++)
		{
			// If child node exists
			if (_mpChildNode[i])
				delete _mpChildNode[i];
		}
	}

	void CQuadTreeNode::debugRenderNodes(CResourceVertexBufferLine* pLine, CResourceVertexBufferLine::Vertex& vertex, CColour colour) const
	{
		// Compute colour
		float fDepthCol = (float)_mpQuadTree->_muiMaxNodeDepth - float(_muiNodeDepth);
		fDepthCol /= (float)_mpQuadTree->_muiMaxNodeDepth;
		clamp(fDepthCol, 0.1f, 1.0f);
		vertex.colour = colour;
		vertex.colour.alpha = fDepthCol;

		// Render this node's four edges
		// Left
		vertex.position.set((float)_mRectRegion.miMinX, (float)_mRectRegion.miMinY, 0.0f);	pLine->addLinePoint(vertex);
		vertex.position.set((float)_mRectRegion.miMinX, (float)_mRectRegion.miMaxY, 0.0f);	pLine->addLinePoint(vertex);
		// Right
		vertex.position.set((float)_mRectRegion.miMaxX, (float)_mRectRegion.miMinY, 0.0f);	pLine->addLinePoint(vertex);
		vertex.position.set((float)_mRectRegion.miMaxX, (float)_mRectRegion.miMaxY, 0.0f);	pLine->addLinePoint(vertex);
		// Top
		vertex.position.set((float)_mRectRegion.miMinX, (float)_mRectRegion.miMaxY, 0.0f);	pLine->addLinePoint(vertex);
		vertex.position.set((float)_mRectRegion.miMaxX, (float)_mRectRegion.miMaxY, 0.0f);	pLine->addLinePoint(vertex);
		// Bottom
		vertex.position.set((float)_mRectRegion.miMinX, (float)_mRectRegion.miMinY, 0.0f);	pLine->addLinePoint(vertex);
		vertex.position.set((float)_mRectRegion.miMaxX, (float)_mRectRegion.miMinY, 0.0f);	pLine->addLinePoint(vertex);

		// Render child nodes if they exist
		if (_mpChildNode[ChildNode::BL])
		{
			colour.set(0, 0, 1, 1);
			_mpChildNode[ChildNode::BL]->debugRenderNodes(pLine, vertex, colour);
		}
		if (_mpChildNode[ChildNode::BR])
		{
			colour.set(1, 1, 0, 1);
			_mpChildNode[ChildNode::BR]->debugRenderNodes(pLine, vertex, colour);
		}
		if (_mpChildNode[ChildNode::TL])
		{
			colour.set(1, 0, 0, 1);
			_mpChildNode[ChildNode::TL]->debugRenderNodes(pLine, vertex, colour);
		}
		if (_mpChildNode[ChildNode::TR])
		{
			colour.set(0, 1, 0, 1);
			_mpChildNode[ChildNode::TR]->debugRenderNodes(pLine, vertex, colour);
		}
	}

	void CQuadTreeNode::debugRenderEntities(CResourceVertexBufferLine* pLine, int iCircleRadius, unsigned int uiCircleNumSegments) const
	{
		// For each entity in this node
		std::map<std::string, CQuadTreeEntity*>::iterator it = _mmapEntities.begin();
		while (it != _mmapEntities.end())
		{
			pLine->addCircle(it->second->_miPosX, it->second->_miPosY, (float)iCircleRadius, uiCircleNumSegments, it->second->_mColDebug);
			it++;
		}

		// Render child nodes if they exist
		for (int i = 0; i < 4; i++)
		{
			if (_mpChildNode[i])
				_mpChildNode[i]->debugRenderEntities(pLine, iCircleRadius, uiCircleNumSegments);
		}
	}

	bool CQuadTreeNode::hasChildNode(ChildNode childNode) const
	{
		return _mpChildNode[childNode];
	}

	bool CQuadTreeNode::hasAnyChildNodes(void) const
	{
		for (int i = 0; i < 4; i++)
		{
			if (_mpChildNode[i])
				return true;
		}
		return false;
	}

	bool CQuadTreeNode::hasEntitiesInThisAndAllChildren(void) const
	{
		// If this node has entities in
		if (_mmapEntities.size() > 0)
			return true;

		// Now recursively go through any existing child nodes
		for (int i = 0; i < 4; i++)
		{
			// If node exists
			if (_mpChildNode[i])
			{
				if (_mpChildNode[i]->hasEntitiesInThisAndAllChildren())
					return true;
			}
		}

		// If we get here, this node has no entities and neither does any of it's children nodes
		return false;
	}

	void CQuadTreeNode::createChildNode(ChildNode childNode)
	{
		if (hasChildNode(childNode))
			return;

		// Compute rect area of the new child node
		CRect childNodeRect = computeChildNodeRegion(childNode);

		_mpChildNode[childNode] = new CQuadTreeNode(childNodeRect, this, _mpQuadTree);
		ThrowIfFalse(_mpChildNode[childNode], "CQuadTreeNode::createChildNode() failed. Unable to allocate memory for the new child node.");
	}

	CRect CQuadTreeNode::computeChildNodeRegion(ChildNode childNode) const
	{
		// Compute dimensions of new child node using the rect region of this parent node.
		int iChildDimsX = _mRectRegion.miMaxX - _mRectRegion.miMinX;
		int iChildDimsY = _mRectRegion.miMaxY - _mRectRegion.miMinY;
		iChildDimsX /= 2;
		iChildDimsY /= 2;

		// Calculate region of the child node
		CRect childNodeRegion;
		switch (childNode)
		{
		case ChildNode::BL:
			childNodeRegion.miMinX = _mRectRegion.miMinX;
			childNodeRegion.miMaxX = _mRectRegion.miMinX + iChildDimsX;
			childNodeRegion.miMinY = _mRectRegion.miMinY;
			childNodeRegion.miMaxY = _mRectRegion.miMinY + iChildDimsY;
			break;
		case ChildNode::BR:
			childNodeRegion.miMinX = _mRectRegion.miMinX + iChildDimsX;
			childNodeRegion.miMaxX = _mRectRegion.miMaxX;
			childNodeRegion.miMinY = _mRectRegion.miMinY;
			childNodeRegion.miMaxY = _mRectRegion.miMinY + iChildDimsY;
			break;
		case ChildNode::TL:
			childNodeRegion.miMinX = _mRectRegion.miMinX;
			childNodeRegion.miMaxX = _mRectRegion.miMinX + iChildDimsX;
			childNodeRegion.miMinY = _mRectRegion.miMinY + iChildDimsY;
			childNodeRegion.miMaxY = _mRectRegion.miMaxY;
			break;
		case ChildNode::TR:
			childNodeRegion.miMinX = _mRectRegion.miMinX + iChildDimsX;
			childNodeRegion.miMaxX = _mRectRegion.miMaxX;
			childNodeRegion.miMinY = _mRectRegion.miMinY + iChildDimsY;
			childNodeRegion.miMaxY = _mRectRegion.miMaxY;
			break;
		default:
			ThrowIfTrue(1, "CQuadTreeNode::computeChildNodeRegion() given invalid ChildNode to compute it's region.");
		}
		return childNodeRegion;
	}

	void CQuadTreeNode::addEntity(CQuadTreeEntity* pEntity)
	{
		// If this node has no children, attempt to add the entity to this node
		if (!hasAnyChildNodes())
		{
			// We haven't reached max capacity for this node
			// OR we've reached maximum node depth with this node
			if (_mmapEntities.size() < _mpQuadTree->_miMaxEntitiesPerNode ||
				_muiNodeDepth == _mpQuadTree->_muiMaxNodeDepth)
			{
				// Add the entity to this node
				// No need to check if the new entity name already exists, as CQuadTree::addEntity() has already checked
				_mmapEntities[pEntity->_mstrName] = pEntity;
				pEntity->_mpNodeOwner = this;	// Set node owner for the entity
				return;
			}
			// If we've reached maximum capacity for this node and max node depth hasn't been reached
			if (_mmapEntities.size() == _mpQuadTree->_miMaxEntitiesPerNode)
			{
				// We need to create child node/s then move all the entities from this node into the children, 
				// as well as the new entity

				// Compute potential child node rects
				CRect rctBL = computeChildNodeRegion(ChildNode::BL);
				CRect rctBR = computeChildNodeRegion(ChildNode::BR);
				CRect rctTL = computeChildNodeRegion(ChildNode::TL);
				CRect rctTR = computeChildNodeRegion(ChildNode::TR);

				// Add the new entity to this node's entity map, so it'll be moved to the correct child node below
				_mmapEntities[pEntity->_mstrName] = pEntity;
				pEntity->_mpNodeOwner = this;	// Set node owner for the entity

				// Move all the entities from this node, into the child nodes
				std::map<std::string, CQuadTreeEntity*>::iterator it = _mmapEntities.begin();
				while (it != _mmapEntities.end())
				{
					// Determine which child node the entity fits in, regardless of whether the child node exists or not
					ChildNode childNode = ChildNode::NONE;
					if (rctBL.doesPositionFitWithin(it->second->_miPosX, it->second->_miPosY))
						childNode = ChildNode::BL;
					else if (rctBR.doesPositionFitWithin(it->second->_miPosX, it->second->_miPosY))
						childNode = ChildNode::BR;
					else if (rctTL.doesPositionFitWithin(it->second->_miPosX, it->second->_miPosY))
						childNode = ChildNode::TL;
					else if (rctTR.doesPositionFitWithin(it->second->_miPosX, it->second->_miPosY))
						childNode = ChildNode::TR;

					// Error checking, making sure the entity could fit in one of the four possible children
					ThrowIfTrue(ChildNode::NONE == childNode, "CQuadTreeNode::addEntity() failed when trying to add entity " + it->second->_mstrName + " to any of the four child nodes as it's position doesn't fit inside any of them.");

					// Create the child node if it doesn't exist
					if (!_mpChildNode[childNode])
					{
						_mpChildNode[childNode] = new CQuadTreeNode(computeChildNodeRegion(childNode), this, _mpQuadTree);
						ThrowIfFalse(_mpChildNode[childNode], "CQuadTreeNode::addEntity() failed when trying to add entity " + it->second->_mstrName + " to any of the four child nodes as it was unable to allocate memory for the child node.");
					}
					_mpChildNode[childNode]->addEntity(it->second);
					it++;
				}
				// Remove all entities from this node as they are now stored in the children
				_mmapEntities.clear();
				return;
			}
		}

		// If we get here, then this node has children, so add the new entity to one of those...
		// Determine which child node the entity fits in
		// Compute potential child node rects
		CRect rctBL = computeChildNodeRegion(ChildNode::BL);
		CRect rctBR = computeChildNodeRegion(ChildNode::BR);
		CRect rctTL = computeChildNodeRegion(ChildNode::TL);
		CRect rctTR = computeChildNodeRegion(ChildNode::TR);

		ChildNode childNode = ChildNode::NONE;
		if (rctBL.doesPositionFitWithin(pEntity->_miPosX, pEntity->_miPosY))
			childNode = ChildNode::BL;
		else if (rctBR.doesPositionFitWithin(pEntity->_miPosX, pEntity->_miPosY))
			childNode = ChildNode::BR;
		else if (rctTL.doesPositionFitWithin(pEntity->_miPosX, pEntity->_miPosY))
			childNode = ChildNode::TL;
		else if (rctTR.doesPositionFitWithin(pEntity->_miPosX, pEntity->_miPosY))
			childNode = ChildNode::TR;
		// Error checking, making sure the entity could fit in one of the four possible children
		ThrowIfTrue(ChildNode::NONE == childNode, "CQuadTreeNode::addEntity() failed when trying to add entity " + pEntity->_mstrName + " to any of the four child nodes as it's position doesn't fit inside any of them.");

		// Create the child node if it doesn't exist
		if (!_mpChildNode[childNode])
		{
			_mpChildNode[childNode] = new CQuadTreeNode(computeChildNodeRegion(childNode), this, _mpQuadTree);
			ThrowIfFalse(_mpChildNode[childNode], "CQuadTreeNode::addEntity() failed when trying to add entity " + pEntity->_mstrName + " to any of the four child nodes as it was unable to allocate memory for the child node.");
		}

		// Add the new entity
		_mpChildNode[childNode]->addEntity(pEntity);
	}

	void CQuadTreeNode::removeEntity(CQuadTreeEntity* pEntity)
	{
		// Attempt to find the entity
		std::map<std::string, CQuadTreeEntity*>::iterator it = _mmapEntities.find(pEntity->_mstrName);
		ThrowIfTrue(it == _mmapEntities.end(), "CQuadTreeNode::removeEntity() failed. The entity named " + pEntity->_mstrName + " could not be found");
		_mmapEntities.erase(it);
		// No need to delete entity, the CQuadTree::removeAllEntities() or CQuadTree::removeEntity() does this
	}

	void CQuadTreeNode::getNodesWithEntities(std::vector<CQuadTreeNode*>& vecNodes)
	{
		// If this node doesn't have any children, just check this node
		if (!hasAnyChildNodes())
		{
			// If this node doesn't have any entities
			if (0 == _mmapEntities.size())
				return;

			// Add this node
			vecNodes.push_back(this);
			return;
		}
		else  // This node has children, check those
		{
			for (int i = 0; i < 4; i++)
			{
				if (_mpChildNode[i])
					_mpChildNode[i]->getNodesWithEntities(vecNodes);
			}
		}
	}

	void CQuadTreeNode::getNodesWithEntitiesWhichIntersect(std::vector<CQuadTreeNode*>& vecNodes, const CRect& rect)
	{
		// If this node doesn't have any children, just check this node
		if (!hasAnyChildNodes())
		{
			// If this node doesn't have any entities
			if (0 == _mmapEntities.size())
				return;

			// If the rect interects with this node, add this node
			if (_mRectRegion.intersects(rect))
			{
				vecNodes.push_back(this);
				return;
			}
		}
		else  // This node has children, check those
		{
			for (int i = 0; i < 4; i++)
			{
				if (_mpChildNode[i])
					_mpChildNode[i]->getNodesWithEntitiesWhichIntersect(vecNodes, rect);
			}
		}
	}

	void CQuadTreeNode::getMaxNodeDepth(unsigned int& uiMaxNodeDepth)
	{
		if (_muiNodeDepth > uiMaxNodeDepth)
			uiMaxNodeDepth = _muiNodeDepth;

		// Call this method for all children of this node
		for (int i = 0; i < 4; i++)
		{
			if (_mpChildNode[i])	// If child exists
				_mpChildNode[i]->getMaxNodeDepth(uiMaxNodeDepth);
		}
	}
}