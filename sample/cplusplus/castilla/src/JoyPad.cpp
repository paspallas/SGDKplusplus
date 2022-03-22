#include "JoyPad.hpp"
#include <cstring>

JoyPad::JoyPad() :
	mOldState(0)
	, mCurrentState(0)
{
	Clear();
}

JoyPad& JoyPad::GetInstance()
{
	static JoyPad instance;
	return instance;
}

void JoyPad::Clear()
{
	std::memset(&State, 0, sizeof(Buttons));
	mOldState = mCurrentState = 0;
}

void JoyPad::Update()
{
	mOldState = mCurrentState;
	mCurrentState = JOY_readJoypad(JOY_1);

	const u16 pressed      = mCurrentState;
	const u16 released     = (~mCurrentState) & mOldState;
	const u16 hold         = mCurrentState & mOldState;
	const u16 pressed_once = (mCurrentState & (~mOldState));

	State.A_Pressed     = pressed & BUTTON_A;
	State.B_Pressed     = pressed & BUTTON_B;
	State.C_Pressed     = pressed & BUTTON_C;
	State.LEFT_Pressed  = pressed & BUTTON_LEFT;
	State.RIGHT_Pressed = pressed & BUTTON_RIGHT;
	State.UP_Pressed    = pressed & BUTTON_UP;
	State.DOWN_Pressed  = pressed & BUTTON_DOWN;

	State.A_Released     = released & BUTTON_A;
	State.B_Released     = released & BUTTON_B;
	State.C_Released     = released & BUTTON_C;
	State.LEFT_Released  = released & BUTTON_LEFT;
	State.RIGHT_Released = released & BUTTON_RIGHT;
	State.UP_Released    = released & BUTTON_UP;
	State.DOWN_Released  = released & BUTTON_DOWN;

	State.A_Hold     = hold & BUTTON_A;
	State.B_Hold     = hold & BUTTON_B;
	State.C_Hold     = hold & BUTTON_C;
	State.LEFT_Hold  = hold & BUTTON_LEFT;
	State.RIGHT_Hold = hold & BUTTON_RIGHT;
	State.UP_Hold    = hold & BUTTON_UP;
	State.DOWN_Hold  = hold & BUTTON_DOWN;

	State.A_PressedOnce     = pressed_once & BUTTON_A;
	State.B_PressedOnce     = pressed_once & BUTTON_B;
	State.C_PressedOnce     = pressed_once & BUTTON_C;
	State.LEFT_PressedOnce  = pressed_once & BUTTON_LEFT;
	State.RIGHT_PressedOnce = pressed_once & BUTTON_RIGHT;
	State.UP_PressedOnce    = pressed_once & BUTTON_UP;
	State.DOWN_PressedOnce  = pressed_once & BUTTON_DOWN;

	State.START_PressedOnce = pressed_once & BUTTON_START;
}
