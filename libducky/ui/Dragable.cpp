#include "stdafx.h"
#include "libducky/ui/Dragable.h"

namespace ducky {
	namespace ui {

		static PTSTR _Dragable_Param_Name = _T("DragableParam");

		Dragable::Dragable(HWND hwnd, HWND hwndTarget) : drag(false), oldWndProc(0)
		{
			ASSERT(hwnd);
			this->hwnd = hwnd;
			this->hwndTarget = hwndTarget ? hwndTarget : this->hwnd;
			::SetProp(this->hwnd, _Dragable_Param_Name, this);
			this->oldWndProc = (WNDPROC)::GetWindowLong(this->hwnd, GWL_WNDPROC);
			::SetWindowLong(this->hwnd, GWL_WNDPROC, (LONG)Dragable::DragableWindowProc);
		}


		Dragable::~Dragable(void)
		{
			::SetWindowLong(this->hwnd, GWL_WNDPROC, (LONG)this->oldWndProc);
			::RemoveProp(this->hwnd, _Dragable_Param_Name);
		}

		LRESULT CALLBACK Dragable::DragableWindowProc(
			_In_ HWND hWnd,
			_In_ UINT Msg,
			_In_ WPARAM wParam,
			_In_ LPARAM lParam)
		{
			Dragable* dObj = (Dragable*)::GetProp(hWnd, _Dragable_Param_Name);
			if (dObj)
			{
				switch (Msg)
				{
				case WM_LBUTTONDOWN:
				{
					POINT p;
					RECT rect;
					::GetCursorPos(&p);
					::GetWindowRect(dObj->hwndTarget, &rect);
					HWND hWndParent = 0;
					if (::GetWindowLong(dObj->hwndTarget, GWL_STYLE) & WS_CHILD)
					{
						hWndParent = ::GetParent(dObj->hwndTarget);
					}
					POINT lt, rb;
					lt.x = rect.left;
					lt.y = rect.top;
					rb.x = rect.right;
					rb.y = rect.bottom;
					if (hWndParent)
					{
						::ScreenToClient(hWndParent, &p);
						::ScreenToClient(hWndParent, &lt);
						::ScreenToClient(hWndParent, &rb);
					}
					rect.left = lt.x;
					rect.top = lt.y;
					rect.right = rb.x;
					rect.bottom = rb.y;

					dObj->startPoint.x = p.x - rect.left;
					dObj->startPoint.y = p.y - rect.top;

					::SetWindowPos(dObj->hwndTarget, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

					dObj->drag = true;
					::SetCapture(dObj->hwnd);
					break;
				}
				case WM_LBUTTONUP:
				{
					dObj->drag = false;
					TCHAR buf[100] = { 0 };
					::GetClassName(dObj->hwnd, buf, 100);
					CString className = buf;
					if ((::GetCapture() == dObj->hwnd) && ("button" != className.MakeLower()))
					{
						::ReleaseCapture();
					}
					::SetWindowPos(dObj->hwndTarget, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
					break;
				}
				case WM_MOUSEMOVE:
				{
					if (dObj->drag)
					{
						POINT p;
						::GetCursorPos(&p);
						RECT rect;
						::GetWindowRect(dObj->hwndTarget, &rect);
						HWND hWndParent = 0;
						if (::GetWindowLong(dObj->hwndTarget, GWL_STYLE) & WS_CHILD)
						{
							hWndParent = ::GetParent(dObj->hwndTarget);
						}
						POINT lt, rb;
						lt.x = rect.left;
						lt.y = rect.top;
						rb.x = rect.right;
						rb.y = rect.bottom;
						if (hWndParent)
						{
							::ScreenToClient(hWndParent, &p);
							::ScreenToClient(hWndParent, &lt);
							::ScreenToClient(hWndParent, &rb);
						}
						rect.left = lt.x;
						rect.top = lt.y;
						rect.right = rb.x;
						rect.bottom = rb.y;

						DWORD x = p.x - dObj->startPoint.x;
						DWORD y = p.y - dObj->startPoint.y;
						DWORD width = rect.right - rect.left;
						DWORD height = rect.bottom - rect.top;

						::MoveWindow(dObj->hwndTarget, x, y, width, height, TRUE);
					}
					break;
				}
				}
				return ::CallWindowProc(dObj->oldWndProc, dObj->hwnd, Msg, wParam, lParam);
			}

			return 0;
		}

	}
}