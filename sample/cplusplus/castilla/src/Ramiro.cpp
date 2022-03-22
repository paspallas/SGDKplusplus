#include "Ramiro.hpp"
#include "IState.hpp"

constexpr fix32 PLAYER_VELOCITY_X { FIX32(1.20) };
constexpr fix32 PLAYER_VELOCITY_Y { FIX32(3.40) };
constexpr fix32 GRAVITY           { FIX32(0.15) };
constexpr fix32 JUMP_ACCELERATION { FIX32(0.03) };

constexpr u8 PLAYER_INITIAL_HITPOINTS  { 3 };
constexpr u8 PLAYER_INITIAL_LIVES      { 3 };
constexpr u8 PLAYER_MAX_LIVES          { 9 };
constexpr u8 PLAYER_WEAPON_COOLDOWN    { 16};

Ramiro::Ramiro(Sprite* spr, const fix16 x, const fix16 y) :
	mSprite(spr)
{
	mPosition.x = x;
	mPosition.y = y;
}

void Ramiro::Update()
{
	mFSM.Update();
}

class StateWalk: public IState
{
	StateWalk()
	{

	}

	~StateWalk() = default;

	virtual void Update() override
	{

	}

	virtual void OnEnter() override
	{

	}

	virtual void OnExit() override
	{

	}
};
