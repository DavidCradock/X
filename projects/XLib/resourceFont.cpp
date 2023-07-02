#include "PCH.h"
#include "resourceFont.h"
#include "utilities.h"
#include "log.h"
#include "resourceManager.h"

namespace X
{
	ResourceFont::ResourceFont(const std::string& strFontFilename)
	{
		// Vertex buffer stuff
		vertexBufferObject = 0;
		vertexArrayObject = 0;
		elementBufferObject = 0;

		// Compute filename pairs from parsed filename base
		std::string strFontFNTFilename = strFontFilename;
		strFontFNTFilename.append(".fnt");
		std::string strFontPNGFilename = strFontFilename;
		strFontPNGFilename.append(".png");

		// Create required resources (Not using manager, locally, as this object is responsible for onloss/reset of context.
		_mpResTexture = new ResourceTexture2D(strFontPNGFilename, true);

		// Now load in the font data
		FILE* file = 0;
		errno_t err;
		err = fopen_s(&file, strFontFNTFilename.c_str(), "rb");
		ThrowIfTrue(bool(err != 0), "ResourceFont() failed to open file " + strFontFNTFilename);
		ThrowIfTrue(1 != fread(&fontTypes.fMaxCharHeight, sizeof(fontTypes.fMaxCharHeight), 1, file), "ResourceFont() failed to read in data for file " + strFontFNTFilename);
		
		// Read in charDesc
		size_t read = fread(&fontTypes.charDesc, sizeof(fontTypes.charDesc), 1, file);
		ThrowIfTrue(1 != read, "ResourceFont() failed to read in data for file " + strFontFNTFilename);

		fclose(file);
		onGLContextCreated();
	}

	ResourceFont::~ResourceFont()
	{
		onGLContextToBeDestroyed();
		delete _mpResTexture;
	}

	void ResourceFont::onGLContextCreated(void)
	{
		_mpResTexture->onGLContextCreated();
	}

	void ResourceFont::onGLContextToBeDestroyed(void)
	{
		_mpResTexture->onGLContextToBeDestroyed();

		if (vertexBufferObject)
		{
			glDeleteBuffers(1, &vertexBufferObject);
			vertexBufferObject = 0;
		}
		if (vertexArrayObject)
		{
			glDeleteVertexArrays(1, &vertexArrayObject);
			vertexArrayObject = 0;
		}
		if (elementBufferObject)
		{
			glDeleteBuffers(1, &elementBufferObject);
			elementBufferObject = 0;
		}
	}

	void ResourceFont::print(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, float fFontScaling, glm::vec4 colour)
	{
		// Get shader used to render the text
		ResourceManager* pResourceManager = ResourceManager::getPointer();
		ResourceShader* pShader = pResourceManager->getShader("X:font");
		
		// Setup the projection matrix as orthographic
		_mmatProjection = glm::ortho(0.0f, float(iRenderTargetWidth), float(iRenderTargetHeight), 0.0f, -1.0f, 1.0f);

		// Setup shader
		pShader->bind();
		pShader->setMat4("transform", _mmatProjection);

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
		glm::vec2 vPosition((float)iPosX, (float)iPosY);
		glm::vec2 vDimensions;
		vDimensions.y = fontTypes.fMaxCharHeight * fFontScaling;
		glm::vec2 tcBL, tcBR, tcTR, tcTL;

		// Remove all geometry from vertex buffer
		vertices.clear();
		indices.clear();
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
			unsigned int iIndex = (unsigned int)vertices.size();
			indices.push_back(iIndex);		// BL
			indices.push_back(iIndex + 1);	// TL
			indices.push_back(iIndex + 2);	// TR

			indices.push_back(iIndex + 2);	// TR
			indices.push_back(iIndex + 3);	// BR
			indices.push_back(iIndex);		// BL

			// Bottom left
			Vertex vertex;	// Holds a vertex's data before adding to the vector
			vertex.position.x = vPosition.x;
			vertex.position.y = vPosition.y + vDimensions.y;
			vertex.position.z = 0.0f;
			vertex.colour = colour;
			vertex.texCoord = tcBL;
			vertices.push_back(vertex);

			// Top left
			vertex.position.y = vPosition.y;
			vertex.texCoord = tcTL;
			vertices.push_back(vertex);

			// Top right
			vertex.position.x = vPosition.x + vDimensions.x;
			vertex.texCoord = tcTR;
			vertices.push_back(vertex);

			// Bottom right
			vertex.position.y = vPosition.y + vDimensions.y;
			vertex.texCoord = tcBR;
			vertices.push_back(vertex);

			vPosition.x += (fontTypes.charDesc[charIndex].fABCb + fontTypes.charDesc[charIndex].fABCc) * fFontScaling;
		}
		// Then finally upload to the GPU and draw everything.
//		if (!vertices.size())	// No need to check both vertices and indices
//			return;
		if (!indices.size())
			return;

		if (!vertexBufferObject)
			glGenBuffers(1, &vertexBufferObject);
		if (!vertexArrayObject)
			glGenVertexArrays(1, &vertexArrayObject);
		if (!elementBufferObject)
			glGenBuffers(1, &elementBufferObject);

		glBindVertexArray(vertexArrayObject);

		// Bind VBO and upload vertex data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);

		// Indicies
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_DYNAMIC_DRAW);

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
		if (!vertexArrayObject)
			return;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(vertexArrayObject);
		glDrawElements(
			GL_TRIANGLES,				// Mode. Specifies what kind of primitives to render. Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted.
			(GLsizei)indices.size(),	// Count. Specifies the number of elements to be rendered.
			GL_UNSIGNED_INT,			// Type. Specifies the type of the values in indices. Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
			0);							// Indicies. Specifies a pointer to the location where the indices are stored. NOTE: We're using element buffer objects and using the indicies in that, so this is 0.
		glBindVertexArray(0);

		// Unbind everything and reset states
		_mpResTexture->unbind(0);
		pShader->unbind();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	void ResourceFont::printCentered(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, float fFontScaling, glm::vec4 colour)
	{
		float fTextWidth = getTextWidth(strText, fFontScaling);
		iPosX -= int(fTextWidth * 0.5f);
		iPosY -= int(getTextHeight(fFontScaling) * 0.5f);
		print(strText, iPosX, iPosY, iRenderTargetWidth, iRenderTargetHeight, fFontScaling, colour);
	}

	void ResourceFont::printInRect(bool bRenderText, const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, std::vector<std::string>& vstrTextLines, int& iTotalRenderedHeight, float fFontScaling, glm::vec4 colour)
	{
		std::stringstream ss(strText);
		std::string strNextWord;
		char delim = ' ';
		std::string strLineWhichFits;	// Current line of text which fits
		std::string strLineWithWord;
		int iTextLineOffset = (int)getTextHeight(fFontScaling);
		int iTextYoffset = iPosY;
		while (std::getline(ss, strNextWord, delim))
		{
			strLineWithWord = strLineWhichFits + strNextWord + " ";
			float fWidthWithWord = getTextWidth(strLineWithWord, fFontScaling);
			if (fWidthWithWord <= (float)iRenderTargetWidth)
			{
				strLineWhichFits += strNextWord + " ";
			}
			else
			{
				vstrTextLines.push_back(strLineWhichFits);
				if (bRenderText)
					print(vstrTextLines[vstrTextLines.size() - 1], iPosX, iPosY + iTextYoffset, iRenderTargetWidth, iRenderTargetHeight, fFontScaling, colour);
				iTextYoffset += iTextLineOffset;
				strLineWhichFits.clear();
				strLineWhichFits += strNextWord + " ";
			}
		}
		if (strLineWhichFits.length())
		{
			vstrTextLines.push_back(strLineWhichFits);
			if (bRenderText)
				print(vstrTextLines[vstrTextLines.size() - 1], iPosX, iPosY + iTextYoffset, iRenderTargetWidth, iRenderTargetHeight, fFontScaling, colour);
		}
		iTotalRenderedHeight = iTextYoffset + iTextLineOffset;
	}

	float ResourceFont::getTextWidth(const std::string& strText, float fFontScaling)
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

	float ResourceFont::getTextHeight(float fFontScaling)
	{
		return fontTypes.fMaxCharHeight * fFontScaling;
	}
}