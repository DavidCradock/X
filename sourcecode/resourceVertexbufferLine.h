#pragma once
#include "PCH.h"
#include "resourceBase.h"

namespace X
{
	// Line strip vertex buffer used for rendering lines
	// We need to add at least 2 vertices, they will form the first line and then
	// each vertex added after those will form an additional line using the new vertex and the previous one.
	class ResourceVertexbufferLine : public ResourceBase
	{
	public:
		ResourceVertexbufferLine();
		~ResourceVertexbufferLine();

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
		};

		// Removes all previously added vertices
		void removeGeom(void);

		// Once all vertices have been added or loaded with either of the the various add?? methods,
		// this creates the OpenGL objects and copies stuff to the GPU, ready for use.
		// If no vertices have previously been added, this silently fails.
		void update(void);

		// Binds the vertexArrayObject and calls glDrawElements()
		// If there's no vertex data, this silently fails.
		void draw(void);

		// Add a new vertex and index, saving us from having to call addVertex and addIndex individually
		void addLinePoint(const Vertex& newVertex);

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

		unsigned int muiIndex;				// Used by addLinePoint to keep track of indicies
	};
}