
// +--------------------------------------------------
// |
// | CustRichEditCtrl.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |
// | DMDC2 : Copyright (c) 2004, 2005, kentaro-k.21
// |

// @ 19:46 2006/01/12

#include "stdafx.h"
#include "CustRichEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustRichEditCtrl

CCustRichEditCtrl::CCustRichEditCtrl()
{

}

CCustRichEditCtrl::~CCustRichEditCtrl()
{

}

BEGIN_MESSAGE_MAP(CCustRichEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CCustRichEditCtrl)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(EN_LINK, OnLink)
END_MESSAGE_MAP()

BEGIN_INTERFACE_MAP(CCustRichEditCtrl, CRichEditCtrl)
	// we use IID_IUnknown because richedit doesn't define an IID
	INTERFACE_PART(CCustRichEditCtrl, IID_IUnknown, RichEditOleCallback)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustRichEditCtrl::RichEditOleCallback

STDMETHODIMP_(ULONG) CCustRichEditCtrl::XRichEditOleCallback::AddRef()
{
	METHOD_PROLOGUE_EX_(CCustRichEditCtrl, RichEditOleCallback)
	return (ULONG)pThis->InternalAddRef();
}

STDMETHODIMP_(ULONG) CCustRichEditCtrl::XRichEditOleCallback::Release()
{
	METHOD_PROLOGUE_EX_(CCustRichEditCtrl, RichEditOleCallback)
	return (ULONG)pThis->InternalRelease();
}

STDMETHODIMP CCustRichEditCtrl::XRichEditOleCallback::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CCustRichEditCtrl, RichEditOleCallback)
	return (HRESULT)pThis->InternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CCustRichEditCtrl::XRichEditOleCallback::GetNewStorage(LPSTORAGE* ppstg)
{
	METHOD_PROLOGUE_EX_(CCustRichEditCtrl, RichEditOleCallback)

	// Create a flat storage and steal it from the client item
	// the client item is only used for creating the storage
	COleClientItem item;
	item.GetItemStorageFlat();
	*ppstg = item.m_lpStorage;
	HRESULT hRes = E_OUTOFMEMORY;
	if (item.m_lpStorage != NULL)
	{
		item.m_lpStorage = NULL;
		hRes = S_OK;
	}
	return hRes;
}

STDMETHODIMP CCustRichEditCtrl::XRichEditOleCallback::GetInPlaceContext(
	LPOLEINPLACEFRAME* lplpFrame, LPOLEINPLACEUIWINDOW* lplpDoc,
	LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	METHOD_PROLOGUE_EX(CCustRichEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}

STDMETHODIMP CCustRichEditCtrl::XRichEditOleCallback::ShowContainerUI(BOOL fShow)
{
	METHOD_PROLOGUE_EX(CCustRichEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}

STDMETHODIMP CCustRichEditCtrl::XRichEditOleCallback::QueryInsertObject(
	LPCLSID /*lpclsid*/, LPSTORAGE /*pstg*/, LONG /*cp*/)
{
	METHOD_PROLOGUE_EX(CCustRichEditCtrl, RichEditOleCallback)
	return S_OK;
}

STDMETHODIMP CCustRichEditCtrl::XRichEditOleCallback::DeleteObject(LPOLEOBJECT /*lpoleobj*/)
{
	METHOD_PROLOGUE_EX_(CCustRichEditCtrl, RichEditOleCallback)
	return S_OK;
}

STDMETHODIMP CCustRichEditCtrl::XRichEditOleCallback::QueryAcceptData(
	LPDATAOBJECT lpdataobj, CLIPFORMAT* lpcfFormat, DWORD reco,
	BOOL fReally, HGLOBAL hMetaPict)
{
	METHOD_PROLOGUE_EX(CCustRichEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}

STDMETHODIMP CCustRichEditCtrl::XRichEditOleCallback::ContextSensitiveHelp(BOOL /*fEnterMode*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCustRichEditCtrl::XRichEditOleCallback::GetClipboardData(
	CHARRANGE* lpchrg, DWORD reco, LPDATAOBJECT* lplpdataobj)
{
	METHOD_PROLOGUE_EX(CCustRichEditCtrl, RichEditOleCallback)
	return E_NOTIMPL;
}

STDMETHODIMP CCustRichEditCtrl::XRichEditOleCallback::GetDragDropEffect(
	BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCustRichEditCtrl::XRichEditOleCallback::GetContextMenu(
	WORD seltype, LPOLEOBJECT lpoleobj, CHARRANGE* lpchrg,
	HMENU* lphmenu)
{
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
// CCustRichEditCtrl メッセージ ハンドラ

void CCustRichEditCtrl::OnLink(ENLINK *pNMHDR, LRESULT *pRes)
{
	if (pNMHDR->msg == WM_LBUTTONDOWN) {
		CWaitCursor wc;
		SetSel(pNMHDR->chrg);
		CString strSel = GetSelText();
		ShellExecute(NULL, "open", strSel, NULL, NULL, SW_SHOW);

		*pRes = TRUE;
		return;
	}

	*pRes = FALSE;
}

bool CCustRichEditCtrl::LoadFromResource(LPCTSTR pszType, LPCTSTR pszName)
{
	HRSRC hRes = FindResource(AfxGetInstanceHandle(), pszName, pszType);
	if (hRes) {
		HGLOBAL hMem = LoadResource(AfxGetInstanceHandle(), hRes);
		if (hRes) {
			LPVOID pvData = LockResource(hMem);
			DWORD cbData = SizeofResource(AfxGetInstanceHandle(), hRes);

			MemVuff mv;
			mv.psz = (PBYTE)pvData;
			mv.iCur = 0;
			mv.nLen = cbData;

			EDITSTREAM es;
			es.dwCookie = (DWORD)&mv;
			es.dwError = 0;
			es.pfnCallback = fnEditStreamCallback;

			StreamIn(SF_RTF, es);
			return true;
		}
	}
	return false;
}

void CCustRichEditCtrl::PreSubclassWindow() 
{
	CRichEditCtrl::PreSubclassWindow();

	VERIFY(SetOLECallback(&m_xRichEditOleCallback));
}
