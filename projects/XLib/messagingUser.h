#pragma once
#include "PCH.h"
#include "messagingMessage.h"
#include "messagingService.h"
#include "templateManager.h"

namespace X
{
	// A message user
	// Can send and receive messages from a message system.
	// By default, a message user isn't subscribed to any message systems, use subscribe() to become subscribed
	// to a message system and be able to send and receive messages to that system.
	class CMessageUser
	{
	public:
		friend class SCMessageSystem;
		CMessageUser();

		// Sets the name of this user
		void setName(const std::string& strUserName);

		// Returns whether this user is subscribed to the named service or not
		bool isSubscribedToService(const std::string& strServiceName);
	private:
		std::string _mstrName;			// Name of this message user.
		std::list<std::string> _mlstSubscribedServiceNames;
	};
}