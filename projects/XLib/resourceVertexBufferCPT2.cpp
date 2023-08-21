#include "PCH.h"
#include "resourceVertexBufferCPT2.h"
#include "logging.h"

namespace X
{

	CResourceVertexBufferCPT2::CResourceVertexBufferCPT2()
	{
		_mVertexBufferObject = 0;
		_mVertexArrayObject = 0;
		_mElementBufferObject = 0;
		//onGLContextCreated();
	}

	CResourceVertexBufferCPT2::~CResourceVertexBufferCPT2()
	{
		onGLContextToBeDestroyed();
	}

	void CResourceVertexBufferCPT2::onGLContextCreated(void)
	{
		update();
	}

	void CResourceVertexBufferCPT2::onGLContextToBeDestroyed(void)
	{
		if (_mVertexBufferObject)
		{
			glDeleteBuffers(1, &_mVertexBufferObject);
			_mVertexBufferObject = 0;
		}
		if (_mVertexArrayObject)
		{
			glDeleteVertexArrays(1, &_mVertexArrayObject);
			_mVertexArrayObject = 0;
		}
		if (_mElementBufferObject)
		{
			glDeleteBuffers(1, &_mElementBufferObject);
			_mElementBufferObject = 0;
		}
	}

	void CResourceVertexBufferCPT2::removeGeom(void)
	{
		_mvecVertices.clear();
		_mvecIndices.clear();
	}

	void CResourceVertexBufferCPT2::update(void)
	{
		if (!_mvecVertices.size())
			return;
		if (!_mvecIndices.size())
			return;

		if (!_mVertexBufferObject)
			glGenBuffers(1, &_mVertexBufferObject);
		if (!_mVertexArrayObject)
			glGenVertexArrays(1, &_mVertexArrayObject);
		if (!_mElementBufferObject)
			glGenBuffers(1, &_mElementBufferObject);

		glBindVertexArray(_mVertexArrayObject);

		// GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
		// GL_STATIC_DRAW: the data is set only once and used many times.
		// GL_DYNAMIC_DRAW : the data is changed a lot and used many times.

		// Bind VBO and upload vertex data
		glBindBuffer(GL_ARRAY_BUFFER, _mVertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _mvecVertices.size(), &_mvecVertices[0], GL_STATIC_DRAW);

		// Indicies
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mElementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * _mvecIndices.size(), &_mvecIndices[0], GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0,			// Index. Specifies the index in the shader of the generic vertex attribute to be modified.
			3,								// Size. Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
			GL_FLOAT,						// Type. Specifies the data type of each component in the array. The symbolic constants GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT and GL_UNSIGNED_INT are accepted
			GL_FALSE,						// Normalized. Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
			sizeof(Vertex),					// Stride. Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
			(void*)0);						// Pointer. Specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
		glEnableVertexAttribArray(0);

		// Colour
		glVertexAttribPointer(1,			// Index. Specifies the index in the shader of the generic vertex attribute to be modified.
			4,								// Size. Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
			GL_FLOAT,						// Type. Specifies the data type of each component in the array. The symbolic constants GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT and GL_UNSIGNED_INT are accepted
			GL_FALSE,						// Normalized. Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
			sizeof(Vertex),					// Stride. Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
			(void*)(3 * sizeof(float)));	// Pointer. Specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
		glEnableVertexAttribArray(1);

		// Texture coordinates
		glVertexAttribPointer(2,			// Index. Specifies the index in the shader of the generic vertex attribute to be modified.
			2,								// Size. Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
			GL_FLOAT,						// Type. Specifies the data type of each component in the array. The symbolic constants GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT and GL_UNSIGNED_INT are accepted
			GL_FALSE,						// Normalized. Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
			sizeof(Vertex),					// Stride. Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
			(void*)(7 * sizeof(float)));	// Pointer. Specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
		glEnableVertexAttribArray(2);

		// 2nd Texture coordinates
		glVertexAttribPointer(3,			// Index. Specifies the index in the shader of the generic vertex attribute to be modified.
			2,								// Size. Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
			GL_FLOAT,						// Type. Specifies the data type of each component in the array. The symbolic constants GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT and GL_UNSIGNED_INT are accepted
			GL_FALSE,						// Normalized. Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
			sizeof(Vertex),					// Stride. Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
			(void*)(9 * sizeof(float)));	// Pointer. Specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
		glEnableVertexAttribArray(3);

		// Unbind stuff as we're done
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void CResourceVertexBufferCPT2::render(bool bWireframeMode) const
	{
		if (!_mVertexArrayObject)
			return;

		if (0 == _mvecIndices.size())
			return;

		if (0 == _mvecVertices.size())
			return;

		if (bWireframeMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindVertexArray(_mVertexArrayObject);
		glDrawElements(
			GL_TRIANGLES,				// Mode. Specifies what kind of primitives to render. Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted.
			(GLsizei)_mvecIndices.size(),	// Count. Specifies the number of elements to be rendered.
			GL_UNSIGNED_INT,			// Type. Specifies the type of the values in indices. Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
			0);							// Indicies. Specifies a pointer to the location where the indices are stored. NOTE: We're using element buffer objects and using the indicies in that, so this is 0.
		glBindVertexArray(0);
	}

	void CResourceVertexBufferCPT2::addVertex(const Vertex& newVertex)
	{
		_mvecVertices.push_back(newVertex);
	}

	void CResourceVertexBufferCPT2::addIndex(int newIndex)
	{
		_mvecIndices.push_back(newIndex);
	}

	void CResourceVertexBufferCPT2::addQuad2D(
		const CVector2f& vPosition,
		const CVector2f& vDimensions,
		const CColour& colour,
		const CVector2f& textureCoordinateBottomLeft,
		const CVector2f& textureCoordinateBottomRight,
		const CVector2f& textureCoordinateTopRight,
		const CVector2f& textureCoordinateTopLeft,
		const CVector2f& textureCoordinate2BottomLeft,
		const CVector2f& textureCoordinate2BottomRight,
		const CVector2f& textureCoordinate2TopRight,
		const CVector2f& textureCoordinate2TopLeft
	)
	{
		// Indicies
		unsigned int iIndex = (unsigned int)_mvecVertices.size();
		addIndex(iIndex);		// BL
		addIndex(iIndex + 1);	// TL
		addIndex(iIndex + 2);	// TR

		addIndex(iIndex + 2);	// TR
		addIndex(iIndex + 3);	// BR
		addIndex(iIndex);		// BL

		// Bottom left
		Vertex vertex;	// Holds a vertex's data before adding to the vector
		vertex.position.x = vPosition.x;
		vertex.position.y = vPosition.y + vDimensions.y;
		vertex.position.z = 0.0f;
		vertex.colour = colour;
		vertex.texCoord = textureCoordinateBottomLeft;
		vertex.texCoord2 = textureCoordinate2BottomLeft;
		addVertex(vertex);

		// Top left
		vertex.position.y = vPosition.y;
		vertex.texCoord = textureCoordinateTopLeft;
		vertex.texCoord2 = textureCoordinate2TopLeft;
		addVertex(vertex);

		// Top right
		vertex.position.x = vPosition.x + vDimensions.x;
		vertex.texCoord = textureCoordinateTopRight;
		vertex.texCoord2 = textureCoordinate2TopRight;
		addVertex(vertex);

		// Bottom right
		vertex.position.y = vPosition.y + vDimensions.y;
		vertex.texCoord = textureCoordinateBottomRight;
		vertex.texCoord2 = textureCoordinate2BottomRight;
		addVertex(vertex);
	}

	void CResourceVertexBufferCPT2::addGroundplane(
		const CVector3f& vPosition,
		const CVector2f& vDimensions,
		const CColour& colour,
		const CVector2f& textureCoordinateFrontLeft,
		const CVector2f& textureCoordinateFrontRight,
		const CVector2f& textureCoordinateBackRight,
		const CVector2f& textureCoordinateBackLeft,
		const CVector2f& textureCoordinate2FrontLeft,
		const CVector2f& textureCoordinate2FrontRight,
		const CVector2f& textureCoordinate2BackRight,
		const CVector2f& textureCoordinate2BackLeft)
	{
		// Indicies
		unsigned int iIndex = (unsigned int)_mvecVertices.size();
		addIndex(iIndex);		// BL
		addIndex(iIndex + 1);	// TL
		addIndex(iIndex + 2);	// TR

		addIndex(iIndex + 2);	// TR
		addIndex(iIndex + 3);	// BR
		addIndex(iIndex);		// BL

		CVector2f vHalfDims = vDimensions;
		vHalfDims *= 0.5f;

		// Front left
		Vertex vertex;	// Holds a vertex's data before adding to the vector
		vertex.position.x = vPosition.x + vHalfDims.x;
		vertex.position.y = vPosition.y;
		vertex.position.z = vPosition.z + vHalfDims.y;
		vertex.colour = colour;
		vertex.texCoord = textureCoordinateFrontLeft;
		vertex.texCoord2 = textureCoordinate2FrontLeft;
		addVertex(vertex);

		// Back left
		vertex.position.z = vPosition.z - vHalfDims.y;
		vertex.texCoord = textureCoordinateBackLeft;
		vertex.texCoord2 = textureCoordinate2BackLeft;
		addVertex(vertex);

		// Back right
		vertex.position.x = vPosition.x - vHalfDims.x;
		vertex.texCoord = textureCoordinateBackRight;
		vertex.texCoord2 = textureCoordinate2BackRight;
		addVertex(vertex);

		// Front right
		vertex.position.z = vPosition.z + vHalfDims.x;
		vertex.texCoord = textureCoordinateFrontRight;
		vertex.texCoord2 = textureCoordinate2FrontRight;
		addVertex(vertex);
	}

	void CResourceVertexBufferCPT2::addCube(const CVector3f& vPosition, const CVector3f& vDimensions, const CVector2f& vTextureRepeat, const CColour& colour)
	{
		// Indicies
		// A cube contains 2 triangles per face and 6 faces for a total of 12 triangles.
		// Each tri is obviously 3 vertices and therefore a cube needs a total of 36 vertices.
		unsigned int iIndex = (unsigned int)_mvecVertices.size();	// Start after last added index
		for (unsigned int iIndex = (unsigned int)_mvecVertices.size(); iIndex < (unsigned int)_mvecVertices.size() + 36; iIndex++)
		{
			addIndex(iIndex);
		}

		// Vertices
		// Each face has 2 triangles which share 4 corners BL, BR, TR and TL
		Vertex vBL, vBR, vTL, vTR;
		// Set texture coordinates for each corner vertex
		vBL.texCoord.set(0.0f, 0.0f);
		vBR.texCoord.set(vTextureRepeat.x, 0.0f);
		vTR.texCoord.set(vTextureRepeat.x, vTextureRepeat.y);
		vTL.texCoord.set(0.0f, vTextureRepeat.y);
		vBL.texCoord2 = vBL.texCoord;
		vBR.texCoord2 = vBR.texCoord;
		vTR.texCoord2 = vTR.texCoord;
		vTL.texCoord2 = vTL.texCoord;
		// Colour
		vBL.colour = vBR.colour = vTR.colour = vTL.colour = colour;
		// Positions...
		CVector3f vHalfDims = vDimensions * 0.5f;
		// Left faces
		vBL.position.set(vPosition.x - vHalfDims.x, vPosition.y - vHalfDims.y, vPosition.z + vHalfDims.z);
		vBR.position.set(vPosition.x - vHalfDims.x, vPosition.y - vHalfDims.y, vPosition.z - vHalfDims.z);
		vTR.position.set(vPosition.x - vHalfDims.x, vPosition.y + vHalfDims.y, vPosition.z - vHalfDims.z);
		vTL.position.set(vPosition.x - vHalfDims.x, vPosition.y + vHalfDims.y, vPosition.z + vHalfDims.z);
		addVertex(vBL);	addVertex(vTL);	addVertex(vTR);	addVertex(vTR);	addVertex(vBR);	addVertex(vBL);
		// Right faces
		vBL.position.set(vPosition.x + vHalfDims.x, vPosition.y - vHalfDims.y, vPosition.z - vHalfDims.z);
		vBR.position.set(vPosition.x + vHalfDims.x, vPosition.y - vHalfDims.y, vPosition.z + vHalfDims.z);
		vTR.position.set(vPosition.x + vHalfDims.x, vPosition.y + vHalfDims.y, vPosition.z + vHalfDims.z);
		vTL.position.set(vPosition.x + vHalfDims.x, vPosition.y + vHalfDims.y, vPosition.z - vHalfDims.z);
		addVertex(vBL);	addVertex(vTL);	addVertex(vTR);	addVertex(vTR);	addVertex(vBR);	addVertex(vBL);
		// Front faces
		vBL.position.set(vPosition.x - vHalfDims.x, vPosition.y - vHalfDims.y, vPosition.z - vHalfDims.z);
		vBR.position.set(vPosition.x + vHalfDims.x, vPosition.y - vHalfDims.y, vPosition.z - vHalfDims.z);
		vTR.position.set(vPosition.x + vHalfDims.x, vPosition.y + vHalfDims.y, vPosition.z - vHalfDims.z);
		vTL.position.set(vPosition.x - vHalfDims.x, vPosition.y + vHalfDims.y, vPosition.z - vHalfDims.z);
		addVertex(vBL);	addVertex(vTL);	addVertex(vTR);	addVertex(vTR);	addVertex(vBR);	addVertex(vBL);
		// Back faces
		vBL.position.set(vPosition.x + vHalfDims.x, vPosition.y - vHalfDims.y, vPosition.z + vHalfDims.z);
		vBR.position.set(vPosition.x - vHalfDims.x, vPosition.y - vHalfDims.y, vPosition.z + vHalfDims.z);
		vTR.position.set(vPosition.x - vHalfDims.x, vPosition.y + vHalfDims.y, vPosition.z + vHalfDims.z);
		vTL.position.set(vPosition.x + vHalfDims.x, vPosition.y + vHalfDims.y, vPosition.z + vHalfDims.z);
		addVertex(vBL);	addVertex(vTL);	addVertex(vTR);	addVertex(vTR);	addVertex(vBR);	addVertex(vBL);
		// Top faces
		vBL.position.set(vPosition.x - vHalfDims.x, vPosition.y + vHalfDims.y, vPosition.z - vHalfDims.z);
		vBR.position.set(vPosition.x + vHalfDims.x, vPosition.y + vHalfDims.y, vPosition.z - vHalfDims.z);
		vTR.position.set(vPosition.x + vHalfDims.x, vPosition.y + vHalfDims.y, vPosition.z + vHalfDims.z);
		vTL.position.set(vPosition.x - vHalfDims.x, vPosition.y + vHalfDims.y, vPosition.z + vHalfDims.z);
		addVertex(vBL);	addVertex(vTL);	addVertex(vTR);	addVertex(vTR);	addVertex(vBR);	addVertex(vBL);
		// Bottom faces
		vBL.position.set(vPosition.x - vHalfDims.x, vPosition.y - vHalfDims.y, vPosition.z + vHalfDims.z);
		vBR.position.set(vPosition.x + vHalfDims.x, vPosition.y - vHalfDims.y, vPosition.z + vHalfDims.z);
		vTR.position.set(vPosition.x + vHalfDims.x, vPosition.y - vHalfDims.y, vPosition.z - vHalfDims.z);
		vTL.position.set(vPosition.x - vHalfDims.x, vPosition.y - vHalfDims.y, vPosition.z - vHalfDims.z);
		addVertex(vBL);	addVertex(vTL);	addVertex(vTR);	addVertex(vTR);	addVertex(vBR);	addVertex(vBL);
	}

	size_t CResourceVertexBufferCPT2::getNumVertices(void) const
	{
		return _mvecVertices.size();
	}

	size_t CResourceVertexBufferCPT2::getNumIndicies(void) const
	{
		return _mvecIndices.size();
	}


}