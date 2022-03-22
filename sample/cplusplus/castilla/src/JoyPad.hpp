#pragma once

#include <genesis.h>

struct Buttons
{
	// pressed at the current frame
	u8 A_Pressed;
	u8 B_Pressed;
	u8 C_Pressed;
	u8 LEFT_Pressed;
	u8 RIGHT_Pressed;
	u8 UP_Pressed;
	u8 DOWN_Pressed;

	// just released
	u8 A_Released;
	u8 B_Released;
	u8 C_Released;
	u8 LEFT_Released;
	u8 RIGHT_Released;
	u8 UP_Released;
	u8 DOWN_Released;

	// didn't change from the previous frame
	u8 A_Hold;
	u8 B_Hold;
	u8 C_Hold;
	u8 LEFT_Hold;
	u8 RIGHT_Hold;
	u8 UP_Hold;
	u8 DOWN_Hold;

	// pressed and changed from the previous frame
	u8 A_PressedOnce;
	u8 B_PressedOnce;
	u8 C_PressedOnce;
	u8 LEFT_PressedOnce;
	u8 RIGHT_PressedOnce;
	u8 UP_PressedOnce;
	u8 DOWN_PressedOnce;

	u8 START_PressedOnce;
};

class JoyPad
{
public:
	static JoyPad& GetInstance();
	void Clear();
	void Update();

	Buttons State;

private:
	JoyPad();
	~JoyPad() = default;
	JoyPad(const JoyPad&) = delete;
	JoyPad& operator=(const JoyPad&) = delete;

	u16 mOldState;
	u16 mCurrentState;
};
