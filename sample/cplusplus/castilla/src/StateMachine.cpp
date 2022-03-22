#include "StateMachine.hpp"
#include "IState.hpp"

StateMachine::StateMachine() :
	mCurrentState(nullptr)
{}

void StateMachine::Update()
{
	mCurrentState->Update();
}

void StateMachine::SetState(IState* state)
{
	if (mCurrentState != nullptr)
		mCurrentState->OnExit();

	mCurrentState = state;
	mCurrentState->OnEnter();
}
