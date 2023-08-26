#include "PCH.h"
#include "UIManager.h"

namespace X
{
	// Updates and renders the UI
	void SCUIManager::render(const std::string& strFramebufferToSampleFrom)
	{

	}

	CUITheme* SCUIManager::themeAdd(const std::string& strName)
	{
		if (_mmanThemes.exists(strName))
			return _mmanThemes.get(strName);
		return _mmanThemes.add(strName);
	}

	CUITheme* SCUIManager::themeGet(const std::string& strName) const
	{
		return _mmanThemes.get(strName);
	}

	bool SCUIManager::themeExists(const std::string& strName) const
	{
		return _mmanThemes.exists(strName);
	}

	void SCUIManager::themeRemove(const std::string& strName)
	{
		if (_mmanThemes.exists(strName))
			_mmanThemes.remove(strName);
	}

	void SCUIManager::themeRemoveAll(void)
	{
		_mmanThemes.removeAll(true);
	}

	size_t SCUIManager::themeGetNumber(void) const
	{
		return _mmanThemes.getNumber();
	}

	std::string SCUIManager::themeGetName(int iIndex) const
	{
		return _mmanThemes.getName(iIndex);
	}

	void SCUIManager::setThemeForAll(const std::string& strName)
	{
		ThrowIfFalse(_mmanThemes.exists(strName), "SCUIManager::setThemeForAll(\"" + strName + "\") failed. Invalid name given.");

		ThrowIfTrue(1, "NOT IMPLEMENTED");
	}
}