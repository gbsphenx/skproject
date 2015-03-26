// DM2GDEDTeView.cpp
//

#include "StdAfx.h"
#include "DM2GDED.h"
#include "DM2GDEDDoc.h"
#include "DM2GDEDTeView.h"
#include "OleByteBin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// {F0674057-615F-4d57-BDE4-3DB21B3E356F}
static const GUID GUID_DM2GDED_REV1 = 
{ 0xf0674057, 0x615f, 0x4d57, { 0xbd, 0xe4, 0x3d, 0xb2, 0x1b, 0x3e, 0x35, 0x6f } };

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDTeView

IMPLEMENT_DYNCREATE(CDM2GDEDTeView, CDM2GDEDView)

CDM2GDEDTeView::CDM2GDEDTeView()
{
	m_nCookie = 0;

	EnableAutomation();
}

CDM2GDEDTeView::~CDM2GDEDTeView()
{
	ConnectAx(false);
}

void CDM2GDEDTeView::OnFinalRelease()
{
	CDM2GDEDView::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CDM2GDEDTeView, CDM2GDEDView)
	//{{AFX_MSG_MAP(CDM2GDEDTeView)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDM2GDEDTeView, CDM2GDEDView)
	//{{AFX_DISPATCH_MAP(CDM2GDEDTeView)
	DISP_FUNCTION(CDM2GDEDTeView, "GetMessageText", theGetMesText, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CDM2GDEDTeView, "SetMessageText", theSetMesText, VT_EMPTY, VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CDM2GDEDTeView, "IsLE", theIsLE, VT_BOOL, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {3A07AF85-AA28-4C28-8504-E2441C90892A}
static const IID IID_IDM2GDEDTeView =
{ 0x3a07af85, 0xaa28, 0x4c28, { 0x85, 0x4, 0xe2, 0x44, 0x1c, 0x90, 0x89, 0x2a } };

BEGIN_INTERFACE_MAP(CDM2GDEDTeView, CDM2GDEDView)
	INTERFACE_PART(CDM2GDEDTeView, IID_IDM2GDEDTeView, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// 

#ifdef _DEBUG
void CDM2GDEDTeView::AssertValid() const
{
	CDM2GDEDView::AssertValid();
}

void CDM2GDEDTeView::Dump(CDumpContext& dc) const
{
	CDM2GDEDView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// 

BSTR CDM2GDEDTeView::theGetMesText(LONG iEC)
{
	CString str;
	if (MkGDEdCtx()) {
		CGDAT &core = m_pEdCtx->GetGDAT();
		for (int i = 0; i < 256; i++) {
			CString strText;
			CGDEntry pos(0x3, 0x0, 0x5, i, iEC, 0);
			if (true
				&& core.GetTextAt(core.GetValueAt(pos), strText)
			) {
				str += strText;
			}
			str += '\b';
		}
	}
	return str.AllocSysString();
}

void CDM2GDEDTeView::theSetMesText(LONG iEC, LONG iIdx, LPCTSTR pszText)
{
	if (MkGDEdCtx()) {
		CGDAT &core = m_pEdCtx->GetGDAT();
		CGDEntry pos(0x3, 0x0, 0x5, iIdx, iEC, 0);
		core.SetTextFor(pos, pszText);

		m_pEdCtx->DispatchGDUpdate(PM_V_UPDATE_ENTRY, &pos);
	}
	return;
}

BOOL CDM2GDEDTeView::theIsLE()
{
	if (MkGDEdCtx()) {
		CGDAT &core = m_pEdCtx->GetGDAT();
		return core.IsLE();
	}
	return false;
}

VARIANT CDM2GDEDTeView::theEnumerate()
{
	_variant_t var;
	if (MkGDEdCtx()) {
		CGDAT &core = m_pEdCtx->GetGDAT();
		list<CGDEntry> v;
		core.Enum(v);
		SizeBuff fv(6 * v.size());
		BYTE *pb = (BYTE *)fv.GetData();
		if (pb != NULL) {
			list<CGDEntry>::iterator
				iterPos = v.begin(),
				iterEnd = v.end();
			BYTE *pbHorz = pb;
			for (; iterPos != iterEnd; iterPos++, pbHorz += 6) {
				memcpy(pbHorz, (*iterPos).c, 6);
			}
			COleByteBin().MkVt(var, fv.GetData(), fv.GetSize());
		}
	}
	return var.Detach();
}

/////////////////////////////////////////////////////////////////////////////
// 

void CDM2GDEDTeView::OnInitialUpdate() 
{
	CDM2GDEDView::OnInitialUpdate();

	ConnectAx();
}

void CDM2GDEDTeView::OnDestroy() 
{
	CDM2GDEDView::OnDestroy();

	ConnectAx(false);
}

bool CDM2GDEDTeView::ConnectAx(bool fReconnect)
{
	IRunningObjectTablePtr pROT;
	HRESULT hr;
	if (FAILED(hr = GetRunningObjectTable(0, &pROT)))
		return false;
	if (m_nCookie != 0) {
		VERIFY(SUCCEEDED(hr = pROT->Revoke(m_nCookie)));
		m_nCookie = 0;
	}

	if (!fReconnect) return true;

	_bstr_t bstrFile = GetDocument()->GetPathName();

	IMonikerPtr pMon1, pMon2, pMonx;
	if (true
		&& !FAILED(hr = CreateClassMoniker(GUID_DM2GDED_REV1, &pMon1))
		&& !FAILED(hr = CreateItemMoniker(L"!", bstrFile, &pMon2))
		&& !FAILED(hr = CreateGenericComposite(pMon1, pMon2, &pMonx))
	) {
		if (SUCCEEDED(hr = pROT->Register(0, GetIDispatch(false), pMonx, &m_nCookie))) {
			return true;
		}
		m_nCookie = 0;
	}
	return false;
}
