#if !defined(AFX_SPLASHVW_H__5AD9C2DA_A39C_44BF_8E33_A06DF64F337E__INCLUDED_)
#define AFX_SPLASHVW_H__5AD9C2DA_A39C_44BF_8E33_A06DF64F337E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashVw.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CSplashVw �r���[

class CSplashVw : public CScrollView
{
	// 
	CBitmap m_bm;
	// 
	BITMAP m_bmp;

public:
	// 
	CSplashVw();
	// 
	virtual ~CSplashVw();
	// 
	void PostNcDestroy() { }

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSplashVw)
	protected:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h���܂����B
	virtual void OnInitialUpdate();     // �\�z��A�ŏ��̏����B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSplashVw)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SPLASHVW_H__5AD9C2DA_A39C_44BF_8E33_A06DF64F337E__INCLUDED_)
