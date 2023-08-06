#include "PCH.h"
#include "GUILineGraph.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "log.h"
#include "GUITooltip.h"

namespace X
{
	void CGUILineGraphDataSet::addValue(float fValue)
	{
		values.push_back(fValue);
	}

	void CGUILineGraphDataSet::removeValue(void)
	{
		if (values.size())
		{
			std::vector<float>::iterator it = values.begin();
			it = values.erase(it);
		}
	}

	unsigned int CGUILineGraphDataSet::getNumValues(void) const
	{
		return (unsigned int)values.size();
	}

	float CGUILineGraphDataSet::getHighestValue(void) const
	{
		if (!values.size())
			return 0.0f;

		float fResult = -9999999.0f;
		for (int i = 0; i < (int)values.size(); ++i)
		{
			if (fResult < values[i])
				fResult = values[i];
		}
		return fResult;
	}

	float CGUILineGraphDataSet::getLowestValue(void) const
	{
		if (!values.size())
			return 0.0f;
		float fResult = 9999999.0f;
		for (int i = 0; i < (int)values.size(); ++i)
		{
			if (fResult > values[i])
				fResult = values[i];
		}
		return fResult;
	}

	CGUILineGraph::CGUILineGraph()
	{
		mpTooltip = new CGUITooltip;
	}

	CGUILineGraph::~CGUILineGraph()
	{
		delete mpTooltip;
	}

	void CGUILineGraph::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		SCGUIManager* pGUIManager = SCGUIManager::getPointer();
		CGUITheme* pTheme = pContainer->getTheme();
		CColour col;
		renderBackground(pParentContainer, strFramebufferToSampleFrom, pTheme->mImages.lineGraphBGColour, pTheme->mImages.lineGraphBGNormal, col);

		// Get required resources needed to render
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceVertexBufferLine* pLine = pRM->getVertexBufferLine("X:default");
		CResourceShader* pShader = pRM->getShader("X:line");
		SCWindow* pWindow = SCWindow::getPointer();

		pShader->bind();

		// Setup the projection matrix as orthographic
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(pWindow->getWidth()), 0.0f, float(pWindow->getHeight()), -1.0f, 1.0f);
		CMatrix matIdentity;

		pShader->setMat4("matrixWorld", matIdentity);
		pShader->setMat4("matrixView", matIdentity);
		pShader->setMat4("matrixProjection", matProjection);
		
		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_colour", 0);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		CResourceTexture2DFromFile* pTexColour = pRM->getTexture2DFromFile("X:default_white");
		CResourceTexture2DFromFile* pTexBG = pRM->getTexture2DFromFile(pTheme->mImages.lineGraphBGColour);
		CVector2f vTexDimsPoint3 = pTexBG->mvDimensions * 0.3333333f;
		CVector2f vTexDimsPoint6 = pTexBG->mvDimensions * 0.6666666f;

		// Bind textures
		pTexColour->bind(0);

		// First compute max number of entries and highest/lowest values
		float fLowestValue = 999999999.0f;
		float fHighestValue = -999999999.0f;
		unsigned int iMaxNumEntries = 0;

		// For each data set
		std::map<std::string, CGUILineGraphDataSet*>::iterator it = _mmapDataSets.begin();
		while (it != _mmapDataSets.end())
		{
			// Get largest amount of entries of all data sets
			unsigned int iCurrentSetSize = (unsigned int)it->second->values.size();
			if (iCurrentSetSize > iMaxNumEntries)
				iMaxNumEntries = iCurrentSetSize;

			// Determine max and min values stored in all data sets
			for (unsigned int i = 0; i < it->second->values.size(); ++i)
			{
				if (it->second->values[i] < fLowestValue)
					fLowestValue = it->second->values[i];
				if (it->second->values[i] > fHighestValue)
					fHighestValue = it->second->values[i];
			}
			it++;
		}

		// Only render if data exists
		if (iMaxNumEntries > 0)
		{
			// Now we have maximum number of entries and highest/lowest values of the values within each dataset
			// vPosMulti is used to determine position of each line segment...
			CVector2f vPosMulti;
			vPosMulti.x = (mfWidth - vTexDimsPoint6.x) / (0.01f + iMaxNumEntries);
			vPosMulti.y = (mfHeight - vTexDimsPoint6.y) / (0.01f + (fHighestValue - fLowestValue));
			float fAbsLowestVal = (float)abs(fLowestValue);

			pLine->removeGeom();
			pLine->setDrawModeAsLineStrip();
			CResourceVertexBufferLine::Vertex vert;

			// For each data set (line)
			it = _mmapDataSets.begin();
			CVector3f vPosBL(pContainer->mfPositionX + mfPositionX + vTexDimsPoint3.x, pContainer->mfPositionY + mfPositionY + mfHeight - vTexDimsPoint3.y, 0.0f);	// Bottom left corner of widget
			vert.position.z = 0.0f;

			while (it != _mmapDataSets.end())
			{
				// For each entry within this data set
				vert.colour = it->second->colour;

				pLine->removeGeom();

				for (int iEntry = 0; iEntry < (int)it->second->getNumValues(); ++iEntry)
				{
					vert.position = vPosBL;
					vert.position.x += float(iEntry) * vPosMulti.x;
					vert.position.y -= (it->second->values[iEntry] - fAbsLowestVal) * vPosMulti.y;
					
					pLine->addLinePoint(vert);
				}
				it++;
				pLine->update();
				pLine->render();
			}

			
		}

		pTexColour->unbind(0);	// Unbind texture
		pShader->unbind();		// Unbind the shader
		glDisable(GL_BLEND);
	}

	void CGUILineGraph::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		_mTimer.update();

		// Update this object's tooltip
		SCInputManager* pInput = SCInputManager::getPointer();
		CVector2f vMousePos = pInput->mouse.getCursorPos();
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		bool bMouseOver = false;
		if (bParentContainerAcceptingMouseClicks)
		{
			// Determine if mouse cursor is over
			if (vMousePos.x > pContainer->mfPositionX + mfPositionX)
				if (vMousePos.x < pContainer->mfPositionX + mfPositionX + mfWidth)
					if (vMousePos.y > pContainer->mfPositionY + mfPositionY)
						if (vMousePos.y < pContainer->mfPositionY + mfPositionY + mfHeight)
							bMouseOver = true;
		}
		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}

	CGUILineGraphDataSet* CGUILineGraph::addDataset(const std::string& strName, const CColour& cCol)
	{
		std::map<std::string, CGUILineGraphDataSet*>::iterator it = _mmapDataSets.find(strName);
		ThrowIfTrue(it != _mmapDataSets.end(), "CGUILineGraph::addDataSet(" + strName + ") failed. The named data set already exists.");
		CGUILineGraphDataSet* pNew = new CGUILineGraphDataSet;
		ThrowIfFalse(pNew, "CGUILineGraph::addDataSet(" + strName + ") failed. Unable to allocate memory for new data set.");
		pNew->colour = cCol;
		_mmapDataSets[strName] = pNew;
		return pNew;
	}

	CGUILineGraphDataSet* CGUILineGraph::getDataset(const std::string& strName)
	{
		std::map<std::string, CGUILineGraphDataSet*>::iterator it = _mmapDataSets.find(strName);
		ThrowIfTrue(it == _mmapDataSets.end(), "CGUILineGraph::getDataset(" + strName + ") failed. The named data set doesn't exist.");
		return it->second;
	}

	void CGUILineGraph::removeDataset(const std::string& strName)
	{
		std::map<std::string, CGUILineGraphDataSet*>::iterator it = _mmapDataSets.find(strName);
		if (it == _mmapDataSets.end())
			return;
		delete it->second;
		_mmapDataSets.erase(it);
	}

	unsigned int CGUILineGraph::getNumDatasets(void)
	{
		return (unsigned int)_mmapDataSets.size();
	}

	std::string CGUILineGraph::getDatasetName(unsigned int iIndex)
	{
		ThrowIfTrue(iIndex < 0 || iIndex >= (int)_mmapDataSets.size(), "CGUILineGraph::getDatasetName() failed. Invalid index of " + std::to_string(iIndex) + " was given.");
		std::map<std::string, CGUILineGraphDataSet*>::iterator it = _mmapDataSets.begin();
		unsigned int iCount = 0;
		while (iCount < iIndex)
			it++;
		return it->first;
	}
}