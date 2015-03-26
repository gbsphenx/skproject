// NamedWndBox.h: CNamedWndBox クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAMEDWNDBOX_H__C63A8140_B2B5_4910_8489_D1C124A17B76__INCLUDED_)
#define AFX_NAMEDWNDBOX_H__C63A8140_B2B5_4910_8489_D1C124A17B76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

using namespace std;

class CNamedWndBox  
{
	// 
	struct ELEM
	{
		// 
		HWND hWndMe;
		// 
		HWND hWndOwner;
		// 
		CString tstrName;
		// 
		CString tstrAffiliation;
	};
	// 
	typedef list<ELEM> ELEMLIST;
	// 
	ELEMLIST m_elemList;

public:
	// 
	void Add2Box(CWnd *pWndTar, CWnd *pWndParent, LPCTSTR lpszAffiliation, LPCTSTR lpszName)
	{
		ELEM m;
		m.hWndMe = pWndTar->GetSafeHwnd();
		m.hWndOwner = pWndParent->GetSafeHwnd();
		m.tstrAffiliation = lpszAffiliation;
		m.tstrName = lpszName;
		m_elemList.push_back(m);
	}
	// 
	void DeleteMeFromBox(CWnd *pWndTar)
	{
		ELEMLIST::iterator
			iterPos = m_elemList.begin(),
			iterEnd = m_elemList.end();
		HWND hWndMe = pWndTar->GetSafeHwnd();
		for (; iterPos != iterEnd; iterPos++) {
			if (iterPos->hWndMe != hWndMe)
				continue;
			iterPos = m_elemList.erase(iterPos);
		}
	}
	// 
	void DeleteAffliationAndReleaseFromBox(LPCTSTR lpszAffiliation)
	{
		ELEMLIST::iterator
			iterPos = m_elemList.begin(),
			iterEnd = m_elemList.end();
		for (; iterPos != iterEnd; iterPos++) {
			if (iterPos->tstrAffiliation != lpszAffiliation)
				continue;
			DestroyWindow(iterPos->hWndMe);
			iterPos = m_elemList.erase(iterPos);
		}
	}
	// 
	CWnd *FindNameFromBox(LPCTSTR lpszAffiliation, LPCTSTR lpszName)
	{
		ELEMLIST::iterator
			iterPos = m_elemList.begin(),
			iterEnd = m_elemList.end();
		for (; iterPos != iterEnd; iterPos++) {
			if (iterPos->tstrAffiliation != lpszAffiliation)
				continue;
			if (iterPos->tstrName != lpszName)
				continue;
			return CWnd::FromHandle(iterPos->hWndMe);
		}
		return NULL;
	}

};

extern CNamedWndBox g_boxWnd;

#endif // !defined(AFX_NAMEDWNDBOX_H__C63A8140_B2B5_4910_8489_D1C124A17B76__INCLUDED_)
