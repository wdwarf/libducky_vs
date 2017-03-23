#include "stdafx.h"
#include "libducky/ui/Dragable.h"

namespace ducky {
	namespace ui {

		static PTSTR _Dragable_Param_Name = _T("DragableParam");

		Dragable::Dragable(HWND hwnd, HWND hwndTarget, DragType type) : dragType(type), drag(false), oldWndProc(0)
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
			Dragable* pThis = (Dragable*)::GetProp(hWnd, _Dragable_Param_Name);
			if (pThis)
			{
				switch (Msg)
				{
				case WM_LBUTTONDOWN:
				{
					POINT p;
					RECT rect;
					::GetCursorPos(&p);
					::GetWindowRect(pThis->hwndTarget, &rect);
					HWND hWndParent = 0;
					if (::GetWindowLong(pThis->hwndTarget, GWL_STYLE) & WS_CHILD)
					{
						hWndParent = ::GetParent(pThis->hwndTarget);
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

					pThis->startPoint.x = p.x - rect.left;
					pThis->startPoint.y = p.y - rect.top;

					::SetWindowPos(pThis->hwndTarget, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

					pThis->drag = true;
					::SetCapture(pThis->hwnd);
					break;
				}
				case WM_LBUTTONUP:
				{
					pThis->drag = false;
					TCHAR buf[100] = { 0 };
					::GetClassName(pThis->hwnd, buf, 100);
					CString className = buf;
					if ((::GetCapture() == pThis->hwnd) && ("button" != className.MakeLower()))
					{
						::ReleaseCapture();
					}
					::SetWindowPos(pThis->hwndTarget, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
					break;
				}
				case WM_MOUSEMOVE:
				{
					if (pThis->drag)
					{
						POINT p;
						::GetCursorPos(&p);
						CRect wndRect;
						CRect rect;
						::GetWindowRect(pThis->hwndTarget, &wndRect);
						rect = wndRect;
						HWND hWndParent = 0;
						if (::GetWindowLong(pThis->hwndTarget, GWL_STYLE) & WS_CHILD)
						{
							hWndParent = ::GetParent(pThis->hwndTarget);
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

						DWORD x = p.x - pThis->startPoint.x;
						DWORD y = p.y - pThis->startPoint.y;
						DWORD width = rect.right - rect.left;
						DWORD height = rect.bottom - rect.top;

						switch (pThis->dragType)
						{
						case DRAG_FREE:
							break;
						case DRAG_INSIDE:
						{

						}
							break;
						case DRAG_EDGE:
						{

						}
							break;
						}

						::MoveWindow(pThis->hwndTarget, x, y, width, height, TRUE);
					}
					break;
				}
				}
				return ::CallWindowProc(pThis->oldWndProc, pThis->hwnd, Msg, wParam, lParam);
			}

			return 0;
		}

	}
}