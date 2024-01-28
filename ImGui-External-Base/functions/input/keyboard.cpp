#include "../../header Files/functions/keyboard.h"

#include <Windows.h>


void pressKey(int key, int holdTime)
{
    keybd_event(key, KEYEVENTF_EXTENDEDKEY, 0, 0);
    Sleep(holdTime);
    keybd_event(key, KEYEVENTF_KEYUP, 0, 0);
}