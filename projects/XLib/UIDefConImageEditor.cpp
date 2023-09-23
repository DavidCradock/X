#include "PCH.h"
#include "UIDefConImageEditor.h"
#include "UIManager.h"
#include "singletons.h"

namespace X
{
	CUIDefConImageEditor::CUIDefConImageEditor()
	{

	}

	void CUIDefConImageEditor::initialise(void)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		CUIWindow* pWindow = x->pUI->windowAdd(pDefCons->names.imageEditor, true);
		pWindow->setVisible(false);
		pWindow->setDimensions(320, 400);
		pWindow->setPosition(200, 200);
	}

	void CUIDefConImageEditor::update(float fTimeDeltaSecs)
	{
		CUIDefaultContainers* pDefCons = x->pUI->getDefaultContainers();

		// Is the window visible? If not, simply return
		CUIWindow* pWindow = x->pUI->windowGet(pDefCons->names.imageEditor);
		if (!pWindow->getVisible())
			return;
	}
}