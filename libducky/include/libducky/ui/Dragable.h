#ifndef LIBDUCKY_UI_Dragable_H_
#define LIBDUCKY_UI_Dragable_H_

#include <Windows.h>

namespace ducky
{
	namespace ui {
		class Dragable
		{
		public:
			Dragable(HWND hwnd, HWND hwndTarget = 0);
			virtual ~Dragable(void);

		protected:
			HWND hwnd;
			HWND hwndTarget;
			bool drag;
			WNDPROC oldWndProc;
			POINT startPoint;

			static LRESULT CALLBACK DragableWindowProc(
				_In_ HWND hWnd,
				_In_ UINT Msg,
				_In_ WPARAM wParam,
				_In_ LPARAM lParam);
		};

	}
}
#endif