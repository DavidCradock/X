#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "vector2f.h"
#include "vector3f.h"
#include "colour.h"

namespace X
{
	// Resource used by SCResourceManager, a vertex buffer with colour, position and 2x texture coordinates
	class CResourceVertexBufferCPT2 : public CResourceBase
	{
	public:
		CResourceVertexBufferCPT2();
		~CResourceVertexBufferCPT2();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

		// A single vertex
		struct Vertex
		{
			CVector3f position;		// Vertex position
			CColour colour;			// Vertex colour
			CVector2f texCoord;		// Vertex texture coordinate
			CVector2f texCoord2;	// A second set of texture coordinates
		};

		// Removes all previously added vertices
		void removeGeom(void);

		// Once all vertices have been added or loaded with either of the the various add?? methods,
		// this creates the OpenGL objects and copies stuff to the GPU, ready for use.
		// If no vertices have previously been added, this silently fails.
		void update(void);

		// Binds the vertexArrayObject and calls glDrawElements()
		// If there's no vertex data, this silently fails.
		void render(bool bWireframeMode = false) const;

		// Adds a new vertex
		void addVertex(const Vertex& newVertex);

		// Add a new index
		void addIndex(int newIndex);

		// Adds a bunch of vertices and indices which represent a 2D quad
		void addQuad2D(
			const CVector2f& vPosition,
			const CVector2f& vDimensions,
			const CColour& colour = CColour(1.0f, 1.0f, 1.0f, 1.0f),
			const CVector2f& textureCoordinateBottomLeft = CVector2f(0, 1),
			const CVector2f& textureCoordinateBottomRight = CVector2f(1, 1),
			const CVector2f& textureCoordinateTopRight = CVector2f(1, 0),
			const CVector2f& textureCoordinateTopLeft = CVector2f(0, 0),
			const CVector2f& textureCoordinate2BottomLeft = CVector2f(0, 1),
			const CVector2f& textureCoordinate2BottomRight = CVector2f(1, 1),
			const CVector2f& textureCoordinate2TopRight = CVector2f(1, 0),
			const CVector2f& textureCoordinate2TopLeft = CVector2f(0, 0)
			);

		// Adds a bunch of vertices and indices which represent a 3D quad pointing up along the Y axis
		void addGroundplane(
			const CVector3f& vPosition,
			const CVector2f& vDimensions,
			const CColour& colour = CColour(1.0f, 1.0f, 1.0f, 1.0f),
			const CVector2f& textureCoordinateFrontLeft = CVector2f(0, 1),
			const CVector2f& textureCoordinateFrontRight = CVector2f(1, 1),
			const CVector2f& textureCoordinateBackRight = CVector2f(1, 0),
			const CVector2f& textureCoordinateBackLeft = CVector2f(0, 0),
			const CVector2f & textureCoordinate2FrontLeft = CVector2f(0, 1),
			const CVector2f & textureCoordinate2FrontRight = CVector2f(1, 1),
			const CVector2f & textureCoordinate2BackRight = CVector2f(1, 0),
			const CVector2f & textureCoordinate2BackLeft = CVector2f(0, 0)
		);

		// Adds a bunch of vertices and indices which represent a 3D cube.
		void addCube(const CVector3f& vPosition, const CVector3f& vDimensions, const CVector2f& vTextureRepeat = CVector2f(1.0f, 1.0f), const CColour& colour = CColour(1.0f, 1.0f, 1.0f, 1.0f));

		// Return number of vertices which have been added so far.
		size_t getNumVertices(void) const;

		// Return nummber of indices which have been added so far.
		size_t getNumIndicies(void) const;

	private:
		unsigned int _mVertexBufferObject;	// OpenGL vertex buffer object ID
		unsigned int _mVertexArrayObject;	// OpenGL vertex array object ID
		unsigned int _mElementBufferObject;	// OpenGL element buffer object ID
		std::vector<Vertex> _mvecVertices;	// Vector holding each unique vertex
		std::vector<int> _mvecIndices;		// Vector holding indicies to each unique vertex
	};
}