
#include "StdAfx.h"
#include "AfxDeclStatic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace Av {

HCURSOR _afxCursors[10];
HBRUSH _afxHatchBrush;
HPEN _afxBlackDottedPen;

void Construct()
{
	static BOOL bInitialized;
	if (!bInitialized)
	{
		// sanity checks for assumptions we make in the code
		ASSERT(sizeof(((RECT*)NULL)->left) == sizeof(int));
		ASSERT(offsetof(RECT, top) > offsetof(RECT, left));
		ASSERT(offsetof(RECT, right) > offsetof(RECT, top));
		ASSERT(offsetof(RECT, bottom) > offsetof(RECT, right));

		if (_afxHatchBrush == NULL)
		{
			// create the hatch pattern + bitmap
			WORD hatchPattern[8];
			WORD wPattern = 0x1111;
			for (int i = 0; i < 4; i++)
			{
				hatchPattern[i] = wPattern;
				hatchPattern[i+4] = wPattern;
				wPattern <<= 1;
			}
			HBITMAP hatchBitmap = CreateBitmap(8, 8, 1, 1, &hatchPattern);
			if (hatchBitmap == NULL)
			{
				AfxThrowResourceException();
			}

			// create black hatched brush
			_afxHatchBrush = CreatePatternBrush(hatchBitmap);
			DeleteObject(hatchBitmap);
			if (_afxHatchBrush == NULL)
			{
				AfxThrowResourceException();
			}
		}

		if (_afxBlackDottedPen == NULL)
		{
			// create black dotted pen
			_afxBlackDottedPen = CreatePen(PS_DOT, 0, RGB(0, 0, 0));
			if (_afxBlackDottedPen == NULL)
			{
				AfxThrowResourceException();
			}
		}

		// Note: all track cursors must live in same module
		HINSTANCE hInst = AfxFindResourceHandle(
			MAKEINTRESOURCE(AFX_IDC_TRACK4WAY), RT_GROUP_CURSOR);

		// initialize the cursor array
		_afxCursors[0] = ::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_TRACKNWSE));
		_afxCursors[1] = ::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_TRACKNESW));
		_afxCursors[2] = _afxCursors[0];
		_afxCursors[3] = _afxCursors[1];
		_afxCursors[4] = ::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_TRACKNS));
		_afxCursors[5] = ::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_TRACKWE));
		_afxCursors[6] = _afxCursors[4];
		_afxCursors[7] = _afxCursors[5];
		_afxCursors[8] = ::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_TRACK4WAY));
		_afxCursors[9] = ::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_MOVE4WAY));

		bInitialized = TRUE;
	}
}

};
