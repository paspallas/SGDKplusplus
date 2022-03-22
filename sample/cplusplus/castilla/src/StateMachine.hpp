#pragma once

class IState;

class StateMachine
{
public:
	StateMachine();
	~StateMachine() = default;

	void SetState(IState* state);
	void Update();

private:
	IState* mCurrentState;
};
