#if !defined(AFX_EDITDMITEMSUBDLG_H__42D00AD7_F099_4022_94C2_7D4332D91AEE__INCLUDED_)
#define AFX_EDITDMITEMSUBDLG_H__42D00AD7_F099_4022_94C2_7D4332D91AEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditDMItemSubDlg.h : ヘッダー ファイル
//

#include "EditDBItemExc.h"

/////////////////////////////////////////////////////////////////////////////
// CEditDMItemSubDlg ダイアログ

class CEditDMItemSubDlg : public CDialog
{
	// 
	UINT m_iDB;
	// 
	BOOL m_bUseAsHex;
	// 
	BOOL m_bLimitless;

	// 
	BOOL _AfxSimpleScanf2(LPCTSTR lpszText,
		LPCTSTR lpszFormat, va_list pData)
	{
		ASSERT(lpszText != NULL);
		ASSERT(lpszFormat != NULL);

		ASSERT(*lpszFormat == '%');
		lpszFormat++;        // skip '%'

		BOOL bLong = FALSE;
		BOOL bShort = FALSE;
		if (*lpszFormat == 'l')
		{
			bLong = TRUE;
			lpszFormat++;
		}
		else if (*lpszFormat == 's')
		{
			bShort = TRUE;
			lpszFormat++;
		}

		ASSERT(*lpszFormat == 'd' || *lpszFormat == 'u' || *lpszFormat == 'x' || *lpszFormat == 'X');
		ASSERT(lpszFormat[1] == '\0');

		while (*lpszText == ' ' || *lpszText == '\t')
			lpszText++;
		TCHAR chFirst = lpszText[0];
		long l, l2;
		if (*lpszFormat == 'd')
		{
			// signed
			l = _tcstol(lpszText, (LPTSTR*)&lpszText, 10);
			l2 = (int)l;
		}
		else if (*lpszFormat == 'u')
		{
			// unsigned
			if (*lpszText == '-')
				return FALSE;
			l = (long)_tcstoul(lpszText, (LPTSTR*)&lpszText, 10);
			l2 = (unsigned int)l;
		} else {
			if (*lpszText == '-')
				return FALSE;
			l = (long)_tcstoul(lpszText, (LPTSTR*)&lpszText, 16);
			l2 = (unsigned int)l;
		}
		if (l == 0 && chFirst != '0')
			return FALSE;   // could not convert

		while (*lpszText == ' ' || *lpszText == '\t')
			lpszText++;
		if (*lpszText != '\0')
			return FALSE;   // not terminated properly

		if (bShort)
		{
			if (*lpszFormat == 'd') {
				if ((short)l != l)
					return FALSE;   // too big for short
			}
			*va_arg(pData, short*) = (short)l;
		}
		else
		{
			ASSERT(sizeof(long) == sizeof(int));
			ASSERT(l == l2);
			*va_arg(pData, long*) = l;
		}

		// all ok
		return TRUE;
	}
	// 
	void _Afx_DDX_TextWithFormat2(CDataExchange* pDX, int nIDC,
		LPCTSTR lpszFormat, UINT nIDPrompt, ...);
		// only supports windows output formats - no floating point
	// 
	void DDX_Text2(CDataExchange* pDX, int nIDC, BYTE &value)
	{
		int n = (int)value;
		if (pDX->m_bSaveAndValidate)
		{
			_Afx_DDX_TextWithFormat2(pDX, nIDC, m_bUseAsHex ? _T("%X") : _T("%u"), AFX_IDP_PARSE_BYTE, &n);
			if (n > 255)
			{
				AfxMessageBox(AFX_IDP_PARSE_BYTE);
				pDX->Fail();        // throws exception
			}
			value = (BYTE)n;
		}
		else
			_Afx_DDX_TextWithFormat2(pDX, nIDC, m_bUseAsHex ? _T("%02X") : _T("%u"), AFX_IDP_PARSE_BYTE, n);
	}
	// 
	void DDX_Text2(CDataExchange* pDX, int nIDC, WORD &value)
	{
		if (pDX->m_bSaveAndValidate)
			_Afx_DDX_TextWithFormat2(pDX, nIDC, m_bUseAsHex ? _T("%sX") : _T("%su"), AFX_IDP_PARSE_UINT, &value);
		else
			_Afx_DDX_TextWithFormat2(pDX, nIDC, m_bUseAsHex ? _T("%04hX") : _T("%hu"), AFX_IDP_PARSE_UINT, value);
	}
public:
	// 
	CEditDBItemExchanger m_ret;
	// 
	CEditDMItemSubDlg(CWnd* lpParent = NULL);   // 標準のコンストラクタ
	// 
	BOOL Create(UINT nIDTemplate, UINT iDB, BOOL bLimitless, CWnd *lpParentWnd)
	{
		m_iDB = iDB;
		m_bLimitless = bLimitless;
		if (!CDialog::Create(nIDTemplate, lpParentWnd))
			return FALSE;
		return TRUE;
	}
	// 
	void InitAsNew(UINT iDB)
	{
		switch (iDB) {
		case 4:
			m_ret.m_item.r4.w2 = 0xfffe; break;
		case 9:
			m_ret.m_item.r9.w2 = 0xfffe; break;
		case 14:
			m_ret.m_item.r14.w2 = 0xfffe; break;
		}
	}
	// 
	BOOL SaveCapable()
	{
		switch (m_iDB) {
		case 11: case 12: case 13: return true;
		case 15: return FALSE;
		}
		return TRUE;
	}
	// 
	BOOL UseHex(BOOL bUseHex)
	{
		BOOL bUpdateNow = IsWindow(m_hWnd);
		m_bUseAsHex = bUseHex;
		if (bUpdateNow) {
			UpdateData(FALSE);
		}
		return TRUE;
	}

// ダイアログ データ
	//{{AFX_DATA(CEditDMItemSubDlg)
	enum { IDD = IDD_EDITDB00 };
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditDMItemSubDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditDMItemSubDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITDMITEMSUBDLG_H__42D00AD7_F099_4022_94C2_7D4332D91AEE__INCLUDED_)
