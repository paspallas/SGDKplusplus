#pragma once

#include "JoyPad.hpp"

class Object;

class IState
{
public:
	virtual ~IState() {}
	virtual void Update(Object& obj, const Buttons& input) = 0;
	virtual void OnEnter(Object& obj) = 0;
	virtual void OnExit(Object& obj) = 0;
};
