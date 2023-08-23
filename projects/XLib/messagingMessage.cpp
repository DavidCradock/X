#include "PCH.h"
#include "messagingMessage.h"

namespace X
{
	CMessage::CMessage(const std::string& strMessageContents, EMessageType eMessageType)
	{
		_mstrContents = strMessageContents;
		_mstrSenderName = "UNKNOWN";
		_meType = eMessageType;
	}

	const std::string& CMessage::getMessageContents(void) const
	{
		return _mstrContents;
	}

	const std::string& CMessage::getMessageSender(void) const
	{
		return _mstrSenderName;
	}

	EMessageType CMessage::getMessageType(void) const
	{
		return _meType;
	}
}