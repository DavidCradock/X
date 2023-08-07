#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "colour.h"
#include "vector3f.h"
#include "vector2f.h"

namespace X
{
	// Line strip/list vertex buffer used for rendering lines
	// We need to add at least 2 vertices, they will form the first line and then
	// each vertex added after those will form an additional line using the new vertex and the previous one.
	// That's the default mode, but we can call setDrawModeAsLineList() in which case, each pair of added vertices
	// then represent a single line.
	class CResourceVertexBufferLine : public CResourceBase
	{
	public:
		CResourceVertexBufferLine();
		~CResourceVertexBufferLine();

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
		};

		// Removes all previously added vertices
		void removeGeom(void);

		// Once all vertices have been added or loaded with either of the the various add?? methods,
		// this creates the OpenGL objects and copies stuff to the GPU, ready for use.
		// If no vertices have previously been added, this silently fails.
		void update(void);

		// Binds the vertexArrayObject and calls glDrawElements()
		// If there's no vertex data, this silently fails.
		// Don't forget to call update() if you've added vertex data
		void render(void) const;

		// Sets drawing of each added vertex as a strip of lines where each additional vertex draws an additional line (This is the default mode)
		void setDrawModeAsLineStrip(void);

		// Sets drawing of each pair of added vertices as a single line.
		void setDrawModeAsLineList(void);

		// Add a new vertex and index, saving us from having to call addVertex and addIndex individually
		// Don't forget to call update() if you're done.
		void addLinePoint(const Vertex& newVertex);

		// Adds a circle
		// Don't forget to call update() if you're done.
		void addCircle(const CVector2f& vCentrePosition, float fRadius, unsigned int uiNumSegments = 6, const CColour& colour = CColour(1.0f, 1.0f, 1.0f, 1.0f));

		// Adds a circle
		// Don't forget to call update() if you're done.
		void addCircle(int iCentrePosX, int iCentrePosY, float fRadius, unsigned int uiNumSegments = 6, const CColour& colour = CColour(1.0f, 1.0f, 1.0f, 1.0f));

		// Adds a quad, centred along the Z axis
		// Don't forget to call update() if you're done.
		void addQuad(const CVector2f& vCentrePosition, float fWidth, float fHeight, const CColour& colour = CColour(1.0f, 1.0f, 1.0f, 1.0f));

		// Adds a three lines representing X, Y and Z axis coloured red, green and blue.
		// The Z axis line is positioned from vCentrePosition along negative Z, as OpenGL used -Z to go into the screen.
		// Don't forget to call update() if you're done.
		void addAxis(const CVector3f& vCentrePosition, float fLineLength);

		// Return number of vertices which have been added so far.
		size_t getNumVertices(void) const;
		
		// Return nummber of indices which have been added so far.
		size_t getNumIndicies(void) const;
		
	private:

		unsigned int _mVertexBufferObject;		// OpenGL vertex buffer object ID
		unsigned int _mVertexArrayObject;		// OpenGL vertex array object ID
		unsigned int _mElementBufferObject;		// OpenGL element buffer object ID
		std::vector<Vertex> _mvecVertices;		// Vector holding each unique vertex
		std::vector<int> _mvecIndices;			// Vector holding indicies to each unique vertex

		unsigned int _muiIndex;				// Used by addLinePoint and addCircle to keep track of indicies

		// Holds either GL_LINE_STRIP or GL_LINES which is used for the drawing mode.
		// GL_LINE_STRIP sets drawing of each added vertex as a strip of lines where each additional vertex draws an additional line (This is the default mode)
		// GL_LINES sets drawing of each pair of added vertices as a single line.
		unsigned int _muiLineMode;
	};
}