#include "PCH.h"
#include "resourceMouseCursor.h"
#include "stringUtils.h"
#include "singletons.h"

namespace X
{

	CResourceMouseCursor::CResourceMouseCursor(const std::string& strFilename, const std::string& strResourceName)
	{
		_mstrFilename = strFilename;
		_mstrResourceName = strResourceName;
		_mhCursor = NULL;
		onGLContextCreated();
	}

	CResourceMouseCursor::~CResourceMouseCursor()
	{
		onGLContextToBeDestroyed();
	}

	void CResourceMouseCursor::onGLContextCreated(void)
	{
		if (NULL == _mhCursor)
		{
			if (!_mstrFilename.size())  // Load default arrow
			{
				_mhCursor = LoadCursor(NULL, IDC_ARROW);
				ThrowIfFalse(_mhCursor, "CResourceMouseCursor::onGLContextCreated() failed. The given filename of the resource has zero length which tries to set the operating system's default mouse cursor. But for some reason, this has failed.");
			}
			else  // Load cursor from file
			{
				_mhCursor = LoadCursorFromFile(StringUtils::stringToWide(_mstrFilename).c_str());
				ThrowIfFalse(_mhCursor, "CResourceMouseCursor::onGLContextCreated() failed. The given filename of " + _mstrFilename + " could not be loaded with a call to LoadCursorFromFile().");
			}
		}
	}

	void CResourceMouseCursor::onGLContextToBeDestroyed(void)
	{
		if (_mhCursor)
		{
			DestroyCursor(_mhCursor);
			_mhCursor = NULL;
		}
	}

	void CResourceMouseCursor::set(void)
	{
		if (_mhCursor)
		{
			SetClassLongPtr(x->pWindow->getWindowHandle(), GCLP_HCURSOR, (LONG_PTR)_mhCursor);
			SetCursor(_mhCursor);
			x->pUI->setPreviouslySetMouseCursor(_mstrResourceName);
		}
	}
}