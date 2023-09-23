#include "PCH.h"
#include "UIDefConSettings.h"
#include "UIManager.h"
#include "singletons.h"

namespace X
{
	CUIDefConSettings::CUIDefConSettings()
	{

	}

	void CUIDefConSettings::initialise(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		CUIWindow* pWindow = x->pUI->windowAdd(pDefCons->names.settings, true);
		pWindow->setVisible(false);
		pWindow->setDimensions(320, 240);
	}

	void CUIDefConSettings::update(float fTimeDeltaSecs)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(pDefCons->names.settings);
		if (!pWindow->getVisible())
			return;
	}
}