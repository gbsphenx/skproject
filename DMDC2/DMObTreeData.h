// DMObTreeData.h: CDMObTreeData クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMOBTREEDATA_H__1F8C1A5A_907C_43E7_9DF0_34E53C3B1935__INCLUDED_)
#define AFX_DMOBTREEDATA_H__1F8C1A5A_907C_43E7_9DF0_34E53C3B1935__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDAT.h"
#include "DMDBRMask.h"

class CDMObTreeData  
{
	// 
	struct Node {
		// 
		BYTE iDB, iDir;
		// 
		DMDBU rec;
		// 
		Node *pSib;
		// 
		Node *pSub;

		// 
		Node()
		{
			iDB = -1, iDir = -1, pSib = NULL, pSub = NULL;
		}
		// 
		~Node()
		{
			if (pSib) delete pSib;
			if (pSub) delete pSub;
		}
	};

	// 
	Node *pRoot;
	// 
	CDDAT *pDAT;
	// 
	CDMDBRMask aLoop;
	// 
	LPCSTR m_pszPos;
	// 
	LPCSTR m_pszEnd;
	// 
	CDDAT *m_pDAT;

	// 
	Node *Conv(RN rn);
	// 
	CString MkNode(Node *p, int i);
	// 
	Node *ConvText(list<CString>::iterator &iterPos, list<CString>::iterator iterEnd, int iDepth);
	// 
	bool ParseDir(int &iDir);
	// 
	bool ParseMark(char c);
	// 
	bool ParseDBName(int &iDB);
	// 
	bool ParseRec(int iDB, DMDBU &rec);
	// 
	bool ParseHexByte(BYTE &x);
	// 
	bool ParseHexWord(WORD &x);
	// 
	bool ParseHex1(char c, BYTE &x);
	// 
	RN MkRec(Node *p, RN rnLast);

public:
	// 
	CDMObTreeData();
	// 
	~CDMObTreeData();
	// 
	bool Create(CDDAT &rDAT, RN rn);
	// 
	void Close();
	// 
	CString Mk();
	// 
	bool CreateFromText(LPCTSTR psz);
	// 
	bool InsertTo(CDDAT &rDAT, RN rn, RN &rnAppended);

};

#endif // !defined(AFX_DMOBTREEDATA_H__1F8C1A5A_907C_43E7_9DF0_34E53C3B1935__INCLUDED_)
