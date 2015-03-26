#if !defined(AFX_DM2SELCOLORKEYSTATIC_H__D790FB4C_93B0_4B15_8D34_E1C1FDE353AB__INCLUDED_)
#define AFX_DM2SELCOLORKEYSTATIC_H__D790FB4C_93B0_4B15_8D34_E1C1FDE353AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DM2SelColorKeyStatic.h : �w�b�_�[ �t�@�C��
//

#include "DMGHLci.h"

/////////////////////////////////////////////////////////////////////////////
// CDM2SelColorKeyStatic �E�B���h�E

class CDM2SelColorKeyStatic : public CStatic
{
	// 
	enum {
		bkNo,
		bkSolid,
		bkLL,
		bkRR,
	};
	// 
	struct Solid {
		// 
		COLORREF clr;
		// 
		UINT f;

		// 
		Solid(): f(0) { }
	};
	// 
	struct Box {
		// 
		CRect rc;
		// 
		int bk;
		// 
		int iSolid;

		// 
		Box(): bk(0), iSolid(-1) { }
	};

	// 
	CArray<Solid, Solid> m_solid;
	// 
	CArray<Box, Box> m_box;
	// 
	int m_iColorSrcKey;
	// 
	int m_iColorKey;
	// 
	CFont m_fontMarlett;
	// 
	int m_iSel;
	// 
	int m_iOffset;
	// 
	int m_nSolidVis;

	// 
	bool Clicked(CPoint pt);

public:
	// 
	CDM2SelColorKeyStatic();
	// 
	void Set4(const DMGHLciPAL &a, int iColorSrcKey, const DMGHLciDM2Pal &DM2Pal);
	// 
	void Set8(int iColorSrcKey, const DMGHLciDM2Pal &DM2Pal);
	// 
	void SetSel(int i, bool fNotify);
	// 
	int GetSel() { return m_iSel; }
	// 
	void Init();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDM2SelColorKeyStatic)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CDM2SelColorKeyStatic)
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DM2SELCOLORKEYSTATIC_H__D790FB4C_93B0_4B15_8D34_E1C1FDE353AB__INCLUDED_)
