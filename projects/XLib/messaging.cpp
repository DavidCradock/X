#include "PCH.h"
#include "messaging.h"
#include "logging.h"

namespace X
{
	CMessageService* SCMessageSystem::serviceAdd(const std::string& strServiceName)
	{
		ThrowIfTrue(services.exists(strServiceName), "SCMessageSystem::serviceAdd(\"" + strServiceName + "\") failed. The named service already exists.");
		return services.add(strServiceName);
	}

	void SCMessageSystem::serviceRemove(const std::string& strServiceName)
	{
		ThrowIfFalse(services.exists(strServiceName), "SCMessageSystem::serviceRemove(\"" + strServiceName + "\") failed. The named service does not exist.");

		// Remove service from each user's subscription list
		for (size_t i = 0; i < users.getNumber(); i++)
		{
			users.get(i)->_mlstSubscribedServiceNames.remove(strServiceName);
		}

		services.remove(strServiceName);
	}

	bool SCMessageSystem::serviceExists(const std::string& strServiceName)
	{
		return services.exists(strServiceName);
	}

	CMessageService* SCMessageSystem::serviceGet(const std::string& strServiceName)
	{
		ThrowIfFalse(services.exists(strServiceName), "SCMessageSystem::serviceGet(\"" + strServiceName + "\") failed. The named service does not exist.");
		return services.get(strServiceName);
	}

	CMessageUser* SCMessageSystem::userAdd(const std::string& strUserName)
	{
		ThrowIfTrue(users.exists(strUserName), "SCMessageSystem::userAdd(\"" + strUserName + "\") failed. The named user already exists.");
		CMessageUser* pUser = users.add(strUserName);
		pUser->setName(strUserName);
		return pUser;
	}

	void SCMessageSystem::userRemove(const std::string& strUserName)
	{
		ThrowIfFalse(users.exists(strUserName), "SCMessageSystem::userRemove(\"" + strUserName + "\") failed. The named user does not exist.");

		// Remove user from each service's subscription list
		for (size_t i = 0; i < services.getNumber(); i++)
		{
			services.get(i)->_mlstSubscribedUserNames.remove(strUserName);
		}

		users.remove(strUserName);
	}

	bool SCMessageSystem::userExists(const std::string& strUserName)
	{
		return users.exists(strUserName);
	}

	CMessageUser* SCMessageSystem::userGet(const std::string& strUserName)
	{
		ThrowIfFalse(users.exists(strUserName), "SCMessageSystem::userGet(\"" + strUserName + "\") failed. The named user does not exist.");
		return users.get(strUserName);
	}

	void SCMessageSystem::subscribeUserToService(const std::string& strUserName, const std::string& strServiceName)
	{
		// Make sure both the user and service exist
		ThrowIfFalse(users.exists(strUserName), "SCMessageSystem::subscribeUserToService(\"" + strUserName + ", " + strServiceName + "\") failed. The named user does not exist.");
		ThrowIfFalse(services.exists(strServiceName), "SCMessageSystem::subscribeUserToService(\"" + strUserName + ", " + strServiceName + "\") failed. The named service does not exist.");

		// Add service to the user's subscribed service list
		CMessageUser* pUser = users.get(strUserName);
		if (!pUser->isSubscribedToService(strServiceName))
			pUser->_mlstSubscribedServiceNames.push_back(strServiceName);

		// Add the user to the service's subscribed user list
		CMessageService* pService = services.get(strServiceName);
		if (!pService->isSubscribedToService(strUserName))
			pService->_mlstSubscribedUserNames.push_back(strUserName);
	}

	void SCMessageSystem::unsubscribeUserFromService(const std::string& strUserName, const std::string& strServiceName)
	{
		// Make sure both the user and service exist
		ThrowIfFalse(users.exists(strUserName), "SCMessageSystem::unsubscribeUserFromService(\"" + strUserName + ", " + strServiceName + "\") failed. The named user does not exist.");
		ThrowIfFalse(services.exists(strServiceName), "SCMessageSystem::unsubscribeUserFromService(\"" + strUserName + ", " + strServiceName + "\") failed. The named service does not exist.");

		// Remove service from the user's subscribed service list
		CMessageUser* pUser = users.get(strUserName);
		if (pUser->isSubscribedToService(strServiceName))
			pUser->_mlstSubscribedServiceNames.remove(strServiceName);

		// Remove the user from the service's subscribed user list
		CMessageService* pService = services.get(strServiceName);
		if (pService->isSubscribedToService(strUserName))
			pService->_mlstSubscribedUserNames.remove(strUserName);
	}
}