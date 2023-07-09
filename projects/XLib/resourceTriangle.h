#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "vector2r.h"
#include "colour.h"

namespace X
{
	class CResourceTriangle : public CResourceBase
	{
	public:
		CResourceTriangle();
		~CResourceTriangle();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

		// A single vertex
		struct Vertex
		{
			glm::vec3 position;		// Vertex position
			glm::vec4 colour;		// Vertex colour
			glm::vec2 texCoord;		// Vertex texture coordinate
			glm::vec3 normal;		// Vertex normal
			glm::vec3 tangent;		// Tangent vector for bump/normal mapping
			glm::vec3 binormal;		// Binormal vector for bump/normal mapping
		};

		// Removes all previously added vertices
		void removeGeom(void);

		// Once all vertices have been added or loaded with either of the the various add?? methods,
		// this creates the OpenGL objects and copies stuff to the GPU, ready for use.
		// If no vertices have previously been added, this silently fails.
		void update(void);

		// Binds the vertexArrayObject and calls glDrawElements()
		// If there's no vertex data, this silently fails.
		void draw(bool bWireframeMode = false);

		// Adds a new vertex
		void addVertex(const Vertex& newVertex);

		// Add a new index
		void addIndex(int newIndex);

		// Adds a bunch of vertices and indices which represent a 2D quad
		void addQuad2D(const glm::vec2& vPosition, const glm::vec2& vDimensions, const glm::vec4& colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec2& textureCoordinateBottomLeft = glm::vec2(0, 1), const glm::vec2& textureCoordinateBottomRight = glm::vec2(1, 1), const glm::vec2& textureCoordinateTopRight = glm::vec2(1, 0), const glm::vec2& textureCoordinateTopLeft = glm::vec2(0, 0));

		// Adds a bunch of vertices and indices which represent a 2D quad
		void addQuad2D(const CVector2r& vPosition, CVector2r& vDimensions, const CColour& colour = CColour(1.0f, 1.0f, 1.0f, 1.0f), const CVector2r& textureCoordinateBottomLeft = CVector2r(0, 1), const CVector2r& textureCoordinateBottomRight = CVector2r(1, 1), const CVector2r& textureCoordinateTopRight = CVector2r(1, 0), const CVector2r& textureCoordinateTopLeft = CVector2r(0, 0));

		// Adds a bunch of vertices and indices which represent a 3D quad pointing up along the Y axis
		void addGroundplane(const glm::vec3& vPosition, const glm::vec2& vDimensions, const glm::vec4& colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec2& textureCoordinateFrontLeft = glm::vec2(0, 1), const glm::vec2& textureCoordinateFrontRight = glm::vec2(1, 1), const glm::vec2& textureCoordinateBackRight = glm::vec2(1, 0), const glm::vec2& textureCoordinateBackLeft = glm::vec2(0, 0));

		// Use the convertObj() method to generate a binary file from an .obj file exported from
		// some 3D modelling package such as Blender.
		// The method will load in, convert and save out the stored geometry information into an efficient to load file which we use here
		// to load the vertex information into memory.
		// bCallUpdate, if set to true will call update(), saving us from having to do it, if we're done adding stuff.
		void addFromFile(const std::string& strGeometryFilename, bool bCallUpdate = true);

		// Return number of vertices which have been added so far.
		inline size_t getNumVertices(void)
		{
			return vertices.size();
		}

		// Return nummber of indices which have been added so far.
		inline size_t getNumIndicies(void)
		{
			return indices.size();
		}

		// Converts an .obj file to our custom geometry file format and saves to disk
		void convertObj(const std::string filename);

	private:

		unsigned int vertexBufferObject;	// OpenGL vertex buffer object ID
		unsigned int vertexArrayObject;		// OpenGL vertex array object ID
		unsigned int elementBufferObject;	// OpenGL element buffer object ID
		std::vector<Vertex> vertices;		// Vector holding each unique vertex
		std::vector<int> indices;			// Vector holding indicies to each unique vertex

		// Compute tangents and binormals from currently added geometry.
		// Called from update()
		void _computeTangentsAndBinormals(void);
	};
}