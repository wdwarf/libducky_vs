#ifndef LIBDUCKY_UI_AlignManager_H_
#define LIBDUCKY_UI_AlignManager_H_

#include "libducky_config.h"
#include <afxwin.h>
#include <list>

using std::list;

namespace ducky
{
	namespace ui {
		enum AlignType {
			AT_Left = 0,
			AT_Top,
			AT_Right,
			AT_Bottom,
			AT_HCenter,
			AT_VMiddle,
			AT_WidthFill,
			AT_HeightFill,
			AT_Client
		};

		class Margin
		{
		public:
			Margin() { memset(this, 0, sizeof(Margin)); }
			Margin(int margin)
				: marginLeft(margin), marginTop(margin),
				marginRight(margin), marginBottom(margin) {}
			Margin(int left, int top, int right, int bottom)
				: marginLeft(left), marginTop(top),
				marginRight(right), marginBottom(bottom) {}

			int marginLeft;
			int marginTop;
			int marginRight;
			int marginBottom;
		};

		class AlignProperty
		{
		public:
			AlignType align;
			HWND wnd;
			Margin margin;

		protected:
			CRect originalWindowRect;

			friend class AlignManager;
		};

		class AlignManager
		{
		public:
			AlignManager(void);
			virtual ~AlignManager(void);

			void AddWnd(HWND wnd, AlignType align, Margin margin = Margin());
			void Execute();

		protected:
			list<AlignProperty> wndList;
			bool IsWndInList(HWND wnd);

			void HandleAlignLeft(AlignProperty& ap);
			void HandleAlignRight(AlignProperty& ap);
			void HandleAlignTop(AlignProperty& ap);
			void HandleAlignBottom(AlignProperty& ap);
			void HandleAlignHCenter(AlignProperty& ap);
			void HandleAlignVMiddle(AlignProperty& ap);
			void HandleAlignWidthFill(AlignProperty& ap);
			void HandleAlignHeightFill(AlignProperty& ap);
			void HandleAlignClient(AlignProperty& ap);
		};

	}
}
#endif