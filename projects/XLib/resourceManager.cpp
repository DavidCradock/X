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
		SCLog* pLog = SCLog::getPointer();
		pLog->add("SCResourceManager::SCResourceManager() called.");
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

		// Vertex buffers CPT
		std::map<std::string, SResourceVertexBufferCPT>::iterator itResourceVertexBufferCPT = _mmapResVertexBufferCPTs.begin();
		while (itResourceVertexBufferCPT != _mmapResVertexBufferCPTs.end())
		{
			itResourceVertexBufferCPT->second.pResource->onGLContextToBeDestroyed();
			itResourceVertexBufferCPT++;
		}

		// Vertex buffers CPT2
		std::map<std::string, SResourceVertexBufferCPT2>::iterator itResourceVertexBufferCPT2 = _mmapResVertexBufferCPT2s.begin();
		while (itResourceVertexBufferCPT2 != _mmapResVertexBufferCPT2s.end())
		{
			itResourceVertexBufferCPT2->second.pResource->onGLContextToBeDestroyed();
			itResourceVertexBufferCPT2++;
		}

		// Vertex buffers BNT
		std::map<std::string, SResourceVertexBufferCPTBNT>::iterator itResourceVertexBufferCPTBNT = _mmapResVertexBufferCPTBNTs.begin();
		while (itResourceVertexBufferCPTBNT != _mmapResVertexBufferCPTBNTs.end())
		{
			itResourceVertexBufferCPTBNT->second.pResource->onGLContextToBeDestroyed();
			itResourceVertexBufferCPTBNT++;
		}

		// Vertex buffers instanced
		std::map<std::string, SResourceVertexBufferCPTInst>::iterator itResourceVertexBufferInstanced = _mmapResVertexBufferCPTInsts.begin();
		while (itResourceVertexBufferInstanced != _mmapResVertexBufferCPTInsts.end())
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

		// Vertex buffers CPT
		std::map<std::string, SResourceVertexBufferCPT>::iterator itResourceVertexBufferCPT = _mmapResVertexBufferCPTs.begin();
		while (itResourceVertexBufferCPT != _mmapResVertexBufferCPTs.end())
		{
			itResourceVertexBufferCPT->second.pResource->onGLContextCreated();
			itResourceVertexBufferCPT++;
		}

		// Vertex buffers CPT2
		std::map<std::string, SResourceVertexBufferCPT2>::iterator itResourceVertexBufferCPT2 = _mmapResVertexBufferCPT2s.begin();
		while (itResourceVertexBufferCPT2 != _mmapResVertexBufferCPT2s.end())
		{
			itResourceVertexBufferCPT2->second.pResource->onGLContextCreated();
			itResourceVertexBufferCPT2++;
		}

		// Vertex buffers BNT
		std::map<std::string, SResourceVertexBufferCPTBNT>::iterator itResourceVertexBufferCPTBNT = _mmapResVertexBufferCPTBNTs.begin();
		while (itResourceVertexBufferCPTBNT != _mmapResVertexBufferCPTBNTs.end())
		{
			itResourceVertexBufferCPTBNT->second.pResource->onGLContextCreated();
			itResourceVertexBufferCPTBNT++;
		}

		// Vertex buffers
		std::map<std::string, SResourceVertexBufferCPTInst>::iterator itResourceVertexBufferInstanced = _mmapResVertexBufferCPTInsts.begin();
		while (itResourceVertexBufferInstanced != _mmapResVertexBufferCPTInsts.end())
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
		//
		// NOTE 2:
		// This is called from the SCSingletons constructor and therefore cannot use the global x object

		SCResourceManager* pRM = SCResourceManager::getPointer();

		/******************************************************************************************************************************
		// Shaders
		******************************************************************************************************************************/
		// A shader which has vertex position, colour, texture coordinates and diffuse, roughness, normals and emission textures and shadows
		pRM->addShader("X:DRNE", "data/X/shaders/DRNE.vert", "data/X/shaders/DRNE.frag", true);
		// A shader which has vertex position, colour, texture coordinates and diffuse, roughness, normals and emission textures and NO shadows
		pRM->addShader("X:DRNE_noshadows", "data/X/shaders/DRNE_noshadows.vert", "data/X/shaders/DRNE_noshadows.frag", true);
		// A shader for rendering a bound depth buffer to a 2D quad so we can view the depth values in the depth buffer
		pRM->addShader("X:depthbuffer_debug", "data/X/shaders/depthbuffer_debug.vert", "data/X/shaders/depthbuffer_debug.frag", true);
		// A shader used by the scene manager to render the depth map used for rendering shadows
		pRM->addShader("X:shadowdepthmap", "data/X/shaders/shadow_depthmap.vert", "data/X/shaders/shadow_depthmap.frag", true);
		// A shader used by the GUI to render everything.
		pRM->addShader("X:gui", "data/X/shaders/gui.vert", "data/X/shaders/gui.frag", true);
		// A shader used with CResourceVertexBufferCPT
		pRM->addShader("X:VBCPT", "data/X/shaders/vertexBufferCPT.vert", "data/X/shaders/vertexBufferCPT.frag", true);
		// A shader used with CResourceVertexBufferCPT2
		pRM->addShader("X:VBCPT2", "data/X/shaders/vertexBufferCPT2.vert", "data/X/shaders/vertexBufferCPT2.frag", true);
		// A shader used with CResourceVertexBufferCPT
		pRM->addShader("X:VBCPTInst", "data/X/shaders/vertexBufferCPTInst.vert", "data/X/shaders/vertexBufferCPTInst.frag", true);
	
		/******************************************************************************************************************************
		// Textures
		******************************************************************************************************************************/
		// A texture which is tiny and white.
		pRM->addTexture2DFromFile("X:default_white", "data/X/textures/default_white.png", false, true);
		// A texture which is grey for diffuse, used if not set
		pRM->addTexture2DFromFile("X:default_diffuse", "data/X/textures/default_diffuse.png", false, true);
		// A texture which is black for emission, used if not set
		pRM->addTexture2DFromFile("X:default_emission", "data/X/textures/default_emission.png", false, true);
		// A texture which is a flat normal map, used if not set
		pRM->addTexture2DFromFile("X:default_normal", "data/X/textures/default_normal.png", false, true);
		// A texture which is black for roughness used if not set
		pRM->addTexture2DFromFile("X:default_roughness", "data/X/textures/default_roughness.png", false, true);

		/******************************************************************************************************************************
		// Texture atlases
		******************************************************************************************************************************/
		// An atlas for use with rendering generic particles
		std::vector<std::string> vStrParticleImages = StringUtils::getFilesInDir("data/x/textures/particles/");
		pRM->addTexture2DAtlas("X:default_particle", vStrParticleImages, true, 1, true);
		
		/******************************************************************************************************************************
		// Depth buffers
		******************************************************************************************************************************/
		// A depth buffer which is used by scene managers to render shadows
		pRM->addDepthbuffer("X:shadows", 2048, 2048, true);

		/******************************************************************************************************************************
		// Framebuffers
		******************************************************************************************************************************/
		// A framebuffer stuff is rendered to and then at the end of the program loop, rendered to the backbuffer
		pRM->addFramebuffer("X:backbuffer_FB", 512, 512, true);	// Dims are set each program loop to match the window's dimensions
		// A framebuffer the GUI tooltips are rendered to
		pRM->addFramebuffer("X:guitooltipFB", 512, 512, true);	// Dims are set when rendering each tooltip's contents
		// A framebuffer which the GUI is rendered to. It is set to the dimensions of the application's window as we don't want the GUI rendered to the possibly scaled back buffer
		pRM->addFramebuffer("X:gui", 512, 512, true);	// Dims are set each program loop to match the window's dimensions

		/******************************************************************************************************************************
		// Vertex buffers CPT
		******************************************************************************************************************************/
		// A vertex buffer resource used for rendering 2D quads to the screen for debugging purposes, by the GUI and SC2DRenderer.
		pRM->addVertexBufferCPT("X:default", true);

		/******************************************************************************************************************************
		// Vertex buffers CPT2
		******************************************************************************************************************************/
		// A vertex buffer resource used for rendering 2D quads to the screen for debugging purposes, by the GUI and SC2DRenderer.
		pRM->addVertexBufferCPT2("X:default", true);

		/******************************************************************************************************************************
		// Vertex buffers instanced
		******************************************************************************************************************************/
		// A vertex buffer resource used for rendering 2D quads to the screen for debugging purposes, by the GUI and SC2DRenderer.
		pRM->addVertexBufferCPTInst("X:default", true);

		/******************************************************************************************************************************
		// Vertex buffers CPTBNT
		******************************************************************************************************************************/
		// A vertex buffer resource used for rendering 2D quads to the screen for debugging purposes, by the GUI and SC2DRenderer.
		pRM->addVertexBufferCPTBNT("X:default", true);

		/******************************************************************************************************************************
		// Vertex buffers line
		******************************************************************************************************************************/
		// A line vertex buffer resource used by the GUI when rendering lines
		pRM->addVertexBufferLine("X:default", true);
	}

	void SCResourceManager::removeAllResources(void)
	{
		// Depth buffers
		std::map<std::string, SResourceDepthbuffer>::iterator itDepthBuffer = _mmapResDepthbuffers.begin();
		while (itDepthBuffer != _mmapResDepthbuffers.end())
		{
			if (itDepthBuffer->second.bLocked)	// Do not remove this resource
			{
				itDepthBuffer++;
				continue;
			}
			delete itDepthBuffer->second.pResource;
			_mmapResDepthbuffers.erase(itDepthBuffer);
			itDepthBuffer = _mmapResDepthbuffers.begin();
		}

		// Fonts
		std::map<std::string, SResourceFont>::iterator itFonts = _mmapResFonts.begin();
		while (itFonts != _mmapResFonts.end())
		{
			if (itFonts->second.bLocked)	// Do not remove this resource
			{
				itFonts++;
				continue;
			}
			delete itFonts->second.pResource;
			_mmapResFonts.erase(itFonts);
			itFonts = _mmapResFonts.begin();
		}

		// Frame buffers
		std::map<std::string, SResourceFramebuffer>::iterator itFrameBuffer = _mmapResFramebuffers.begin();
		while (itFrameBuffer != _mmapResFramebuffers.end())
		{
			if (itFrameBuffer->second.bLocked)	// Do not remove this resource
			{
				itFrameBuffer++;
				continue;
			}
			delete itFrameBuffer->second.pResource;
			_mmapResFramebuffers.erase(itFrameBuffer);
			itFrameBuffer = _mmapResFramebuffers.begin();
		}

		// Shaders
		std::map<std::string, SResourceShader>::iterator itShader = _mmapResShaders.begin();
		while (itShader != _mmapResShaders.end())
		{
			if (itShader->second.bLocked)	// Do not remove this resource
			{
				itShader++;
				continue;
			}
			delete itShader->second.pResource;
			_mmapResShaders.erase(itShader);
			itShader = _mmapResShaders.begin();
		}

		// Texture atlases
		std::map<std::string, SResourceTexture2DAtlas>::iterator itAtlas = _mmapResTextures2DAtlases.begin();
		while (itAtlas != _mmapResTextures2DAtlases.end())
		{
			if (itAtlas->second.bLocked)	// Do not remove this resource
			{
				itAtlas++;
				continue;
			}
			delete itAtlas->second.pResource;
			_mmapResTextures2DAtlases.erase(itAtlas);
			itAtlas = _mmapResTextures2DAtlases.begin();
		}

		// Textures from file
		std::map<std::string, SResourceTexture2DFromFile>::iterator itTextureFile = _mmapResTextures2D.begin();
		while (itTextureFile != _mmapResTextures2D.end())
		{
			if (itTextureFile->second.bLocked)	// Do not remove this resource
			{
				itTextureFile++;
				continue;
			}
			delete itTextureFile->second.pResource;
			_mmapResTextures2D.erase(itTextureFile);
			itTextureFile = _mmapResTextures2D.begin();
		}

		// Textures from image
		std::map<std::string, SResourceTexture2DFromImage>::iterator itTextureImage = _mmapResTextures2DFromImage.begin();
		while (itTextureImage != _mmapResTextures2DFromImage.end())
		{
			if (itTextureImage->second.bLocked)	// Do not remove this resource
			{
				itTextureImage++;
				continue;
			}
			delete itTextureImage->second.pResource;
			_mmapResTextures2DFromImage.erase(itTextureImage);
			itTextureImage = _mmapResTextures2DFromImage.begin();
		}

		// Vertex buffers CPT
		std::map<std::string, SResourceVertexBufferCPT>::iterator itVertexBufferCPT = _mmapResVertexBufferCPTs.begin();
		while (itVertexBufferCPT != _mmapResVertexBufferCPTs.end())
		{
			if (itVertexBufferCPT->second.bLocked)	// Do not remove this resource
			{
				itVertexBufferCPT++;
				continue;
			}
			delete itVertexBufferCPT->second.pResource;
			_mmapResVertexBufferCPTs.erase(itVertexBufferCPT);
			itVertexBufferCPT = _mmapResVertexBufferCPTs.begin();
		}

		// Vertex buffers CPT2
		std::map<std::string, SResourceVertexBufferCPT2>::iterator itVertexBufferCPT2 = _mmapResVertexBufferCPT2s.begin();
		while (itVertexBufferCPT2 != _mmapResVertexBufferCPT2s.end())
		{
			if (itVertexBufferCPT2->second.bLocked)	// Do not remove this resource
			{
				itVertexBufferCPT2++;
				continue;
			}
			delete itVertexBufferCPT2->second.pResource;
			_mmapResVertexBufferCPT2s.erase(itVertexBufferCPT2);
			itVertexBufferCPT2 = _mmapResVertexBufferCPT2s.begin();
		}

		// Vertex buffers CPTBNT
		std::map<std::string, SResourceVertexBufferCPTBNT>::iterator itVertexBufferCPTBNT = _mmapResVertexBufferCPTBNTs.begin();
		while (itVertexBufferCPTBNT != _mmapResVertexBufferCPTBNTs.end())
		{
			if (itVertexBufferCPTBNT->second.bLocked)	// Do not remove this resource
			{
				itVertexBufferCPTBNT++;
				continue;
			}
			delete itVertexBufferCPTBNT->second.pResource;
			_mmapResVertexBufferCPTBNTs.erase(itVertexBufferCPTBNT);
			itVertexBufferCPTBNT = _mmapResVertexBufferCPTBNTs.begin();
		}

		// Vertex buffers instanced
		std::map<std::string, SResourceVertexBufferCPTInst>::iterator itVertexBufferInstance = _mmapResVertexBufferCPTInsts.begin();
		while (itVertexBufferInstance != _mmapResVertexBufferCPTInsts.end())
		{
			if (itVertexBufferInstance->second.bLocked)	// Do not remove this resource
			{
				itVertexBufferInstance++;
				continue;
			}
			delete itVertexBufferInstance->second.pResource;
			_mmapResVertexBufferCPTInsts.erase(itVertexBufferInstance);
			itVertexBufferInstance = _mmapResVertexBufferCPTInsts.begin();
		}

		// Vertex buffers lines
		std::map<std::string, SResourceVertexBufferLine>::iterator itVertexBufferLine = _mmapResVertexBufferLines.begin();
		while (itVertexBufferLine != _mmapResVertexBufferLines.end())
		{
			if (itVertexBufferLine->second.bLocked)	// Do not remove this resource
			{
				itVertexBufferLine++;
				continue;
			}
			delete itVertexBufferLine->second.pResource;
			_mmapResVertexBufferLines.erase(itVertexBufferLine);
			itVertexBufferLine = _mmapResVertexBufferLines.begin();
		}

	}

	CResourceDepthbuffer* SCResourceManager::addDepthbuffer(const std::string& strResourceName, unsigned int uiWidth, unsigned int uiHeight, bool bLocked)
	{
		// If resource already exists
		std::map<std::string, SResourceDepthbuffer>::iterator it = _mmapResDepthbuffers.find(strResourceName);
		if (it != _mmapResDepthbuffers.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceDepthbuffer newRes;
		newRes.bLocked = bLocked;
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
		if (it->second.bLocked)	// Locked
			return;
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResDepthbuffers.erase(it);
	}

	CResourceFont* SCResourceManager::addFont(const std::string& strResourceName, const std::string& strFontFilename, bool bLocked)
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
		newRes.bLocked = bLocked;
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
		if (it->second.bLocked)	// Locked
			return;
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResFonts.erase(it);
	}

	CResourceFramebuffer* SCResourceManager::addFramebuffer(const std::string& strResourceName, unsigned int uiWidth, unsigned int uiHeight, bool bLocked)
	{
		// If resource already exists
		std::map<std::string, SResourceFramebuffer>::iterator it = _mmapResFramebuffers.find(strResourceName);
		if (it != _mmapResFramebuffers.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceFramebuffer newRes;
		newRes.bLocked = bLocked;
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
		if (it->second.bLocked)	// Locked
			return;
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResFramebuffers.erase(it);
	}

	CResourceShader* SCResourceManager::addShader(const std::string& strResourceName, const std::string& strVertexProgramFilename, const std::string& strFragmentProgramFilename, bool bLocked)
	{
		// If resource already exists
		std::map<std::string, SResourceShader>::iterator it = _mmapResShaders.find(strResourceName);
		if (it != _mmapResShaders.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceShader newRes;
		newRes.bLocked = bLocked;
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
		if (it->second.bLocked)	// Locked
			return;
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResShaders.erase(it);
	}

	CResourceTexture2DAtlas* SCResourceManager::addTexture2DAtlas(const std::string& strResourceName, const std::vector<std::string>& vecStrImageFilenames, bool bAllowRotationOfImages, unsigned int uiImageSpacing, bool bLocked)
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
		newRes.bLocked = bLocked;
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
		if (it->second.bLocked)	// Locked
			return;
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
//		it->second.pResource->
		delete it->second.pResource;
		_mmapResTextures2DAtlases.erase(it);
	}

	CResourceTexture2DFromFile* SCResourceManager::addTexture2DFromFile(const std::string& strResourceName, const std::string& strImageFilename, bool bFlipYaxis, bool bLocked)
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
		newRes.bLocked = bLocked;
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
		if (it->second.bLocked)	// Locked
			return;
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResTextures2D.erase(it);
	}

	CResourceTexture2DFromImage* SCResourceManager::addTexture2DFromImage(const std::string& strResourceName, const CImage& image, bool bLocked)
	{
		// If resource already exists
		std::map<std::string, SResourceTexture2DFromImage>::iterator it = _mmapResTextures2DFromImage.find(strResourceName);
		if (it != _mmapResTextures2DFromImage.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceTexture2DFromImage newRes;
		newRes.bLocked = bLocked;
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
		if (it->second.bLocked)	// Locked
			return;
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResTextures2DFromImage.erase(it);
	}

	CResourceVertexBufferCPT* SCResourceManager::addVertexBufferCPT(const std::string& strResourceName, bool bLocked)
	{
		// If resource already exists
		std::map<std::string, SResourceVertexBufferCPT>::iterator it = _mmapResVertexBufferCPTs.find(strResourceName);
		if (it != _mmapResVertexBufferCPTs.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceVertexBufferCPT newRes;
		newRes.bLocked = bLocked;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceVertexBufferCPT();
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addVertexBufferCPT(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResVertexBufferCPTs[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceVertexBufferCPT* SCResourceManager::getVertexBufferCPT(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferCPT>::iterator it = _mmapResVertexBufferCPTs.find(strResourceName);
		ThrowIfTrue(it == _mmapResVertexBufferCPTs.end(), "SCResourceManager::getVertexBufferCPT(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getVertexBufferCPTExists(const std::string& strResourceName)
	{
		return _mmapResVertexBufferCPTs.find(strResourceName) != _mmapResVertexBufferCPTs.end();
	}

	void SCResourceManager::removeVertexBufferCPT(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferCPT>::iterator it = _mmapResVertexBufferCPTs.find(strResourceName);
		if (it == _mmapResVertexBufferCPTs.end())
			return;	// Doesn't exist.
		if (it->second.bLocked)	// Locked
			return;
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResVertexBufferCPTs.erase(it);
	}

	CResourceVertexBufferCPT2* SCResourceManager::addVertexBufferCPT2(const std::string& strResourceName, bool bLocked)
	{
		// If resource already exists
		std::map<std::string, SResourceVertexBufferCPT2>::iterator it = _mmapResVertexBufferCPT2s.find(strResourceName);
		if (it != _mmapResVertexBufferCPT2s.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceVertexBufferCPT2 newRes;
		newRes.bLocked = bLocked;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceVertexBufferCPT2();
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addVertexBufferCPT2(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResVertexBufferCPT2s[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceVertexBufferCPT2* SCResourceManager::getVertexBufferCPT2(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferCPT2>::iterator it = _mmapResVertexBufferCPT2s.find(strResourceName);
		ThrowIfTrue(it == _mmapResVertexBufferCPT2s.end(), "SCResourceManager::getVertexBufferCPT2(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getVertexBufferCPT2Exists(const std::string& strResourceName)
	{
		return _mmapResVertexBufferCPT2s.find(strResourceName) != _mmapResVertexBufferCPT2s.end();
	}

	void SCResourceManager::removeVertexBufferCPT2(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferCPT2>::iterator it = _mmapResVertexBufferCPT2s.find(strResourceName);
		if (it == _mmapResVertexBufferCPT2s.end())
			return;	// Doesn't exist.
		if (it->second.bLocked)	// Locked
			return;
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResVertexBufferCPT2s.erase(it);
	}

	CResourceVertexBufferCPTBNT* SCResourceManager::addVertexBufferCPTBNT(const std::string& strResourceName, bool bLocked)
	{
		// If resource already exists
		std::map<std::string, SResourceVertexBufferCPTBNT>::iterator it = _mmapResVertexBufferCPTBNTs.find(strResourceName);
		if (it != _mmapResVertexBufferCPTBNTs.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceVertexBufferCPTBNT newRes;
		newRes.bLocked = bLocked;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceVertexBufferCPTBNT();
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addVertexBufferCPTBNT(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResVertexBufferCPTBNTs[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceVertexBufferCPTBNT* SCResourceManager::getVertexBufferCPTBNT(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferCPTBNT>::iterator it = _mmapResVertexBufferCPTBNTs.find(strResourceName);
		ThrowIfTrue(it == _mmapResVertexBufferCPTBNTs.end(), "SCResourceManager::getVertexBufferCPTBNT(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getVertexBufferCPTBNTExists(const std::string& strResourceName)
	{
		return _mmapResVertexBufferCPTBNTs.find(strResourceName) != _mmapResVertexBufferCPTBNTs.end();
	}

	void SCResourceManager::removeVertexBufferCPTBNT(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferCPTBNT>::iterator it = _mmapResVertexBufferCPTBNTs.find(strResourceName);
		if (it == _mmapResVertexBufferCPTBNTs.end())
			return;	// Doesn't exist.
		if (it->second.bLocked)	// Locked
			return;
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResVertexBufferCPTBNTs.erase(it);
	}

	CResourceVertexBufferCPTInst* SCResourceManager::addVertexBufferCPTInst(const std::string& strResourceName, bool bLocked)
	{
		// If resource already exists
		std::map<std::string, SResourceVertexBufferCPTInst>::iterator it = _mmapResVertexBufferCPTInsts.find(strResourceName);
		if (it != _mmapResVertexBufferCPTInsts.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceVertexBufferCPTInst newRes;
		newRes.bLocked = bLocked;
		newRes.uiCount = 1;
		newRes.pResource = new CResourceVertexBufferCPTInst();
		ThrowIfFalse(newRes.pResource, "SCResourceManager::addVertexBufferCPTInst(" + strResourceName + ") failed to allocate memory for new resource.");
		_mmapResVertexBufferCPTInsts[strResourceName] = newRes;
		return newRes.pResource;
	}

	CResourceVertexBufferCPTInst* SCResourceManager::getVertexBufferCPTInst(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferCPTInst>::iterator it = _mmapResVertexBufferCPTInsts.find(strResourceName);
		ThrowIfTrue(it == _mmapResVertexBufferCPTInsts.end(), "SCResourceManager::getVertexBufferCPTInst(" + strResourceName + ") failed. Named resource doesn't exist.");
		return it->second.pResource;
	}

	bool SCResourceManager::getVertexBufferCPTInstExists(const std::string& strResourceName)
	{
		return _mmapResVertexBufferCPTInsts.find(strResourceName) != _mmapResVertexBufferCPTInsts.end();
	}

	void SCResourceManager::removeVertexBufferCPTInst(const std::string& strResourceName)
	{
		std::map<std::string, SResourceVertexBufferCPTInst>::iterator it = _mmapResVertexBufferCPTInsts.find(strResourceName);
		if (it == _mmapResVertexBufferCPTInsts.end())
			return;	// Doesn't exist.
		if (it->second.bLocked)	// Locked
			return;
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResVertexBufferCPTInsts.erase(it);
	}

	CResourceVertexBufferLine* SCResourceManager::addVertexBufferLine(const std::string& strResourceName, bool bLocked)
	{
		// If resource already exists
		std::map<std::string, SResourceVertexBufferLine>::iterator it = _mmapResVertexBufferLines.find(strResourceName);
		if (it != _mmapResVertexBufferLines.end())
		{
			it->second.uiCount++;
			return it->second.pResource;
		}
		SResourceVertexBufferLine newRes;
		newRes.bLocked = bLocked;
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
		if (it->second.bLocked)	// Locked
			return;
		if (it->second.uiCount > 1)
		{
			it->second.uiCount--;
			return;
		}
		delete it->second.pResource;
		_mmapResVertexBufferLines.erase(it);
	}
}