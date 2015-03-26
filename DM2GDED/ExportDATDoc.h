#if !defined(AFX_EXPORTDATDOC_H__4C27B64D_A1C2_4A50_95A8_97A5B1A3E2E5__INCLUDED_)
#define AFX_EXPORTDATDOC_H__4C27B64D_A1C2_4A50_95A8_97A5B1A3E2E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDATDoc.h : ヘッダー ファイル
//

#include "GDEdCtx.h"
#include "ExportDATHintDlg.h"

#define PM_V_SEL_EXPTREEITEM 0x0101

class TSelExpTreeItem
{
public:
	// 
	CGDEntry lower, upper;
};

/////////////////////////////////////////////////////////////////////////////
// CExportDATDoc ドキュメント

class CExportDATDoc : public CDocument
{
public:
	// 
	struct NodeKey {
		// 
		BYTE c1, c2;

		// 
		NodeKey() { }
		// 
		NodeKey(BYTE c1, BYTE c2): c1(c1), c2(c2) { }

		// 
		bool operator <(const NodeKey &s) const
		{
			return memcmp(this, &s, sizeof(NodeKey)) < 0;
		}
	};
	// 
	struct NodeVal {
		// 
		bool fCheck;

		// 
		NodeVal(): fCheck(false) { }
	};

private:
	// 
	typedef map<NodeKey, NodeVal> NodeMap;

	// 
	CGDEdCtx *m_pEdCtx;
	// 
	NodeMap m_mapTn;
	// 
	CExportDATComment m_aHintExp;

protected:
	// 
	CExportDATDoc();
	// 
	void CreateTree();
	// 
	void DeleteTree();

	// 
	DECLARE_DYNCREATE(CExportDATDoc)

public:
	// 
	void InitEdCtx(CGDEdCtx *pEdCtx);
	// 
	void Enum(list<NodeKey> &l);
	// 
	CGDEdCtx *GetEdCtx(BOOL fAddRef)
	{
		if (m_pEdCtx && fAddRef)
			m_pEdCtx->AddRef();
		return m_pEdCtx;
	}
	// 
	bool SetCheck(NodeKey k, bool fCheck);
	// 
	BOOL ExportToDir(LPCTSTR pszDir, bool fAllowExplore = true);
	// 
	bool TryExportToDir(LPCTSTR pszDir);

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CExportDATDoc)
	public:
	virtual void Serialize(CArchive& ar);   // ドキュメント I/O に対してオーバーライドされます。
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CExportDATDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// メッセージ マップ関数の生成
protected:
	//{{AFX_MSG(CExportDATDoc)
	afx_msg void OnFileExportNow();
	afx_msg void OnFileExportQuickNow();
	afx_msg void OnEditExportComment();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EXPORTDATDOC_H__4C27B64D_A1C2_4A50_95A8_97A5B1A3E2E5__INCLUDED_)
