#pragma once
#include "PCH.h"

namespace X
{
	class CFiniteStateMachine;

	// Finite state base class with which to derive our FSM state classes from for use with the CFiniteStateMachine class.
	// This represents a single state in a finite state machine and has several pure virtual methods which need to be 
	// implemented within any class which is derived from this one.
	class CFiniteStateBase
	{
	public:
		// This is called when this state is first entered.
		virtual void onEnter(void) = 0;

		// This is called when this state is left.
		virtual void onExit(void) = 0;

		// This is called whilst the state is active.
		// pFSM is a pointer to the CFiniteStateMachine this state is stored in.
		// This is usefull to allow the state to change to another named state by calling pFSM->switchToState()
		virtual void onActive(CFiniteStateMachine* pFSM) = 0;
	};

	// This is a generic finite state machine used alongside classes derived from CFiniteStateBase.
	// Each state has three methods which need to be implemented (See CFiniteStateBase)
	// First create one or more classes derived from the base class and implement each of the three methods,
	// onEnter(), onExit() and onActive().
	// We create an object of this class and objects of each of our classes derived from the CFiniteStateBase class.
	// Then we add each of those objects to this one, specifying a unique name for each state.
	// Initially, the state machine has no state and upon calling update() will do nothing.
	// We need to call switchToState(std::string strStateName) to set the initial state.
	// Upon doing so, the state's onEnter() method will be called, followed by it's onActive() method, until the
	// state machine is asked to switch to another state with a call of switchToState() either externally, or from
	// within a state itself.
	// Usage example:
	// First, create a couple of classes derived from the CFiniteStateBase class which will be used for the FSM's states
	// class CMyFirstState : public CFiniteStateBase
	// {
	// public:
	//		void onEnter(void){	// Do stuff for when this state is entered	}
	//		void onExit(void){	// Do stuff for when this state is left	}
	//		void onActive(CFiniteStateMachine* pFSM){ pFSM->switchToState("MySecondState");	// Switch to the second state }
	// };
	// class CMySecondState : public CFiniteStateBase
	// {
	// public:
	//		void onEnter(void){	// Do stuff for when this state is entered	}
	//		void onExit(void){	// Do stuff for when this state is left	}
	//		void onActive(CFiniteStateMachine* pFSM){ // Do stuff in this state }
	// };
	// // Now that the two state classes have been created, we need to create the FSM itself and allocate memory for each of the state classes.
	// CFiniteStateMachine fsm;
	// CMyFirstState* pStateOne = new CMyFirstState;
	// CMySecondState* pStateTwo = new CMySecondState;
	// // Now we need to add each state to the FSM, giving them each a unique name...
	// fsm.addState("state1", pStateOne);
	// fsm.addState("state2", pStateTwo);
	// // Note: There's no need to delete either of the states, as this is done for us
	// // when the FSM is destroyed, or by calling either removeState() or removeAllStates() of the FSM object.
	// // Now call fsm.update() to call the currently set state's onActive() method.
	class CFiniteStateMachine
	{
	public:

		// Frees the state pointers which have been added with a call to addState()
		~CFiniteStateMachine();

		// Updates the state machine, calling the currently set state's onActive() method.
		// Remember that initially, no current state is set, so this will initially do nothing until switchToState() is called.
		void update(void);

		// Switches to the named state.
		// This usually calls the currently set state's onExit() method, followed by the new state's onEnter() method.
		// But the first time this is called, the FSM has no currently set state, so the call to onExit() is not performed.
		// If the named state doesn't exist, then an exception occurs.
		void switchToState(const std::string& strStateName);

		// Adds a new uniquely named state
		// If the name already exists, an exception occurs.
		// The CFiniteStateBase object has to be allocated yourself prior to calling this method.
		// Once the state object has been added here, there's no need to keep it around for deletion later,
		// as this class's removeState(), removeAllStates() and destructor will delete them for us.
		// Also note that this method checks to see if the given state pointer is NULL and throws an exception if so.
		void addState(const std::string& strStateName, CFiniteStateBase* pState);

		// Removes a previously added state
		// If the named state doesn't exist, an exception occurs
		// The state given during a call to addState() is deleted for us here, no need to delete it ourselves.
		void removeState(const std::string& strStateName);

		// Returns whether a named state has already been added or not
		bool getStateExists(const std::string& strStateName);

		// Removes all previously added states
		// The states given during calls to addState() are deleted for us here, no need to delete them ourselves.
		void removeAllStates(void);

	private:
		std::string _mstrCurrentState;							// The name of the currently set state. This is initially empty, IE, there's no state.
		std::map<std::string, CFiniteStateBase*> _mmapStates;	// Holds each uniquely named state and a pointer to the object.
	};
}