#include "PCH.h"
#include "UILineGraph.h"
#include "utilities.h"
#include "UIContainer.h"
#include "singletons.h"
#include "UITheme.h"

namespace X
{
	void CUILineGraphDataSet::addValue(float fValue)
	{
		values.push_back(fValue);
	}

	void CUILineGraphDataSet::removeValue(void)
	{
		if (values.size())
		{
			std::vector<float>::iterator it = values.begin();
			it = values.erase(it);
		}
	}

	unsigned int CUILineGraphDataSet::getNumValues(void) const
	{
		return (unsigned int)values.size();
	}

	float CUILineGraphDataSet::getHighestValue(void) const
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

	float CUILineGraphDataSet::getLowestValue(void) const
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

	CUILineGraph::CUILineGraph(CUIContainer* pContainer)
	{
		_mpContainer = pContainer;
		_mvDimensions.set(200, 48);
		_mbVisible = true;
	}

	CUILineGraph::~CUILineGraph()
	{

	}

	void CUILineGraph::setDimensions(float fX, float fY)
	{
		_mvDimensions.x = fX;
		_mvDimensions.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUILineGraph::setDimensions(const CVector2f& vDimensions)
	{
		_mvDimensions = vDimensions;
		_mpContainer->computeScrollbars();
	}

	CVector2f CUILineGraph::getDimensions(void) const
	{
		return _mvDimensions;
	}

	void CUILineGraph::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUILineGraph::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
		_mpContainer->computeScrollbars();
	}

	CVector2f CUILineGraph::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUILineGraph::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
		_mpContainer->computeScrollbars();
	}

	bool CUILineGraph::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUILineGraph::renderBG(CResourceVertexBufferCPT2* pVB)
	{
		if (!_mbVisible)
			return;

		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		// Add geometry for the 9 grid cells
		x->pUI->_helperAddWidgetGridGeometry(
			_mvPosition + _mpContainer->getWidgetOffset(),
			_mvDimensions,
			pThemeSettings->images.lineGraphBG,
			pThemeSettings->colours.linegraphBG,
			_mpContainer,
			pVB);
	}

	void CUILineGraph::renderLines(void)
	{
		// Get required resources needed to render
		CResourceVertexBufferLine* pLine = x->pResource->getVertexBufferLine(x->pResource->defaultRes.vertexbufferLine_default);
		CResourceShader* pShader = x->pResource->getShader(x->pResource->defaultRes.shader_VBCPT);

		pShader->bind();

		// Setup the projection matrix as orthographic
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(x->pWindow->getWidth()), 0.0f, float(x->pWindow->getHeight()), -1.0f, 1.0f);
		CMatrix matIdentity;

		pShader->setMat4("matrixWorld", matIdentity);
		pShader->setMat4("matrixView", matIdentity);
		pShader->setMat4("matrixProjection", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		CResourceTexture2DFromFile* pTexColour = x->pResource->getTexture2DFromFile(x->pResource->defaultRes.texture2DFromFile_default_white);

		// Bind textures
		pTexColour->bind(0);

		// First compute max number of entries and highest/lowest values
		float fLowestValue = 999999999.0f;
		float fHighestValue = -999999999.0f;
		unsigned int iMaxNumEntries = 0;

		// For each data set
		std::map<std::string, CUILineGraphDataSet*>::iterator it = _mmapDataSets.begin();
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
			// We could just render to the extents of the widget's area, but this would render the lines RIGHT up the the edges
			// and this looks wrong. We need to offset the lines "inwards", taking the widget's background edges into consideration.
			// So we need to get the image dimensions for the TL and BR corners so we can inset correctly.
			CUITheme* pTheme = _mpContainer->themeGet();
			CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
			const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();
			CImageAtlasDetails idTL = pAtlas->getImageDetails(pThemeSettings->images.lineGraphBG.colour.cornerTL);
			CImageAtlasDetails idBR = pAtlas->getImageDetails(pThemeSettings->images.lineGraphBG.colour.cornerBR);
			// Now we have access to idTL.vDims and idBR.vDims.



			// Now we have maximum number of entries and highest/lowest values of the values within each dataset
			// vPosMulti is used to determine position of each line segment...
			CVector2f vPosMulti;
			vPosMulti.x = _mvDimensions.x - idTL.vDims.x - idBR.vDims.x;
			vPosMulti.x /= (0.01f + iMaxNumEntries);
			vPosMulti.y = _mvDimensions.y - idTL.vDims.y - idBR.vDims.y;
			vPosMulti.y /= (0.01f + (fHighestValue - fLowestValue));
			float fAbsLowestVal = (float)abs(fLowestValue);

			pLine->removeGeom();
			pLine->setDrawModeAsLineStrip();
			CResourceVertexBufferLine::Vertex vert;

			// For each data set (line)
			it = _mmapDataSets.begin();
			CVector3f vPosBL(
				_mpContainer->getWidgetAreaTLCornerPosition().x + _mpContainer->getWidgetOffset().x + idTL.vDims.x,
				_mpContainer->getWidgetAreaTLCornerPosition().y + _mpContainer->getWidgetOffset().y + _mvDimensions.y - idBR.vDims.y,
				0.0f);	// Bottom left corner of widget
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



	void CUILineGraph::update(float fTimeDeltaSec)
	{

	}



	/******************************************************************* Widget specific *******************************************************************/

	CUILineGraphDataSet* CUILineGraph::addDataset(const std::string& strName, const CColour& cCol)
	{
		std::map<std::string, CUILineGraphDataSet*>::iterator it = _mmapDataSets.find(strName);
		ThrowIfTrue(it != _mmapDataSets.end(), "CUILineGraph::addDataSet(" + strName + ") failed. The named data set already exists.");
		CUILineGraphDataSet* pNew = new CUILineGraphDataSet;
		ThrowIfFalse(pNew, "CUILineGraph::addDataSet(" + strName + ") failed. Unable to allocate memory for new data set.");
		pNew->colour = cCol;
		_mmapDataSets[strName] = pNew;
		return pNew;
	}

	CUILineGraphDataSet* CUILineGraph::getDataset(const std::string& strName)
	{
		std::map<std::string, CUILineGraphDataSet*>::iterator it = _mmapDataSets.find(strName);
		ThrowIfTrue(it == _mmapDataSets.end(), "CUILineGraph::getDataset(" + strName + ") failed. The named data set doesn't exist.");
		return it->second;
	}

	void CUILineGraph::removeDataset(const std::string& strName)
	{
		std::map<std::string, CUILineGraphDataSet*>::iterator it = _mmapDataSets.find(strName);
		if (it == _mmapDataSets.end())
			return;
		delete it->second;
		_mmapDataSets.erase(it);
	}

	unsigned int CUILineGraph::getNumDatasets(void)
	{
		return (unsigned int)_mmapDataSets.size();
	}

	std::string CUILineGraph::getDatasetName(unsigned int iIndex)
	{
		ThrowIfTrue(iIndex < 0 || iIndex >= (int)_mmapDataSets.size(), "CUILineGraph::getDatasetName() failed. Invalid index of " + std::to_string(iIndex) + " was given.");
		std::map<std::string, CUILineGraphDataSet*>::iterator it = _mmapDataSets.begin();
		unsigned int iCount = 0;
		while (iCount < iIndex)
			it++;
		return it->first;
	}
}