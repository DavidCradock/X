#include "PCH.h"
#include "messagingUser.h"
#include "singletons.h"

namespace X
{
	CMessageUser::CMessageUser()
	{
		setName("UNNAMED");
	}

	void CMessageUser::setName(const std::string& strUserName)
	{
		_mstrName = strUserName;
	}

	bool CMessageUser::isSubscribedToService(const std::string& strServiceName)
	{
		auto it = _mlstSubscribedServiceNames.begin();
		while (it != _mlstSubscribedServiceNames.end())
		{
			if (*it == strServiceName)
				return true;
			it++;
		}
		return false;
	}
}