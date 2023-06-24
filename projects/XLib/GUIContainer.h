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
		void render(const std::string& strFramebufferToSampleFrom);

		// Updates this container, returning true if the mouse is over this container.
		// Will only return true if the mouse isn't over a previously updated container.
		bool update(bool bMouseIsOverContainerWhichIsAboveThisOne);

		std::string mstrThemename;	// Theme name used by this container
		
		bool mbContainerIsWindow;	// If true, this container is set as a window, which enables dragging and rendering of a window's borders, titlebar text etc
		
	private:
		bool _mbWindowBeingMoved;	// Whether this window is being moved or not
	};
}