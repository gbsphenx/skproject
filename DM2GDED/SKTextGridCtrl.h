#if !defined(AFX_SKTEXTGRIDCTRL_H__9B98B38F_7090_480A_928F_C08E10413430__INCLUDED_)
#define AFX_SKTEXTGRIDCTRL_H__9B98B38F_7090_480A_928F_C08E10413430__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SKTextGridCtrl.h : �w�b�_�[ �t�@�C��
//

#include "GridCtrl.h"
#include "DM2SkCmdList.h"

/////////////////////////////////////////////////////////////////////////////
//

class CSKTextCell : public CGridCell
{
	// 
	DECLARE_DYNCREATE(CSKTextCell);

public:
	// 
	virtual CString GetValue() const 
	{
		return GetText();
	}
	// 
	virtual void SetValue(LPCTSTR szText)
	{
		SetText(szText);
	}

protected:
	// 
	virtual void OnDblClick(CPoint PointCellRelative)
	{

	}

};

/////////////////////////////////////////////////////////////////////////////
// 

class CSKTextCMCell : public CSKTextCell
{
	// 
	DECLARE_DYNCREATE(CSKTextCMCell);

public:
	// 
	int iSel;
	// 
	int skc;

	// 
	CSKTextCMCell()
	{
		iSel = -1;
		skc = skcCM;
	}

	// 
	virtual CString GetValue() const;
	// 
	virtual void SetValue(LPCTSTR szText);

	// 
	virtual LPCTSTR GetText() const;
	// 
    virtual BOOL Edit( int /* nRow */, int /* nCol */, CRect /* rect */, CPoint /* point */, 
                       UINT /* nID */, UINT /* nChar */);

protected:
	// 
	virtual void OnDblClick(CPoint PointCellRelative);

};

/////////////////////////////////////////////////////////////////////////////
// CSKTextGridCtrl �E�B���h�E

class CSKTextGridCtrl : public CGridCtrl
{
	// 
	int m_iRowCM;

public:
	// 
	void CreateList(const CDM2SkCmdList &r, const CDM2SkCmdDesc *p);
	// 
	void SaveList(CDM2SkCmdList &r);

public:
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CSKTextGridCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

public:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CSKTextGridCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SKTEXTGRIDCTRL_H__9B98B38F_7090_480A_928F_C08E10413430__INCLUDED_)
