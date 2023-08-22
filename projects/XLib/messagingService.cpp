#include "PCH.h"
#include "messagingService.h"
#include "singletons.h"

namespace X
{
	CMessageService::CMessageService()
	{

	}

	CMessageService::~CMessageService()
	{

	}

	bool CMessageService::isSubscribedToService(const std::string& strUserName)
	{
		auto it = _mlstSubscribedUserNames.begin();
		while (it != _mlstSubscribedUserNames.end())
		{
			if (*it == strUserName)
				return true;
			it++;
		}
		return false;
	}
}