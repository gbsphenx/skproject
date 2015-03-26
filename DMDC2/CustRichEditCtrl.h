
// +--------------------------------------------------
// |
// | CustRichEditCtrl.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |
// | DMDC2 : Copyright (c) 2004, 2005, kentaro-k.21
// |

#if !defined(AFX_CUSTRICHEDITCTRL_H__147BDB37_E0DA_43D1_8805_7B1350DFE1A8__INCLUDED_)
#define AFX_CUSTRICHEDITCTRL_H__147BDB37_E0DA_43D1_8805_7B1350DFE1A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustRichEditCtrl.h : �w�b�_�[ �t�@�C��
//

#include <richole.h>

/////////////////////////////////////////////////////////////////////////////
// CCustRichEditCtrl �E�B���h�E

class CCustRichEditCtrl : public CRichEditCtrl
{
	// 
	struct MemVuff {
		// 
		PBYTE psz;
		// 
		UINT iCur, nLen;

		// 
		UINT Read(void *pData, UINT nDataLen)
		{
			UINT nAvail = __min(nLen - iCur, nDataLen);

			memcpy(pData, psz + iCur, nAvail);

			iCur += nAvail;

			return nAvail;
		}

	};

	// 
	static DWORD CALLBACK fnEditStreamCallback(DWORD nCookie, LPBYTE pbBuff, LONG cbBuff, LONG *pcb)
	{
		MemVuff *pmv = (MemVuff *)nCookie;
		*pcb = pmv->Read(pbBuff, cbBuff);
		return 0;
	}

public:
	// 
	CCustRichEditCtrl();
	// 
	bool LoadFromResource(LPCTSTR pszType, LPCTSTR pszName);
	// 
	void EnableLink4Browse()
	{
		SetEventMask(GetEventMask() |ENM_LINK);
	}
	// 
	void EnableAutoUrlDetect()
	{
		SendMessage(EM_AUTOURLDETECT, TRUE, 0);
	}

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CCustRichEditCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CCustRichEditCtrl();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CCustRichEditCtrl)
	//}}AFX_MSG
	void OnLink(ENLINK *pNMHDR, LRESULT *pRet);

	void OnLink(NMHDR *pNMHDR, LRESULT *pRet) {
		OnLink(reinterpret_cast<ENLINK *>(pNMHDR), pRet);
	}

	DECLARE_MESSAGE_MAP()

	BEGIN_INTERFACE_PART(RichEditOleCallback, IRichEditOleCallback)
		INIT_INTERFACE_PART(CRichEditView, RichEditOleCallback)
		STDMETHOD(GetNewStorage) (LPSTORAGE*);
		STDMETHOD(GetInPlaceContext) (LPOLEINPLACEFRAME*,
									  LPOLEINPLACEUIWINDOW*,
									  LPOLEINPLACEFRAMEINFO);
		STDMETHOD(ShowContainerUI) (BOOL);
		STDMETHOD(QueryInsertObject) (LPCLSID, LPSTORAGE, LONG);
		STDMETHOD(DeleteObject) (LPOLEOBJECT);
		STDMETHOD(QueryAcceptData) (LPDATAOBJECT, CLIPFORMAT*, DWORD,BOOL, HGLOBAL);
		STDMETHOD(ContextSensitiveHelp) (BOOL);
		STDMETHOD(GetClipboardData) (CHARRANGE*, DWORD, LPDATAOBJECT*);
		STDMETHOD(GetDragDropEffect) (BOOL, DWORD, LPDWORD);
		STDMETHOD(GetContextMenu) (WORD, LPOLEOBJECT, CHARRANGE*, HMENU*);
	END_INTERFACE_PART(RichEditOleCallback)

	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CUSTRICHEDITCTRL_H__147BDB37_E0DA_43D1_8805_7B1350DFE1A8__INCLUDED_)
