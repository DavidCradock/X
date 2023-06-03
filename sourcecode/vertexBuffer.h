#pragma once
#include "PCH.h"

namespace X
{
	// Holds vertices for submission to the GPU
	// Upon construction, the OpenGL IDs are obtained.
	// We then add the vertex information (position, texture coordinates etc) with
	// the various add??? methods and then we upload this information to the GPU with a 
	// call to uploadToOpenGL() and finally draw the thing with a call to draw()
	class VertexBuffer
	{
	public:
		// A single vertex
		struct Vertex
		{
			glm::vec3 position;		// Vertex position
			glm::vec3 normal;		// Vertex normal
			glm::vec4 colour;		// Vertex colour
			glm::vec2 texCoord;		// Vertex texture coordinate
		};

		VertexBuffer();
		~VertexBuffer();

		// Removes all previously added vertices and deletes OpenGL objects
		void reset(void);

		// Once all vertices have been added or loaded, prepares vertex buffer, ready for use.
		void uploadToOpenGL(void);

		// Binds the vertexArrayObject and calls glDrawElements()
		void draw(bool bWireframeMode = false);

		// Adds a new vertex, once all indicies and vertices are added, call upload()
		void addVertex(const Vertex& newVertex);

		// Add a new index, once all indicies and vertices have been added, call upload()
		void addIndex(int newIndex);

		// Adds a bunch of vertices and indices which represent a 2D quad
		void addQuad(const glm::vec2& vPosition, const glm::vec2& vDimensions, const glm::vec4& colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec2& textureCoordinateBottomLeft = glm::vec2(0, 0), const glm::vec2& textureCoordinateBottomRight = glm::vec2(1, 0), const glm::vec2& textureCoordinateTopRight = glm::vec2(1, 1), const glm::vec2& textureCoordinateTopLeft = glm::vec2(0, 1));

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
	private:
		unsigned int vertexBufferObject;	// OpenGL vertex buffer object ID
		unsigned int vertexArrayObject;		// OpenGL vertex array object ID
		unsigned int elementBufferObject;	// OpenGL element buffer object ID
		std::vector<Vertex> vertices;		// Vector holding each unique vertex
		std::vector<int> indices;			// Vector holding indicies to each unique vertex
	};
}
