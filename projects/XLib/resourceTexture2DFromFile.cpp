#include "PCH.h"
#include "resourceTexture2DFromFile.h"
#include "image.h"
#include "log.h"
#include "resourceManager.h"

namespace X
{

	CResourceTexture2DFromFile::CResourceTexture2DFromFile(const std::string& strImageFilename, bool bFlipYaxis)
	{
		_mstrImageFilename = strImageFilename;
		_muiTextureID = 0;
		_mbFlipYaxis = bFlipYaxis;
		onGLContextCreated();
	}

	CResourceTexture2DFromFile::~CResourceTexture2DFromFile()
	{
		onGLContextToBeDestroyed();
	}

	void CResourceTexture2DFromFile::onGLContextCreated(void)
	{
		CImage image;
		ThrowIfFalse(image.load(_mstrImageFilename, _mbFlipYaxis), "CResourceTexture2DFromFile::onGLContextCreated() failed to load image from file (" + _mstrImageFilename + ") containing image data.");
		mvDimensions.x = (float)image.getWidth();
		mvDimensions.y = (float)image.getHeight();
		glGenTextures(1, &_muiTextureID);
		glBindTexture(GL_TEXTURE_2D, _muiTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (3==image.getNumChannels())
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getData());
		else  // We'll assume 4
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());

		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void CResourceTexture2DFromFile::onGLContextToBeDestroyed(void)
	{
		glDeleteTextures(1, &_muiTextureID);
		_muiTextureID = 0;
	}

	void CResourceTexture2DFromFile::bind(unsigned int uiTextureUnit) const
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
		glBindTexture(GL_TEXTURE_2D, _muiTextureID);
	}

	void CResourceTexture2DFromFile::unbind(unsigned int uiTextureUnit) const
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

	void CResourceTexture2DFromFile::unbindAll(void) const
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

	void CResourceTexture2DFromFile::renderTo2DQuad(int iPosX, int iPosY, int iWidth, int iHeight, CColour colour)
	{
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceTriangle* pTri = pRM->getTriangle("X:default");
		CResourceShader* pShader = pRM->getShader("X:pos_col_tex");
		SCWindow* pWindow = SCWindow::getPointer();

		// Setup triangle geometry
		pTri->removeGeom();
		pTri->addQuad2D(CVector2f(float(iPosX), float(iPosY)), CVector2f(float(iWidth), float(iHeight)),
			colour,	// Colour
			CVector2f(0, 0), CVector2f(1, 0), CVector2f(1, 1), CVector2f(0, 1));	// Texture coordinates
		pTri->update();

		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(pWindow->getWidth()), 0.0f, float(pWindow->getHeight()), -1.0f, 1.0f);
		pShader->bind();
		pShader->setMat4("transform", matProjection);
		pShader->setInt("texture0", 0);

		bind(0);
		glDisable(GL_DEPTH_TEST);
//		glEnable(GL_BLEND);
		pTri->draw();
		pShader->unbind();
		unbind(0);
	}
}