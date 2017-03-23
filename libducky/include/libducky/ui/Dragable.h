#ifndef LIBDUCKY_UI_Dragable_H_
#define LIBDUCKY_UI_Dragable_H_

#include "libducky_config.h"
#include <libducky/Object.h>
#include <Windows.h>

namespace ducky
{
	namespace ui
	{
		enum DragType
		{
			DRAG_FREE,
			DRAG_INSIDE,
			DRAG_EDGE
		};

		class Dragable : virtual public Object
		{
		public:
			Dragable(HWND hwnd, HWND hwndTarget = 0, 
				DragType type = DRAG_FREE);
			virtual ~Dragable(void);

		protected:
			HWND hwnd;
			HWND hwndTarget;
			DragType dragType;
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