#include "PCH.h"
#include "GUIBaseObject.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"

namespace X
{
	CGUIBaseObject::CGUIBaseObject()
	{
		mfPositionX = mfPositionY = 0.0f;
		mfWidth = mfHeight = 32.0f;

		// Compute texture coordinates for the nine components of a GUI object/widget
		// Compute texture coordinates for each of the 9 components
		float point3 = 0.3333333f;
		float point6 = 0.6666666f;

		// Centre quad
		mTC.centre.BL.x = point3;
		mTC.centre.TR.x = point6;
		mTC.centre.BR.x = point6;
		mTC.centre.TL.x = point3;
		mTC.centre.BL.y = point3;
		mTC.centre.TR.y = point6;
		mTC.centre.BR.y = point3;
		mTC.centre.TL.y = point6;

		// Top left corner
		mTC.topLeft.BL.x = 0.0f;
		mTC.topLeft.TR.x = point3;
		mTC.topLeft.BR.x = point3;
		mTC.topLeft.TL.x = 0.0f;
		mTC.topLeft.BL.y = point3;
		mTC.topLeft.TR.y = 0.0f;
		mTC.topLeft.BR.y = point3;
		mTC.topLeft.TL.y = 0.0f;

		// Top right corner
		mTC.topRight.BL.x = point6;
		mTC.topRight.TR.x = 1.0f;
		mTC.topRight.BR.x = 1.0f;
		mTC.topRight.TL.x = point6;
		mTC.topRight.BL.y = point3;
		mTC.topRight.TR.y = 0.0f;
		mTC.topRight.BR.y = point3;
		mTC.topRight.TL.y = 0.0f;

		// Bottom left corner
		mTC.bottomLeft.BL.x = 0.0f;
		mTC.bottomLeft.TR.x = point3;
		mTC.bottomLeft.BR.x = point3;
		mTC.bottomLeft.TL.x = 0.0f;
		mTC.bottomLeft.BL.y = 1.0f;
		mTC.bottomLeft.TR.y = point6;
		mTC.bottomLeft.BR.y = 1.0f;
		mTC.bottomLeft.TL.y = point6;

		// Bottom right corner
		mTC.bottomRight.BL.x = point6;
		mTC.bottomRight.TR.x = 1.0f;
		mTC.bottomRight.BR.x = 1.0f;
		mTC.bottomRight.TL.x = point6;
		mTC.bottomRight.BL.y = 1.0f;
		mTC.bottomRight.TR.y = point6;
		mTC.bottomRight.BR.y = 1.0f;
		mTC.bottomRight.TL.y = point6;

		// Top edge
		mTC.top.BL.x = point3;
		mTC.top.TR.x = point6;
		mTC.top.BR.x = point6;
		mTC.top.TL.x = point3;
		mTC.top.BL.y = point3;
		mTC.top.TR.y = 0.0f;
		mTC.top.BR.y = point3;
		mTC.top.TL.y = 0.0f;

		// Bottom edge
		mTC.bottom.BL.x = point3;
		mTC.bottom.TR.x = point6;
		mTC.bottom.BR.x = point6;
		mTC.bottom.TL.x = point3;
		mTC.bottom.BL.y = 1.0f;
		mTC.bottom.TR.y = point6;
		mTC.bottom.BR.y = 1.0f;
		mTC.bottom.TL.y = point6;

		// Left edge
		mTC.left.BL.x = 0.0f;
		mTC.left.TR.x = point3;
		mTC.left.BR.x = point3;
		mTC.left.TL.x = 0.0f;
		mTC.left.BL.y = point3;
		mTC.left.TR.y = point6;
		mTC.left.BR.y = point3;
		mTC.left.TL.y = point6;

		// Right edge
		mTC.right.BL.x = point6;
		mTC.right.TR.x = 1.0f;
		mTC.right.BR.x = 1.0f;
		mTC.right.TL.x = point6;
		mTC.right.BL.y = point3;
		mTC.right.TR.y = point6;
		mTC.right.BR.y = point3;
		mTC.right.TL.y = point6;
	}

	void CGUIBaseObject::setPosition(float fPosX, float fPosY)
	{
		mfPositionX = fPosX;
		mfPositionY = fPosY;
	}

	void CGUIBaseObject::setPositionCentreWindow(void)
	{
		SCWindow* pWindow = SCWindow::getPointer();
		mfPositionX = (pWindow->getWidth() / 2) - (mfWidth / 2);
		mfPositionY = (pWindow->getHeight() / 2) - (mfHeight / 2);
	}

	void CGUIBaseObject::setDimensions(float fWidth, float fHeight)
	{
		mfWidth = fWidth;
		mfHeight = fHeight;
	}

	void CGUIBaseObject::renderBackground(void* pParentContainer,	// Pointer to the object's parent container
		const std::string& strFramebufferToSampleFrom,				// The framebuffer to use as background when sampling (Given to SCGUIManager::render() and passed down)
		const std::string& strObjectColourTextureName,				// The name of the object's background colour texture
		const std::string& strObjectNormalTextureName,				// The name of the object's background normal texture
		CColour& colour)											// Colour
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;

		// Get required resources needed to render
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCWindow* pWindow = SCWindow::getPointer();
		CResourceTriangle* pTri = pRM->getTriangle("X:gui");
		CResourceShader* pShader = pRM->getShader("X:gui");
		CGUITheme* pTheme = pGUI->getTheme(pContainer->mstrThemename);
		SCInputManager* pInput = SCInputManager::getPointer();

		pShader->bind();

		// Setup the projection matrix as orthographic
		glm::mat4 matProjection = glm::ortho(0.0f, float(pWindow->getWidth()), float(pWindow->getHeight()), 0.0f, -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_colour", 0);
		pShader->setInt("texture1_normal", 1);
		pShader->setInt("texture2_reflection", 2);
		pShader->setInt("texture3_background", 3);
		pShader->setFloat("fNormalAmount", pTheme->mfNormalAmount);
		pShader->setFloat("fReflectionAmount", pTheme->mfReflectionAmount);
		pShader->setFloat("fMouseCursorDistance", pTheme->mfMouseCursorDistance);

		// Set mouse position, inverting Y position
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
		vMousePos.y = float(pWindow->getHeight()) - vMousePos.y;
		pShader->setVec2("v2MousePos", vMousePos);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		CResourceTexture2DFromFile* pTexColour = pRM->getTexture2DFromFile(strObjectColourTextureName);
		CResourceTexture2DFromFile* pTexNormal = pRM->getTexture2DFromFile(strObjectNormalTextureName);
		CResourceTexture2DFromFile* pTexReflection = pRM->getTexture2DFromFile(pTheme->mImages.reflection);
		CResourceFramebuffer* pFBSample = pRM->getFramebuffer(strFramebufferToSampleFrom);

		// Bind textures
		pTexColour->bind(0);
		pTexNormal->bind(1);
		pTexReflection->bind(2);
		pFBSample->bindAsTexture(3);

		// Render the centre
		glm::vec2 vTexDimsPoint3 = pTexColour->mvDimensions * 0.3333333f;
		glm::vec2 vTexDimsPoint6 = pTexColour->mvDimensions * 0.6666666f;
		glm::vec2 vPos = glm::vec2(pContainer->mfPositionX + mfPositionX, pContainer->mfPositionY + mfPositionY);

		float fPosX = pContainer->mfPositionX + mfPositionX;
		float fPosY = pContainer->mfPositionY + mfPositionY;
		pTri->removeGeom();
		pTri->addQuad2D(vPos + vTexDimsPoint3,									// Position
			glm::vec2(mfWidth - vTexDimsPoint6.x, mfHeight - vTexDimsPoint6.y),	// Dimensions
			colour.get(),
			mTC.centre.BL,
			mTC.centre.BR,
			mTC.centre.TR,
			mTC.centre.TL);

		// Render the left edge
		pTri->addQuad2D(
			glm::vec2(vPos.x,											// Position X
				vPos.y + vTexDimsPoint3.y),								// Position Y
			glm::vec2(vTexDimsPoint3.x, mfHeight - vTexDimsPoint6.y),	// Dimensions
			colour.get(),
			mTC.left.BL,
			mTC.left.BR,
			mTC.left.TR,
			mTC.left.TL);

		// Render the right edge
		pTri->addQuad2D(
			glm::vec2(vPos.x + mfWidth - vTexDimsPoint3.x,				// Position X
				vPos.y + vTexDimsPoint3.y),								// Position Y
			glm::vec2(vTexDimsPoint3.x, mfHeight - vTexDimsPoint6.y),	// Dimensions
			colour.get(),
			mTC.right.BL,
			mTC.right.BR,
			mTC.right.TR,
			mTC.right.TL);

		// Render the top edge
		pTri->addQuad2D(
			glm::vec2(
				vPos.x + vTexDimsPoint3.x,								// Position X
				vPos.y),												// Position Y
			glm::vec2(mfWidth - vTexDimsPoint6.x, vTexDimsPoint3.y),	// Dimensions
			colour.get(),
			mTC.top.BL,
			mTC.top.BR,
			mTC.top.TR,
			mTC.top.TL);

		// Render the bottom edge
		pTri->addQuad2D(
			glm::vec2(
				vPos.x + vTexDimsPoint3.x,								// Position X
				vPos.y + mfHeight - vTexDimsPoint3.y),					// Position Y
			glm::vec2(mfWidth - vTexDimsPoint6.x, vTexDimsPoint3.y),	// Dimensions
			colour.get(),
			mTC.bottom.BL,
			mTC.bottom.BR,
			mTC.bottom.TR,
			mTC.bottom.TL);

		// Render the top left corner
		pTri->addQuad2D(
			glm::vec2(
				vPos.x,										// Position X
				vPos.y),									// Position Y
			glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
			colour.get(),
			mTC.topLeft.BL,
			mTC.topLeft.BR,
			mTC.topLeft.TR,
			mTC.topLeft.TL);

		// Render the top right corner
		pTri->addQuad2D(
			glm::vec2(
				vPos.x + mfWidth - vTexDimsPoint3.x,		// Position X
				vPos.y),									// Position Y
			glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
			colour.get(),
			mTC.topRight.BL,
			mTC.topRight.BR,
			mTC.topRight.TR,
			mTC.topRight.TL);

		// Render the bottom left corner
		pTri->addQuad2D(
			glm::vec2(
				vPos.x,										// Position X
				vPos.y + mfHeight - vTexDimsPoint3.y),		// Position Y
			glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
			colour.get(),
			mTC.bottomLeft.BL,
			mTC.bottomLeft.BR,
			mTC.bottomLeft.TR,
			mTC.bottomLeft.TL);

		// Render the bottom right corner
		pTri->addQuad2D(
			glm::vec2(
				vPos.x + mfWidth - vTexDimsPoint3.x,		// Position X
				vPos.y + mfHeight - vTexDimsPoint3.y),		// Position Y
			glm::vec2(vTexDimsPoint3.x, vTexDimsPoint3.y),	// Dimensions
			colour.get(),
			mTC.bottomRight.BL,
			mTC.bottomRight.BR,
			mTC.bottomRight.TR,
			mTC.bottomRight.TL);

		pTri->update();
		pTri->draw();

		pTexColour->unbindAll();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);
	}
}