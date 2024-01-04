#include "../header Files/globals.hh"

void keyboard::pressKey(int key, int holdTime)
{
    keybd_event(key, KEYEVENTF_EXTENDEDKEY, 0, 0);
    Sleep(holdTime);
    keybd_event(key, KEYEVENTF_KEYUP, 0, 0);
}