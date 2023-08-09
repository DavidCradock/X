#include "PCH.h"
#include "resourceFont.h"
#include "utilities.h"
#include "stringUtils.h"
#include "singletons.h"

namespace X
{
	CResourceFont::CResourceFont(const std::string& strFontFilename)
	{
		// Vertex buffer stuff
		_mVertexBufferObject = 0;
		_mVertexArrayObject = 0;
		_mElementBufferObject = 0;

		// Compute filename pairs from parsed filename base
		std::string strFontFNTFilename = strFontFilename;
		strFontFNTFilename.append(".fnt");
		std::string strFontPNGFilename = strFontFilename;
		strFontPNGFilename.append(".png");

		// Create required resources (Not using manager, locally, as this object is responsible for onloss/reset of context.
		_mpResTexture = new CResourceTexture2DFromFile(strFontPNGFilename, true);

		// Now load in the font data
		FILE* file = 0;
		errno_t err;
		err = fopen_s(&file, strFontFNTFilename.c_str(), "rb");
		ThrowIfTrue(bool(err != 0), "CResourceFont() failed to open file " + strFontFNTFilename);
		ThrowIfTrue(1 != fread(&fontTypes.fMaxCharHeight, sizeof(fontTypes.fMaxCharHeight), 1, file), "CResourceFont() failed to read in data for file " + strFontFNTFilename);
		
		// Read in charDesc
		size_t read = fread(&fontTypes.charDesc, sizeof(fontTypes.charDesc), 1, file);
		ThrowIfTrue(1 != read, "CResourceFont() failed to read in data for file " + strFontFNTFilename);

		fclose(file);
		onGLContextCreated();
	}

	CResourceFont::~CResourceFont()
	{
		onGLContextToBeDestroyed();
		delete _mpResTexture;
	}

	void CResourceFont::onGLContextCreated(void)
	{
		_mpResTexture->onGLContextCreated();
	}

	void CResourceFont::onGLContextToBeDestroyed(void)
	{
		_mpResTexture->onGLContextToBeDestroyed();

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

	void CResourceFont::print(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, float fFontScaling, CColour colour) const
	{
		// Get shader used to render the text
		CResourceShader* pShader = x->pResource->getShader("X:VBCPT");
		
		// Setup the projection matrix as orthographic
		_mmatProjection.setProjectionOrthographic(0.0f, float(iRenderTargetWidth), 0.0f, float(iRenderTargetHeight), -1.0f, 1.0f);

		// Setup shader
		pShader->bind();
		CMatrix matrixWorld, matrixView;
		pShader->setMat4("matrixWorld", matrixWorld);
		pShader->setMat4("matrixView", matrixView);
		pShader->setMat4("matrixProjection", _mmatProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		// Bind texture
		_mpResTexture->bind(0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		// fABCa is the distance to add to the current position before drawing the character glyph.
		// fABCb is the width of the drawn portion of the character glyph. 
		// fABCc is the distance to add to the current position to provide white space to the right of the character glyph.

		// Build the vertex buffer for the given text
		char charIndex = 0;
		CVector2f vPosition((float)iPosX, (float)iPosY);
		CVector2f vDimensions;
		vDimensions.y = fontTypes.fMaxCharHeight * fFontScaling;
		CVector2f tcBL, tcBR, tcTR, tcTL;

		// Remove all geometry from vertex buffer
		_mvecVertices.clear();
		_mvecIndices.clear();
		for (unsigned int i = 0; i < strText.length(); ++i)
		{
			// Obtain character index
			const char* pCh = strText.c_str();
			charIndex = pCh[i];

			// Compute everything for this character 
			vPosition.x += fontTypes.charDesc[charIndex].fABCa;
			vDimensions.x = fontTypes.charDesc[charIndex].fABCb * fFontScaling;
			tcBL.x = fontTypes.charDesc[charIndex].vTexMin.x;
			tcBL.y = fontTypes.charDesc[charIndex].vTexMin.y;
			tcTR.x = fontTypes.charDesc[charIndex].vTexMax.x;
			tcTR.y = fontTypes.charDesc[charIndex].vTexMax.y;
			tcBR.x = fontTypes.charDesc[charIndex].vTexMax.x;
			tcBR.y = fontTypes.charDesc[charIndex].vTexMin.y;
			tcTL.x = fontTypes.charDesc[charIndex].vTexMin.x;
			tcTL.y = fontTypes.charDesc[charIndex].vTexMax.y;

			// Add quad to vertex buffer

			// Indicies
			unsigned int iIndex = (unsigned int)_mvecVertices.size();
			_mvecIndices.push_back(iIndex);		// BL
			_mvecIndices.push_back(iIndex + 1);	// TL
			_mvecIndices.push_back(iIndex + 2);	// TR

			_mvecIndices.push_back(iIndex + 2);	// TR
			_mvecIndices.push_back(iIndex + 3);	// BR
			_mvecIndices.push_back(iIndex);		// BL

			// Bottom left
			Vertex vertex;	// Holds a vertex's data before adding to the vector
			vertex.position.x = vPosition.x;
			vertex.position.y = vPosition.y + vDimensions.y;
			vertex.position.z = 0.0f;
			vertex.colour = colour;
			vertex.texCoord = tcBL;
			_mvecVertices.push_back(vertex);

			// Top left
			vertex.position.y = vPosition.y;
			vertex.texCoord = tcTL;
			_mvecVertices.push_back(vertex);

			// Top right
			vertex.position.x = vPosition.x + vDimensions.x;
			vertex.texCoord = tcTR;
			_mvecVertices.push_back(vertex);

			// Bottom right
			vertex.position.y = vPosition.y + vDimensions.y;
			vertex.texCoord = tcBR;
			_mvecVertices.push_back(vertex);

			vPosition.x += (fontTypes.charDesc[charIndex].fABCb + fontTypes.charDesc[charIndex].fABCc) * fFontScaling;
		}
		// Then finally upload to the GPU and draw everything.
//		if (!_mvecVertices.size())	// No need to check both vertices and indices
//			return;
		if (!_mvecIndices.size())
			return;

		if (!_mVertexBufferObject)
			glGenBuffers(1, &_mVertexBufferObject);
		if (!_mVertexArrayObject)
			glGenVertexArrays(1, &_mVertexArrayObject);
		if (!_mElementBufferObject)
			glGenBuffers(1, &_mElementBufferObject);

		glBindVertexArray(_mVertexArrayObject);

		// Bind VBO and upload vertex data
		glBindBuffer(GL_ARRAY_BUFFER, _mVertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _mvecVertices.size(), &_mvecVertices[0], GL_DYNAMIC_DRAW);

		// Indicies
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mElementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * _mvecIndices.size(), &_mvecIndices[0], GL_DYNAMIC_DRAW);

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

		// Draw everything
		if (!_mVertexArrayObject)
			return;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(_mVertexArrayObject);
		glDrawElements(
			GL_TRIANGLES,					// Mode. Specifies what kind of primitives to render. Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted.
			(GLsizei)_mvecIndices.size(),	// Count. Specifies the number of elements to be rendered.
			GL_UNSIGNED_INT,				// Type. Specifies the type of the values in indices. Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
			0);								// Indicies. Specifies a pointer to the location where the indices are stored. NOTE: We're using element buffer objects and using the indicies in that, so this is 0.
		glBindVertexArray(0);

		// Unbind everything and reset states
		_mpResTexture->unbind(0);
		pShader->unbind();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	void CResourceFont::printCentered(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, float fFontScaling, CColour colour) const
	{
		float fTextWidth = getTextWidth(strText, fFontScaling);
		iPosX -= int(fTextWidth * 0.5f);
		iPosY -= int(getTextHeight(fFontScaling) * 0.5f);
		print(strText, iPosX, iPosY, iRenderTargetWidth, iRenderTargetHeight, fFontScaling, colour);
	}

	void CResourceFont::printInRect(
		bool bRenderText,
		const std::string& strText,
		int iPosX,
		int iPosY,
		int iRenderTargetWidth,
		int iRenderTargetHeight,
		std::vector<std::string>& vstrTextLines,
		int& iTotalRenderedHeight,
		float fFontScaling,
		CColour colour) const
	{
		std::stringstream ss(strText);
		std::string strNextWord;
		char delim = ' ';
		std::string strLineWhichFits;	// Current line of text which fits
		std::string strLineWithWord;
		int iTextLineOffset = (int)getTextHeight(fFontScaling);
		int iTextYoffset = iPosY;
		// Get next word in ss
		while (std::getline(ss, strNextWord, delim))	// While there is a word
		{
			// Add next word to current line
			strLineWithWord = strLineWhichFits + strNextWord + " ";
			// Get rendered width of current line with next word
			float fWidthWithWord = getTextWidth(strLineWithWord, fFontScaling);
			// If the current line + word fits
			if (fWidthWithWord <= (float)iRenderTargetWidth)
			{
				// Add word to line
				strLineWhichFits += strNextWord + " ";
			}
			else  // If the current line + word does NOT fit
			{
				// Add current line to vector
				vstrTextLines.push_back(strLineWhichFits);
				// Render the line of text
				if (bRenderText)
					print(vstrTextLines[vstrTextLines.size() - 1], iPosX, iPosY + iTextYoffset, iRenderTargetWidth, iRenderTargetHeight, fFontScaling, colour);
				// Move Y position down based upon font height
				iTextYoffset += iTextLineOffset;
				strLineWhichFits.clear();
				strLineWhichFits += strNextWord + " ";
			}
		}
		// If the current line being checked still has characters
		if (strLineWhichFits.length())
		{
			// Add current line to vector
			vstrTextLines.push_back(strLineWhichFits);
			// Render the line of text
			if (bRenderText)
				print(vstrTextLines[vstrTextLines.size() - 1], iPosX, iPosY + iTextYoffset, iRenderTargetWidth, iRenderTargetHeight, fFontScaling, colour);
		}
		iTotalRenderedHeight = iTextYoffset + iTextLineOffset;
	}

	void CResourceFont::printInRectNewline(
		bool bRenderText,
		const std::string& strText,
		int iPosX,
		int iPosY,
		int iRenderTargetWidth,
		int iRenderTargetHeight,
		std::vector<std::string>& vstrTextLines,
		int& iTotalRenderedHeight,
		const std::string& strNewlineCode,
		float fFontScaling,
		CColour colour) const
	{
		// Split given text into multiple lines using the newline code, taking out the newline code from the string in the process.
		std::vector<std::string> vstrTextSplit = StringUtils::splitString(strText, strNewlineCode);

		int iTextYoffset = iPosY;	// Holds Y position of a line of text

		// For each line of text
		for (unsigned int ui = 0; ui < vstrTextSplit.size(); ui++)
		{
			// Get line of text
			std::stringstream ss(vstrTextSplit[ui]);

			std::string strNextWord;
			char delim = ' ';
			std::string strLineWhichFits;	// Current line of text which fits
			std::string strLineWithWord;	// Current line of text with the next word appended, for checking render width
			int iTextLineOffset = (int)getTextHeight(fFontScaling);
			
			// Get next word in ss
			while (std::getline(ss, strNextWord, delim))	// While there is a word
			{
				// Add next word to current line
				strLineWithWord = strLineWhichFits + strNextWord + " ";
				// Get rendered width of current line with next word
				float fWidthWithWord = getTextWidth(strLineWithWord, fFontScaling);
				// If the current line + word fits
				if (fWidthWithWord <= (float)iRenderTargetWidth)
				{
					// Add word to line
					strLineWhichFits += strNextWord + " ";
				}
				else  // If the current line + word does NOT fit
				{
					// Add current line to vector
					vstrTextLines.push_back(strLineWhichFits);
					// Render the line of text
					if (bRenderText)
						print(vstrTextLines[vstrTextLines.size() - 1], iPosX, iPosY + iTextYoffset, iRenderTargetWidth, iRenderTargetHeight, fFontScaling, colour);
					// Move Y position down based upon font height
					iTextYoffset += iTextLineOffset;
					strLineWhichFits.clear();
					strLineWhichFits += strNextWord + " ";
				}
			}
			// If the current line being checked still has characters
			if (strLineWhichFits.length())
			{
				// Add current line to vector
				vstrTextLines.push_back(strLineWhichFits);
				// Render the line of text
				if (bRenderText)
					print(vstrTextLines[vstrTextLines.size() - 1], iPosX, iPosY + iTextYoffset, iRenderTargetWidth, iRenderTargetHeight, fFontScaling, colour);
				iTextYoffset += iTextLineOffset;
			}
			iTotalRenderedHeight = iTextYoffset + iTextLineOffset;
		}	// Each line of text

	}

	float CResourceFont::getTextWidth(const std::string& strText, float fFontScaling) const
	{
		float fWidth = 0;
		unsigned char ch;
		for (int i = 0; i < (int)strText.length(); ++i)
		{
			ch = unsigned char(strText[i]);
			fWidth += fontTypes.charDesc[ch].fABCa;
			fWidth += fontTypes.charDesc[ch].fABCb;
			fWidth += fontTypes.charDesc[ch].fABCc;
		}
		return fWidth * fFontScaling;
	}

	float CResourceFont::getTextHeight(float fFontScaling) const
	{
		return fontTypes.fMaxCharHeight * fFontScaling;
	}
}