#pragma once
#include "PCH.h"

namespace X
{
	// This container has functionality for editing images
	class CUIDefConImageEditor
	{
	public:
		CUIDefConImageEditor();

		// Called from CUIDefaultContainers::initialise() to initialise everything
		void initialise(void);

		// Called from CUIDefaultContainers::update() to update everything
		void update(float fTimeDeltaSecs);

	private:
	};
}