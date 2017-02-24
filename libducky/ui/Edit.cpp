// Edit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "libducky/ui/Edit.h"
#include <string>

using namespace std;

// Edit
namespace ducky {
	namespace ui {

		IMPLEMENT_DYNAMIC(Edit, CEdit)

			Edit::Edit(bool isRepaint)
			: borderColor(RGB(150, 160, 180))
			, bgColor(RGB(240, 245, 245))
		{
			this->isRepaint = isRepaint;
		}

		Edit::~Edit()
		{
		}


		BEGIN_MESSAGE_MAP(Edit, CEdit)
			ON_WM_CTLCOLOR_REFLECT()
			ON_WM_CREATE()
			ON_WM_ERASEBKGND()
			ON_WM_NCPAINT()
			ON_WM_PAINT()
		END_MESSAGE_MAP()

		// Edit ��Ϣ�������


		HBRUSH Edit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
		{
			// TODO:  �ڴ˸��� DC ���κ�����
			return (HBRUSH)GetStockObject(WHITE_BRUSH);
			// TODO:  �����Ӧ���ø����Ĵ�������򷵻ط� null ����
			return NULL;
		}


		void Edit::AddLine(CString str, bool newLine)
		{
			if (this->GetLineCount() > 500)
			{
				CString s;
				this->GetWindowText(s);
				_W(string) info = s.GetBuffer();
				info = info.substr(info.length() * (2 / 3.f));
				info += str.GetBuffer();
				this->SetWindowText(info.c_str());
			}
			else
			{
				this->SetSel(this->GetWindowTextLength() + 10, -1);
				if (newLine)
				{
					str += _T("\r\n");
				}
				this->ReplaceSel(str);
			}

			this->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
		}

		void Edit::Clear()
		{
			CEdit::Clear();
			this->SetWindowText(_T(""));
		}

		int Edit::OnCreate(LPCREATESTRUCT lpCreateStruct)
		{
			if (CEdit::OnCreate(lpCreateStruct) == -1)
				return -1;

			// TODO:  �ڴ������ר�õĴ�������
			this->SetLimitText(UINT_MAX);

			return 0;
		}

		void Edit::SetRepaint(bool isRepaint)
		{
			this->isRepaint = isRepaint;
			this->Invalidate();
		}

		BOOL Edit::OnEraseBkgnd(CDC* pDC)
		{
			// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

			if (this->isRepaint)
				return true;
			return CEdit::OnEraseBkgnd(pDC);
		}


		void Edit::OnNcPaint()
		{
			// TODO: �ڴ˴������Ϣ����������
			// ��Ϊ��ͼ��Ϣ���� CEdit::OnNcPaint()
			if (this->isRepaint)
			{
				CClientDC dc(this);
				CPen pen;
				CBrush brush;
				pen.CreatePen(PS_SOLID, 1, this->borderColor);
				//brush.CreateSolidBrush(this->bgColor);
				dc.SelectObject(pen);
				dc.SelectObject(GetStockObject(NULL_BRUSH));
				CRect rect;
				this->GetClientRect(rect);
				rect.left -= 1;
				rect.top -= 1;
				rect.right += 1;
				rect.bottom += 1;
				//dc.Rectangle(rect);
				dc.RoundRect(rect, CPoint(1, 1));

				pen.DeleteObject();
				brush.DeleteObject();
			}
			else
			{
				CEdit::OnNcPaint();
			}
		}

		void Edit::OnPaint()
		{
			CEdit::OnPaint();

			//CPaintDC dc(this);
			//// TODO: �ڴ˴������Ϣ����������
			//// ��Ϊ��ͼ��Ϣ���� CEdit::OnPaint()
			//if(this->isRepaint)
			//{
			//	CClientDC dc(this);
			//	CPen pen;
			//	CBrush brush;
			//	pen.CreatePen(PS_SOLID, 1, this->borderColor);
			//	brush.CreateSolidBrush(this->bgColor);
			//	dc.SelectObject(pen);
			//	dc.SelectObject(brush);
			//	CRect rect;
			//	this->GetClientRect(rect);
			//	rect.left -= 1;
			//	rect.top -= 1;
			//	rect.right += 1;
			//	rect.bottom += 1;
			//	//dc.Rectangle(rect);
			//	dc.RoundRect(rect, CPoint(1, 1));

			//	pen.DeleteObject();
			//	brush.DeleteObject();
			//}
			//else
			//{
			//	CEdit::OnPaint();
			//}
		}

	}
}