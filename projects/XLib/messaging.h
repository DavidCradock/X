#pragma once
#include "PCH.h"
#include "singleton.h"
#include "templateManager.h"
#include "messagingService.h"

namespace X
{
	// The message system responsible for handling everything message related.
	// There are three other classes in the messaging system and they are...
	// CMessage which represents a single message that is passed around the messaging system.
	// CMessageUser which represents a single message user which can send and receive messages to and from a...
	// CMessageService which is responsible for receiving and sending messages to/from CMessageUsers.
	class SCMessageSystem : public CSingleton<SCMessageSystem>
	{
	public:
		// Adds a new message service to the system and returns a pointer to it if needed
		// If the named service already exists, an exception occurs.
		CMessageService* serviceAdd(const std::string& strServiceName);

		// Removes a message service from the system
		// If the named service does not exist, an exception occurs.
		void serviceRemove(const std::string& strServiceName);

		// Returns whether the named service currently exists or not
		bool serviceExists(const std::string& strServiceName);

		// Returns a pointer to the named message service.
		// If the named message system does not exist, an exception occurs
		CMessageService* serviceGet(const std::string& strServiceName);

		// Adds a new message user to the system and returns a pointer to it if needed
		// If the named user already exists, an exception occurs.
		CMessageUser* userAdd(const std::string& strUserName);

		// Removes a message user from the system
		// If the named user does not exist, an exception occurs.
		void userRemove(const std::string& strUserName);

		// Returns whether the named user currently exists or not
		bool userExists(const std::string& strUserName);

		// Returns a pointer to the named message user.
		// If the named message user does not exist, an exception occurs
		CMessageUser* userGet(const std::string& strUserName);

		// Subscribes the named message user to the named service
		// If the user does not exist, an exception occurs.
		// If the service does not exist, an exception occurs.
		// If the user is already subscribed, nothing happens.
		void subscribeUserToService(const std::string& strUserName, const std::string& strServiceName);

		// Unsubscribes the named message user from the named service
		// If the user does not exist, an exception occurs.
		// If the service does not exist, an exception occurs.
		// If the user is already unsubscribed, nothing happens.
		void unsubscribeUserFromService(const std::string& strUserName, const std::string& strServiceName);

	private:
		CManager<CMessageService> services;
		CManager<CMessageUser> users;
	};
}