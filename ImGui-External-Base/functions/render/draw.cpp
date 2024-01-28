#include "../../header Files/functions/render/draw.h"

void DrawBox(HDC hdc, int screenX, int screenY, int width, int height) {
    // Create a red pen for drawing the box
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);

    // Draw the box
    Rectangle(hdc, screenX, screenY, screenX + width, screenY + height);
}