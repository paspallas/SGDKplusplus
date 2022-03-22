#pragma once

#include <genesis.h>
#include "StateMachine.hpp"
#include "Vector2F.hpp"

class Ramiro
{
public:
	Ramiro(Sprite* spr, const fix16 x, const fix16 y);
	~Ramiro() = default;

	void Update();

private:
	Sprite* 		mSprite;
	Vector2F<fix16> mPosition;
	Vector2F<fix16> mVelocity;
	StateMachine 	mFSM;
};
