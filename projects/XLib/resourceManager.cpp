#include "PCH.h"
#include "resourceManager.h"
#include "log.h"
#include "image.h"	// For building the CResourceFont files with buildFontFiles()
namespace X
{
	SCResourceManager::SCResourceManager()
	{

	}

	void SCResourceManager::onGLContextToBeDestroyed(void)
	{
		// For each of the resources, call their onGLContextToBeDestroyed() methods.

		// Depthbuffers
		std::map<std::string, SResourceDepthbuffer>::iterator itDepthbuffer = _mmapResDepthbuffers.begin();
		while (itDepthbuffer != _mmapResDepthbuffers.end())
		{
			itDepthbuffer->second.pResource->onGLContextToBeDestroyed();
			itDepthbuffer++;
		}

		// Fonts
		std::map<std::string, SResourceFont>::iterator itFont = _mmapResFonts.begin();
		while (itFont != _mmapResFonts.end())
		{
			itFont->second.pResource->onGLContextToBeDestroyed();
			itFont++;
		}

		// Framebuffers
		std::map<std::string, SResourceFramebuffer>::iterator itFramebuffer = _mmapResFramebuffers.begin();
		while (itFramebuffer != _mmapResFramebuffers.end())
		{
			itFramebuffer->second.pResource->onGLContextToBeDestroyed();
			itFramebuffer++;
		}

		// Lines
		std::map<std::string, SResourceLine>::iterator itLine = _mmapResLines.begin();
		while (itLine != _mmapResLines.end())
		{
			itLine->second.pResource->onGLContextToBeDestroyed();
			itLine++;
		}

		// Shaders
		std::map<std::string, SResourceShader>::iterator itShaders = _mmapResShaders.begin();
		while (itShaders != _mmapResShaders.end())
		{
			itShaders->second.pResource->onGLContextToBeDestroyed();
			itShaders++;
		}

		// Texture atlases
		std::map<std::string, SResourceTexture2DAtlas>::iterator itTextureAtlas = _mmapResTextures2DAtlases.begin();
		while (itTextureAtlas != _mmapResTextures2DAtlases.end())
		{
			itTextureAtlas->second.pResource->onGLContextToBeDestroyed();
			itTextureAtlas++;
		}

		// Texture2DFromFiles
		std::map<std::string, SResourceTexture2DFromFile>::iterator itTextures2D = _mmapResTextures2D.begin();
		while (itTextures2D != _mmapResTextures2D.end())
		{
			itTextures2D->second.pResource->onGLContextToBeDestroyed();
			itTextures2D++;
		}

		// Texture2DFromImages
		std::map<std::string, SResourceTexture2DFromImage>::iterator itTextures2DFromImage = _mmapResTextures2DFromImage.begin();
		while (itTextures2DFromImage != _mmapResTextures2DFromImage.end())
		{
			itTextures2DFromImage->second.pResource->onGLContextToBeDestroyed();
			itTextures2DFromImage++;
		}

		// Triangle
		std::map<std::string, SResourceTriangle>::iterator itResourceTriangles = _mmapResTriangles.begin();
		while (itResourceTriangles != _mmapResTriangles.end())
		{
			itResourceTriangles->second.pResource->onGLContextToBeDestroyed();
			itResourceTriangles++;
		}
	}

	void SCResourceManager::onGLContextRecreated(void)
	{
		// For each of the resources, call their onGLContextCreated() methods.

		// Depthbuffers
		std::map<std::string, SResourceDepthbuffer>::iterator itDepthbuffer = _mmapResDepthbuffers.begin();
		while (itDepthbuffer != _mmapResDepthbuffers.end())
		{
			itDepthbuffer->second.pResource->onGLContextCreated();
			itDepthbuffer++;
		}

		// Fonts
		std::map<std::string, SResourceFont>::iterator itFont = _mmapResFonts.begin();
		while (itFont != _mmapResFonts.end())
		{
			itFont->second.pResource->onGLContextCreated();
			itFont++;
		}

		// Framebuffers
		std::map<std::string, SResourceFramebuffer>::iterator itFramebuffer = _mmapResFramebuffers.begin();
		while (itFramebuffer != _mmapResFramebuffers.end())
		{
			itFramebuffer->second.pResource->onGLContextCreated();
			itFramebuffer++;
		}

		// Lines
		std::map<std::string, SResourceLine>::iterator itLine = _mmapResLines.begin();
		while (itLine != _mmapResLines.end())
		{
			itLine->second.pResource->onGLContextCreated();
			itLine++;
		}

		// Shaders
		std::map<std::string, SResourceShader>::iterator itShaders = _mmapResShaders.begin();
		while (itShaders != _mmapResShaders.end())
		{
			itShaders->second.pResource->onGLContextCreated();
			itShaders++;
		}

		// Texture atlases
		std::map<std::string, SResourceTexture2DAtlas>::iterator itTextureAtlas = _mmapResTextures2DAtlases.begin();
		while (itTextureAtlas != _mmapResTextures2DAtlases.end())
		{
			itTextureAtlas->second.pResource->onGLContextCreated();
			itTextureAtlas++;
		}

		// Texture2DFromFiles
		std::map<std::string, SResourceTexture2DFromFile>::iterator itTextures2D = _mmapResTextures2D.begin();
		while (itTextures2D != _mmapResTextures2D.end())
		{
			itTextures2D->second.pResource->onGLContextCreated();
			itTextures2D++;
		}

		// Texture2DFromImages
		std::map<std::string, SResourceTexture2DFromImage>::iterator itTextures2DFromImage = _mmapResTextures2DFromImage.begin();
		while (itTextures2DFromImage != _mmapResTextures2DFromImage.end())
		{
			itTextures2DFromImage->second.pResource->onGLContextCreated();
			itTextures2DFromImage++;
		}

		// Triangle
		std::map<std::string, SResourceTriangle>::iterator itResourceTriangles = _mmapResTriangles.begin();
		while (itResourceTriangles != _mmapResTriangles.end())
		{
			itResourceTriangles->second.pResource->onGLContextCreated();
			itResourceTriangles++;
		}
	}

	void SCResourceManager::buildFontFiles(const std::string& strOutputBaseName, const std::string& strFontName, unsigned int iFontHeight, bool bAntialiased, bool bBold, bool bItalic, bool bUnderlined, bool bStrikeout) const
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
		ThrowIfTrue(font == NULL, "SCResourceManager::buildFontFiles() failed upon call to Win32 API call CreateFont()");

		hDC = CreateCompatibleDC(0);
		if (0 == hDC)
		{
			DeleteObject(font);
			ThrowIfTrue(1, "SCResourceManager::buildFontFiles() failed upon call to Win32 API call CreateCompatibleDC()");
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
		CResourceFont::CharDesc charDesc[256];
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
			ThrowIfTrue(1, "SCResourceManager::buildFontFiles() failed upon call to Win32 API call Create DIBSection()");
		}
		// Draw chars into bitmap
		SelectObject(hDC, hBMP);
		for (int i = 0; i < 256; ++i)
		{
			char ch = (char)i;
			TextOutA(hDC, int(vTLPospx[i].x - charDesc[i].fABCa), (int)vTLPospx[i].y, &ch, 1);
		}
		GdiFlush();

		// Create CImage object from DIBsection ready to save to disk
		CImage imgOut;
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
		ThrowIfTrue(!f, "SCResourceManager::buildFontFiles() failed. Unable to open file for saving.");

		// Write out max char height of each of the fonts
		if (1 != fwrite(&fMaxHeight, sizeof(float), 1, f))
			ThrowIfTrue(1, "SCResourceManager::buildFontFiles() failed. Error while saving.");

		// Write out each of the characters' info, for the normal font
		if (256 != fwrite(charDesc, sizeof(CResourceFont::CharDesc), 256, f))
		{
			fclose(f);
			ThrowIfTrue(!f, "SCResourceManager::buildFontFiles() failed. Error while saving.");
		}

		fclose(f);

		// Cleanup
		DeleteObject(hBMP);
		SelectObject(hDC, oldFont);	// Return old object
		DeleteObject(font);
		DeleteDC(hDC);
	}

	CResourceDepthbuffer* SCResourceManager::addDepthbuffer(const std::string& strResourceName, unsigned int uiWidth, unsigned int uiHeight)
	{
		// If resource already exists
		std::map<std::string, SResourceDepthbuffer>::iterator it = _mmapResDepthbuffers.find(strResourceName);
		if (it != _mmapResDepthbuffers.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceDepthbuffer newRes;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceDepthbuffer(uiWidth, uiHeight);
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addDepthbuffer(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResDepthbuffers[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceDepthbuffer* SCResourceManager::getDepthbuffer(const std::string& strResourceName)
	{
		std::map<std::string, SResourceDepthbuffer>::iterator it = _mmapResDepthbuffers.find(strResourceName);
		ThrowIfTrue(it == _mmapResDepthbuffers.end(), "SCResourceManager::getDepthbuffer(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getDepthbufferExists(const std::string& strResourceName)
	{
		return _mmapResDepthbuffers.find(strResourceName) != _mmapResDepthbuffers.end();
	}

	void SCResourceManager::removeDepthbuffer(const std::string& strResourceName)
	{
		std::map<std::string, SResourceDepthbuffer>::iterator it = _mmapResDepthbuffers.find(strResourceName);
		if (it == _mmapResDepthbuffers.end())
			return;	// Doesn't exist.
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResDepthbuffers.erase(it);
	}

	CResourceFont* SCResourceManager::addFont(const std::string& strResourceName, const std::string& strFontFilename)
	{
		// If resource already exists
		std::map<std::string, SResourceFont>::iterator it = _mmapResFonts.find(strResourceName);
		if (it != _mmapResFonts.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceFont newRes;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceFont(strFontFilename);
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addFont(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResFonts[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceFont* SCResourceManager::getFont(const std::string& strResourceName)
	{
		std::map<std::string, SResourceFont>::iterator it = _mmapResFonts.find(strResourceName);
		ThrowIfTrue(it == _mmapResFonts.end(), "SCResourceManager::getFont(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getFontExists(const std::string& strResourceName)
	{
		return _mmapResFonts.find(strResourceName) != _mmapResFonts.end();
	}

	void SCResourceManager::removeFont(const std::string& strResourceName)
	{
		std::map<std::string, SResourceFont>::iterator it = _mmapResFonts.find(strResourceName);
		if (it == _mmapResFonts.end())
			return;	// Doesn't exist.
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResFonts.erase(it);
	}

	CResourceFramebuffer* SCResourceManager::addFramebuffer(const std::string& strResourceName, unsigned int uiWidth, unsigned int uiHeight)
	{
		// If resource already exists
		std::map<std::string, SResourceFramebuffer>::iterator it = _mmapResFramebuffers.find(strResourceName);
		if (it != _mmapResFramebuffers.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceFramebuffer newRes;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceFramebuffer(uiWidth, uiHeight);
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addFramebuffer(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResFramebuffers[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceFramebuffer* SCResourceManager::getFramebuffer(const std::string& strResourceName)
	{
		std::map<std::string, SResourceFramebuffer>::iterator it = _mmapResFramebuffers.find(strResourceName);
		ThrowIfTrue(it == _mmapResFramebuffers.end(), "SCResourceManager::getFramebuffer(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getFramebufferExists(const std::string& strResourceName)
	{
		return _mmapResFramebuffers.find(strResourceName) != _mmapResFramebuffers.end();
	}

	void SCResourceManager::removeFramebuffer(const std::string& strResourceName)
	{
		std::map<std::string, SResourceFramebuffer>::iterator it = _mmapResFramebuffers.find(strResourceName);
		if (it == _mmapResFramebuffers.end())
			return;	// Doesn't exist.
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResFramebuffers.erase(it);
	}

	CResourceLine* SCResourceManager::addLine(const std::string& strResourceName)
	{
		// If resource already exists
		std::map<std::string, SResourceLine>::iterator it = _mmapResLines.find(strResourceName);
		if (it != _mmapResLines.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceLine newRes;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceLine();
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addLine(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResLines[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceLine* SCResourceManager::getLine(const std::string& strResourceName)
	{
		std::map<std::string, SResourceLine>::iterator it = _mmapResLines.find(strResourceName);
		ThrowIfTrue(it == _mmapResLines.end(), "SCResourceManager::getLine(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getLineExists(const std::string& strResourceName)
	{
		return _mmapResLines.find(strResourceName) != _mmapResLines.end();
	}

	void SCResourceManager::removeLine(const std::string& strResourceName)
	{
		std::map<std::string, SResourceLine>::iterator it = _mmapResLines.find(strResourceName);
		if (it == _mmapResLines.end())
			return;	// Doesn't exist.
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResLines.erase(it);
	}

	CResourceShader* SCResourceManager::addShader(const std::string& strResourceName, const std::string& strVertexProgramFilename, const std::string& strFragmentProgramFilename)
	{
		// If resource already exists
		std::map<std::string, SResourceShader>::iterator it = _mmapResShaders.find(strResourceName);
		if (it != _mmapResShaders.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceShader newRes;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceShader(strVertexProgramFilename, strFragmentProgramFilename);
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addShader(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResShaders[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceShader* SCResourceManager::getShader(const std::string& strResourceName)
	{
		std::map<std::string, SResourceShader>::iterator it = _mmapResShaders.find(strResourceName);
		ThrowIfTrue(it == _mmapResShaders.end(), "SCResourceManager::getShader(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getShaderExists(const std::string& strResourceName)
	{
		return _mmapResShaders.find(strResourceName) != _mmapResShaders.end();
	}

	void SCResourceManager::removeShader(const std::string& strResourceName)
	{
		std::map<std::string, SResourceShader>::iterator it = _mmapResShaders.find(strResourceName);
		if (it == _mmapResShaders.end())
			return;	// Doesn't exist.
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResShaders.erase(it);
	}

	CResourceTexture2DAtlas* SCResourceManager::addTexture2DAtlas(const std::string& strResourceName, const std::vector<std::string>& vecStrImageFilenames, bool bAllowRotationOfImages, unsigned int uiImageSpacing)
	{
		// If resource already exists
		std::map<std::string, SResourceTexture2DAtlas>::iterator it = _mmapResTextures2DAtlases.find(strResourceName);
		if (it != _mmapResTextures2DAtlases.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceTexture2DAtlas newRes;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceTexture2DAtlas(vecStrImageFilenames, bAllowRotationOfImages, uiImageSpacing);
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addTexture2DAtlas(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResTextures2DAtlases[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceTexture2DAtlas* SCResourceManager::getTexture2DAtlas(const std::string& strResourceName)
	{
		std::map<std::string, SResourceTexture2DAtlas>::iterator it = _mmapResTextures2DAtlases.find(strResourceName);
		ThrowIfTrue(it == _mmapResTextures2DAtlases.end(), "SCResourceManager::getTexture2DAtlas(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getTexture2DAtlasExists(const std::string& strResourceName)
	{
		return _mmapResTextures2DAtlases.find(strResourceName) != _mmapResTextures2DAtlases.end();
	}

	void SCResourceManager::removeTexture2DAtlas(const std::string& strResourceName)
	{
		std::map<std::string, SResourceTexture2DAtlas>::iterator it = _mmapResTextures2DAtlases.find(strResourceName);
		if (it == _mmapResTextures2DAtlases.end())
			return;	// Doesn't exist.
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
//		it->second.pResource->
		delete it->second.pResource;
		_mmapResTextures2DAtlases.erase(it);
	}

	CResourceTexture2DFromFile* SCResourceManager::addTexture2DFromFile(const std::string& strResourceName, const std::string& strImageFilename, bool bFlipYaxis)
	{
		// If resource already exists
		std::map<std::string, SResourceTexture2DFromFile>::iterator it = _mmapResTextures2D.find(strResourceName);
		if (it != _mmapResTextures2D.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceTexture2DFromFile newRes;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceTexture2DFromFile(strImageFilename, bFlipYaxis);
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addTexture2DFromFile(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResTextures2D[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceTexture2DFromFile* SCResourceManager::getTexture2DFromFile(const std::string& strResourceName)
	{
		std::map<std::string, SResourceTexture2DFromFile>::iterator it = _mmapResTextures2D.find(strResourceName);
		ThrowIfTrue(it == _mmapResTextures2D.end(), "SCResourceManager::getTexture2DFromFile(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getTexture2DFromFileExists(const std::string& strResourceName)
	{
		return _mmapResTextures2D.find(strResourceName) != _mmapResTextures2D.end();
	}

	void SCResourceManager::removeTexture2DFromFile(const std::string& strResourceName)
	{
		std::map<std::string, SResourceTexture2DFromFile>::iterator it = _mmapResTextures2D.find(strResourceName);
		if (it == _mmapResTextures2D.end())
			return;	// Doesn't exist.
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResTextures2D.erase(it);
	}

	CResourceTexture2DFromImage* SCResourceManager::addTexture2DFromImage(const std::string& strResourceName, const CImage& image)
	{
		// If resource already exists
		std::map<std::string, SResourceTexture2DFromImage>::iterator it = _mmapResTextures2DFromImage.find(strResourceName);
		if (it != _mmapResTextures2DFromImage.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceTexture2DFromImage newRes;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceTexture2DFromImage(image);
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addTexture2DFromImage(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResTextures2DFromImage[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceTexture2DFromImage* SCResourceManager::getTexture2DFromImage(const std::string& strResourceName)
	{
		std::map<std::string, SResourceTexture2DFromImage>::iterator it = _mmapResTextures2DFromImage.find(strResourceName);
		ThrowIfTrue(it == _mmapResTextures2DFromImage.end(), "SCResourceManager::getTexture2DFromImage(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getTexture2DFromImageExists(const std::string& strResourceName)
	{
		return _mmapResTextures2DFromImage.find(strResourceName) != _mmapResTextures2DFromImage.end();
	}

	void SCResourceManager::removeTexture2DFromImage(const std::string& strResourceName)
	{
		std::map<std::string, SResourceTexture2DFromImage>::iterator it = _mmapResTextures2DFromImage.find(strResourceName);
		if (it == _mmapResTextures2DFromImage.end())
			return;	// Doesn't exist.
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResTextures2DFromImage.erase(it);
	}

	CResourceTriangle* SCResourceManager::addTriangle(const std::string& strResourceName)
	{
		// If resource already exists
		std::map<std::string, SResourceTriangle>::iterator it = _mmapResTriangles.find(strResourceName);
		if (it != _mmapResTriangles.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceTriangle newRes;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceTriangle();
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addTriangle(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResTriangles[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceTriangle* SCResourceManager::getTriangle(const std::string& strResourceName)
	{
		std::map<std::string, SResourceTriangle>::iterator it = _mmapResTriangles.find(strResourceName);
		ThrowIfTrue(it == _mmapResTriangles.end(), "SCResourceManager::getTriangle(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getTriangleExists(const std::string& strResourceName)
	{
		return _mmapResTriangles.find(strResourceName) != _mmapResTriangles.end();
	}

	void SCResourceManager::removeTriangle(const std::string& strResourceName)
	{
		std::map<std::string, SResourceTriangle>::iterator it = _mmapResTriangles.find(strResourceName);
		if (it == _mmapResTriangles.end())
			return;	// Doesn't exist.
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResTriangles.erase(it);
	}
}