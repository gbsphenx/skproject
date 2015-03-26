#if !defined(AFX_HEXEDITDOC_H__294FCCEB_1E0A_4055_811F_14B1B2F0E65B__INCLUDED_)
#define AFX_HEXEDITDOC_H__294FCCEB_1E0A_4055_811F_14B1B2F0E65B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HexEditDoc.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CHexEditDoc ドキュメント

class CHexEditDoc : public CDocument
{
protected:
	CHexEditDoc();           // 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CHexEditDoc)

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CHexEditDoc)
	public:
	virtual void Serialize(CArchive& ar);   // ドキュメント I/O に対してオーバーライドされます。
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CHexEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// メッセージ マップ関数の生成
protected:
	//{{AFX_MSG(CHexEditDoc)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_HEXEDITDOC_H__294FCCEB_1E0A_4055_811F_14B1B2F0E65B__INCLUDED_)
