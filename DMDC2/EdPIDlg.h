#if !defined(AFX_EDPIDLG_H__163CEB55_DFF7_4C3B_BEB3_7DB06CA87D9B__INCLUDED_)
#define AFX_EDPIDLG_H__163CEB55_DFF7_4C3B_BEB3_7DB06CA87D9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdPIDlg.h : �w�b�_�[ �t�@�C��
//

#include "DDAT.h"
#include "EdPIRTSink.h"
#include "DM2NameDic.h"

/////////////////////////////////////////////////////////////////////////////
// CEdPIDlg �E�B���h�E

class CEdPIDlg : public CDialog
{
	// 
	int m_nNotifyLock;

	// 
	DECLARE_DYNAMIC(CEdPIDlg)

#if 0
	// 
	LPCTSTR m_lpszTemplateName;
#endif

public:
	// 
	CEdPIDlg(): CDialog()
	{
		m_pSink = NULL;

		m_lpszTemplateName = NULL;

		m_nNotifyLock = 0;
	}
	// 
	CEdPIDlg(UINT nIDTemplate, UINT nIDCaption = 0): CDialog(nIDTemplate)
	{
		m_pSink = NULL;

		m_lpszTemplateName = MAKEINTRESOURCE(nIDTemplate);

		m_nNotifyLock = 0;
	}
	// 
	void SetPISink(CEdPIRTSink *pSink)
	{
		m_pSink = pSink;
	}

	// 
	virtual BOOL OnKillActive();
	// 
	virtual BOOL OnSetActive();

	// 
	bool Create(CWnd *pParentWnd = NULL)
	{
		if (CDialog::Create(m_lpszTemplateName, pParentWnd))
			return true;
		return false;
	}

	// 
	virtual bool Revert(bool fRevert);
	// 
	virtual void AfterRevert() { }

protected:
	// 
	CEdPIRTSink *m_pSink;
	// 
	CDM2OrnateIdxMig *m_oim;

	// 
	virtual bool RevertRecord(DMDBU &rec, bool fRevert) = NULL;

	// 
	BYTE GetDB()
	{
		if (m_pSink) return m_pSink->GetDB();

		return 0xFF;
	}
	// 
	bool GetRecord(DMDBU &rec)
	{
		if (m_pSink) return m_pSink->GetRecord(rec);

		return false;
	}
	// 
	bool SetRecord(const DMDBU &rec)
	{
		if (m_pSink) return m_pSink->SetRecord(rec);

		return false;
	}
	// 
	bool MkPISink()
	{
		return m_pSink != NULL;
	}
	// 
	bool TileIsWall()
	{
		if (m_pSink) return m_pSink->TileIsWall();

		return false;
	}
	// 
	CDDAT *GetDDIf()
	{
		if (m_pSink) return m_pSink->GetDDIf();

		return NULL;
	}
	// 
	bool GetTilePos(AtlasPos_t &ap)
	{
		if (m_pSink) return m_pSink->GetTilePos(ap);

		return false;
	}
	// 
	bool MkOrnateIdxMig()
	{
		m_oim = NULL;

		if (m_pSink)
			m_oim = m_pSink->GetOIM();

		return m_oim != NULL;
	}
	// 
	DM2NameResolver_t *GetNameRes()
	{
		if (m_pSink) return m_pSink->GetNameRes();

		return NULL;
	}

	// 
	void LockedSendNotifyToParent(UINT nCode)
	{
		if (m_nNotifyLock++ == 0) {
			SendNotifyToParent(nCode);
		}
		m_nNotifyLock--;
	}
	// 
	void SendNotifyToParent(UINT nCode)
	{
		CWnd *p;
		if (nCode == 0)
			nCode = ID_REFRESH_PROPERTY_PAGE;
		if (p = GetParent()) {
			p->SendMessage(WM_COMMAND, nCode, (LPARAM)(HWND)*this);
		}
	}

	// 
	static void MaskedModify(WORD &w, WORD wMask, BOOL f)
	{
		if (f) {
			w |= wMask;
		} else {
			w &=~wMask;
		}
	}
	// 
	static void ModifySetVal(WORD &w, WORD wMask, WORD wValue)
	{
		w &=~wMask;
		w |= wValue;
	}
	// 
	static void ModifySetVal(BYTE &x, BYTE nMask, BYTE nValue)
	{
		x &=~nMask;
		x |= nValue;
	}

	// 
	virtual bool IsAcc(UINT nID) { return true; }

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CEdPIDlg)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CEdPIDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EDPIDLG_H__163CEB55_DFF7_4C3B_BEB3_7DB06CA87D9B__INCLUDED_)
