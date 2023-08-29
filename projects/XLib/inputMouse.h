#pragma once
#include "PCH.h"
#include "vector2f.h"

namespace X
{
	// Mouse Input class, use SCInputManager to access it's "mouse" member.
	// There is an object of this class called "mouse" which is initialised and updated in the SCInputManager class.
	// Use that object to access information about the device.
	class CInputMouse
	{
	public:
		CInputMouse(void);
		~CInputMouse(void);

		// Initialize mouse object
		bool init(LPDIRECTINPUT8 pMainDirectXinputDevice, HWND hApplicationWindow);

		// Update
		void update(bool fullscreen, unsigned int screenWidth, unsigned int screenHeight);

		// Free everything
		void release(void);

		// Returns true if the left mouse button is pressed.
		inline bool leftButDown(void) const { return mouseLeft; }

		// Returns true if the middle mouse button is pressed.
		inline bool midButDown(void) const { return mouseMiddle; }

		// Returns true if the right mouse button is pressed.
		inline bool rightButDown(void) const { return mouseRight; }

		// Returns true if the mouse button was pressed once
		inline bool leftButtonOnce(void) const { return leftButOnce[0]; }

		// Returns true if the mouse button was pressed once
		inline bool midButtonOnce(void) const { return midButOnce[0]; }

		// Returns true if the mouse button was pressed once
		inline bool rightButtonOnce(void) const { return rightButOnce[0]; }

		// Returns the mouse cursor position on screen, along X axis
		inline int posX(void) const { return int(mouseX); }

		// Returns the mouse cursor position on screen, along Y axis
		inline int posY(void) const { return int(mouseY); }

		// Returns the mouse cursor position on screen
		inline CVector2f pos(void) const { CVector2f vPos(mouseX, mouseY);	return vPos; }

		// Returns the distance which the mouse has moved since the last frame, along the X axis
		inline float deltaX(void) const { return mouseDeltaX; }

		// Returns the distance which the mouse has moved since the last frame, along the Y axis
		inline float deltaY(void) const { return mouseDeltaY; }

		// Returns the distance which the mouse wheel has moved since the last frame.
		inline float deltaZ(void) const { return mouseDeltaZ; }

		// Sets the mouse position on screen
		void setMousePos(int ix, int iy);

		// Sets the mouse position on screen
		void setMousePos(const CVector2f& vNewPosition);

		// Gets mouse cursor delta, for GUI
		inline CVector2f getMouseDeltaGUI(void) const { return CVector2f(mouseX - mouseXold, mouseY - mouseYold); }

		// Returns mouse cursor position
		inline CVector2f getCursorPos(void) const { return CVector2f(mouseX, mouseY); }

		// Sets the mouse cursor to use the specified ".ani" file
		// If the file couldn't be loaded, an exception occurs.
		// If passing an empty string, the default system mouse cursor is set.
		void setMouseCursorImage(const std::string& strAniFilename = "data/X/cursors/default.ani");
	private:
		LPDIRECTINPUT8 lpDI;				// Pointer to main DirectX input object
		LPDIRECTINPUTDEVICE8 lpDIDeviceM;	// DirectX input device object for the mouse

		bool mouseLeft;			// Left mouse button down?
		bool leftButOnce[2];	// Element 0 holds actual return value for mouseButOnce?(). 1 holds if button was held last frame
		bool mouseMiddle;		// Middle mouse button down?
		bool midButOnce[2];		// Element 0 holds actual return value for mouseButOnce?(). 1 holds if button was held last frame
		bool mouseRight;		// Right mouse button down?
		bool rightButOnce[2];	// Element 0 holds actual return value for mouseButOnce?(). 1 holds if button was held last frame
		float mouseX;			// Position of mouse on screen (Horizontal)
		float mouseY;			// Position of mouse on screen (Vertical)
		float mouseXold;		// Old position of mouse on screen (Horizontal)
		float mouseYold;		// Old position of mouse on screen (Vertical)
		float mouseDeltaX;		// Change in X position of mouse
		float mouseDeltaY;		// Change in Y position of mouse
		float mouseDeltaZ;		// Change in Z position of mouse (Mouse wheel)
		HWND windowHandle;		// Application window handle
	};

}