#include "CMDGui.h"
#include <iostream>
//----------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------------

HANDLE CMDGui::GetOutputHandle()
{
	return GetStdHandle(STD_OUTPUT_HANDLE);
}
//----------------------------------------------------------------------

void CMDGui::ShowColorTable()
{
	// you can loop k higher to see more color choices
	for (int k = 0; k < 255; ++k)
	{
		// pick the colorattribute k you want
		SetConsoleTextAttribute(CMDGui::GetOutputHandle(), k);
		cout << k << " I want to be nice today!" << endl;
	}
}
//----------------------------------------------------------------------

void CMDGui::SetConsoleSize(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);

	MoveWindow(console, r.left, r.top, 800, 600, TRUE); 
}
//----------------------------------------------------------------------

void CMDGui::SetConsoleTextColor(Color foreground, Color background)
{
	SetConsoleTextAttribute(CMDGui::GetOutputHandle(), foreground + (background * 16));
}
//----------------------------------------------------------------------

void CMDGui::SetConsoleTextColor(uint8_t color)
{
	SetConsoleTextAttribute(CMDGui::GetOutputHandle(), color);
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------

