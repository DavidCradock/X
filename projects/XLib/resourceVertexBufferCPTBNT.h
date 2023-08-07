#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "vector2f.h"
#include "vector3f.h"
#include "colour.h"

namespace X
{
	// Resource used by SCResourceManager, a vertex buffer with position, colour, texture coordinates, Binormal, Normal and Tangents
	class CResourceVertexBufferCPTBNT : public CResourceBase
	{
	public:
		CResourceVertexBufferCPTBNT();
		~CResourceVertexBufferCPTBNT();

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
			CVector3f tangent;		// Tangent vector for bump/normal mapping
			CVector3f binormal;		// Binormal vector for bump/normal mapping
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
		unsigned int _mVertexBufferObject;	// OpenGL vertex buffer object ID
		unsigned int _mVertexArrayObject;	// OpenGL vertex array object ID
		unsigned int _mElementBufferObject;	// OpenGL element buffer object ID
		std::vector<Vertex> _mvecVertices;	// Vector holding each unique vertex
		std::vector<int> _mvecIndices;		// Vector holding indicies to each unique vertex

		// Compute tangents and binormals from currently added geometry.
		// Called from update()
		void _computeTangentsAndBinormals(void);
	};
}