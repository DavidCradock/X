#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "image.h"
#include "log.h"
#include "window.h"
#include "resourceManager.h"

namespace X
{

	CResourceTexture2DAtlas::CResourceTexture2DAtlas(const std::vector<std::string>& vecStrImageFilenames, bool bAllowRotationOfImages, unsigned int uiImageSpacing)
	{
		// Upon construction, we need to load each image in and store inside atlas images.
		SCWindow* pWindow = SCWindow::getPointer();
		unsigned int uiMaxTextureSize = pWindow->getMaxTextureSize();

		// Load all images and create atlas images
		_mAtlases.createAtlasImages(vecStrImageFilenames, uiMaxTextureSize, uiMaxTextureSize, bAllowRotationOfImages, uiImageSpacing);

		// Create texture IDs for each of the atlas images
		for (unsigned int ui = 0; ui < _mAtlases.getNumAtlases(); ui++)
		{
			_mvAtlasTextureIDs.push_back(0);
		}

		onGLContextCreated();
	}

	CResourceTexture2DAtlas::~CResourceTexture2DAtlas()
	{
		onGLContextToBeDestroyed();
		_mvAtlasTextureIDs.clear();
	}

	void CResourceTexture2DAtlas::onGLContextCreated(void)
	{
		// For each of the atlases, create a texture
		for (unsigned int ui = 0; ui < _mAtlases.getNumAtlases(); ui++)
		{
			glGenTextures(1, &_mvAtlasTextureIDs[ui]);
			glBindTexture(GL_TEXTURE_2D, _mvAtlasTextureIDs[ui]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			CImage* pImage = _mAtlases.getAtlasImage(ui);
			if (3 == pImage->getNumChannels())
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pImage->getWidth(), pImage->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->getData());
			else  // We'll assume 4
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pImage->getWidth(), pImage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage->getData());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	void CResourceTexture2DAtlas::onGLContextToBeDestroyed(void)
	{
		// For each of the atlases, delete texture
		for (unsigned int ui = 0; ui < _mAtlases.getNumAtlases(); ui++)
		{
			glDeleteTextures(1, &_mvAtlasTextureIDs[ui]);
			_mvAtlasTextureIDs[ui] = 0;
		}
	}

	void CResourceTexture2DAtlas::bind(unsigned int uiTextureUnit, unsigned int uiImageNumber) const
	{
		std::vector<CImageAtlasDetails>* pviad = _mAtlases.getAllImageDetailsPointer();
		
		ThrowIfTrue(uiImageNumber >= pviad->size(), "CResourceTexture2DAtlas::bind() given invalid image number.");

		switch (uiTextureUnit)
		{
		case 0:
			glActiveTexture(GL_TEXTURE0);
			break;
		case 1:
			glActiveTexture(GL_TEXTURE1);
			break;
		case 2:
			glActiveTexture(GL_TEXTURE2);
			break;
		case 3:
			glActiveTexture(GL_TEXTURE3);
			break;
		case 4:
			glActiveTexture(GL_TEXTURE4);
			break;
		case 5:
			glActiveTexture(GL_TEXTURE5);
			break;
		case 6:
			glActiveTexture(GL_TEXTURE6);
			break;
		case 7:
			glActiveTexture(GL_TEXTURE7);
			break;
		}
		glBindTexture(GL_TEXTURE_2D, _mvAtlasTextureIDs[(*pviad)[uiImageNumber].uiAtlasImage]);
	}

	void CResourceTexture2DAtlas::bind(unsigned int uiTextureUnit, const std::string& strImageName) const
	{
		CImageAtlasDetails iad = _mAtlases.getImageDetails(strImageName);
		
		switch (uiTextureUnit)
		{
		case 0:
			glActiveTexture(GL_TEXTURE0);
			break;
		case 1:
			glActiveTexture(GL_TEXTURE1);
			break;
		case 2:
			glActiveTexture(GL_TEXTURE2);
			break;
		case 3:
			glActiveTexture(GL_TEXTURE3);
			break;
		case 4:
			glActiveTexture(GL_TEXTURE4);
			break;
		case 5:
			glActiveTexture(GL_TEXTURE5);
			break;
		case 6:
			glActiveTexture(GL_TEXTURE6);
			break;
		case 7:
			glActiveTexture(GL_TEXTURE7);
			break;
		}
		glBindTexture(GL_TEXTURE_2D, _mvAtlasTextureIDs[iad.uiAtlasImage]);
	}

	void CResourceTexture2DAtlas::bindAtlas(unsigned int uiTextureUnit, unsigned int uiAtlasImageNumber) const
	{
		ThrowIfTrue(uiAtlasImageNumber >= _mvAtlasTextureIDs.size(), "CResourceTexture2DAtlas::bindAtlas() given invalid image number.");

		switch (uiTextureUnit)
		{
		case 0:
			glActiveTexture(GL_TEXTURE0);
			break;
		case 1:
			glActiveTexture(GL_TEXTURE1);
			break;
		case 2:
			glActiveTexture(GL_TEXTURE2);
			break;
		case 3:
			glActiveTexture(GL_TEXTURE3);
			break;
		case 4:
			glActiveTexture(GL_TEXTURE4);
			break;
		case 5:
			glActiveTexture(GL_TEXTURE5);
			break;
		case 6:
			glActiveTexture(GL_TEXTURE6);
			break;
		case 7:
			glActiveTexture(GL_TEXTURE7);
			break;
		}
		glBindTexture(GL_TEXTURE_2D, _mvAtlasTextureIDs[uiAtlasImageNumber]);
	}

	void CResourceTexture2DAtlas::unbind(unsigned int uiTextureUnit) const
	{
		switch (uiTextureUnit)
		{
		case 0:
			glActiveTexture(GL_TEXTURE0);
			break;
		case 1:
			glActiveTexture(GL_TEXTURE1);
			break;
		case 2:
			glActiveTexture(GL_TEXTURE2);
			break;
		case 3:
			glActiveTexture(GL_TEXTURE3);
			break;
		case 4:
			glActiveTexture(GL_TEXTURE4);
			break;
		case 5:
			glActiveTexture(GL_TEXTURE5);
			break;
		case 6:
			glActiveTexture(GL_TEXTURE6);
			break;
		case 7:
			glActiveTexture(GL_TEXTURE7);
			break;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void CResourceTexture2DAtlas::unbindAll(void) const
	{
		glActiveTexture(GL_TEXTURE7);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE6);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE5);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE4);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE3);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE2);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);	glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);	glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned int CResourceTexture2DAtlas::getNumImages(void) const
	{
		return _mAtlases.getNumIndividualImages();
	}

	void CResourceTexture2DAtlas::getTextureCoords(unsigned int uiImageNumber, CVector2f& vTCMin, CVector2f& vTCMax) const
	{
		std::vector<CImageAtlasDetails>* pviad = _mAtlases.getAllImageDetailsPointer();
		ThrowIfTrue(uiImageNumber >= pviad->size(), "CResourceTexture2DAtlas::getTextureCoords() given invalid image number.");
		vTCMin.x = (float)(*pviad)[uiImageNumber].sTexCoords.top_left.x;
		vTCMin.y = (float)(*pviad)[uiImageNumber].sTexCoords.top_left.y;

		vTCMax.x = (float)(*pviad)[uiImageNumber].sTexCoords.bottom_right.x;
		vTCMax.y = (float)(*pviad)[uiImageNumber].sTexCoords.bottom_right.y;
	}

	void CResourceTexture2DAtlas::getTextureCoords(unsigned int uiImageNumber, CVector2f& vtcTopLeft, CVector2f& vtcTopRight, CVector2f& vtcBottomRight, CVector2f& vtcBottomLeft) const
	{
		std::vector<CImageAtlasDetails>* pviad = _mAtlases.getAllImageDetailsPointer();
		ThrowIfTrue(uiImageNumber >= pviad->size(), "CResourceTexture2DAtlas::getTextureCoords() given invalid image number.");
		vtcTopLeft = (*pviad)[uiImageNumber].sTexCoords.top_left;
		vtcTopRight = (*pviad)[uiImageNumber].sTexCoords.top_right;
		vtcBottomRight = (*pviad)[uiImageNumber].sTexCoords.bottom_right;
		vtcBottomLeft = (*pviad)[uiImageNumber].sTexCoords.bottom_left;
	}

	void CResourceTexture2DAtlas::getTextureCoords(const std::string& strImageName, CVector2f& vTCMin, CVector2f& vTCMax) const
	{
		CImageAtlasDetails iad = _mAtlases.getImageDetails(strImageName);
		vTCMin.x = (float)iad.sTexCoords.top_left.x;
		vTCMin.y = (float)iad.sTexCoords.top_left.y;
		vTCMax.x = (float)iad.sTexCoords.bottom_right.x;
		vTCMax.y = (float)iad.sTexCoords.bottom_right.y;
	}

	void CResourceTexture2DAtlas::getTextureCoords(const std::string& strImageName, CVector2f& vtcTopLeft, CVector2f& vtcTopRight, CVector2f& vtcBottomRight, CVector2f& vtcBottomLeft) const
	{
		CImageAtlasDetails iad = _mAtlases.getImageDetails(strImageName);

		vtcTopLeft = iad.sTexCoords.top_left;
		vtcTopRight = iad.sTexCoords.top_right;
		vtcBottomRight = iad.sTexCoords.bottom_right;
		vtcBottomLeft = iad.sTexCoords.bottom_left;
	}

	unsigned int CResourceTexture2DAtlas::getNumAtlases(void) const
	{
		return _mAtlases.getNumAtlases();
	}

	CVector2f CResourceTexture2DAtlas::getImageDims(unsigned int uiImageNumber) const
	{
		std::vector<CImageAtlasDetails>* pviad = _mAtlases.getAllImageDetailsPointer();
		ThrowIfTrue(uiImageNumber >= pviad->size(), "CResourceTexture2DAtlas::getImageDims() given invalid image number.");
		return (*pviad)[uiImageNumber].v2fDimensions;
	}

	CVector2f CResourceTexture2DAtlas::getImageDims(const std::string& strImageName) const
	{
		CImageAtlasDetails iad = _mAtlases.getImageDetails(strImageName);
		return iad.v2fDimensions;
	}
	
	std::string CResourceTexture2DAtlas::getImageFilename(unsigned int uiImageNumber) const
	{
		std::vector<CImageAtlasDetails>* pviad = _mAtlases.getAllImageDetailsPointer();
		ThrowIfTrue(uiImageNumber >= pviad->size(), "CResourceTexture2DAtlas::getImageFilename() given invalid image number.");
		return (*pviad)[uiImageNumber].strImageFilename;
	}

	bool CResourceTexture2DAtlas::getImageWasRotated(unsigned int uiImageNumber) const
	{
		std::vector<CImageAtlasDetails>* pviad = _mAtlases.getAllImageDetailsPointer();
		ThrowIfTrue(uiImageNumber >= pviad->size(), "CResourceTexture2DAtlas::getImageWasRotated() given invalid image number.");
		return (*pviad)[uiImageNumber].bRotated;
	}

	bool CResourceTexture2DAtlas::getImageWasRotated(const std::string& strImageName) const
	{
		CImageAtlasDetails iad = _mAtlases.getImageDetails(strImageName);
		return iad.bRotated;
	}

	unsigned int CResourceTexture2DAtlas::getImageStoredInAtlasNumber(unsigned int uiImageNumber) const
	{
		std::vector<CImageAtlasDetails>* pviad = _mAtlases.getAllImageDetailsPointer();
		ThrowIfTrue(uiImageNumber >= pviad->size(), "CResourceTexture2DAtlas::getImageWasRotated() given invalid image number.");
		return (*pviad)[uiImageNumber].uiAtlasImage;
	}

	unsigned int CResourceTexture2DAtlas::getImageStoredInAtlasNumber(const std::string& strImageName) const
	{
		CImageAtlasDetails iad = _mAtlases.getImageDetails(strImageName);
		return iad.uiAtlasImage;
	}

	std::string CResourceTexture2DAtlas::getImageNameAtIndex(unsigned int uiImageNumber) const
	{
		std::vector<CImageAtlasDetails>* pviad = _mAtlases.getAllImageDetailsPointer();
		ThrowIfTrue(uiImageNumber >= pviad->size(), "CResourceTexture2DAtlas::getImageNameAtIndex() given invalid image number.");
		return (*pviad)[uiImageNumber].strImageFilename;
	}

	CImageAtlasDetails CResourceTexture2DAtlas::getImageDetails(const std::string& strImageName) const
	{
		return _mAtlases.getImageDetails(strImageName);
	}

	unsigned int CResourceTexture2DAtlas::getImageTextureID(const std::string& strImageName) const
	{
		CImageAtlasDetails iad = _mAtlases.getImageDetails(strImageName);
		return _mvAtlasTextureIDs[iad.uiAtlasImage];
	}

	bool CResourceTexture2DAtlas::getImageNameExists(const std::string& strImageName) const
	{
		return _mAtlases.getImageExists(strImageName);
	}

	void CResourceTexture2DAtlas::renderAtlasTo2DQuad(int iPosX, int iPosY, int iWidth, int iHeight, unsigned int uiAtlasImageNumber, CColour colour)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceVertexBuffer* pVB = pRM->getVertexBuffer("X:default");
		CResourceShader* pShader = pRM->getShader("X:pos_col_tex");
		SCWindow* pWindow = SCWindow::getPointer();

		// Setup triangle geometry
		pVB->removeGeom();
		pVB->addQuad2D(CVector2f(float(iPosX), float(iPosY)), CVector2f(float(iWidth), float(iHeight)),
			colour,	// Colour
			CVector2f(0, 0), CVector2f(1, 0), CVector2f(1, 1), CVector2f(0, 1));	// Texture coordinates
		pVB->update();

		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(pWindow->getWidth()), 0.0f, float(pWindow->getHeight()), -1.0f, 1.0f);
		pShader->bind();
		pShader->setMat4("transform", matProjection);
		pShader->setInt("texture0", 0);

		bindAtlas(0, uiAtlasImageNumber);
		glDisable(GL_DEPTH_TEST);
//		glEnable(GL_BLEND);
		pVB->draw();
		pShader->unbind();
		unbind(0);
	}
}