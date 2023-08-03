#pragma once
#include "PCH.h"
#include "timer.h"

namespace X
{
	// Keyboard codes
	#define KC_ESCAPE          0x01		//
	#define KC_1               0x02		//
	#define KC_2               0x03		//
	#define KC_3               0x04		//
	#define KC_4               0x05		//
	#define KC_5               0x06		//
	#define KC_6               0x07		//
	#define KC_7               0x08		//
	#define KC_8               0x09		//
	#define KC_9               0x0A		//
	#define KC_0               0x0B		//
	#define KC_MINUS           0x0C		// On main keyboard. 
	#define KC_EQUALS          0x0D		//  
	#define KC_BACK            0x0E		// Backspace. 
	#define KC_TAB             0x0F		//  
	#define KC_Q               0x10		//  
	#define KC_W               0x11		//  
	#define KC_E               0x12		//  
	#define KC_R               0x13		//  
	#define KC_T               0x14		//  
	#define KC_Y               0x15		//  
	#define KC_U               0x16		//  
	#define KC_I               0x17		//  
	#define KC_O               0x18		//  
	#define KC_P               0x19		//  
	#define KC_LBRACKET        0x1A		//  
	#define KC_RBRACKET        0x1B		//  
	#define KC_RETURN          0x1C		// Enter on main keyboard 
	#define KC_LCONTROL        0x1D		//  
	#define KC_A               0x1E		//  
	#define KC_S               0x1F		//  
	#define KC_D               0x20		//  
	#define KC_F               0x21		//  
	#define KC_G               0x22		//  
	#define KC_H               0x23		//  
	#define KC_J               0x24		//  
	#define KC_K               0x25		//  
	#define KC_L               0x26		//  
	#define KC_SEMICOLON       0x27		//  
	#define KC_APOSTROPHE      0x28		//  
	#define KC_GRAVE           0x29		// Accent grave 
	#define KC_LSHIFT          0x2A		//  
	#define KC_BACKSLASH       0x2B		//  
	#define KC_Z               0x2C		//  
	#define KC_X               0x2D		//  
	#define KC_C               0x2E		//  
	#define KC_V               0x2F		//  
	#define KC_B               0x30		//  
	#define KC_N               0x31		//  
	#define KC_M               0x32		//  
	#define KC_COMMA           0x33		//  
	#define KC_PERIOD          0x34		// . on main keyboard 
	#define KC_SLASH           0x35		// on main keyboard 
	#define KC_RSHIFT          0x36		//  
	#define KC_MULTIPLY        0x37		// * on numeric keypad 
	#define KC_LMENU           0x38		// left Alt 
	#define KC_SPACE           0x39		//  
	#define KC_CAPITAL         0x3A		//  
	#define KC_F1              0x3B		//  
	#define KC_F2              0x3C		//  
	#define KC_F3              0x3D		//  
	#define KC_F4              0x3E		//  
	#define KC_F5              0x3F		//  
	#define KC_F6              0x40		//  
	#define KC_F7              0x41		//  
	#define KC_F8              0x42		//  
	#define KC_F9              0x43		//  
	#define KC_F10             0x44		//  
	#define KC_NUMLOCK         0x45		//  
	#define KC_SCROLL          0x46		// Scroll Lock 
	#define KC_NUMPAD7         0x47		//  
	#define KC_NUMPAD8         0x48		//  
	#define KC_NUMPAD9         0x49		//  
	#define KC_SUBTRACT        0x4A		// - on numeric keypad 
	#define KC_NUMPAD4         0x4B		//  
	#define KC_NUMPAD5         0x4C		//  
	#define KC_NUMPAD6         0x4D		//  
	#define KC_ADD             0x4E		// + on numeric keypad 
	#define KC_NUMPAD1         0x4F		//  
	#define KC_NUMPAD2         0x50		//  
	#define KC_NUMPAD3         0x51		//  
	#define KC_NUMPAD0         0x52		//  
	#define KC_DECIMAL         0x53		// . on numeric keypad 
	#define KC_OEM_102         0x56		// <> or \| on RT 102-key keyboard (Non-U.S.) 
	#define KC_F11             0x57		//  
	#define KC_F12             0x58		//  
	#define KC_F13             0x64		//                    (NEC PC98) 
	#define KC_F14             0x65		//                    (NEC PC98) 
	#define KC_F15             0x66		//                    (NEC PC98) 
	#define KC_KANA            0x70		// (Japanese keyboard)            
	#define KC_ABNT_C1         0x73		// /? on Brazilian keyboard 
	#define KC_CONVERT         0x79		// (Japanese keyboard)            
	#define KC_NOCONVERT       0x7B		// (Japanese keyboard)            
	#define KC_YEN             0x7D		// (Japanese keyboard)            
	#define KC_ABNT_C2         0x7E		// Numpad . on Brazilian keyboard 
	#define KC_NUMPADEQUALS    0x8D		// = on numeric keypad (NEC PC98) 
	#define KC_PREVTRACK       0x90		// Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) 
	#define KC_AT              0x91		//                     (NEC PC98) 
	#define KC_COLON           0x92		//                     (NEC PC98) 
	#define KC_UNDERLINE       0x93		//                     (NEC PC98) 
	#define KC_KANJI           0x94		// (Japanese keyboard)            
	#define KC_STOP            0x95		//                     (NEC PC98) 
	#define KC_AX              0x96		//                     (Japan AX) 
	#define KC_UNLABELED       0x97		//                        (J3100) 
	#define KC_NEXTTRACK       0x99		// Next Track 
	#define KC_NUMPADENTER     0x9C		// Enter on numeric keypad 
	#define KC_RCONTROL        0x9D		// 
	#define KC_MUTE            0xA0		// Mute 
	#define KC_CALCULATOR      0xA1		// Calculator 
	#define KC_PLAYPAUSE       0xA2		// Play / Pause 
	#define KC_MEDIASTOP       0xA4		// Media Stop 
	#define KC_VOLUMEDOWN      0xAE		// Volume - 
	#define KC_VOLUMEUP        0xB0		// Volume + 
	#define KC_WEBHOME         0xB2		// Web home 
	#define KC_NUMPADCOMMA     0xB3		// , on numeric keypad (NEC PC98) 
	#define KC_DIVIDE          0xB5		// / on numeric keypad 
	#define KC_SYSRQ           0xB7		// 
	#define KC_RMENU           0xB8		// right Alt 
	#define KC_PAUSE           0xC5		// Pause 
	#define KC_HOME            0xC7		// Home on arrow keypad 
	#define KC_UP              0xC8		// UpArrow on arrow keypad 
	#define KC_PRIOR           0xC9		// PgUp on arrow keypad 
	#define KC_LEFT            0xCB		// LeftArrow on arrow keypad 
	#define KC_RIGHT           0xCD		// RightArrow on arrow keypad 
	#define KC_END             0xCF		// End on arrow keypad 
	#define KC_DOWN            0xD0		// DownArrow on arrow keypad 
	#define KC_NEXT            0xD1		// PgDn on arrow keypad 
	#define KC_INSERT          0xD2		// Insert on arrow keypad 
	#define KC_DELETE          0xD3		// Delete on arrow keypad 
	#define KC_LWIN            0xDB		// Left Windows key 
	#define KC_RWIN            0xDC		// Right Windows key 
	#define KC_APPS            0xDD		// AppMenu key 
	#define KC_POWER           0xDE		// System Power 
	#define KC_SLEEP           0xDF		// System Sleep 
	#define KC_WAKE            0xE3		// System Wake 
	#define KC_WEBSEARCH       0xE5		// Web Search 
	#define KC_WEBFAVORITES    0xE6		// Web Favorites 
	#define KC_WEBREFRESH      0xE7		// Web Refresh 
	#define KC_WEBSTOP         0xE8		// Web Stop 
	#define KC_WEBFORWARD      0xE9		// Web Forward 
	#define KC_WEBBACK         0xEA		// Web Back 
	#define KC_MYCOMPUTER      0xEB		// My Computer 
	#define KC_MAIL            0xEC		// Mail 
	#define KC_MEDIASELECT     0xED		// Media Select 

	// Keyboard Input class, use SCInputManager to access it's "key" member.
	// There is an object of this class called "key" which is initialised and updated in the SCInputManager class.
	// Use that object to access information about the device.
	class CInputKeyboard
	{
	public:
		CInputKeyboard(void);
		~CInputKeyboard(void);

		// Initialize keyboard object
		bool init(LPDIRECTINPUT8 pMainDirectXinputObject, HWND hApplicationWindow);

		// Update everything
		void update(void);

		// Free everything
		void release(void);

		// Returns true if any key is pressed.
		bool anyPressed(void) const;

		// Returns true if any key is pressed, taking into consideration key repeat timings
		bool anyRepeat(void) const;

		// Returns true if the given key is pressed.
		bool pressed(unsigned char KC_KEYCODE) const;

		// Returns true if the key has changed from the up state, to the down state since last call to update()
		bool once(unsigned char KC_KEYCODE) const;

		// Returns a const reference to the string object object, which the key being pressed represents.
		// The string has zero length if no key has been pressed.
		// Only returns a char if the key has been toggled from off to on since previous call to update(), not if the key is held down.
		// Upper and lower case chars are supported with shift keys.
		// If multiple non case changing keys are pressed, the results are unexpected. 
		const std::string& once2Char(void) const;

		// Key repeat timing.
		// Key repeat using timing, if a key is held down for a short delay, key repeat is started and this
		// method returns true at set timing intervals.
		// Both the initial time delay until repeating starts and the time delay between each key repeat can be set
		// using setKeyRepeatInitialDelay and setKeyRepeatRate 
		bool repeat(unsigned char KC_KEYCODE) const;

		// Set time in milliseconds until keyRepeat method will start to return true for a given key
		void setRepeatInitialDelay(float fMilliseconds = 500.0f);

		// Set time in milliseconds for the time between keyRepeat returning true for a key, once the initial delay has passed.
		void setRepeatRate(float fMilliseconds = 33.33f);

	private:
		LPDIRECTINPUT8 lpDI;				// Pointer to main DirectX input object
		LPDIRECTINPUTDEVICE8 lpDIDeviceK;	// DirectX input device object for the keyboard

		char keyState[256];				// Holds each keys state (up/down) (Use bitwise AND operator for down state (if (mchKeystate[1] & 0x80) //Escape key down
		bool anyKeyPressed;				// Any key is down?
		bool keyOncePrev[256];			// Key state on previous update
		bool keyOnceResult[256];		// Hold the actual status key. (keyOnce() method returns this value)
		char keyOnce2char[2];			// Used by keyOnce2Char()

		// Acquires the device so I can receive input data from it.
		// Returns false if device could not be acquired. 
		bool acquire(void);

		// Resets members which hold key status.
		void reset(void);

		// Updates _mpchKeyOnce2char for keyOnce2Char method
		void updateOnceChar(void);

		std::string keyOnce2Char;	    /// Used by keyOnce2Char()

		// Key repeat stuff
		struct KeyRep
		{
			float fTimeDownMS;			// Time since last time bRepeatOk==true
			bool bRepeatingStarted;		// The intial delay time period has passed, key repeating has started
			bool bRepeatOK;				// Return result for keyRepeat method for this key
		};
		KeyRep keyRep[256];
		KeyRep keyAny;
		CTimer timing;
		void updateKeyRepeat(void);

		// Key repeat settings
		float keyRepRateMS;			// The time duration between keyRepeat method returning true
		float keyRepDelayMS;		// Time from first time key is pressed, to key repeats starting.
	};

}