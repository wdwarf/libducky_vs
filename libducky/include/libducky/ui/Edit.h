#ifndef LIBDUCKY_UI_Edit_H_
#define LIBDUCKY_UI_Edit_H_

#include "libducky_config.h"
#include <libducky/Object.h>
#include <afxwin.h>

// Edit
namespace ducky
{
	namespace ui {
		class Edit : public CEdit
		{
			DECLARE_DYNAMIC(Edit)

		public:
			Edit(bool isRepaint = false);
			virtual ~Edit();

			void AddLine(CString str, bool newLine = false);
			void Clear();

			void SetRepaint(bool isRepaint);

		protected:
			bool isRepaint;
			COLORREF borderColor;
			COLORREF bgColor;

		public:
			DECLARE_MESSAGE_MAP()
			afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
			afx_msg BOOL OnEraseBkgnd(CDC* pDC);
			afx_msg void OnNcPaint();
			afx_msg void OnPaint();
		};

	}
}
#endif