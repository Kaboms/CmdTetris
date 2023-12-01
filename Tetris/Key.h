#pragma once
//----------------------------------------------------------------------
#include <stdint.h>
//----------------------------------------------------------------------

class Key
{
public:
	Key(uint8_t code)
	{
		Code = code;
	}

	static uint8_t GetKey();

	static bool KeyPressed(uint8_t key);

	bool IsPressed();

	bool HandlePressedOnce();

	void Handle();

protected:
	uint8_t Code;

	bool PressedOnce = false;

	bool WaitRelease = false;
};
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------

