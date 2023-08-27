#include "PCH.h"
#include "UIManager.h"

namespace X
{
	SCUIManager::SCUIManager()
	{
	}

	void SCUIManager::render(const std::string& strFramebufferToSampleFrom)
	{

	}

	/************************************************************************************************************************************************************/
	/* Container related */
	/************************************************************************************************************************************************************/

	CUIContainer* SCUIManager::containerAdd(const std::string& strName)
	{
		return _mmanContainers.add(strName);
	}

	bool SCUIManager::containerExists(const std::string& strName) const
	{
		return _mmanContainers.exists(strName);
	}

	CUIContainer* SCUIManager::containerGet(const std::string& strName) const
	{
		return _mmanContainers.get(strName);
	}

	CUIContainer* SCUIManager::containerGet(size_t index) const
	{
		return _mmanContainers.get(index);
	}

	std::string SCUIManager::containerGetName(size_t index) const
	{
		return _mmanContainers.getName(index);
	}

	size_t SCUIManager::containerGetNumber(void) const
	{
		return _mmanContainers.getNumber();
	}

	void SCUIManager::containerRemove(const std::string& strName)
	{
		if (_mmanContainers.exists(strName))
			_mmanContainers.remove(strName);
	}

	void SCUIManager::containerRemoveAll(void)
	{
		_mmanContainers.removeAll(true);
	}

	/************************************************************************************************************************************************************/
	/* Theme related */
	/************************************************************************************************************************************************************/

	CUITheme* SCUIManager::themeAdd(const std::string& strName)
	{
		return _mmanThemes.add(strName);
	}

	bool SCUIManager::themeExists(const std::string& strName) const
	{
		return _mmanThemes.exists(strName);
	}

	CUITheme* SCUIManager::themeGet(const std::string& strName) const
	{
		return _mmanThemes.get(strName);
	}

	CUITheme* SCUIManager::themeGet(size_t index) const
	{
		return _mmanThemes.get(index);
	}

	std::string SCUIManager::themeGetName(size_t index) const
	{
		return _mmanThemes.getName(index);
	}

	size_t SCUIManager::themeGetNumber(void) const
	{
		return _mmanThemes.getNumber();
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

	void SCUIManager::themeSetForAll(const std::string& strName)
	{
		ThrowIfFalse(_mmanThemes.exists(strName), "SCUIManager::setThemeForAll(\"" + strName + "\") failed. Invalid name given.");

		ThrowIfTrue(1, "NOT IMPLEMENTED");
	}
}