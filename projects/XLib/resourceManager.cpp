#include "PCH.h"
#include "resourceManager.h"
#include "log.h"
#include "image.h"	// For building the ResourceFont files with buildFontFiles()
namespace X
{
	ResourceManager::ResourceManager()
	{

	}

	void ResourceManager::onGLContextToBeDestroyed(void)
	{
		// For each of the resources, call their onGLContextToBeDestroyed() methods.

		// Fonts
		std::map<std::string, ResourceFont*>::iterator itFont = _mmapResFonts.begin();
		while (itFont != _mmapResFonts.end())
		{
			itFont->second->onGLContextToBeDestroyed();
			itFont++;
		}

		// Depthbuffers
		std::map<std::string, ResourceDepthbuffer*>::iterator itDepthbuffer = _mmapResDepthbuffers.begin();
		while (itDepthbuffer != _mmapResDepthbuffers.end())
		{
			itDepthbuffer->second->onGLContextToBeDestroyed();
			itDepthbuffer++;
		}

		// Framebuffers
		std::map<std::string, ResourceFramebuffer*>::iterator itFramebuffer = _mmapResFramebuffers.begin();
		while (itFramebuffer != _mmapResFramebuffers.end())
		{
			itFramebuffer->second->onGLContextToBeDestroyed();
			itFramebuffer++;
		}

		// Shaders
		std::map<std::string, ResourceShader*>::iterator itShaders = _mmapResShaders.begin();
		while (itShaders != _mmapResShaders.end())
		{
			itShaders->second->onGLContextToBeDestroyed();
			itShaders++;
		}

		// Texture2Ds
		std::map<std::string, ResourceTexture2D*>::iterator itTextures2D = _mmapResTextures2D.begin();
		while (itTextures2D != _mmapResTextures2D.end())
		{
			itTextures2D->second->onGLContextToBeDestroyed();
			itTextures2D++;
		}

		// Triangle
		std::map<std::string, ResourceTriangle*>::iterator itResourceTriangles = _mmapResTriangles.begin();
		while (itResourceTriangles != _mmapResTriangles.end())
		{
			itResourceTriangles->second->onGLContextToBeDestroyed();
			itResourceTriangles++;
		}

		// Line
		std::map<std::string, ResourceLine*>::iterator itLine = _mmapResLines.begin();
		while (itLine != _mmapResLines.end())
		{
			itLine->second->onGLContextToBeDestroyed();
			itLine++;
		}
	}

	void ResourceManager::onGLContextRecreated(void)
	{
		// For each of the resources, call their onGLContextCreated() methods.

		// Fonts
		std::map<std::string, ResourceFont*>::iterator itFont = _mmapResFonts.begin();
		while (itFont != _mmapResFonts.end())
		{
			itFont->second->onGLContextCreated();
			itFont++;
		}

		// Depthbuffers
		std::map<std::string, ResourceDepthbuffer*>::iterator itDepthbuffer = _mmapResDepthbuffers.begin();
		while (itDepthbuffer != _mmapResDepthbuffers.end())
		{
			itDepthbuffer->second->onGLContextCreated();
			itDepthbuffer++;
		}

		// Framebuffers
		std::map<std::string, ResourceFramebuffer*>::iterator itFramebuffer = _mmapResFramebuffers.begin();
		while (itFramebuffer != _mmapResFramebuffers.end())
		{
			itFramebuffer->second->onGLContextCreated();
			itFramebuffer++;
		}

		// Shaders
		std::map<std::string, ResourceShader*>::iterator itShaders = _mmapResShaders.begin();
		while (itShaders != _mmapResShaders.end())
		{
			itShaders->second->onGLContextCreated();
			itShaders++;
		}

		// Texture 2Ds
		std::map<std::string, ResourceTexture2D*>::iterator itTextures2D = _mmapResTextures2D.begin();
		while (itTextures2D != _mmapResTextures2D.end())
		{
			itTextures2D->second->onGLContextCreated();
			itTextures2D++;
		}

		// Triangle
		std::map<std::string, ResourceTriangle*>::iterator itResourceTriangles = _mmapResTriangles.begin();
		while (itResourceTriangles != _mmapResTriangles.end())
		{
			itResourceTriangles->second->onGLContextCreated();
			itResourceTriangles++;
		}

		// Lines
		std::map<std::string, ResourceLine*>::iterator itLine = _mmapResLines.begin();
		while (itLine != _mmapResLines.end())
		{
			itLine->second->onGLContextCreated();
			itLine++;
		}
	}

	ResourceFont* ResourceManager::addFont(const std::string& strResourceName, const std::string& strFontFilename)
	{
		ResourceFont* pNewResource = new ResourceFont(strFontFilename);
		ThrowIfFalse(pNewResource, "ResourceManager::addFont(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResFonts[strResourceName] = pNewResource;
		return pNewResource;
	}

	ResourceFont* ResourceManager::getFont(const std::string& strResourceName)
	{
		std::map<std::string, ResourceFont*>::iterator it = _mmapResFonts.find(strResourceName);
		ThrowIfTrue(it == _mmapResFonts.end(), "ResourceManager::getFont(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second;
	}

	bool ResourceManager::getFontExists(const std::string& strResourceName)
	{
		return _mmapResFonts.find(strResourceName) != _mmapResFonts.end();
	}

	void ResourceManager::removeFont(const std::string& strResourceName)
	{
		std::map<std::string, ResourceFont*>::iterator it = _mmapResFonts.find(strResourceName);
		if (it == _mmapResFonts.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapResFonts.erase(it);
	}

	ResourceFramebuffer* ResourceManager::addFramebuffer(const std::string& strResourceName, unsigned int uiWidth, unsigned int uiHeight)
	{
		ResourceFramebuffer* pNewResource = new ResourceFramebuffer(uiWidth, uiHeight);
		ThrowIfFalse(pNewResource, "ResourceManager::addFramebuffer(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResFramebuffers[strResourceName] = pNewResource;
		return pNewResource;
	}

	ResourceFramebuffer* ResourceManager::getFramebuffer(const std::string& strResourceName)
	{
		std::map<std::string, ResourceFramebuffer*>::iterator it = _mmapResFramebuffers.find(strResourceName);
		ThrowIfTrue(it == _mmapResFramebuffers.end(), "ResourceManager::getFramebuffer(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second;
	}

	bool ResourceManager::getFramebufferExists(const std::string& strResourceName)
	{
		return _mmapResFramebuffers.find(strResourceName) != _mmapResFramebuffers.end();
	}

	void ResourceManager::removeFramebuffer(const std::string& strResourceName)
	{
		std::map<std::string, ResourceFramebuffer*>::iterator it = _mmapResFramebuffers.find(strResourceName);
		if (it == _mmapResFramebuffers.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapResFramebuffers.erase(it);
	}

	ResourceDepthbuffer* ResourceManager::addDepthbuffer(const std::string& strResourceName, unsigned int uiWidth, unsigned int uiHeight)
	{
		ResourceDepthbuffer* pNewResource = new ResourceDepthbuffer(uiWidth, uiHeight);
		ThrowIfFalse(pNewResource, "ResourceManager::addDepthbuffer(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResDepthbuffers[strResourceName] = pNewResource;
		return pNewResource;
	}

	ResourceDepthbuffer* ResourceManager::getDepthbuffer(const std::string& strResourceName)
	{
		std::map<std::string, ResourceDepthbuffer*>::iterator it = _mmapResDepthbuffers.find(strResourceName);
		ThrowIfTrue(it == _mmapResDepthbuffers.end(), "ResourceManager::getDepthbuffer(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second;
	}

	bool ResourceManager::getDepthbufferExists(const std::string& strResourceName)
	{
		return _mmapResDepthbuffers.find(strResourceName) != _mmapResDepthbuffers.end();
	}

	void ResourceManager::removeDepthbuffer(const std::string& strResourceName)
	{
		std::map<std::string, ResourceDepthbuffer*>::iterator it = _mmapResDepthbuffers.find(strResourceName);
		if (it == _mmapResDepthbuffers.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapResDepthbuffers.erase(it);
	}

	ResourceShader* ResourceManager::addShader(const std::string& strResourceName, const std::string& strVertexProgramFilename, const std::string& strFragmentProgramFilename)
	{
		ResourceShader* pNewResource = new ResourceShader(strVertexProgramFilename, strFragmentProgramFilename);
		ThrowIfFalse(pNewResource, "ResourceManager::addShader(" + strResourceName + ", " + strVertexProgramFilename + ", " + strFragmentProgramFilename + ") failed to allocate memory for new resource.");
		_mmapResShaders[strResourceName] = pNewResource;
		return pNewResource;
	}

	ResourceShader* ResourceManager::getShader(const std::string& strResourceName)
	{
		std::map<std::string, ResourceShader*>::iterator it = _mmapResShaders.find(strResourceName);
		ThrowIfTrue(it == _mmapResShaders.end(), "ResourceManager::getShader(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second;
	}

	bool ResourceManager::getShaderExists(const std::string& strResourceName)
	{
		return _mmapResShaders.find(strResourceName) != _mmapResShaders.end();
	}

	void ResourceManager::removeShader(const std::string& strResourceName)
	{
		std::map<std::string, ResourceShader*>::iterator it = _mmapResShaders.find(strResourceName);
		if (it == _mmapResShaders.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapResShaders.erase(it);
	}

	ResourceTexture2D* ResourceManager::addTexture2D(const std::string& strResourceName, const std::string& strImageFilename, bool bFlipYaxis)
	{
		ResourceTexture2D* pNewResource = new ResourceTexture2D(strImageFilename, bFlipYaxis);
		ThrowIfFalse(pNewResource, "ResourceManager::addTexture2D(" + strResourceName + ", " + strImageFilename + ") failed to allocate memory for new resource.");
		_mmapResTextures2D[strResourceName] = pNewResource;
		return pNewResource;
	}

	ResourceTexture2D* ResourceManager::getTexture2D(const std::string& strResourceName)
	{
		std::map<std::string, ResourceTexture2D*>::iterator it = _mmapResTextures2D.find(strResourceName);
		ThrowIfTrue(it == _mmapResTextures2D.end(), "ResourceManager::getTexture2D(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second;
	}

	bool ResourceManager::getTexture2DExists(const std::string& strResourceName)
	{
		return _mmapResTextures2D.find(strResourceName) != _mmapResTextures2D.end();
	}

	void ResourceManager::removeTexture2D(const std::string& strResourceName)
	{
		std::map<std::string, ResourceTexture2D*>::iterator it = _mmapResTextures2D.find(strResourceName);
		if (it == _mmapResTextures2D.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapResTextures2D.erase(it);
	}

	ResourceTriangle* ResourceManager::addTriangle(const std::string& strResourceName)
	{
		ResourceTriangle* pNewResource = new ResourceTriangle();
		ThrowIfFalse(pNewResource, "ResourceManager::addTriangle(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResTriangles[strResourceName] = pNewResource;
		return pNewResource;
	}

	ResourceTriangle* ResourceManager::getTriangle(const std::string& strResourceName)
	{
		std::map<std::string, ResourceTriangle*>::iterator it = _mmapResTriangles.find(strResourceName);
		ThrowIfTrue(it == _mmapResTriangles.end(), "ResourceManager::getTriangle(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second;
	}

	bool ResourceManager::getTriangleExists(const std::string& strResourceName)
	{
		return _mmapResTriangles.find(strResourceName) != _mmapResTriangles.end();
	}

	void ResourceManager::removeTriangle(const std::string& strResourceName)
	{
		std::map<std::string, ResourceTriangle*>::iterator it = _mmapResTriangles.find(strResourceName);
		if (it == _mmapResTriangles.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapResTriangles.erase(it);
	}

	ResourceLine* ResourceManager::addLine(const std::string& strResourceName)
	{
		ResourceLine* pNewResource = new ResourceLine();
		ThrowIfFalse(pNewResource, "ResourceManager::addLine(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResLines[strResourceName] = pNewResource;
		return pNewResource;
	}

	ResourceLine* ResourceManager::getLine(const std::string& strResourceName)
	{
		std::map<std::string, ResourceLine*>::iterator it = _mmapResLines.find(strResourceName);
		ThrowIfTrue(it == _mmapResLines.end(), "ResourceManager::getLine(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second;
	}

	bool ResourceManager::getLineExists(const std::string& strResourceName)
	{
		return _mmapResLines.find(strResourceName) != _mmapResLines.end();
	}

	void ResourceManager::removeLine(const std::string& strResourceName)
	{
		std::map<std::string, ResourceLine*>::iterator it = _mmapResLines.find(strResourceName);
		if (it == _mmapResLines.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapResLines.erase(it);
	}

	void ResourceManager::buildFontFiles(const std::string& strOutputBaseName, const std::string& strFontName, unsigned int iFontHeight, bool bAntialiased, bool bBold, bool bItalic, bool bUnderlined, bool bStrikeout)
	{
		// We need to use Windows GDI text rendering to obtain character spacing and dimension information.
		// We then take that, create an image holding each characters' image and saves the image to disk.
		// We also save all the character information to a .fnt file
		// This does NOT modify this object in anyway. It just builds the required files for later use.

		HFONT font;
		HFONT oldFont;
		HDC hDC = 0;
		TEXTMETRIC tm;

		int iAntiAliased = ANTIALIASED_QUALITY;
		if (!bAntialiased)
			iAntiAliased = NONANTIALIASED_QUALITY;
		int iActualHeight = -(int)iFontHeight;

		int iWeight = 400;	// 800 for BOLD
		if (bBold)
			iWeight = 800;

		// Attempt to generate font
		font = CreateFont(iActualHeight,// Height Of Font
			0,							// Width Of Font
			0,							// Angle Of Escapement
			0,							// Orientation Angle
			iWeight,					// Font Weight 400 for normal, 800 to bold
			bItalic,					// Italic
			bUnderlined,				// Underline
			bStrikeout,					// Strikeout
			ANSI_CHARSET,				// Character Set Identifier
			OUT_TT_PRECIS,				// Output Precision
			CLIP_DEFAULT_PRECIS,		// Clipping Precision
			iAntiAliased,				// Output Quality. Either ANTIALIASED_QUALITY or NONANTIALIASED_QUALITY
			FF_DONTCARE | DEFAULT_PITCH,// Family And Pitch
			StringToWString(strFontName).c_str());		// Font Name
		ThrowIfTrue(font == NULL, "ResourceManager::buildFontFiles() failed upon call to Win32 API call CreateFont()");

		hDC = CreateCompatibleDC(0);
		if (0 == hDC)
		{
			DeleteObject(font);
			ThrowIfTrue(1, "ResourceManager::buildFontFiles() failed upon call to Win32 API call CreateCompatibleDC()");
		}

		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkColor(hDC, RGB(0, 0, 0));
		SetBkMode(hDC, TRANSPARENT);
		SetMapMode(hDC, MM_TEXT);
		SetTextAlign(hDC, TA_TOP | TA_LEFT);
		oldFont = (HFONT)SelectObject(hDC, font);	// Store oldFont to return it when we're done.

		SelectObject(hDC, font);
		GetTextMetrics(hDC, &tm);
		float fMaxHeight = (float)tm.tmHeight;
		ResourceFont::CharDesc charDesc[256];
		ABCFLOAT abcf;
		// Get information for the font
		SelectObject(hDC, font);
		for (int i = 0; i < 256; ++i)
		{
			// Get dims of each character
			GetCharABCWidthsFloat(hDC, i, i, &abcf);
			charDesc[i].fABCa = abcf.abcfA;
			charDesc[i].fABCb = abcf.abcfB;
			charDesc[i].fABCc = abcf.abcfC;

			// Add some spacing (Due to possible antialiasing)
			charDesc[i].fABCa -= 1;
			charDesc[i].fABCb += 2;
			charDesc[i].fABCc -= 1;
		}

		glm::vec2 vTLPospx[256];	// Top left position of character within texture, in pixels

		// Now compute required size of power of 2 texture to store all characters
		glm::vec2 vFinalTextureDims(128, 128);

		// Find final texture dimensions required
		bool bFoundFinalTextureDims = false;
		while (false == bFoundFinalTextureDims)
		{
			bool bAllCharsFit = true;
			float fXCurPos = 0;
			float fYCurPos = 0;
			glm::vec2 vTexSpan;
			vTexSpan.x = 1.0f / vFinalTextureDims.x;
			vTexSpan.y = 1.0f / vFinalTextureDims.y;
			// Go through each char, trying to fit into currently sized texture dimensions.
			// If at any time, we discover that not all of the characters can not fit,
			// we simply increase the texture size and try again.
			for (int i = 0; i < 256; ++i)
			{
				// From current position, would current character fit?
				if (fXCurPos + charDesc[i].fABCb >= vFinalTextureDims.x)
				{
					// If we get here, the current character doesn't fit on current row
					// Move down to the next row, left edge, as might be enough room on next row
					fXCurPos = 0;
					fYCurPos += fMaxHeight;
					// Would char fit along X axis?
					if (charDesc[i].fABCb >= vFinalTextureDims.x)
					{
						// The width of the texture isn't even big enough to fit this single character
						bAllCharsFit = false;
						break;
					}
					// Is there enough room along Y axis?
					if (fYCurPos + fMaxHeight >= vFinalTextureDims.y)
					{
						bAllCharsFit = false;
						break;	// Not enough room along Y axis
					}
				}
				// If we get here, then char fits, simply store location and increase current X position
				vTLPospx[i].x = float(fXCurPos);
				vTLPospx[i].y = float(fYCurPos);
				fXCurPos += charDesc[i].fABCb;

			}	// For each character image

			// If bAllCharsFit = false. Then, we need to re-size texture dimensions
			if (false == bAllCharsFit)
			{
				if (vFinalTextureDims.y < vFinalTextureDims.x)
					vFinalTextureDims.y *= 2;
				else
					vFinalTextureDims.x *= 2;
			}
			else	// If bAllCharsFit = true, then all chars fit to texture
				bFoundFinalTextureDims = true;
		}

		// Create DC bitmap to put char images into
		BITMAPINFO bmi;
		HBITMAP hBMP;
		DWORD* pPixels;

		memset(&bmi, 0, sizeof(BITMAPINFO));
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

		bmi.bmiHeader.biHeight = (LONG)vFinalTextureDims.y;
		bmi.bmiHeader.biWidth = (LONG)vFinalTextureDims.x;
		hBMP = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, (void**)&pPixels, 0, 0);
		if (!hBMP)
		{
			SelectObject(hDC, oldFont);
			DeleteObject(font);
			DeleteDC(hDC);
			ThrowIfTrue(1, "ResourceManager::buildFontFiles() failed upon call to Win32 API call Create DIBSection()");
		}
		// Draw chars into bitmap
		SelectObject(hDC, hBMP);
		for (int i = 0; i < 256; ++i)
		{
			char ch = (char)i;
			TextOutA(hDC, int(vTLPospx[i].x - charDesc[i].fABCa), (int)vTLPospx[i].y, &ch, 1);
		}
		GdiFlush();

		// Create Image object from DIBsection ready to save to disk
		Image imgOut;
		imgOut.createBlank((unsigned int)vFinalTextureDims.x, (unsigned int)vFinalTextureDims.y, 4);

		// Compute filenames for the texture
		std::string strOutputNameBase = strOutputBaseName;
		transform(strOutputNameBase.begin(), strOutputNameBase.end(), strOutputNameBase.begin(), ::tolower);
		strOutputNameBase.append("_");
		strOutputNameBase.append(std::to_string(iFontHeight));

		unsigned char ucColour;
		unsigned int iIndex;
		for (int iX = 0; iX < (int)vFinalTextureDims.x; iX++)
		{
			for (int iY = 0; iY < (int)vFinalTextureDims.y; iY++)
			{
				iIndex = iX + (iY * int(vFinalTextureDims.x));
				ucColour = (unsigned char)pPixels[iIndex];
				imgOut.setPixel(iX, iY, ucColour, ucColour, ucColour, ucColour);
			}
		}

		// Save the image
		std::string strImagename = strOutputNameBase;
		strImagename.append(".png");
		imgOut.saveAsPNG(strImagename, true);

		// Compute texture coordinates
		// vTLPospx = Top left position of character within texture, in pixels
		glm::vec2 vTexSpan(1.0f / float(vFinalTextureDims.x), 1.0f / float(vFinalTextureDims.y));
		for (int i = 0; i < 256; ++i)
		{
			charDesc[i].vTexMin.x = vTLPospx[i].x;// - charDesc[i].fABCa;
			charDesc[i].vTexMin.y = vTLPospx[i].y;

			charDesc[i].vTexMax.x = charDesc[i].vTexMin.x;
			charDesc[i].vTexMax.y = charDesc[i].vTexMin.y;
			charDesc[i].vTexMax.x += charDesc[i].fABCb;	// Character image's width in pixels
			charDesc[i].vTexMax.y += fMaxHeight;		// Character image's height in pixels

			// Invert and swap
			charDesc[i].vTexMin.y = vFinalTextureDims.y - charDesc[i].vTexMin.y;
			charDesc[i].vTexMax.y = vFinalTextureDims.y - charDesc[i].vTexMax.y;
			float fTmp = charDesc[i].vTexMin.y;
			charDesc[i].vTexMin.y = charDesc[i].vTexMax.y;
			charDesc[i].vTexMax.y = fTmp;

			// Convert to 0.0f to 1.0f range
			charDesc[i].vTexMax.x *= vTexSpan.x;
			charDesc[i].vTexMax.y *= vTexSpan.y;
			charDesc[i].vTexMin.x *= vTexSpan.x;
			charDesc[i].vTexMin.y *= vTexSpan.y;
		}

		// Save final font data file
		FILE* f = 0;
		strOutputNameBase.append(".fnt");
		fopen_s(&f, strOutputNameBase.c_str(), "wb");
		ThrowIfTrue(!f, "ResourceManager::buildFontFiles() failed. Unable to open file for saving.");

		// Write out max char height of each of the fonts
		if (1 != fwrite(&fMaxHeight, sizeof(float), 1, f))
			ThrowIfTrue(1, "ResourceManager::buildFontFiles() failed. Error while saving.");

		// Write out each of the characters' info, for the normal font
		if (256 != fwrite(charDesc, sizeof(ResourceFont::CharDesc), 256, f))
		{
			fclose(f);
			ThrowIfTrue(!f, "ResourceManager::buildFontFiles() failed. Error while saving.");
		}

		fclose(f);

		// Cleanup
		DeleteObject(hBMP);
		SelectObject(hDC, oldFont);	// Return old object
		DeleteObject(font);
		DeleteDC(hDC);
	}
}