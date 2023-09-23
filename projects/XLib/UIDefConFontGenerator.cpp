#include "PCH.h"
#include "UIDefConFontGenerator.h"
#include "UIManager.h"
#include "singletons.h"

namespace X
{
	CUIDefConFontGenerator::CUIDefConFontGenerator()
	{

	}

	void CUIDefConFontGenerator::initialise(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		CUIWindow* pWindow = x->pUI->windowAdd(pDefCons->names.fontGenerator, true);
		pWindow->setVisible(false);
		pWindow->setDimensions(320, 240);
	}

	void CUIDefConFontGenerator::update(float fTimeDeltaSecs)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(pDefCons->names.fontGenerator);
		if (!pWindow->getVisible())
			return;
	}
}