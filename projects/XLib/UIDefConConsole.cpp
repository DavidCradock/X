#include "PCH.h"
#include "UIDefConConsole.h"
#include "UIManager.h"
#include "singletons.h"

namespace X
{
	CUIDefConConsole::CUIDefConConsole()
	{

	}

	void CUIDefConConsole::initialise(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();
		CUIContainer* pContainer = x->pUI->containerAdd(pDefCons->names.console, true);
		pContainer->setVisible(false);
		pContainer->setDimensions(x->pWindow->getDimensions());
		pContainer->setPosition(0, 0);

		pContainer->textAdd("Title", 0.0f, 0.0f, x->pWindow->getDimensions().x, 20.0f, "Console...");
	}

	void CUIDefConConsole::update(float fTimeDeltaSecs)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		// Is the container visible? If not, simply return
		CUIContainer* pContainer = x->pUI->containerGet(pDefCons->names.console);
		if (!pContainer->getVisible())
			return;
	}
}