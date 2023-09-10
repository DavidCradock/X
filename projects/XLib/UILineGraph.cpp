#include "PCH.h"
#include "UILineGraph.h"
#include "utilities.h"
#include "UIContainer.h"
#include "singletons.h"
#include "UITheme.h"

namespace X
{
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

	void CUILineGraph::render(void)
	{
		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();


	}



	void CUILineGraph::update(float fTimeDeltaSec)
	{

	}



	/******************************************************************* Widget specific *******************************************************************/


}