#include "Key.h"
#include <conio.h>
#include <Windows.h>
//----------------------------------------------------------------------

uint8_t Key::GetKey()
{
	return _getch();
}

bool Key::KeyPressed(uint8_t key)
{
	return GetKeyState(key) & 0x8000;
}

bool Key::IsPressed()
{
	return Key::KeyPressed(Code);
}

bool Key::HandlePressedOnce()
{
	if (PressedOnce)
	{
		PressedOnce = false;
		return true;
	}

	return false;

}

void Key::Handle()
{
	if (IsPressed() && !WaitRelease)
	{
		PressedOnce = true;
		WaitRelease = true;
	}
	else if (!IsPressed())
	{
		WaitRelease = false;
	}
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
