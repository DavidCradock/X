#include "PCH.h"
#include "resourceManager.h"
#include "log.h"
#include "image.h"	// For building the CResourceFont files with buildFontFiles()
#include "stringUtils.h"
#include "resourceLoadingScreen.h"

namespace X
{
	SCResourceManager::SCResourceManager()
	{

	}

	void SCResourceManager::onGLContextToBeDestroyed(void)
	{
		// For each of the resources, call their onGLContextToBeDestroyed() methods.

		// Depth buffers
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

		// Vertex buffers
		std::map<std::string, SResourceVertexBuffer>::iterator itResourceVertexBuffer = _mmapResVertexBuffers.begin();
		while (itResourceVertexBuffer != _mmapResVertexBuffers.end())
		{
			itResourceVertexBuffer->second.pResource->onGLContextToBeDestroyed();
			itResourceVertexBuffer++;
		}

		// Vertex buffers BNT
		std::map<std::string, SResourceVertexBufferBNT>::iterator itResourceVertexBufferBNT = _mmapResVertexBufferBNTs.begin();
		while (itResourceVertexBufferBNT != _mmapResVertexBufferBNTs.end())
		{
			itResourceVertexBufferBNT->second.pResource->onGLContextToBeDestroyed();
			itResourceVertexBufferBNT++;
		}

		// Vertex buffers instanced
		std::map<std::string, SResourceVertexBufferInstanced>::iterator itResourceVertexBufferInstanced = _mmapResVertexBufferInstanceds.begin();
		while (itResourceVertexBufferInstanced != _mmapResVertexBufferInstanceds.end())
		{
			itResourceVertexBufferInstanced->second.pResource->onGLContextToBeDestroyed();
			itResourceVertexBufferInstanced++;
		}

		// Vertex buffer lines
		std::map<std::string, SResourceVertexBufferLine>::iterator itVertexBufferLine = _mmapResVertexBufferLines.begin();
		while (itVertexBufferLine != _mmapResVertexBufferLines.end())
		{
			itVertexBufferLine->second.pResource->onGLContextToBeDestroyed();
			itVertexBufferLine++;
		}
	}

	void SCResourceManager::onGLContextRecreated(void)
	{
		// For each of the resources, call their onGLContextCreated() methods.

		// Depth buffers
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

		// Vertex buffers
		std::map<std::string, SResourceVertexBuffer>::iterator itResourceVertexBuffer = _mmapResVertexBuffers.begin();
		while (itResourceVertexBuffer != _mmapResVertexBuffers.end())
		{
			itResourceVertexBuffer->second.pResource->onGLContextCreated();
			itResourceVertexBuffer++;
		}

		// Vertex buffers BNT
		std::map<std::string, SResourceVertexBufferBNT>::iterator itResourceVertexBufferBNT = _mmapResVertexBufferBNTs.begin();
		while (itResourceVertexBufferBNT != _mmapResVertexBufferBNTs.end())
		{
			itResourceVertexBufferBNT->second.pResource->onGLContextCreated();
			itResourceVertexBufferBNT++;
		}

		// Vertex buffers
		std::map<std::string, SResourceVertexBufferInstanced>::iterator itResourceVertexBufferInstanced = _mmapResVertexBufferInstanceds.begin();
		while (itResourceVertexBufferInstanced != _mmapResVertexBufferInstanceds.end())
		{
			itResourceVertexBufferInstanced->second.pResource->onGLContextCreated();
			itResourceVertexBufferInstanced++;
		}

		// Vertex buffer lines
		std::map<std::string, SResourceVertexBufferLine>::iterator itVertexBufferLine = _mmapResVertexBufferLines.begin();
		while (itVertexBufferLine != _mmapResVertexBufferLines.end())
		{
			itVertexBufferLine->second.pResource->onGLContextCreated();
			itVertexBufferLine++;
		}
	}

	void SCResourceManager::buildFontFiles(const std::string& strOutputBaseName, const std::string& strFontName, unsigned int iFontHeight, int iWeight, bool bAntialiased, bool bItalic, bool bUnderlined, bool bStrikeout, bool bAppendHeightToFilename) const
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

//		int iWeight = 400;	// 800 for BOLD
//		if (bBold)
//			iWeight = 800;

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
			StringUtils::stringToWide(strFontName).c_str());		// Font Name
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

		CVector2f vTLPospx[256];	// Top left position of character within texture, in pixels

		// Now compute required size of power of 2 texture to store all characters
		CVector2f vFinalTextureDims(128, 128);

		// Find final texture dimensions required
		bool bFoundFinalTextureDims = false;
		while (false == bFoundFinalTextureDims)
		{
			bool bAllCharsFit = true;
			float fXCurPos = 0;
			float fYCurPos = 0;
			CVector2f vTexSpan;
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
		if (bAppendHeightToFilename)
		{
			strOutputNameBase.append("_");
			strOutputNameBase.append(std::to_string(iFontHeight));
		}
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
		CVector2f vTexSpan(1.0f / float(vFinalTextureDims.x), 1.0f / float(vFinalTextureDims.y));
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

	void SCResourceManager::addDefaultResources(void)
	{
		// NOTE:
		// When adding resources here, remember to add the resource to the comments at the top of SCResourceManager class in resourceManager.h

		SCResourceManager* pRM = SCResourceManager::getPointer();

		/******************************************************************************************************************************
		// Shaders
		******************************************************************************************************************************/
		// A shader used by the CResourceFont class to render text.
		pRM->addShader("X:font", "data/X/shaders/font.vert", "data/X/shaders/font.frag");
		// A shader used by the CResourceLine class to render lines.
		pRM->addShader("X:line", "data/X/shaders/line.vert", "data/X/shaders/line.frag");
		// A shader which has vertex position, colour, texture coordinates and diffuse, roughness, normals and emission textures and shadows
		pRM->addShader("X:DRNE", "data/X/shaders/DRNE.vert", "data/X/shaders/DRNE.frag");
		// A shader which has vertex position, colour, texture coordinates and diffuse, roughness, normals and emission textures and NO shadows
		pRM->addShader("X:DRNE_noshadows", "data/X/shaders/DRNE_noshadows.vert", "data/X/shaders/DRNE_noshadows.frag");
		// A shader which accepts vertex position, colour and texture coodinates
		pRM->addShader("X:pos_col_tex", "data/X/shaders/pos_col_tex.vert", "data/X/shaders/pos_col_tex.frag");
		// A shader for rendering a bound depth buffer to a 2D quad so we can view the depth values in the depth buffer
		pRM->addShader("X:depthbuffer_debug", "data/X/shaders/depthbuffer_debug.vert", "data/X/shaders/depthbuffer_debug.frag");
		// A shader used by the scene manager to render the depth map used for rendering shadows
		pRM->addShader("X:shadowdepthmap", "data/X/shaders/shadow_depthmap.vert", "data/X/shaders/shadow_depthmap.frag");
		// A shader used by the GUI to render everything.
		pRM->addShader("X:gui", "data/X/shaders/gui.vert", "data/X/shaders/gui.frag");
		// A shader used by SC2DRenderer for C2DEntity
		pRM->addShader("X:2DEntity", "data/X/shaders/2DEntity.vert", "data/X/shaders/2DEntity.frag");
		// A shader used by SC2DRenderer for C2DEntityRot
		pRM->addShader("X:2DEntityRot", "data/X/shaders/2DEntityRot.vert", "data/X/shaders/2DEntityRot.frag");
		// A shader used be C2DParticleSystem to render non-instanced particles.
		pRM->addShader("X:2DParticleNoInst", "data/X/shaders/2DParticleNoInstancing.vert", "data/X/shaders/2DParticleNoInstancing.frag");

		/******************************************************************************************************************************
		// Textures
		******************************************************************************************************************************/
		// A texture which is tiny and white.
		pRM->addTexture2DFromFile("X:default_white", "data/X/textures/default_white.png");
		// A texture which is grey for diffuse, used if not set
		pRM->addTexture2DFromFile("X:default_diffuse", "data/X/textures/default_diffuse.png");
		// A texture which is black for emission, used if not set
		pRM->addTexture2DFromFile("X:default_emission", "data/X/textures/default_emission.png");
		// A texture which is a flat normal map, used if not set
		pRM->addTexture2DFromFile("X:default_normal", "data/X/textures/default_normal.png");
		// A texture which is black for roughness used if not set
		pRM->addTexture2DFromFile("X:default_roughness", "data/X/textures/default_roughness.png");

		/******************************************************************************************************************************
		// Texture atlases
		******************************************************************************************************************************/
		// An atlas for use with rendering generic particles
		std::vector<std::string> vStrParticleImages = StringUtils::getFilesInDir("data/x/textures/particles/");
		pRM->addTexture2DAtlas("X:default_particle", vStrParticleImages, true, 1);
		
		/******************************************************************************************************************************
		// Depth buffers
		******************************************************************************************************************************/
		// A depth buffer which is used by scene managers to render shadows
		pRM->addDepthbuffer("X:shadows", 2048, 2048);

		/******************************************************************************************************************************
		// Framebuffers
		******************************************************************************************************************************/
		// A framebuffer stuff is rendered to and then at the end of the program loop, rendered to the backbuffer
		pRM->addFramebuffer("X:backbuffer_FB", 512, 512);	// Dims are set each program loop to match the window's dimensions
		// A framebuffer the GUI tooltips are rendered to
		pRM->addFramebuffer("X:guitooltipFB", 512, 512);	// Dims are set when rendering each tooltip's contents

		/******************************************************************************************************************************
		// Vertex buffers
		******************************************************************************************************************************/
		// A vertex buffer resource used for rendering 2D quads to the screen for debugging purposes, by the GUI and SC2DRenderer.
		pRM->addVertexBuffer("X:default");

		/******************************************************************************************************************************
		// Vertex buffers instanced
		******************************************************************************************************************************/
		// A vertex buffer resource used for rendering 2D quads to the screen for debugging purposes, by the GUI and SC2DRenderer.
		pRM->addVertexBufferInstanced("X:default");

		/******************************************************************************************************************************
		// Vertex buffers BNT
		******************************************************************************************************************************/
		// A vertex buffer resource used for rendering 2D quads to the screen for debugging purposes, by the GUI and SC2DRenderer.
		pRM->addVertexBufferBNT("X:default");

		/******************************************************************************************************************************
		// Vertex buffers line
		******************************************************************************************************************************/
		// A line vertex buffer resource used by the GUI when rendering lines
		pRM->addVertexBufferLine("X:default");
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
		// Call loading screen
		SCResourceLoadingScreen::getPointer()->loadingResource("Font", strResourceName);

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
		// Call loading screen
		SCResourceLoadingScreen::getPointer()->loadingResource("Texture2DAtlas", strResourceName);

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
		// Call loading screen
		SCResourceLoadingScreen::getPointer()->loadingResource("Texture2DFromFile", strResourceName);

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

	CResourceVertexBuffer* SCResourceManager::addVertexBuffer(const std::string& strResourceName)
	{
		// If resource already exists
		std::map<std::string, SResourceVertexBuffer>::iterator it = _mmapResVertexBuffers.find(strResourceName);
		if (it != _mmapResVertexBuffers.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceVertexBuffer newRes;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceVertexBuffer();
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addVertexBuffer(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResVertexBuffers[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceVertexBuffer* SCResourceManager::getVertexBuffer(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBuffer>::iterator it = _mmapResVertexBuffers.find(strResourceName);
		ThrowIfTrue(it == _mmapResVertexBuffers.end(), "SCResourceManager::getVertexBuffer(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getVertexBufferExists(const std::string& strResourceName)
	{
		return _mmapResVertexBuffers.find(strResourceName) != _mmapResVertexBuffers.end();
	}

	void SCResourceManager::removeVertexBuffer(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBuffer>::iterator it = _mmapResVertexBuffers.find(strResourceName);
		if (it == _mmapResVertexBuffers.end())
			return;	// Doesn't exist.
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResVertexBuffers.erase(it);
	}

	CResourceVertexBufferBNT* SCResourceManager::addVertexBufferBNT(const std::string& strResourceName)
	{
		// If resource already exists
		std::map<std::string, SResourceVertexBufferBNT>::iterator it = _mmapResVertexBufferBNTs.find(strResourceName);
		if (it != _mmapResVertexBufferBNTs.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceVertexBufferBNT newRes;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceVertexBufferBNT();
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addVertexBufferBNT(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResVertexBufferBNTs[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceVertexBufferBNT* SCResourceManager::getVertexBufferBNT(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferBNT>::iterator it = _mmapResVertexBufferBNTs.find(strResourceName);
		ThrowIfTrue(it == _mmapResVertexBufferBNTs.end(), "SCResourceManager::getVertexBufferBNT(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getVertexBufferBNTExists(const std::string& strResourceName)
	{
		return _mmapResVertexBufferBNTs.find(strResourceName) != _mmapResVertexBufferBNTs.end();
	}

	void SCResourceManager::removeVertexBufferBNT(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferBNT>::iterator it = _mmapResVertexBufferBNTs.find(strResourceName);
		if (it == _mmapResVertexBufferBNTs.end())
			return;	// Doesn't exist.
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResVertexBufferBNTs.erase(it);
	}

	CResourceVertexBufferInstanced* SCResourceManager::addVertexBufferInstanced(const std::string& strResourceName)
	{
		// If resource already exists
		std::map<std::string, SResourceVertexBufferInstanced>::iterator it = _mmapResVertexBufferInstanceds.find(strResourceName);
		if (it != _mmapResVertexBufferInstanceds.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceVertexBufferInstanced newRes;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceVertexBufferInstanced();
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addVertexBufferInstanced(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResVertexBufferInstanceds[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceVertexBufferInstanced* SCResourceManager::getVertexBufferInstanced(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferInstanced>::iterator it = _mmapResVertexBufferInstanceds.find(strResourceName);
		ThrowIfTrue(it == _mmapResVertexBufferInstanceds.end(), "SCResourceManager::getVertexBufferInstanced(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getVertexBufferInstancedExists(const std::string& strResourceName)
	{
		return _mmapResVertexBufferInstanceds.find(strResourceName) != _mmapResVertexBufferInstanceds.end();
	}

	void SCResourceManager::removeVertexBufferInstanced(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferInstanced>::iterator it = _mmapResVertexBufferInstanceds.find(strResourceName);
		if (it == _mmapResVertexBufferInstanceds.end())
			return;	// Doesn't exist.
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResVertexBufferInstanceds.erase(it);
	}

	CResourceVertexBufferLine* SCResourceManager::addVertexBufferLine(const std::string& strResourceName)
	{
		// If resource already exists
		std::map<std::string, SResourceVertexBufferLine>::iterator it = _mmapResVertexBufferLines.find(strResourceName);
		if (it != _mmapResVertexBufferLines.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceVertexBufferLine newRes;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceVertexBufferLine();
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addVertexBufferLine(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResVertexBufferLines[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceVertexBufferLine* SCResourceManager::getVertexBufferLine(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferLine>::iterator it = _mmapResVertexBufferLines.find(strResourceName);
		ThrowIfTrue(it == _mmapResVertexBufferLines.end(), "SCResourceManager::getVertexBufferLine(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getVertexBufferLineExists(const std::string& strResourceName)
	{
		return _mmapResVertexBufferLines.find(strResourceName) != _mmapResVertexBufferLines.end();
	}

	void SCResourceManager::removeVertexBufferLine(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferLine>::iterator it = _mmapResVertexBufferLines.find(strResourceName);
		if (it == _mmapResVertexBufferLines.end())
			return;	// Doesn't exist.
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResVertexBufferLines.erase(it);
	}
}