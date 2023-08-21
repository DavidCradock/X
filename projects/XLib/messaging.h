#pragma once
#include "PCH.h"

namespace X
{
	// Message types
	// Used when constructing a message to set it's type.
	// Used to differenciate between different types of messages.
	enum EMessageType
	{

	};

	// A message object used by a messaging system and message user
	class CMessage
	{
	public:
		// Constructor, preparing the message
		CMessage(const std::string& strMessage, EMessageType eMessageType);

		// Returns a string holding the message
		std::string& getMessageContents(void) const;

		// Returns the type of this message
		EMessageType getMessageType(void) const;
	private:
		std::string _mstrMessage;	// String holding the message, given to constructor
		EMessageType _mType;		// The type of message this is
	};

	// A message user
	// Can send and recieve messages from a message system
	class CMessageUser
	{
	public:
		// Constructor, subscribes the user to listen to messages from the given message system
		CMessageUser(const std::string& strUserName, const CMessageSystem* pMessageSystem);

		// Sends the given message to the message system to be sent to all subscribed users of that message system
		void sendMessage(const CMessage& message);

		// Recieves a vector holding all messages sent from the subscribed message system
		std::vector<CMessage> recieveMessages(void);
	private:
		std::string _mstrUserName;			// Name of this message user.
		CMessageSystem* _mpMessageSystem;	// The message system this user is subscribed to.
	};

	// A message system
	class CMessageSystem
	{
	public:
		// Recieve a message from a subsribed user
		void recieveMessage(CMessage& message);

		// Dispatches all recieved messages to all subscribed users
		void dispatchMessages(void);
	private:
	};
}