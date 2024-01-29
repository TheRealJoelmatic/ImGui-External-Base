#include "renderer.h"
#include "../../header Files/globals.hh"
#include "../hijack/hijack.h"
#include <iostream>
#include <limits>
#include <d2d1.h>

namespace renderer {
	namespace scene {
		void begin() {
			internal::render_target->BeginDraw();
		}
		void start() {
			begin();
			clear();
		}
		
		void end() {
			internal::render_target->EndDraw();

			// code below is really just for convience, without the if statement you would have to
			// manually close each window as when clicking onto the game the focus doesnt close the 
			// other window. Comment it out and u will see what I mean. the statement fixes it
			// by brigning overlay to topmost (removes flag imdeallty) then putting game winodw
			// right under it 
			try {
				static bool toggle = true;
				if (GetForegroundWindow() == sdk.hwnd) {
					if (toggle) {
						SetWindowPos(hijack::hijacked_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
						SetWindowPos(hijack::hijacked_hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
						ShowWindow(hijack::hijacked_hwnd, SW_SHOW);
						toggle = false;
					}
				}
				else {
					ShowWindow(hijack::hijacked_hwnd, SW_HIDE);
					toggle = true;
				}

				// sets game window under overlay
				SetWindowPos(sdk.hwnd, hijack::hijacked_hwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
			catch (const std::exception& e){}
		}

		void clear() {
			internal::render_target->Clear();
		}

		void text(Vector2 pos, const wchar_t* text, ID2D1SolidColorBrush* color, IDWriteTextFormat* font) {
			D2D1_RECT_F rect = D2D1::RectF(
				pos.x, pos.y,
				static_cast<FLOAT>(INT_MAX), static_cast<FLOAT>(INT_MAX)
			);

			internal::render_target->DrawTextA( text, lstrlenW( text ), font, rect, color, D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL );
		}

		void box(int x, int y, int width, int height, int thick, ID2D1SolidColorBrush* color) {
			internal::render_target->DrawRectangle(D2D1::RectF(x, y, x + width, y + height), color, thick);
		}

		void shutdown() {
			begin();
			clear();
			end();
		}

		void renderThread() {
			while (true) {
				start();
				if (modules.isCrosshairOn) {
					text(Vector2(globals.screenX / 2, globals.screenY / 2), L"+", renderer::colors::red_color, renderer::fonts::watermark_font);
				}

				end();
			}
		}
	}
}