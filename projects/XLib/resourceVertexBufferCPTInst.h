#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "vector2f.h"
#include "vector3f.h"
#include "colour.h"
#include "matrix.h"

namespace X
{
	// Resource used by SCResourceManager, a vertex buffer with position, colour, texture coordinates and normal.
	// Just like the non instanced vertex buffer CResourceVertexbufferCPT resource, we add vertices as normal.
	// The difference is that if we're rendering multiple objects, with the CResourceVertexBufferCPT we'd either make multiple
	// calls to render(), or add all the geometry multiple times before calling render.
	// This is a waste of CPU/GPU cycles as we'd have to add a ton of vertices and send them over to the GPU, or call render many times.
	// With instancing however, we add just one set of vertices used to represent an object to be rendered multiple times and then
	// set an additional bunch of data that's unique to each instance of the object to be rendered, this data for each instance and
	// then call render just once and then the GPU uses the data stored in this instance data to render the added geometry multiple times.
	// This instanced data is a world matrix which holds translation, rotation and scale of each instance.
	// Once the vertex data is added, we call addInstanceMatrix() for each instance of the vertex data we wish to render.
	class CResourceVertexBufferCPTInst : public CResourceBase
	{
	public:
		CResourceVertexBufferCPTInst();
		~CResourceVertexBufferCPTInst();

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
			CVector3f normal;		// Vertex normal
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

		// Adds a transformation matrix for one of the instances rendered during a call to render()
		void addInstanceMatrix(const CMatrix& matrixInstanceTransformation);

		// Removes all previously added instance matrices
		void removeAllInstanceMatrices(void);

		// Adds a new vertex
		void addVertex(const Vertex& newVertex);

		// Add a new index
		void addIndex(int newIndex);

		// Adds a bunch of vertices and indices which represent a 2D quad
		void addQuad2D(const CVector2f& vPosition, const CVector2f& vDimensions, const CColour& colour = CColour(1.0f, 1.0f, 1.0f, 1.0f), const CVector2f& textureCoordinateBottomLeft = CVector2f(0, 1), const CVector2f& textureCoordinateBottomRight = CVector2f(1, 1), const CVector2f& textureCoordinateTopRight = CVector2f(1, 0), const CVector2f& textureCoordinateTopLeft = CVector2f(0, 0));

		// Adds a bunch of vertices and indices which represent a 3D quad pointing up along the Y axis
		void addGroundplane(const CVector3f& vPosition, const CVector2f& vDimensions, const CColour& colour = CColour(1.0f, 1.0f, 1.0f, 1.0f), const CVector2f& textureCoordinateFrontLeft = CVector2f(0, 1), const CVector2f& textureCoordinateFrontRight = CVector2f(1, 1), const CVector2f& textureCoordinateBackRight = CVector2f(1, 0), const CVector2f& textureCoordinateBackLeft = CVector2f(0, 0));

		// Adds a bunch of vertices and indices which represent a 3D cube.
		void addCube(const CVector3f& vPosition, const CVector3f& vDimensions, const CVector2f& vTextureRepeat = CVector2f(1.0f, 1.0f), const CColour& colour = CColour(1.0f, 1.0f, 1.0f, 1.0f));

		// Use the convertObj() method to generate a binary file from an .obj file exported from
		// some 3D modelling package such as Blender.
		// The method will load in, convert and save out the stored geometry information into an efficient to load file which we use here
		// to load the vertex information into memory.
		// bCallUpdate, if set to true will call update(), saving us from having to do it, if we're done adding stuff.
		void addFromFile(const std::string& strGeometryFilename, bool bCallUpdate = true);

		// Return number of vertices which have been added so far.
		size_t getNumVertices(void) const;

		// Return nummber of indices which have been added so far.
		size_t getNumIndicies(void) const;

		// Converts an .obj file to our custom geometry file format and saves to disk
		void convertObj(const std::string filename) const;
	private:
		unsigned int _mVertexBufferObject;		// OpenGL vertex buffer object ID
		unsigned int _mVertexArrayObject;		// OpenGL vertex array object ID
		unsigned int _mElementBufferObject;		// OpenGL element buffer object ID
		std::vector<Vertex> _mvecVertices;		// Vector holding each unique vertex
		std::vector<int> _mvecIndices;			// Vector holding indicies to each unique vertex
		std::vector<CMatrix> _mInstanceMatrix;	// Each matrix holding transformation for an instance to be rendered.

	};
}