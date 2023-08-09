#include "PCH.h"
#include "resourceTexture2DFromImage.h"
#include "image.h"
#include "singletons.h"

namespace X
{
	CResourceTexture2DFromImage::CResourceTexture2DFromImage(const CImage& image)
	{
		ThrowIfTrue(0 == image.getWidth() || 0 == image.getHeight(), "CResourceTexture2DFromImage::CResourceTexture2DFromImage() failed. Passed image has zero dimensions.");
		image.copyTo(_mImage);
		_muiTextureID = 0;
		onGLContextCreated();
	}

	CResourceTexture2DFromImage::~CResourceTexture2DFromImage()
	{
		onGLContextToBeDestroyed();
		_mImage.free();
	}

	void CResourceTexture2DFromImage::onGLContextCreated(void)
	{
		ThrowIfTrue(0 == _mImage.getWidth() || 0 == _mImage.getHeight(), "CResourceTexture2DFromImage::onGLContextCreated() failed. Passed image has zero dimensions.");
		_mvDimensions.x = (float)_mImage.getWidth();
		_mvDimensions.y = (float)_mImage.getHeight();
		glGenTextures(1, &_muiTextureID);
		glBindTexture(GL_TEXTURE_2D, _muiTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (3== _mImage.getNumChannels())
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _mImage.getWidth(), _mImage.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, _mImage.getData());
		else  // We'll assume 4
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _mImage.getWidth(), _mImage.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _mImage.getData());

		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void CResourceTexture2DFromImage::onGLContextToBeDestroyed(void)
	{
		glDeleteTextures(1, &_muiTextureID);
		_muiTextureID = 0;
	}

	void CResourceTexture2DFromImage::bind(unsigned int uiTextureUnit) const
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

	void CResourceTexture2DFromImage::unbind(unsigned int uiTextureUnit) const
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

	void CResourceTexture2DFromImage::unbindAll(void) const
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

	void CResourceTexture2DFromImage::update(const CImage& image)
	{
		ThrowIfTrue(0 == image.getWidth() || 0 == image.getHeight(), "CResourceTexture2DFromImage::update() failed. Passed image has zero dimensions.");
		image.copyTo(_mImage);
		onGLContextToBeDestroyed();
		onGLContextCreated();
	}

	void CResourceTexture2DFromImage::renderTo2DQuad(int iPosX, int iPosY, int iWidth, int iHeight, CColour colour)
	{
		CResourceVertexBufferCPT* pVB = x->pResource->getVertexBufferCPT("X:default");
		CResourceShader* pShader = x->pResource->getShader("X:VBCPT");

		// Setup triangle geometry
		pVB->removeGeom();
		pVB->addQuad2D(CVector2f(float(iPosX), float(iPosY)), CVector2f(float(iWidth), float(iHeight)),
			colour,	// Colour
			CVector2f(0, 0), CVector2f(1, 0), CVector2f(1, 1), CVector2f(0, 1));	// Texture coordinates
		pVB->update();

		// Setup the matrices
		CMatrix matWorld, matView;
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(x->pWindow->getWidth()), 0.0f, float(x->pWindow->getHeight()), -1.0f, 1.0f);
		pShader->bind();
		pShader->setMat4("matrixWorld", matWorld);
		pShader->setMat4("matrixView", matView);
		pShader->setMat4("matrixProjection", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		bind(0);
		glDisable(GL_DEPTH_TEST);
//		glEnable(GL_BLEND);
		pVB->render();
		pShader->unbind();
		unbind(0);
	}

	CVector2f CResourceTexture2DFromImage::getDimensions(void) const
	{
		return _mvDimensions;
	}
}