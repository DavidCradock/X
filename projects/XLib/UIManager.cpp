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

	CUIContainer* SCUIManager::containerAdd(const std::string& strName, bool bLocked)
	{
		return _mmanContainers.add(strName, bLocked);
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

	void SCUIManager::containerRemove(const std::string& strName, bool bForceRemoveLocked)
	{
		if (_mmanContainers.exists(strName))
			_mmanContainers.remove(strName, bForceRemoveLocked);
	}

	void SCUIManager::containerRemoveAll(bool bForceRemoveLocked)
	{
		_mmanContainers.removeAll(bForceRemoveLocked);
	}

	/************************************************************************************************************************************************************/
	/* Theme related */
	/************************************************************************************************************************************************************/

	CUITheme* SCUIManager::themeAdd(const std::string& strName, bool bLocked)
	{
		return _mmanThemes.add(strName, bLocked);
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

	void SCUIManager::themeRemove(const std::string& strName, bool bForceRemoveLocked)
	{
		if (_mmanThemes.exists(strName))
			_mmanThemes.remove(strName, bForceRemoveLocked);
	}

	void SCUIManager::themeRemoveAll(bool bForceRemoveLocked)
	{
		_mmanThemes.removeAll(bForceRemoveLocked);
	}

	void SCUIManager::themeSetForAll(const std::string& strName)
	{
		ThrowIfFalse(_mmanThemes.exists(strName), "SCUIManager::setThemeForAll(\"" + strName + "\") failed. Invalid name given.");

		ThrowIfTrue(1, "NOT IMPLEMENTED");
	}

	/************************************************************************************************************************************************************/
	/* Window related */
	/************************************************************************************************************************************************************/

	CUIWindow* SCUIManager::windowAdd(const std::string& strName, bool bLocked)
	{
		// Add window name to the z order list
		_mlistWindowZOrder.push_back(strName);

		return _mmanWindows.add(strName, bLocked);
	}

	bool SCUIManager::windowExists(const std::string& strName) const
	{
		return _mmanWindows.exists(strName);
	}

	CUIWindow* SCUIManager::windowGet(const std::string& strName) const
	{
		return _mmanWindows.get(strName);
	}

	CUIWindow* SCUIManager::windowGet(size_t index) const
	{
		return _mmanWindows.get(index);
	}

	std::string SCUIManager::windowGetName(size_t index) const
	{
		return _mmanWindows.getName(index);
	}

	size_t SCUIManager::windowGetNumber(void) const
	{
		return _mmanWindows.getNumber();
	}

	void SCUIManager::windowRemove(const std::string& strName, bool bForceRemoveLocked)
	{
		if (_mmanWindows.exists(strName))
			_mmanWindows.remove(strName, bForceRemoveLocked);

		// If the window no longer exists, remove window name from the Z order list
		if (!_mmanWindows.exists(strName))
		{
			auto itlist = std::find(_mlistWindowZOrder.begin(), _mlistWindowZOrder.end(), strName);
			if (itlist != _mlistWindowZOrder.end())
				_mlistWindowZOrder.erase(itlist);
		}
	}

	void SCUIManager::windowRemoveAll(bool bForceRemoveLocked)
	{
		// Get names of each window for use in a bit.
		std::vector<std::string> vecstrWindowNames;
		for (size_t i = 0; i < _mmanWindows.getNumber(); i++)
			vecstrWindowNames.push_back(_mmanWindows.getName(i));

		// Attempt to remove all containers
		_mmanWindows.removeAll(bForceRemoveLocked);

		// Remove all windows from the z order list if they no longer exist
		auto itWindowNames = vecstrWindowNames.begin();
		while (itWindowNames != vecstrWindowNames.end())
		{
			if (!_mmanWindows.exists(*itWindowNames))
			{
				auto itlist = std::find(_mlistWindowZOrder.begin(), _mlistWindowZOrder.end(), *itWindowNames);
				if (itlist != _mlistWindowZOrder.end())
					_mlistWindowZOrder.erase(itlist);
			}
			itWindowNames++;
		}
	}
}