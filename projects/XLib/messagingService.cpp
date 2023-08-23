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

	void CMessageService::_receiveMessageFromUser(const CMessage& message)
	{
		_mvecMessageStorage.push_back(message);
	}

	void CMessageService::dispatch(void)
	{
		SCMessageSystem* pMessageSystem = SCMessageSystem::getPointer();
		// For each message in message storage
		for (size_t iMessage = 0; iMessage < _mvecMessageStorage.size(); iMessage++)
		{
			// For each subscribed user name
			auto it = _mlstSubscribedUserNames.begin();
			while (it != _mlstSubscribedUserNames.end())
			{
				// Get user and send message to it's inbox
				CMessageUser* pUser = pMessageSystem->userGet(*it);
				pUser->_receiveMessageFromService(_mvecMessageStorage[iMessage]);
				it++;
			}
		}

		// Empty message storage of service as all messages are now sent
		_mvecMessageStorage.clear();
	}
}