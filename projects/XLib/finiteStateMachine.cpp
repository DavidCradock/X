#include "PCH.h"
#include "finiteStateMachine.h"
#include "log.h"

namespace X
{
	CFiniteStateMachine::~CFiniteStateMachine()
	{
		removeAllStates();
	}

	void CFiniteStateMachine::update(void)
	{
		// Find currently set state
		std::map<std::string, CFiniteStateBase*>::iterator it = _mmapStates.find(_mstrCurrentState);
		// _mstrCurrentState might not be set, aka empty
		if (it != _mmapStates.end())
			it->second->onActive(this);
	}

	void CFiniteStateMachine::switchToState(const std::string& strStateName)
	{
		// Find named state
		std::map<std::string, CFiniteStateBase*>::iterator it = _mmapStates.find(strStateName);
		ThrowIfTrue(it == _mmapStates.end(), "CFiniteStateMachine::switchToState(\"" + strStateName + "\") failed. State name doesn't exist.");

		// Find currently set state
		std::map<std::string, CFiniteStateBase*>::iterator itCurrentState = _mmapStates.find(_mstrCurrentState);
		// _mstrCurrentState might not be set, aka empty
		if (itCurrentState != _mmapStates.end())
			itCurrentState->second->onExit();	// Exit current state

		// Enter new state
		it->second->onEnter();

		// Set new state to currently set state
		_mstrCurrentState = strStateName;
	}

	void CFiniteStateMachine::addState(const std::string& strStateName, CFiniteStateBase* pState)
	{
		std::map<std::string, CFiniteStateBase*>::iterator it = _mmapStates.find(strStateName);
		ThrowIfTrue(it != _mmapStates.end(), "CFiniteStateMachine::addState(\"" + strStateName + "\") failed. State name already exists.");
		_mmapStates[strStateName] = pState;
	}

	void CFiniteStateMachine::removeState(const std::string& strStateName)
	{
		std::map<std::string, CFiniteStateBase*>::iterator it = _mmapStates.find(strStateName);
		ThrowIfTrue(it == _mmapStates.end(), "CFiniteStateMachine::removeState(\"" + strStateName + "\") failed. State name doesn't exist.");
		delete it->second;
		_mmapStates.erase(it);
	}

	bool CFiniteStateMachine::getStateExists(const std::string& strStateName)
	{
		std::map<std::string, CFiniteStateBase*>::iterator it = _mmapStates.find(strStateName);
		return (it != _mmapStates.end());
	}

	void CFiniteStateMachine::removeAllStates(void)
	{
		std::map<std::string, CFiniteStateBase*>::iterator it = _mmapStates.begin();
		while (it != _mmapStates.end())
		{
			delete it->second;
			_mmapStates.erase(it);
			it = _mmapStates.begin();
		}
	}
}