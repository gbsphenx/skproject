
#pragma once

#include <SkWin.h>
#include <sblast.h>

class SkWinCore;

// CSkApp

class CSkApp : public CWinApp {
public:
	int ML();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnHelpAbout();
};

extern CSkApp theApp;

// CSkWinMFC

class CSkWinMFC : public CFrameWnd, public CSkWin
{
public:
	SkWinCore* skw;
	int sxfact;
	int sblast;
	int sbdsbf;
	int sbdsaf;
	int spfact;	// speed factor
	int lang;
	int dung;	// dungeon selection
	int sksave;		// savegame to resume
	int newgame;	// start new game
	int freq;		// standard playback frequency (V4/PC-9821 and V5/PC-DOS are not the same)
	char*	sCustomGraphicsDatFilename;
	char*	sCustomDungeonDatFilename;
	char*	sCustomDataFolder;
	CPoint ptfrm;
	sblast_sys::DS sbds;
	sblast_sys::Openal sboa;

public:
	CSkWinMFC();
	virtual ~CSkWinMFC();

	void UpdateRect(i16 x, i16 y, i16 cx, i16 cy);
	void SndPlayHi(const U8 *buff, U32 buffSize, i8 vol);
	void SndPlayLo(const U8 *buff, U32 buffSize, i8 dX, i8 dY);
	void SndPlayFile(const char* sFilename, i8 vol);

	void SndSetFrequency(U16 iSoundPlaybackFrequency);

	bool ML();

	void ProcessArgs(int argc, char** argv);	// SPX: added to handle options from command line

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
	BYTE GetLang() {
		return (BYTE)lang;
	}


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


