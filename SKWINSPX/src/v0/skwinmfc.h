//------------------------------------------------------------------------------
// SKWINMFC
//	Specific code for Win MFC windows
//	This is derivated from original SKWin code
//	Note: this code is not compiled for non-MFC platforms.
//------------------------------------------------------------------------------

#ifndef _SKCODE_WINMFC_H_
#define _SKCODE_WINMFC_H_

//..............................................................................

#include <skver.h>

//..............................................................................

#if !defined (__NO_MFC__)
#if (_MSC_VER >= 1200) || defined(_USE_MFC80) || defined(_USE_MFC60)

#pragma once

//------------------------------------------------------------------------------

#include <sktypes.h>
#include <skinput.h>
#include <skvram.h>
#include <afxwin.h> // CWinApp

//..............................................................................
// CSkApp

class CSkApp : public CWinApp {
public:
	int ML();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnHelpAbout();
};

//..............................................................................
//------------------------------------------------------------------------------

class CSkWinMFC : public CFrameWnd, public CSkWin
{
public:
	CPoint ptfrm;

	int sxfact;	// screen scale factor
	int sblast;
	int sbdsbf;
	int sbdsaf;
	int spfact;	// speed factor
	int lang;

	SkVRAM* xVRAM;
	X8 *xVGAPalette;

public:
	CSkWinMFC();
	virtual ~CSkWinMFC();

	void SetVRAM(SkVRAM* xVRAM);
	void SetVGAPaletteRGB(X8 *xVGAPalette);

	void UpdateRect(i16 x, i16 y, i16 cx, i16 cy);
	void SndPlayHi(const U8 *buff, U32 buffSize, i8 vol);
	void SndPlayLo(const U8 *buff, U32 buffSize, i8 dX, i8 dY);
	void SndPlayFile(const char* sFilename, i8 vol);

	void SndSetFrequency(U16 iSoundPlaybackFrequency);

	bool ML();

	static inline U32 GetTickCount() {
		return ::GetTickCount();
	}
	static inline void Sleep(U32 millisecs) {
		::Sleep(millisecs);
	}
	inline bool IsAvail() {
		return (m_hWnd != NULL);
	}
	void ShowMessage(const char *psz);
	bool AskMe(const char *psz);
//	BYTE GetLang() {
//		return (BYTE)lang;
//	}


protected:
	void processMinput(UINT nFlags, CPoint point);
	void processKinput(UINT nChar, bool press);
	void selectSBlast();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVideo1x(UINT nID);
	afx_msg void OnUpdateVideo1x(CCmdUI *pCmdUI);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnExtraDoubleStepMove();
};




//------------------------------------------------------------------------------

#endif // _MSC_VER / _USE_MFC80 / _USE_MFC60
#endif // not __NO_MFC__

//..............................................................................

#endif // _SKCODE_WINMFC_H_
