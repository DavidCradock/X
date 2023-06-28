#include "PCH.h"
#include "GUILineGraph.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"
#include "log.h"

namespace X
{
	void GUILineGraphDataSet::addValue(float fValue)
	{
		values.push_back(fValue);
	}

	void GUILineGraphDataSet::removeValue(void)
	{
		if (values.size())
		{
			std::vector<float>::iterator it = values.begin();
			it = values.erase(it);
		}
	}

	unsigned int GUILineGraphDataSet::getNumValues(void)
	{
		return (unsigned int)values.size();
	}

	float GUILineGraphDataSet::getHighestValue(void)
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

	float GUILineGraphDataSet::getLowestValue(void)
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

	void GUILineGraph::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		GUIManager* pGUIManager = GUIManager::getPointer();
		GUITheme* pTheme = pGUIManager->getTheme(pContainer->mstrThemename);
		renderBackground(pParentContainer, strFramebufferToSampleFrom, pTheme->mImages.lineGraphBGColour, pTheme->mImages.lineGraphBGNormal);

		// Get required resources needed to render
		ResourceManager* pRM = ResourceManager::getPointer();
		ResourceLine* pLine = pRM->getLine("X:gui");
		ResourceShader* pShader = pRM->getShader("X:line");
		Window* pWindow = Window::getPointer();

		pShader->bind();

		// Setup the projection matrix as orthographic
		glm::mat4 matProjection = glm::ortho(0.0f, float(pWindow->getWidth()), float(pWindow->getHeight()), 0.0f, -1.0f, 1.0f);
		glm::mat4 matIdentity(1.0f);

		pShader->setMat4("matrixWorld", matIdentity);
		pShader->setMat4("matrixView", matIdentity);
		pShader->setMat4("matrixProjection", matProjection);
		
		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_colour", 0);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		ResourceTexture2D* pTexColour = pRM->getTexture2D("X:default_white");
		ResourceTexture2D* pTexBG = pRM->getTexture2D(pTheme->mImages.lineGraphBGColour);
		glm::vec2 vTexDimsPoint3 = pTexBG->mvDimensions * 0.3333333f;
		glm::vec2 vTexDimsPoint6 = pTexBG->mvDimensions * 0.6666666f;

		// Bind textures
		pTexColour->bind(0);

		// First compute max number of entries and highest/lowest values
		float fLowestValue = 999999999.0f;
		float fHighestValue = -999999999.0f;
		unsigned int iMaxNumEntries = 0;

		// For each data set
		std::map<std::string, GUILineGraphDataSet*>::iterator it = _mmapDataSets.begin();
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
			glm::vec2 vPosMulti;
			vPosMulti.x = (mfWidth - vTexDimsPoint6.x) / (0.01f + iMaxNumEntries);
			vPosMulti.y = (mfHeight - vTexDimsPoint6.y) / (0.01f + (fHighestValue - fLowestValue));
			float fAbsLowestVal = (float)abs(fLowestValue);

			pLine->removeGeom();
			pLine->setDrawModeAsLineStrip();
			ResourceLine::Vertex vert;

			// For each data set (line)
			it = _mmapDataSets.begin();
			glm::vec3 vPosBL(pContainer->mfPositionX + mfPositionX + vTexDimsPoint3.x, pContainer->mfPositionY + mfPositionY + mfHeight - vTexDimsPoint3.y, 0.0f);	// Bottom left corner of widget
			vert.position.z = 0.0f;

			while (it != _mmapDataSets.end())
			{
				// For each entry within this data set
				vert.colour = it->second->colour.get();

				for (int iEntry = 0; iEntry < (int)it->second->getNumValues(); ++iEntry)
				{
					vert.position = vPosBL;
					vert.position.x += float(iEntry) * vPosMulti.x;
					vert.position.y -= (it->second->values[iEntry] - fAbsLowestVal) * vPosMulti.y;
					pLine->addLinePoint(vert);
				}
				it++;
			}

			pLine->update();
			pLine->draw();
		}

		pTexColour->unbind(0);	// Unbind texture
		pShader->unbind();		// Unbind the shader
		glDisable(GL_BLEND);
	}

	void GUILineGraph::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;

		_mTimer.update();
		float fSecondsPast = _mTimer.getSecondsPast();
		if (fSecondsPast > 0.1f)
			fSecondsPast = 0.1f;
	}

	GUILineGraphDataSet* GUILineGraph::addDataset(const std::string& strName, const GUIColour& cCol)
	{
		std::map<std::string, GUILineGraphDataSet*>::iterator it = _mmapDataSets.find(strName);
		ThrowIfTrue(it != _mmapDataSets.end(), "GUILineGraph::addDataSet(" + strName + ") failed. The named data set already exists.");
		GUILineGraphDataSet* pNew = new GUILineGraphDataSet;
		ThrowIfFalse(pNew, "GUILineGraph::addDataSet(" + strName + ") failed. Unable to allocate memory for new data set.");
		_mmapDataSets[strName] = pNew;
		return pNew;
	}

	GUILineGraphDataSet* GUILineGraph::getDataset(const std::string& strName)
	{
		std::map<std::string, GUILineGraphDataSet*>::iterator it = _mmapDataSets.find(strName);
		ThrowIfTrue(it == _mmapDataSets.end(), "GUILineGraph::getDataset(" + strName + ") failed. The named data set doesn't exist.");
		return it->second;
	}

	void GUILineGraph::removeDataset(const std::string& strName)
	{
		std::map<std::string, GUILineGraphDataSet*>::iterator it = _mmapDataSets.find(strName);
		if (it == _mmapDataSets.end())
			return;
		delete it->second;
		_mmapDataSets.erase(it);
	}

	unsigned int GUILineGraph::getNumDatasets(void)
	{
		return (unsigned int)_mmapDataSets.size();
	}

	std::string GUILineGraph::getDatasetName(unsigned int iIndex)
	{
		ThrowIfTrue(iIndex < 0 || iIndex >= (int)_mmapDataSets.size(), "GUILineGraph::getDatasetName() failed. Invalid index of " + std::to_string(iIndex) + " was given.");
		std::map<std::string, GUILineGraphDataSet*>::iterator it = _mmapDataSets.begin();
		unsigned int iCount = 0;
		while (iCount < iIndex)
			it++;
		return it->first;
	}
}