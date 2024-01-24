#include "../../header Files/functions/mouse.h"

#include <Windows.h>

void Move(int x, int y, int s)
{
	mouse_event(0x0001, x, y, 0, 0);
	Sleep(s);
}

void click(int holdTime)
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(holdTime);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}