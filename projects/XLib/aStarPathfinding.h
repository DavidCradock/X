#pragma once
#include "PCH.h"

namespace X
{
	// An A star pathfinding class.
	// The A star algorithm is awesome at finding a path between two points within a weighted 3D graph.
	// The weights for each node in the graph is the cost to travel into that node, the greater the cost, the less likely the node
	// will be used when generating a path. A node can be set as impassable which makes it so that node will never be used in a generated path.
	// If you wish to use this class for 2D, you can set the area above and below the 2D area of the graph with impassable nodes.
	//
	// How to use the thing...
	class CAStarPathfinding
	{
	public:
		// Default constructor
		CAStarPathfinding();
	};
}