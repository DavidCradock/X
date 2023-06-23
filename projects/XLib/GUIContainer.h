#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"

namespace X
{
	class GUIContainer : public GUIBaseObject
	{
	public:
		GUIContainer();

		// Render this container
		void render(void);

		// Updates this container, returning true if the mouse is over this container
		bool update(bool bMouseIsOverContainerWhichIsAboveThisOne);

		std::string mstrThemename;	// Theme name used by this container
		
	};
}