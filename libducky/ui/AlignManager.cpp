#include "stdafx.h"
#include "libducky/ui/AlignManager.h"

namespace ducky {
	namespace ui {

		AlignManager::AlignManager(void)
		{

		}


		AlignManager::~AlignManager(void)
		{
		}

		void AlignManager::AddWnd(HWND wnd, AlignType align, Margin margin)
		{
			ASSERT(wnd);
			//if(!this->IsWndInList(wnd))
			{
				AlignProperty ap;
				ap.wnd = wnd;
				ap.align = align;
				ap.margin = margin;
				GetWindowRect(wnd, &ap.originalWindowRect);
				this->wndList.push_back(ap);
			}
		}

		bool AlignManager::IsWndInList(HWND wnd)
		{
			for (list<AlignProperty>::iterator it = this->wndList.begin();
			it != this->wndList.end(); ++it)
			{
				if (it->wnd == wnd)
				{
					return true;
				}
			}
			return false;
		}

		void AlignManager::Execute()
		{
			for (list<AlignProperty>::iterator it = this->wndList.begin();
			it != this->wndList.end(); ++it)
			{
				AlignProperty& ap = *it;
				switch (ap.align)
				{
				case AT_Left:
					HandleAlignLeft(ap);
					break;
				case AT_Top:
					HandleAlignTop(ap);
					break;
				case AT_Right:
					HandleAlignRight(ap);
					break;
				case AT_Bottom:
					HandleAlignBottom(ap);
					break;
				case AT_HCenter:
					HandleAlignHCenter(ap);
					break;
				case AT_VMiddle:
					HandleAlignVMiddle(ap);
					break;
				case AT_WidthFill:
					HandleAlignWidthFill(ap);
					break;
				case AT_HeightFill:
					HandleAlignHeightFill(ap);
					break;
				case AT_Client:
					HandleAlignClient(ap);
					break;
				}
			}
		}

		void AlignManager::HandleAlignLeft(AlignProperty& ap)
		{
			CRect clientRect;
			CRect wndRect;
			HWND parentHWnd = ::GetParent(ap.wnd);

			::GetClientRect(parentHWnd, &clientRect);
			::GetWindowRect(ap.wnd, &wndRect);
			CPoint lt(wndRect.left, wndRect.top);
			CPoint rb(wndRect.right, wndRect.bottom);
			::ScreenToClient(parentHWnd, &lt);
			::ScreenToClient(parentHWnd, &rb);
			wndRect = CRect(lt, rb);

			CRect rect(
				ap.margin.marginLeft,
				wndRect.top,
				ap.margin.marginLeft + wndRect.Width(),
				wndRect.bottom);

			::MoveWindow(ap.wnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		}

		void AlignManager::HandleAlignRight(AlignProperty& ap)
		{
			CRect clientRect;
			CRect wndRect;
			HWND parentHWnd = ::GetParent(ap.wnd);

			::GetClientRect(parentHWnd, &clientRect);
			::GetWindowRect(ap.wnd, &wndRect);
			CPoint lt(wndRect.left, wndRect.top);
			CPoint rb(wndRect.right, wndRect.bottom);
			::ScreenToClient(parentHWnd, &lt);
			::ScreenToClient(parentHWnd, &rb);
			wndRect = CRect(lt, rb);

			CRect rect(
				clientRect.Width() - wndRect.Width() - ap.margin.marginRight,
				wndRect.top,
				clientRect.right - ap.margin.marginRight,
				wndRect.bottom);

			::MoveWindow(ap.wnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		}

		void AlignManager::HandleAlignTop(AlignProperty& ap)
		{
			CRect clientRect;
			CRect wndRect;
			HWND parentHWnd = ::GetParent(ap.wnd);

			::GetClientRect(parentHWnd, &clientRect);
			::GetWindowRect(ap.wnd, &wndRect);
			CPoint lt(wndRect.left, wndRect.top);
			CPoint rb(wndRect.right, wndRect.bottom);
			::ScreenToClient(parentHWnd, &lt);
			::ScreenToClient(parentHWnd, &rb);
			wndRect = CRect(lt, rb);

			CRect rect(
				wndRect.left,
				ap.margin.marginTop,
				wndRect.left + wndRect.Width(),
				ap.margin.marginTop + wndRect.Height());

			::MoveWindow(ap.wnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		}

		void AlignManager::HandleAlignBottom(AlignProperty& ap)
		{
			CRect clientRect;
			CRect wndRect;
			HWND parentHWnd = ::GetParent(ap.wnd);

			::GetClientRect(parentHWnd, &clientRect);
			::GetWindowRect(ap.wnd, &wndRect);
			CPoint lt(wndRect.left, wndRect.top);
			CPoint rb(wndRect.right, wndRect.bottom);
			::ScreenToClient(parentHWnd, &lt);
			::ScreenToClient(parentHWnd, &rb);
			wndRect = CRect(lt, rb);

			int top = clientRect.bottom - wndRect.Height() - ap.margin.marginBottom;
			CRect rect(
				wndRect.left,
				top,
				wndRect.left + wndRect.Width(),
				top + wndRect.Height());

			::MoveWindow(ap.wnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		}

		void AlignManager::HandleAlignHCenter(AlignProperty& ap)
		{
			CRect clientRect;
			CRect wndRect;
			HWND parentHWnd = ::GetParent(ap.wnd);

			::GetClientRect(parentHWnd, &clientRect);
			::GetWindowRect(ap.wnd, &wndRect);
			CPoint lt(wndRect.left, wndRect.top);
			CPoint rb(wndRect.right, wndRect.bottom);
			::ScreenToClient(parentHWnd, &lt);
			::ScreenToClient(parentHWnd, &rb);
			wndRect = CRect(lt, rb);

			int left = (clientRect.Width() - wndRect.Width()) / 2;
			CRect rect(
				left,
				wndRect.top,
				left + wndRect.Width(),
				wndRect.bottom);

			::MoveWindow(ap.wnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		}

		void AlignManager::HandleAlignVMiddle(AlignProperty& ap)
		{
			CRect clientRect;
			CRect wndRect;
			HWND parentHWnd = ::GetParent(ap.wnd);

			::GetClientRect(parentHWnd, &clientRect);
			::GetWindowRect(ap.wnd, &wndRect);
			CPoint lt(wndRect.left, wndRect.top);
			CPoint rb(wndRect.right, wndRect.bottom);
			::ScreenToClient(parentHWnd, &lt);
			::ScreenToClient(parentHWnd, &rb);
			wndRect = CRect(lt, rb);

			CRect rect(
				wndRect.left,
				(clientRect.Height() - wndRect.Height()) / 2,
				wndRect.Width(),
				wndRect.Height());

			::MoveWindow(ap.wnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		}

		void AlignManager::HandleAlignWidthFill(AlignProperty& ap)
		{
			CRect clientRect;
			CRect wndRect;
			HWND parentHWnd = ::GetParent(ap.wnd);

			::GetClientRect(parentHWnd, &clientRect);
			::GetWindowRect(ap.wnd, &wndRect);
			CPoint lt(wndRect.left, wndRect.top);
			CPoint rb(wndRect.right, wndRect.bottom);
			::ScreenToClient(parentHWnd, &lt);
			::ScreenToClient(parentHWnd, &rb);
			wndRect = CRect(lt, rb);

			CRect rect(
				clientRect.left + ap.margin.marginLeft,
				wndRect.top,
				clientRect.right - ap.margin.marginRight,
				wndRect.bottom);

			::MoveWindow(ap.wnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		}

		void AlignManager::HandleAlignHeightFill(AlignProperty& ap)
		{
			CRect clientRect;
			CRect wndRect;
			HWND parentHWnd = ::GetParent(ap.wnd);

			::GetClientRect(parentHWnd, &clientRect);
			::GetWindowRect(ap.wnd, &wndRect);
			CPoint lt(wndRect.left, wndRect.top);
			CPoint rb(wndRect.right, wndRect.bottom);
			::ScreenToClient(parentHWnd, &lt);
			::ScreenToClient(parentHWnd, &rb);
			wndRect = CRect(lt, rb);

			CRect rect(
				wndRect.left,
				clientRect.top + ap.margin.marginTop,
				wndRect.right,
				clientRect.bottom - ap.margin.marginBottom);

			::MoveWindow(ap.wnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		}

		void AlignManager::HandleAlignClient(AlignProperty& ap)
		{
			CRect clientRect;
			HWND parentHWnd = ::GetParent(ap.wnd);
			::GetClientRect(parentHWnd, &clientRect);

			CRect rect(
				ap.margin.marginLeft,
				ap.margin.marginTop,
				clientRect.right - ap.margin.marginRight,
				clientRect.bottom - ap.margin.marginBottom);

			::MoveWindow(ap.wnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		}

	}
}