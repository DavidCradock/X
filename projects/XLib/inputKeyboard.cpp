#include "PCH.h"
#include "inputKeyboard.h"

namespace X
{

	CInputKeyboard::CInputKeyboard(void)
	{
		lpDIDeviceK = NULL;
		reset();
		// Default key repeat settings
		setRepeatInitialDelay();
		setRepeatRate();
	}

	CInputKeyboard::~CInputKeyboard(void)
	{
	}

	void CInputKeyboard::reset(void)
	{
		ZeroMemory(keyState, sizeof(char) * 256);
		anyKeyPressed = false;
		ZeroMemory(keyOncePrev, sizeof(bool) * 256);
		ZeroMemory(keyOnceResult, sizeof(bool) * 256);
		keyOnce2Char.clear();
	}

	bool CInputKeyboard::init(LPDIRECTINPUT8 pMainDirectXinputDevice, HWND hApplicationWindow)
	{
		lpDI = pMainDirectXinputDevice;
		HRESULT hr;

		hr = lpDI->CreateDevice(GUID_SysKeyboard, &lpDIDeviceK, NULL);
		if (hr != DI_OK)
		{
			release();
			return false;
		}

		hr = lpDIDeviceK->SetDataFormat(&c_dfDIKeyboard);
		if (hr != DI_OK)
		{
			release();
			return false;
		}

		hr = lpDIDeviceK->SetCooperativeLevel(hApplicationWindow, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		if (hr != DI_OK)
		{
			release();
			return false;
		}

		acquire();

		return true;
	}

	void CInputKeyboard::update(void)
	{
		timing.update();

		if (!acquire())
		{
			reset();
			return;
		}

		ZeroMemory(keyState, 256 * sizeof(char));
		lpDIDeviceK->GetDeviceState(256, (void*)&keyState);

		anyKeyPressed = false;

		for (int i = 0; i < 256; ++i)	// Check each key state.
		{
			if (keyState[i] & 0x80)	// Key pressed down?
			{
				anyKeyPressed = true;

				// Compute keyOnce stuff
				if (keyOncePrev[i] == false)	// Key has just been pressed down?
				{
					keyOncePrev[i] = true;	// It's been pressed now! (For next check)
					keyOnceResult[i] = true;
				}
				else	// Key was pressed on last call
				{
					keyOnceResult[i] = false;
				}
			}
			else	// Key not pressed
			{
				// Key is no longer pressed
				keyOncePrev[i] = false;
				keyOnceResult[i] = false;
			}	// Key not pressed
		}	// Check each key state

		// Update _mpchKeyOnce2char
		updateOnceChar();

		updateKeyRepeat();
	}

	void CInputKeyboard::updateOnceChar(void)
	{
		keyOnce2Char.clear();
		if (!anyPressed())
			return;

		int iFirstKey = 0;	// First key pressed and found
		while (iFirstKey < 256)
		{
			if (keyOnceResult[iFirstKey])
				break;
			iFirstKey++;
		}

		if (pressed(KC_LSHIFT) || pressed(KC_RSHIFT))	// uppercase
		{
			switch (iFirstKey)
			{
			case KC_A:			keyOnce2Char.append("A"); break;
			case KC_B:			keyOnce2Char.append("B"); break;
			case KC_C:			keyOnce2Char.append("C"); break;
			case KC_D:			keyOnce2Char.append("D"); break;
			case KC_E:			keyOnce2Char.append("E"); break;
			case KC_F:			keyOnce2Char.append("F"); break;
			case KC_G:			keyOnce2Char.append("G"); break;
			case KC_H:			keyOnce2Char.append("H"); break;
			case KC_I:			keyOnce2Char.append("I"); break;
			case KC_J:			keyOnce2Char.append("J"); break;
			case KC_K:			keyOnce2Char.append("K"); break;
			case KC_L:			keyOnce2Char.append("L"); break;
			case KC_M:			keyOnce2Char.append("M"); break;
			case KC_N:			keyOnce2Char.append("N"); break;
			case KC_O:			keyOnce2Char.append("O"); break;
			case KC_P:			keyOnce2Char.append("P"); break;
			case KC_Q:			keyOnce2Char.append("Q"); break;
			case KC_R:			keyOnce2Char.append("R"); break;
			case KC_S:			keyOnce2Char.append("S"); break;
			case KC_T:			keyOnce2Char.append("T"); break;
			case KC_U:			keyOnce2Char.append("U"); break;
			case KC_V:			keyOnce2Char.append("V"); break;
			case KC_W:			keyOnce2Char.append("W"); break;
			case KC_X:			keyOnce2Char.append("X"); break;
			case KC_Y:			keyOnce2Char.append("Y"); break;
			case KC_Z:			keyOnce2Char.append("Z"); break;
			case KC_1:			keyOnce2Char.append("!"); break;
			case KC_2:			keyOnce2Char.append("\""); break;
			case KC_3:			keyOnce2Char.append("£"); break;
			case KC_4:			keyOnce2Char.append("$"); break;
			case KC_5:			keyOnce2Char.append("%"); break;
			case KC_6:			keyOnce2Char.append("^"); break;
			case KC_7:			keyOnce2Char.append("&"); break;
			case KC_8:			keyOnce2Char.append("*"); break;
			case KC_9:			keyOnce2Char.append("("); break;
			case KC_0:			keyOnce2Char.append(")"); break;
			case KC_SLASH:		keyOnce2Char.append("?"); break;
			case KC_PERIOD:		keyOnce2Char.append(">"); break;
			case KC_COMMA:		keyOnce2Char.append("<"); break;
			case KC_SEMICOLON:	keyOnce2Char.append(":"); break;
			case KC_APOSTROPHE: keyOnce2Char.append("@"); break;
			case KC_GRAVE:		keyOnce2Char.append("¬"); break;
			case KC_LBRACKET:	keyOnce2Char.append("{"); break;
			case KC_RBRACKET:	keyOnce2Char.append("}"); break;
			case KC_MINUS:		keyOnce2Char.append("_"); break;
			case KC_EQUALS:		keyOnce2Char.append("+"); break;
			default:		break;
			}
		}
		else	// lowercase
		{
			switch (iFirstKey)
			{
			case KC_A:			keyOnce2Char.append("a"); break;
			case KC_B:			keyOnce2Char.append("b"); break;
			case KC_C:			keyOnce2Char.append("c"); break;
			case KC_D:			keyOnce2Char.append("d"); break;
			case KC_E:			keyOnce2Char.append("e"); break;
			case KC_F:			keyOnce2Char.append("f"); break;
			case KC_G:			keyOnce2Char.append("g"); break;
			case KC_H:			keyOnce2Char.append("h"); break;
			case KC_I:			keyOnce2Char.append("i"); break;
			case KC_J:			keyOnce2Char.append("j"); break;
			case KC_K:			keyOnce2Char.append("k"); break;
			case KC_L:			keyOnce2Char.append("l"); break;
			case KC_M:			keyOnce2Char.append("m"); break;
			case KC_N:			keyOnce2Char.append("n"); break;
			case KC_O:			keyOnce2Char.append("o"); break;
			case KC_P:			keyOnce2Char.append("p"); break;
			case KC_Q:			keyOnce2Char.append("q"); break;
			case KC_R:			keyOnce2Char.append("r"); break;
			case KC_S:			keyOnce2Char.append("s"); break;
			case KC_T:			keyOnce2Char.append("t"); break;
			case KC_U:			keyOnce2Char.append("u"); break;
			case KC_V:			keyOnce2Char.append("v"); break;
			case KC_W:			keyOnce2Char.append("w"); break;
			case KC_X:			keyOnce2Char.append("x"); break;
			case KC_Y:			keyOnce2Char.append("y"); break;
			case KC_Z:			keyOnce2Char.append("z"); break;
			case KC_1:			keyOnce2Char.append("1"); break;
			case KC_2:			keyOnce2Char.append("2"); break;
			case KC_3:			keyOnce2Char.append("3"); break;
			case KC_4:			keyOnce2Char.append("4"); break;
			case KC_5:			keyOnce2Char.append("5"); break;
			case KC_6:			keyOnce2Char.append("6"); break;
			case KC_7:			keyOnce2Char.append("7"); break;
			case KC_8:			keyOnce2Char.append("8"); break;
			case KC_9:			keyOnce2Char.append("9"); break;
			case KC_0:			keyOnce2Char.append("0"); break;
			case KC_NUMPAD0:	keyOnce2Char.append("0"); break;
			case KC_NUMPAD1:	keyOnce2Char.append("1"); break;
			case KC_NUMPAD2:	keyOnce2Char.append("2"); break;
			case KC_NUMPAD3:	keyOnce2Char.append("3"); break;
			case KC_NUMPAD4:	keyOnce2Char.append("4"); break;
			case KC_NUMPAD5:	keyOnce2Char.append("5"); break;
			case KC_NUMPAD6:	keyOnce2Char.append("6"); break;
			case KC_NUMPAD7:	keyOnce2Char.append("7"); break;
			case KC_NUMPAD8:	keyOnce2Char.append("8"); break;
			case KC_NUMPAD9:	keyOnce2Char.append("9"); break;
			case KC_SLASH:		keyOnce2Char.append("/"); break;
			case KC_DECIMAL:	keyOnce2Char.append("."); break;
			case KC_PERIOD:		keyOnce2Char.append("."); break;
			case KC_COMMA:		keyOnce2Char.append(","); break;
			case KC_SEMICOLON:	keyOnce2Char.append(";"); break;
			case KC_APOSTROPHE: keyOnce2Char.append("'"); break;
			case KC_GRAVE:		keyOnce2Char.append("`"); break;
			case KC_LBRACKET:	keyOnce2Char.append("["); break;
			case KC_RBRACKET:	keyOnce2Char.append("]"); break;
			case KC_MINUS:		keyOnce2Char.append("-"); break;
			case KC_EQUALS:		keyOnce2Char.append("="); break;
			default:		break;
			}
		}
	}

	void CInputKeyboard::release(void)
	{
		if (lpDIDeviceK)
		{
			lpDIDeviceK->Unacquire();
			lpDIDeviceK->Release();
			lpDIDeviceK = NULL;
		}
		reset();
	}

	bool CInputKeyboard::acquire(void)
	{
		if (!lpDIDeviceK)
			return false;
		HRESULT	hr = lpDIDeviceK->Acquire();
		if (hr == S_FALSE || hr == DI_OK)	// Already acquired (S_FALSE) or re-acquired (DI_OK)
			return true;
		return false;
	}

	void CInputKeyboard::updateKeyRepeat(void)
	{
		float fMS = float(timing.getSecondsPast()) * 1000.0f;	// Already updated in main update method.
		unsigned int iKey = 0;
		bool bAnyKeyOnce = false;	// For any key repeat

		while (iKey < 256)
		{
			if (keyState[iKey] & 0x80)	// if key is pressed
			{
				keyRep[iKey].bRepeatOK = false;
				keyRep[iKey].fTimeDownMS += fMS;
				if (keyRep[iKey].bRepeatingStarted == false)	// Check for start of repeating
				{
					if (keyRep[iKey].fTimeDownMS >= keyRepDelayMS)
					{
						keyRep[iKey].fTimeDownMS = 0.0f;
						keyRep[iKey].bRepeatingStarted = true;
					}
				}
				else	// Repeating has started
				{
					if (keyRep[iKey].fTimeDownMS >= keyRepRateMS)
					{
						keyRep[iKey].fTimeDownMS = 0.0f;
						keyRep[iKey].bRepeatOK = true;
					}
				}
			}
			else	// Key not down, reset
			{
				keyRep[iKey].fTimeDownMS = keyRepRateMS;
				keyRep[iKey].bRepeatOK = false;
				keyRep[iKey].bRepeatingStarted = false;
			}

			// If this is the first time the key has been pressed, set state of key pressed as true
			if (once(iKey))
			{
				bAnyKeyOnce = true;
				keyRep[iKey].bRepeatOK = true;
			}
			++iKey;
		}

		// Now do same, but for any key repeat
		if (anyKeyPressed)
		{
			keyAny.bRepeatOK = false;
			keyAny.fTimeDownMS += fMS;
			if (keyAny.bRepeatingStarted == false)
			{
				if (keyAny.fTimeDownMS >= keyRepDelayMS)
				{
					keyAny.fTimeDownMS = 0.0f;
					keyAny.bRepeatingStarted = true;
				}
			}
			else	// Repeating has started
			{
				if (keyAny.fTimeDownMS >= keyRepRateMS)
				{
					keyAny.fTimeDownMS = 0.0f;
					keyAny.bRepeatOK = true;
				}
			}
		}
		else	// No key down, reset
		{
			keyAny.fTimeDownMS = keyRepRateMS;
			keyAny.bRepeatOK = false;
			keyAny.bRepeatingStarted = false;
		}
		// If this is the first time a key has been pressed, set state of key pressed as true
		if (bAnyKeyOnce)
			keyAny.bRepeatOK = true;
	}

	bool CInputKeyboard::anyPressed(void) const
	{
		return anyKeyPressed;
	}

	bool CInputKeyboard::anyRepeat(void) const
	{
		return keyAny.bRepeatOK;
	}

	bool CInputKeyboard::pressed(unsigned char KC_KEYCODE) const
	{
		if (keyState[KC_KEYCODE] & 0x80)
			return true;
		return false;
	}

	bool CInputKeyboard::once(unsigned char KC_KEYCODE) const
	{
		return keyOnceResult[KC_KEYCODE];
	}

	const std::string& CInputKeyboard::once2Char(void) const
	{
		return keyOnce2Char;
	}

	bool CInputKeyboard::repeat(unsigned char KC_KEYCODE) const
	{
		return keyRep[KC_KEYCODE].bRepeatOK;
	}

	void CInputKeyboard::setRepeatInitialDelay(float fMilliseconds)
	{
		keyRepDelayMS = fMilliseconds;
	}

	void CInputKeyboard::setRepeatRate(float fMilliseconds)
	{
		keyRepRateMS = fMilliseconds;
	}

}