#include "PCH.h"
#include "octTreeNode.h"
#include "log.h"
#include "octTree.h"

namespace X
{
	COctTreeNode::COctTreeNode(const CAABB& region, COctTreeNode* pParentNode, COctTree* pQuadTree)
	{
		_mRegion = region;
		_mpParentNode = pParentNode;
		_mpOctTree = pQuadTree;
		// Is this the root node?
		if (!pParentNode)
			_muiNodeDepth = 0;
		else
			_muiNodeDepth = pParentNode->_muiNodeDepth + 1;

		if (_muiNodeDepth > pQuadTree->_muiCurrentMaxNodeDepth)
			pQuadTree->_muiCurrentMaxNodeDepth = _muiNodeDepth;

		// No children yet
		for (int i = 0; i < 8; i++)
			_mpChildNode[i] = 0;
	}

	COctTreeNode::~COctTreeNode()
	{
		// Destroy each child node
		// This will recursively call each child node's destructor
		for (int i = 0; i < 8; i++)
		{
			// If child node exists
			if (_mpChildNode[i])
				delete _mpChildNode[i];
		}
	}


	void COctTreeNode::debugRenderNodes(CResourceLine* pLine, CColour colour) const
	{
		// Compute colour
		float fDepthCol = (float)_mpOctTree->_muiMaxNodeDepth - float(_muiNodeDepth);
		fDepthCol /= (float)_mpOctTree->_muiMaxNodeDepth;
		clamp(fDepthCol, 0.75f, 1.0f);

		// Compute vertex positions for the AABB
		CResourceLine::Vertex v[8];
		for (int i = 0; i < 8; i++)
		{
			v[i].colour.red = colour.red;
			v[i].colour.green = colour.green;
			v[i].colour.blue = colour.blue;
			v[i].colour.alpha = fDepthCol;
		}
		v[ChildNode::NEGX_NEGY_NEGZ].position = _mRegion.getMin();
		v[ChildNode::POSX_POSY_POSZ].position = _mRegion.getMax();
		v[ChildNode::NEGX_NEGY_POSZ].position.set(v[ChildNode::NEGX_NEGY_NEGZ].position.x, v[ChildNode::NEGX_NEGY_NEGZ].position.y, v[ChildNode::POSX_POSY_POSZ].position.z);
		v[ChildNode::NEGX_POSY_NEGZ].position.set(v[ChildNode::NEGX_NEGY_NEGZ].position.x, v[ChildNode::POSX_POSY_POSZ].position.y, v[ChildNode::NEGX_NEGY_NEGZ].position.z);
		v[ChildNode::NEGX_POSY_POSZ].position.set(v[ChildNode::NEGX_NEGY_NEGZ].position.x, v[ChildNode::POSX_POSY_POSZ].position.y, v[ChildNode::POSX_POSY_POSZ].position.z);
		v[ChildNode::POSX_NEGY_NEGZ].position.set(v[ChildNode::POSX_POSY_POSZ].position.x, v[ChildNode::NEGX_NEGY_NEGZ].position.y, v[ChildNode::NEGX_NEGY_NEGZ].position.z);
		v[ChildNode::POSX_NEGY_POSZ].position.set(v[ChildNode::POSX_POSY_POSZ].position.x, v[ChildNode::NEGX_NEGY_NEGZ].position.y, v[ChildNode::POSX_POSY_POSZ].position.z);
		v[ChildNode::POSX_POSY_NEGZ].position.set(v[ChildNode::POSX_POSY_POSZ].position.x, v[ChildNode::POSX_POSY_POSZ].position.y, v[ChildNode::NEGX_NEGY_NEGZ].position.z);

		// Render this node's twelve edges
		pLine->addLinePoint(v[NEGX_NEGY_NEGZ]);	pLine->addLinePoint(v[POSX_NEGY_NEGZ]);		// Back, bottom, left to right
		pLine->addLinePoint(v[NEGX_POSY_NEGZ]);	pLine->addLinePoint(v[POSX_POSY_NEGZ]);		// Back, top, left to right
		pLine->addLinePoint(v[NEGX_NEGY_POSZ]);	pLine->addLinePoint(v[POSX_NEGY_POSZ]);		// Front, bottom, left to right
		pLine->addLinePoint(v[NEGX_POSY_POSZ]);	pLine->addLinePoint(v[POSX_POSY_POSZ]);		// Front, top, left to right
		pLine->addLinePoint(v[NEGX_POSY_NEGZ]);	pLine->addLinePoint(v[NEGX_NEGY_NEGZ]);		// Back, left, top to bottom
		pLine->addLinePoint(v[POSX_POSY_NEGZ]);	pLine->addLinePoint(v[POSX_NEGY_NEGZ]);		// Back, right, top to bottom
		pLine->addLinePoint(v[NEGX_POSY_POSZ]);	pLine->addLinePoint(v[NEGX_NEGY_POSZ]);		// Front, left, top to bottom
		pLine->addLinePoint(v[POSX_POSY_POSZ]);	pLine->addLinePoint(v[POSX_NEGY_POSZ]);		// Front, right, top to bottom
		pLine->addLinePoint(v[NEGX_POSY_POSZ]);	pLine->addLinePoint(v[NEGX_POSY_NEGZ]);		// Top, left, front to back
		pLine->addLinePoint(v[POSX_POSY_POSZ]);	pLine->addLinePoint(v[POSX_POSY_NEGZ]);		// Top, right, front to back
		pLine->addLinePoint(v[NEGX_NEGY_POSZ]);	pLine->addLinePoint(v[NEGX_NEGY_NEGZ]);		// Bottom, left, front to back
		pLine->addLinePoint(v[POSX_NEGY_POSZ]);	pLine->addLinePoint(v[POSX_NEGY_NEGZ]);		// Bottom, right, front to back

		// Render child nodes if they exist
		for (int i = 0; i < 8; i++)
		{
			if (_mpChildNode[i])
				_mpChildNode[i]->debugRenderNodes(pLine, colour);
		}
	}

	/*
	void COctTreeNode::debugRenderEntities(CResourceLine* pLine, int iCircleRadius, unsigned int uiCircleNumSegments) const
	{
		// For each entity in this node
		std::map<std::string, COctTreeEntity*>::iterator it = _mmapEntities.begin();
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
	*/

	bool COctTreeNode::hasChildNode(ChildNode childNode) const
	{
		return _mpChildNode[childNode];
	}

	bool COctTreeNode::hasAnyChildNodes(void) const
	{
		for (int i = 0; i < 8; i++)
		{
			if (_mpChildNode[i])
				return true;
		}
		return false;
	}

	bool COctTreeNode::hasEntitiesInThisAndAllChildren(void) const
	{
		// If this node has entities in
		if (_mmapEntities.size() > 0)
			return true;

		// Now recursively go through any existing child nodes
		for (int i = 0; i < 8; i++)
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

	void COctTreeNode::createChildNode(ChildNode childNode)
	{
		if (hasChildNode(childNode))
			return;

		// Compute region of the new child node
		CAABB childNodeRegion = computeChildNodeRegion(childNode);

		_mpChildNode[childNode] = new COctTreeNode(childNodeRegion, this, _mpOctTree);
		ThrowIfFalse(_mpChildNode[childNode], "COctTreeNode::createChildNode() failed. Unable to allocate memory for the new child node.");
	}

	CAABB COctTreeNode::computeChildNodeRegion(ChildNode childNode) const
	{
		// Compute dimensions of new child node using the region of this parent node.
		CVector3f vChildDims = _mRegion.getHalfDimensions();
		CVector3f vParentPosition = _mRegion.getPosition();
		CVector3f vHalfChildDims = vChildDims * 0.5f;

		// Calculate region of the child node
		CAABB childNodeRegion;
		CVector3f vChildPosition = vParentPosition;
		switch (childNode)
		{
		case ChildNode::NEGX_NEGY_NEGZ:
			vChildPosition.x -= vHalfChildDims.x;
			vChildPosition.y -= vHalfChildDims.y;
			vChildPosition.z -= vHalfChildDims.z;
			childNodeRegion.setPosDims(vChildPosition, vChildDims);
			break;
		case ChildNode::NEGX_POSY_NEGZ:
			vChildPosition.x -= vHalfChildDims.x;
			vChildPosition.y += vHalfChildDims.y;
			vChildPosition.z -= vHalfChildDims.z;
			childNodeRegion.setPosDims(vChildPosition, vChildDims);
			break;
		case ChildNode::NEGX_NEGY_POSZ:
			vChildPosition.x -= vHalfChildDims.x;
			vChildPosition.y -= vHalfChildDims.y;
			vChildPosition.z += vHalfChildDims.z;
			childNodeRegion.setPosDims(vChildPosition, vChildDims);
			break;
		case ChildNode::NEGX_POSY_POSZ:
			vChildPosition.x -= vHalfChildDims.x;
			vChildPosition.y += vHalfChildDims.y;
			vChildPosition.z += vHalfChildDims.z;
			childNodeRegion.setPosDims(vChildPosition, vChildDims);
			break;
		case ChildNode::POSX_NEGY_NEGZ:
			vChildPosition.x += vHalfChildDims.x;
			vChildPosition.y -= vHalfChildDims.y;
			vChildPosition.z -= vHalfChildDims.z;
			childNodeRegion.setPosDims(vChildPosition, vChildDims);
			break;
		case ChildNode::POSX_POSY_NEGZ:
			vChildPosition.x += vHalfChildDims.x;
			vChildPosition.y += vHalfChildDims.y;
			vChildPosition.z -= vHalfChildDims.z;
			childNodeRegion.setPosDims(vChildPosition, vChildDims);
			break;
		case ChildNode::POSX_NEGY_POSZ:
			vChildPosition.x += vHalfChildDims.x;
			vChildPosition.y -= vHalfChildDims.y;
			vChildPosition.z += vHalfChildDims.z;
			childNodeRegion.setPosDims(vChildPosition, vChildDims);
			break;
		case ChildNode::POSX_POSY_POSZ:
			vChildPosition.x += vHalfChildDims.x;
			vChildPosition.y += vHalfChildDims.y;
			vChildPosition.z += vHalfChildDims.z;
			childNodeRegion.setPosDims(vChildPosition, vChildDims);
			break;
		default:
			ThrowIfTrue(1, "COctTreeNode::computeChildNodeRegion() given invalid ChildNode to compute it's region.");
		}
		return childNodeRegion;
	}

	void COctTreeNode::addEntity(COctTreeEntity* pEntity)
	{
		// If this node has no children, attempt to add the entity to this node
		if (!hasAnyChildNodes())
		{
			// We haven't reached max capacity for this node
			// OR we've reached maximum node depth with this node
			if (_mmapEntities.size() < _mpOctTree->_miMaxEntitiesPerNode ||
				_muiNodeDepth == _mpOctTree->_muiMaxNodeDepth)
			{
				// Add the entity to this node
				// No need to check if the new entity name already exists, as COctTree::addEntity() has already checked
				_mmapEntities[pEntity->_mstrName] = pEntity;
				pEntity->_mpNodeOwner = this;	// Set node owner for the entity
				return;
			}
			// If we've reached maximum capacity for this node and max node depth hasn't been reached
			if (_mmapEntities.size() == _mpOctTree->_miMaxEntitiesPerNode)
			{
				// We need to create child node/s then move all the entities from this node into the children, 
				// as well as the new entity

				// Compute potential child node regions
				CAABB aabbNNN = computeChildNodeRegion(ChildNode::NEGX_NEGY_NEGZ);
				CAABB aabbNNP = computeChildNodeRegion(ChildNode::NEGX_NEGY_POSZ);
				CAABB aabbNPN = computeChildNodeRegion(ChildNode::NEGX_POSY_NEGZ);
				CAABB aabbNPP = computeChildNodeRegion(ChildNode::NEGX_POSY_POSZ);
				CAABB aabbPNN = computeChildNodeRegion(ChildNode::POSX_NEGY_NEGZ);
				CAABB aabbPNP = computeChildNodeRegion(ChildNode::POSX_NEGY_POSZ);
				CAABB aabbPPN = computeChildNodeRegion(ChildNode::POSX_POSY_NEGZ);
				CAABB aabbPPP = computeChildNodeRegion(ChildNode::POSX_POSY_POSZ);

				// Add the new entity to this node's entity map, so it'll be moved to the correct child node below
				_mmapEntities[pEntity->_mstrName] = pEntity;
				pEntity->_mpNodeOwner = this;	// Set node owner for the entity

				// Move all the entities from this node, into the child nodes
				std::map<std::string, COctTreeEntity*>::iterator it = _mmapEntities.begin();
				while (it != _mmapEntities.end())
				{
					// Determine which child node the entity fits in, regardless of whether the child node exists or not
					ChildNode childNode = ChildNode::NONE;
					if (aabbNNN.getPointIsInside(it->second->_mvPosition))
						childNode = ChildNode::NEGX_NEGY_NEGZ;
					else if (aabbNNP.getPointIsInside(it->second->_mvPosition))
						childNode = ChildNode::NEGX_NEGY_POSZ;
					else if (aabbNPN.getPointIsInside(it->second->_mvPosition))
						childNode = ChildNode::NEGX_POSY_NEGZ;
					else if (aabbNPP.getPointIsInside(it->second->_mvPosition))
						childNode = ChildNode::NEGX_POSY_POSZ;
					else if (aabbPNN.getPointIsInside(it->second->_mvPosition))
						childNode = ChildNode::POSX_NEGY_NEGZ;
					else if (aabbPNP.getPointIsInside(it->second->_mvPosition))
						childNode = ChildNode::POSX_NEGY_POSZ;
					else if (aabbPPN.getPointIsInside(it->second->_mvPosition))
						childNode = ChildNode::POSX_POSY_NEGZ;
					else if (aabbPPP.getPointIsInside(it->second->_mvPosition))
						childNode = ChildNode::POSX_POSY_POSZ;

					// Error checking, making sure the entity could fit in one of the eight possible children
					ThrowIfTrue(ChildNode::NONE == childNode, "COctTreeNode::addEntity() failed when trying to add entity " + it->second->_mstrName + " to any of the eight child nodes as it's position doesn't fit inside any of them.");

					// Create the child node if it doesn't exist
					if (!_mpChildNode[childNode])
					{
						_mpChildNode[childNode] = new COctTreeNode(computeChildNodeRegion(childNode), this, _mpOctTree);
						ThrowIfFalse(_mpChildNode[childNode], "COctTreeNode::addEntity() failed when trying to add entity " + it->second->_mstrName + " to any of the eight child nodes as it was unable to allocate memory for the child node.");
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
		// Compute potential child node regions
		CAABB aabbNNN = computeChildNodeRegion(ChildNode::NEGX_NEGY_NEGZ);
		CAABB aabbNNP = computeChildNodeRegion(ChildNode::NEGX_NEGY_POSZ);
		CAABB aabbNPN = computeChildNodeRegion(ChildNode::NEGX_POSY_NEGZ);
		CAABB aabbNPP = computeChildNodeRegion(ChildNode::NEGX_POSY_POSZ);
		CAABB aabbPNN = computeChildNodeRegion(ChildNode::POSX_NEGY_NEGZ);
		CAABB aabbPNP = computeChildNodeRegion(ChildNode::POSX_NEGY_POSZ);
		CAABB aabbPPN = computeChildNodeRegion(ChildNode::POSX_POSY_NEGZ);
		CAABB aabbPPP = computeChildNodeRegion(ChildNode::POSX_POSY_POSZ);

		ChildNode childNode = ChildNode::NONE;
		if (aabbNNN.getPointIsInside(pEntity->_mvPosition))
			childNode = ChildNode::NEGX_NEGY_NEGZ;
		else if (aabbNNP.getPointIsInside(pEntity->_mvPosition))
			childNode = ChildNode::NEGX_NEGY_POSZ;
		else if (aabbNPN.getPointIsInside(pEntity->_mvPosition))
			childNode = ChildNode::NEGX_POSY_NEGZ;
		else if (aabbNPP.getPointIsInside(pEntity->_mvPosition))
			childNode = ChildNode::NEGX_POSY_POSZ;
		else if (aabbPNN.getPointIsInside(pEntity->_mvPosition))
			childNode = ChildNode::POSX_NEGY_NEGZ;
		else if (aabbPNP.getPointIsInside(pEntity->_mvPosition))
			childNode = ChildNode::POSX_NEGY_POSZ;
		else if (aabbPPN.getPointIsInside(pEntity->_mvPosition))
			childNode = ChildNode::POSX_POSY_NEGZ;
		else if (aabbPPP.getPointIsInside(pEntity->_mvPosition))
			childNode = ChildNode::POSX_POSY_POSZ;

		// Error checking, making sure the entity could fit in one of the eight possible children
		ThrowIfTrue(ChildNode::NONE == childNode, "COctTreeNode::addEntity() failed when trying to add entity " + pEntity->_mstrName + " to any of the eight child nodes as it's position doesn't fit inside any of them.");

		// Create the child node if it doesn't exist
		if (!_mpChildNode[childNode])
		{
			_mpChildNode[childNode] = new COctTreeNode(computeChildNodeRegion(childNode), this, _mpOctTree);
			ThrowIfFalse(_mpChildNode[childNode], "COctTreeNode::addEntity() failed when trying to add entity " + pEntity->_mstrName + " to any of the eight child nodes as it was unable to allocate memory for the child node.");
		}

		// Add the new entity
		_mpChildNode[childNode]->addEntity(pEntity);
	}

	void COctTreeNode::removeEntity(COctTreeEntity* pEntity)
	{
		// Attempt to find the entity
		std::map<std::string, COctTreeEntity*>::iterator it = _mmapEntities.find(pEntity->_mstrName);
		ThrowIfTrue(it == _mmapEntities.end(), "COctTreeNode::removeEntity() failed. The entity named " + pEntity->_mstrName + " could not be found");
		_mmapEntities.erase(it);
		// No need to delete entity, the COctTree::removeAllEntities() or COctTree::removeEntity() does this
	}

	void COctTreeNode::getNodesWithEntities(std::vector<COctTreeNode*>& vecNodes)
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
			for (int i = 0; i < 8; i++)
			{
				if (_mpChildNode[i])
					_mpChildNode[i]->getNodesWithEntities(vecNodes);
			}
		}
	}

	void COctTreeNode::getNodesWithEntitiesWhichIntersect(std::vector<COctTreeNode*>& vecNodes, const CAABB& aabb)
	{
		// If this node doesn't have any children, just check this node
		if (!hasAnyChildNodes())
		{
			// If this node doesn't have any entities
			if (0 == _mmapEntities.size())
				return;

			// If the AABB interects with this node, add this node
			if (_mRegion.getAABBintersects(aabb))
			{
				vecNodes.push_back(this);
				return;
			}
		}
		else  // This node has children, check those
		{
			for (int i = 0; i < 8; i++)
			{
				if (_mpChildNode[i])
					_mpChildNode[i]->getNodesWithEntitiesWhichIntersect(vecNodes, aabb);
			}
		}
	}

	void COctTreeNode::getNodesWithEntitiesWhichIntersect(std::vector<COctTreeNode*>& vecNodes, const CFrustum& frustum)
	{
		// If this node doesn't have any children, just check this node
		if (!hasAnyChildNodes())
		{
			// If this node doesn't have any entities
			if (0 == _mmapEntities.size())
				return;

			// If the AABB interects with this node, add this node
			if (frustum.isAABBIntersecting(_mRegion))
			{
				vecNodes.push_back(this);
				return;
			}
		}
		else  // This node has children, check those
		{
			for (int i = 0; i < 8; i++)
			{
				if (_mpChildNode[i])
					_mpChildNode[i]->getNodesWithEntitiesWhichIntersect(vecNodes, frustum);
			}
		}
	}

	void COctTreeNode::getMaxNodeDepth(unsigned int& uiMaxNodeDepth)
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