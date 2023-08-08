#include "PCH.h"
#include "resourceVertexBufferLine.h"
#include "log.h"

namespace X
{

	CResourceVertexBufferLine::CResourceVertexBufferLine()
	{
		_mVertexBufferObject = 0;
		_mVertexArrayObject = 0;
		_mElementBufferObject = 0;
		_muiIndex = 0;
		//onGLContextCreated();
		_muiLineMode = GL_LINE_STRIP;
	}

	CResourceVertexBufferLine::~CResourceVertexBufferLine()
	{
		onGLContextToBeDestroyed();
	}

	void CResourceVertexBufferLine::onGLContextCreated(void)
	{
		update();
	}

	void CResourceVertexBufferLine::onGLContextToBeDestroyed(void)
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

	void CResourceVertexBufferLine::removeGeom(void)
	{
		_mvecVertices.clear();
		_mvecIndices.clear();
		_muiIndex = 0;
	}

	void CResourceVertexBufferLine::update(void)
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

		// Unbind stuff as we're done
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void CResourceVertexBufferLine::render(void) const
	{
		if (!_mVertexArrayObject)
			return;

		if (0 == _mvecIndices.size())
			return;

		if (0 == _mvecVertices.size())
			return;

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindVertexArray(_mVertexArrayObject);
		glDrawElements(
			_muiLineMode,					// Mode. Specifies what kind of primitives to render. Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted.
			(GLsizei)_mvecIndices.size(),	// Count. Specifies the number of elements to be rendered.
			GL_UNSIGNED_INT,				// Type. Specifies the type of the values in indices. Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
			0);								// Indicies. Specifies a pointer to the location where the indices are stored. NOTE: We're using element buffer objects and using the indicies in that, so this is 0.
		glBindVertexArray(0);
	}

	void CResourceVertexBufferLine::addLinePoint(const Vertex& newVertex)
	{
		_mvecVertices.push_back(newVertex);
		_mvecIndices.push_back(_muiIndex);
		_muiIndex++;
	}

	void CResourceVertexBufferLine::addCircle(const CVector2f& vCentrePosition, float fRadius, unsigned int uiNumSegments, const CColour &colour)
	{
		ThrowIfFalse(uiNumSegments, "CResourceVertexBufferLine::addCircle() failed. given zero number of segments");
		Vertex v;
		v.colour = colour;
		float fAngDeltaPerSeg = k2Pi / float(uiNumSegments);
		if (GL_LINE_STRIP == _muiLineMode)
		{
			float fAng = 0.0f;
			for (unsigned int uiSegment = 0; uiSegment < uiNumSegments + 1; uiSegment++)
			{
				v.position.x = vCentrePosition.x + (sinf(fAng) * fRadius);
				v.position.y = vCentrePosition.y + (cosf(fAng) * fRadius);

				_mvecVertices.push_back(v);
				_mvecIndices.push_back(_muiIndex);
				_muiIndex++;
				fAng += fAngDeltaPerSeg;
			}
		}
		else if (GL_LINES == _muiLineMode)
		{
			float fAng = 0.0f;
			for (unsigned int uiSegment = 0; uiSegment < uiNumSegments + 1; uiSegment++)
			{
				v.position.x = vCentrePosition.x + (sinf(fAng) * fRadius);
				v.position.y = vCentrePosition.y + (cosf(fAng) * fRadius);

				_mvecVertices.push_back(v);
				_mvecIndices.push_back(_muiIndex);
				_muiIndex++;

				v.position.x = vCentrePosition.x + (sinf(fAng + fAngDeltaPerSeg) * fRadius);
				v.position.y = vCentrePosition.y + (cosf(fAng + fAngDeltaPerSeg) * fRadius);
				_mvecVertices.push_back(v);
				_mvecIndices.push_back(_muiIndex);
				_muiIndex++;

				fAng += fAngDeltaPerSeg;
			}
		}
	}

	void CResourceVertexBufferLine::addCircle(int iCentrePosX, int iCentrePosY, float fRadius, unsigned int uiNumSegments, const CColour& colour)
	{
		CVector2f vCentrePos;
		vCentrePos.set(float(iCentrePosX), float(iCentrePosY));
		addCircle(vCentrePos, fRadius, uiNumSegments, colour);
	}

	void CResourceVertexBufferLine::addQuad(const CVector2f& vCentrePosition, float fWidth, float fHeight, const CColour& colour)
	{
		// Set colour and positions of each of the four vertices which represent the quad
		Vertex vBL, vBR, vTL, vTR;
		vBL.colour = colour;
		vBR.colour = colour;
		vTL.colour = colour;
		vTR.colour = colour;
		
		float fHalfWidth = fWidth * 0.5f;
		float fHalfHeight = fHeight * 0.5f;

		vBL.position.x = vCentrePosition.x - fHalfWidth;
		vBL.position.y = vCentrePosition.y - fHalfHeight;
		vTR.position.x = vCentrePosition.x + fHalfWidth;
		vTR.position.y = vCentrePosition.y + fHalfHeight;
		vBR.position.x = vTR.position.x;
		vBR.position.y = vBL.position.y;
		vTL.position.x = vBL.position.x;
		vTL.position.y = vTR.position.y;

		if (GL_LINE_STRIP == _muiLineMode)
		{
			_mvecVertices.push_back(vBL);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			_mvecVertices.push_back(vTL);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			_mvecVertices.push_back(vTR);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			_mvecVertices.push_back(vBR);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			_mvecVertices.push_back(vBL);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
		}
		else if (GL_LINES == _muiLineMode)
		{
			_mvecVertices.push_back(vBL);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			_mvecVertices.push_back(vTL);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;

			_mvecVertices.push_back(vTL);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			_mvecVertices.push_back(vTR);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;

			_mvecVertices.push_back(vTR);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			_mvecVertices.push_back(vBR);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;

			_mvecVertices.push_back(vBR);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			_mvecVertices.push_back(vBL);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
		}
	}

	void CResourceVertexBufferLine::addAxis(const CVector3f& vCentrePosition, float fLineLength)
	{
		Vertex v;

		if (GL_LINE_STRIP == _muiLineMode)
		{
			// X
			v.colour.set(1.0f, 0.0f, 0.0f, 1.0f);
			v.position = vCentrePosition;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			v.position.x += fLineLength;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;

			// Y
			v.colour.set(0.0f, 1.0f, 0.0f, 1.0f);
			v.position = vCentrePosition;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			v.position.y += fLineLength;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;

			// Z
			v.colour.set(0.0f, 0.0f, 1.0f, 1.0f);
			v.position = vCentrePosition;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			v.position.z -= fLineLength;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;

		}
		else if (GL_LINES == _muiLineMode)
		{
			// X
			v.colour.set(1.0f, 0.0f, 0.0f, 1.0f);
			v.position = vCentrePosition;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			v.position.x += fLineLength;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			v.position.x = vCentrePosition.x;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;

			// Y
			v.colour.set(0.0f, 1.0f, 0.0f, 1.0f);
			v.position = vCentrePosition;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			v.position.y += fLineLength;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			v.position.y = vCentrePosition.y;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;

			// Z
			v.colour.set(0.0f, 0.0f, 1.0f, 1.0f);
			v.position = vCentrePosition;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			v.position.z -= fLineLength;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
			v.position.z = vCentrePosition.z;
			_mvecVertices.push_back(v);	_mvecIndices.push_back(_muiIndex);	_muiIndex++;
		}
	}

	void CResourceVertexBufferLine::setDrawModeAsLineStrip(void)
	{
		_muiLineMode = GL_LINE_STRIP;
	}
	
	void CResourceVertexBufferLine::setDrawModeAsLineList(void)
	{
		_muiLineMode = GL_LINES;
	}

	size_t CResourceVertexBufferLine::getNumVertices(void) const
	{
		return _mvecVertices.size();
	}

	size_t CResourceVertexBufferLine::getNumIndicies(void) const
	{
		return _mvecIndices.size();
	}
}