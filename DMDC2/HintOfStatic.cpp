// HintOfStatic.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HintOfStatic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CHintOfStatic

IMPLEMENT_DYNAMIC(CHintOfStatic, CStatic)
CHintOfStatic::CHintOfStatic()
{
}

CHintOfStatic::~CHintOfStatic()
{
}


BEGIN_MESSAGE_MAP(CHintOfStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CHintOfStatic メッセージ ハンドラ

inline int RoundTo4(int x)
{
	return (x + 3) & (~3);
}

void CHintOfStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);

	int cx = rc.Width(), cy = rc.Height();
	int pitch = RoundTo4(3 * cx);
	int total = pitch * cy;
	if (total > sizeof(szVuff)) return;

	BITMAPINFO bi;
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = cx;
	bi.bmiHeader.biHeight = cy;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = 0;
	bi.bmiHeader.biXPelsPerMeter = 0;
	bi.bmiHeader.biYPelsPerMeter = 0;
	bi.bmiHeader.biClrImportant = 0;
	bi.bmiHeader.biClrUsed = 0;

	ZeroMemory(szVuff, sizeof(szVuff));

	if (nCur <= nMax && 1 <= nMax) {
		const int nWidth = cx;
		const int nHeight = cy;
		const int nValue = nCur;
		const int nRange = nMax;
		const UINT nPitch = pitch;
		BYTE *pbBits = (BYTE *)szVuff;

		BYTE *pbLine = pbBits;

		CONST int iXNoMax = 64, iXNoLim = nValue * iXNoMax / nRange;

		for (int iY = 0; iY < nHeight; iY++) {
			int iX = 0;
			for (int iXNo = 0; iXNo < (iXNoLim + 1); iXNo++) {
				int iXUntil = (iXNo) * nWidth / iXNoMax;
				if (iXUntil < 0)
					iXUntil = 0;
				int iXValNo = (iXNo / 2);
				int iXValNoMax = (iXNoMax / 2);
				BYTE bX0T0 = ((iXValNo + 0) * 128 / iXValNoMax) + 128 - 1;
				BYTE bX0T1 = ((iXValNo + 0) * 192 / iXValNoMax) +  64 - 1;
				BYTE bX0T2 = ((iXValNo + 0) * 255 / iXValNoMax) +   0 - 0;
				BYTE bX1T0 = ((iXValNo + 1) * 128 / iXValNoMax) + 128 - 1;
				BYTE bX1T1 = ((iXValNo + 1) * 192 / iXValNoMax) +  64 - 1;
				BYTE bX1T2 = ((iXValNo + 1) * 255 / iXValNoMax) +   0 - 0;

				DWORD dwX0 = (bX0T0) | (bX0T1 << 8) | (bX0T2 << 16);
				DWORD dwX1 = (bX1T0) | (bX1T1 << 8) | (bX1T2 << 16);

				if ((iXNo & 1) == 0) {
					for (; iX < iXUntil; iX++, pbLine += 3) {
						*(DWORD *)pbLine = (*(DWORD *)pbLine & 0xFF000000);
						*(DWORD *)pbLine|= dwX0;
					}
				} else {
					for (; (iY & 1) == 1; ) {
						if ((iX < iXUntil) == 0)
							break;
						*(DWORD *)pbLine = (*(DWORD *)pbLine & 0xFF000000);
						*(DWORD *)pbLine|= dwX1;

						iX++, pbLine += 3;

						break;
					}
					for (; ; ){
						if ((iX < iXUntil) == 0)
							break;
						*(DWORD *)pbLine = (*(DWORD *)pbLine & 0xFF000000);
						*(DWORD *)pbLine|= dwX0;

						iX++, pbLine += 3;

						if ((iX < iXUntil) == 0)
							break;
						*(DWORD *)pbLine = (*(DWORD *)pbLine & 0xFF000000);
						*(DWORD *)pbLine|= dwX1;

						iX++, pbLine += 3;
					}

				}

			}
			pbLine = pbBits + nPitch * (iY + 1);
		}
	}

	SetDIBitsToDevice(
		dc,
		0, 0, cx, cy,
		0, 0,
		0, cy,
		szVuff,
		&bi,
		DIB_RGB_COLORS
		);
}

BOOL CHintOfStatic::OnEraseBkgnd(CDC* pDC)
{
	return true;
}

void CHintOfStatic::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();

	nCur = 0;
	nMax = 0;
}
