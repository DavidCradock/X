#include "PCH.h"
#include "messagingUser.h"
#include "singletons.h"
#include "messaging.h"

namespace X
{
	CMessageUser::CMessageUser()
	{
		_mstrName = "UNNAMED";
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

	void CMessageUser::postNewMessage(CMessage& newMessage, const std::string& strServiceName)
	{
		SCMessageSystem* pMessageSystem = SCMessageSystem::getPointer();
		ThrowIfFalse(pMessageSystem->serviceExists(strServiceName), "CMessageUser::postNewMessage() failed. The given service to send the message to called \"" + strServiceName + "\" does not exist.");
		CMessageService* pService = pMessageSystem->serviceGet(strServiceName);

		// Set name of the sender to this user
		newMessage._mstrSenderName = _mstrName;

		// Set message to the named service
		pService->_receiveMessageFromUser(newMessage);
	}

	void CMessageUser::_receiveMessageFromService(const CMessage& message)
	{
		_mlistInbox.push_back(message);
	}

	bool CMessageUser::doesInboxContainMessages(void)
	{
		return _mlistInbox.size() > 0;
	}

	CMessage CMessageUser::getMessageFromInbox(void)
	{
		ThrowIfTrue(!_mlistInbox.size(), "CMessageUser::getMessageFromInbox() failed. There are no more messages in the inbox.");
		CMessage message = *_mlistInbox.begin();
		_mlistInbox.pop_front();
		return message;
	}

	std::vector<CMessage> CMessageUser::getMessagesFromInbox(void)
	{
		std::vector<CMessage> messages;
		auto it = _mlistInbox.begin();
		while (it != _mlistInbox.end())
		{
			messages.push_back(*it);
			it++;
		}
		_mlistInbox.clear();
		return messages;
	}

	void CMessageUser::emptyInbox(void)
	{
		_mlistInbox.clear();
	}
}