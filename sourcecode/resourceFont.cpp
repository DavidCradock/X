#include "PCH.h"
#include "resourceFont.h"
#include "utilities.h"
#include "log.h"
#include "resourceManager.h"

namespace X
{
	ResourceFont::ResourceFont(const std::string& strFontFilename)
	{
		// Compute filename pairs from parsed filename base
		std::string strFontFNTFilename = strFontFilename;
		strFontFNTFilename.append(".fnt");
		std::string strFontPNGFilename = strFontFilename;
		strFontPNGFilename.append(".png");

		// Create required resources (Not using manager, locally, as this object is responsible for onloss/reset of context.
		_mpResTexture = new ResourceTexture2D(strFontPNGFilename, true);
		_mpResVertexbuffer = new ResourceVertexbuffer();

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
		delete _mpResVertexbuffer;
	}

	void ResourceFont::onGLContextCreated(void)
	{
		_mpResTexture->onGLContextCreated();
		_mpResVertexbuffer->onGLContextCreated();
	}

	void ResourceFont::onGLContextToBeDestroyed(void)
	{
		_mpResTexture->onGLContextToBeDestroyed();
		_mpResVertexbuffer->onGLContextToBeDestroyed();
	}

	void ResourceFont::print(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, glm::vec4 colour)
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
		vDimensions.y = fontTypes.fMaxCharHeight;
		glm::vec2 tcBL, tcBR, tcTR, tcTL;
		_mpResVertexbuffer->removeGeom();
		for (unsigned int i = 0; i < strText.length(); ++i)
		{
			// Obtain character index
			const char* pCh = strText.c_str();
			charIndex = pCh[i];

			// Compute everything for this character 
			vPosition.x += fontTypes.charDesc[charIndex].fABCa;
			vDimensions.x = fontTypes.charDesc[charIndex].fABCb;
			tcBL.x = fontTypes.charDesc[charIndex].vTexMin.x;
			tcBL.y = fontTypes.charDesc[charIndex].vTexMin.y;
			tcTR.x = fontTypes.charDesc[charIndex].vTexMax.x;
			tcTR.y = fontTypes.charDesc[charIndex].vTexMax.y;
			tcBR.x = fontTypes.charDesc[charIndex].vTexMax.x;
			tcBR.y = fontTypes.charDesc[charIndex].vTexMin.y;
			tcTL.x = fontTypes.charDesc[charIndex].vTexMin.x;
			tcTL.y = fontTypes.charDesc[charIndex].vTexMax.y;

			// Add quad to vertex buffer
			_mpResVertexbuffer->addQuad2D(vPosition, vDimensions, colour, tcBL, tcBR, tcTR, tcTL);

			vPosition.x += fontTypes.charDesc[charIndex].fABCb + fontTypes.charDesc[charIndex].fABCc;
		}
		// Then finally upload to the GPU and draw everything.
		_mpResVertexbuffer->update();
		_mpResVertexbuffer->draw(false);

		// Unbind everything and reset states
		_mpResTexture->unbind(0);
		pShader->unbind();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	void ResourceFont::printCentered(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, glm::vec4 colour)
	{
		float fTextWidth = getTextWidth(strText);
		iPosX -= int(fTextWidth * 0.5f);
		iPosY -= int(getTextHeight() * 0.5f);
		print(strText, iPosX, iPosY, iRenderTargetWidth, iRenderTargetHeight, colour);
	}

	float ResourceFont::getTextWidth(const std::string& strText)
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
		return fWidth;
	}

	float ResourceFont::getTextHeight(void)
	{
		return fontTypes.fMaxCharHeight;
	}
}