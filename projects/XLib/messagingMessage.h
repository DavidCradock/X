#pragma once
#include "PCH.h"

namespace X
{
	// Message types
	// Used when constructing a message to set it's type.
	// Used to differenciate between different types of messages.
	// Add more here if you need them
	enum EMessageType
	{
		UNKNOWN,	// Unknown type
		WINDOW		// Window related message
	};

	// A message object used by a messaging system and message user
	class CMessage
	{
	public:
		friend class CMessageUser;

		// Constructor, preparing the message
		CMessage(const std::string& strMessageContents, EMessageType eMessageType = EMessageType::UNKNOWN);

		// Returns a string holding the message
		const std::string& getMessageContents(void) const;

		// Returns a string holding the sender's name
		const std::string& getMessageSender(void) const;

		// Returns the type of this message
		EMessageType getMessageType(void) const;
	private:
		std::string _mstrContents;		// String holding the message, given to constructor.
		std::string _mstrSenderName;	// String holding the message's sender's name, given to constructor.
		EMessageType _meType;			// The type of message this is
	};

}