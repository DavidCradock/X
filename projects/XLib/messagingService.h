#pragma once
#include "PCH.h"
#include "messagingMessage.h"
#include "messagingUser.h"
#include "templateManager.h"

namespace X
{
	// A message system service
	// A message service is responsible for receiving and sending messages from/to CMessageUsers.
	class CMessageService
	{
		friend class SCMessageSystem;
	public:
		CMessageService();

		~CMessageService();

		
		// Returns whether the given user is subscribed to this service or not
		bool isSubscribedToService(const std::string& strUserName);
	private:
		std::list<std::string> _mlstSubscribedUserNames;
		
	};
}