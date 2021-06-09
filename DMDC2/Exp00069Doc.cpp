// Exp00069Doc.cpp : CExp00069Doc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "Exp00069.h"

#include "Exp00069Doc.h"
#include "DM2WorldShiftDlg.h"
#include "ProfStr.h"

#include <crtdbg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExp00069Doc

IMPLEMENT_DYNCREATE(CExp00069Doc, CDocument)

BEGIN_MESSAGE_MAP(CExp00069Doc, CDocument)
	//{{AFX_MSG_MAP(CExp00069Doc)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_APP_DEV, OnAppDev)
	ON_COMMAND(ID_POPUP_LIMITLESS, OnPopupLimitless)
	ON_UPDATE_COMMAND_UI(ID_POPUP_LIMITLESS, OnUpdatePopupLimitless)
	ON_COMMAND(ID_EDIT_WORLD_SHIFT, OnEditWorldShift)
	ON_COMMAND(ID_DEBUG_MOVE2ACTU2, OnDebugMove2actu2)
	//}}AFX_MSG_MAP
	ON_COMMAND_EX(ID_OPT_SAVE_LE, OnOptxxx)
	ON_UPDATE_COMMAND_UI(ID_OPT_SAVE_LE, OnUpdateOptxxx)
	ON_COMMAND_EX(ID_OPT_ATTACH_CS, OnOptxxx)
	ON_UPDATE_COMMAND_UI(ID_OPT_ATTACH_CS, OnUpdateOptxxx)
	ON_COMMAND_EX(ID_OPT_CHECK_LEVEL_DESC, OnOptxxx)
	ON_UPDATE_COMMAND_UI(ID_OPT_CHECK_LEVEL_DESC, OnUpdateOptxxx)
	ON_COMMAND_EX(ID_OPT_DM2_EXTENDED_MAP, OnOptxxx)
	ON_UPDATE_COMMAND_UI(ID_OPT_DM2_EXTENDED_MAP, OnUpdateOptxxx)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExp00069Doc クラスの構築/消滅

#pragma warning(disable: 4355)

CExp00069Doc::CExp00069Doc()
{
	m_pEdCtx = new CDMEdCtx();
	m_pEdCtx->AddCB(this);

	m_fLE = ProfStr::GetProfileIntFrom("DMDC2", "Save as Little Endian", TRUE, GetApp()->GetCnf());
	m_fCheckSum = ProfStr::GetProfileIntFrom("DMDC2", "Attach Check Sum", TRUE, GetApp()->GetCnf());
	m_fCheckDesc = ProfStr::GetProfileIntFrom("DMDC2", "Auto tweak LevelDesc on save", TRUE, GetApp()->GetCnf());
	m_fDM2_EXTENDED_MAP = (ProfStr::GetProfileIntFrom("DMDC2", "DM2_EXTENDED_MAP", FALSE, GetApp()->GetCnf()) == 1 ? true : false);
}

#pragma warning(default: 4355)

CExp00069Doc::~CExp00069Doc()
{
	m_pEdCtx->RemoveCB(this);
	m_pEdCtx->Release();
}

BOOL CExp00069Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	//UpdateAllViews(NULL, DPM_RELOAD);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CExp00069Doc シリアライゼーション

void CExp00069Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		if (m_fCheckDesc) GetDDAT().CheckAllLevelDesc();

		ar << *this;
	}
	else
	{
		//ar >> *this;

		//m_pEdCtx->DispatchDMUpdate(DPM_RELOAD, NULL);
		//m_pEdCtx->DispatchDMUpdate(DPM_SELECTDMLV, NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CExp00069Doc クラスの診断

#ifdef _DEBUG
void CExp00069Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CExp00069Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExp00069Doc コマンド

CArchive &operator <<(CArchive &ar, CExp00069Doc &r)
{
	ar.Flush();
	CDDATx::CEnclosedWisoFileIo fInto(*ar.GetFile(), r.m_fLE ? true : false);
#if _MSC_VER <= 1200
	if (!r.GetDDAT().SaveTo(fInto, r.m_fLE, r.m_fCheckSum, r.m_fDM2_EXTENDED_MAP)) AfxThrowArchiveException(CArchiveException::generic);
#else
	if (!r.GetDDAT().SaveTo(fInto, r.m_fLE, r.m_fCheckSum, r.m_fDM2_EXTENDED_MAP)) AfxThrowArchiveException(CArchiveException::genericException);
#endif
	r.GetDDAT().SetModified(FALSE);

	return ar;
}

void CExp00069Doc::DeleteContents() 
{
	CWaitCursor wc;

	GetDDAT().DeleteContents();
//	UINT i;
//	HRESULT hr;
//	VERIFY(SUCCEEDED(hr = GetDDAT().AddEmptyMap(i)));
	GetDDAT().DeleteUndoHistory();
	GetDDAT().SetModified(FALSE);

	CDocument::DeleteContents();
}

void CExp00069Doc::OnEditUndo() 
{
	GetDDAT().DoUndo();
}

void CExp00069Doc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDDAT().IsUndoable());
}

void CExp00069Doc::OnAppDev() 
{
#ifdef _DEBUG
	GetDDAT().Dev();
#endif
}

void CExp00069Doc::OnPopupLimitless() 
{
	if (!m_conf.bLimitless) {
		//LPCTSTR lpsz =
		//	"You are to enable Unlimited mode.\n\n"
		//	"Unlimited mode brings to you...\n"
		//	"- All possible functions work for you.\n"
		//	"- Some manipulate functions are highly detailed which you will feel annoying.\n"
		//	"- Any manipulation can be accepted w/out warning even if your dungeon dat will be damaged.\n"
		//	"- Some available function may destroy your dungeon dat due to any bug of current implement."
		//	;
		int r = AfxMessageBox(IDS_WARN_UNLIMITED_MODE, MB_YESNO|MB_ICONEXCLAMATION|MB_DEFBUTTON2);
		if (r != IDYES)
			return;
	}
	m_conf.bLimitless = m_conf.bLimitless ? 0 : 1;

	m_pEdCtx->SetAccessibility(m_conf);
}

void CExp00069Doc::OnUpdatePopupLimitless(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_conf.bLimitless ? 1 : 0);
}

void CExp00069Doc::OnDMUpdate(UINT nHint, void *pHint)
{
	switch (nHint) {
	case DPM_INITVIEW:
		{
			m_pEdCtx->GetAccessibility(m_conf);
			break;
		}
	case DPM_CHANGED_CONF1:
		{
			m_pEdCtx->GetAccessibility(m_conf);
			break;
		}
	}
}

BOOL CExp00069Doc::IsModified()
{
	if (!m_pEdCtx) return FALSE;

	return GetDDAT().GetModified();
}

BOOL CExp00069Doc::OnOptxxx(UINT nID) 
{
	switch (nID) {
		case ID_OPT_SAVE_LE: ProfStr::SaveProfileIntTo("DMDC2", "Save as Little Endian", (m_fLE = !m_fLE), GetApp()->GetCnf()); return true;
		case ID_OPT_ATTACH_CS: ProfStr::SaveProfileIntTo("DMDC2", "Attach Check Sum", (m_fCheckSum = !m_fCheckSum), GetApp()->GetCnf()); return true;
		case ID_OPT_CHECK_LEVEL_DESC: ProfStr::SaveProfileIntTo("DMDC2", "Auto tweak LevelDesc on save", (m_fCheckDesc = !m_fCheckDesc), GetApp()->GetCnf()); return true;
		case ID_OPT_DM2_EXTENDED_MAP: ProfStr::SaveProfileIntTo("DMDC2", "DM2_EXTENDED_MAP", (m_fDM2_EXTENDED_MAP = !m_fDM2_EXTENDED_MAP), GetApp()->GetCnf()); return true;
	}
	return false;
}

void CExp00069Doc::OnUpdateOptxxx(CCmdUI* pCmdUI) 
{
	switch (pCmdUI->m_nID) {
		case ID_OPT_SAVE_LE: pCmdUI->SetCheck(m_fLE); break;
		case ID_OPT_ATTACH_CS: pCmdUI->SetCheck(m_fCheckSum); break;
		case ID_OPT_CHECK_LEVEL_DESC: pCmdUI->SetCheck(m_fCheckDesc); break;
		case ID_OPT_DM2_EXTENDED_MAP: pCmdUI->SetCheck(m_fDM2_EXTENDED_MAP); break;
	}
}

BOOL CExp00069Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CFileException fe;
	CFile* pFile = GetFile(lpszPathName, CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL) {
		ReportSaveLoadException(lpszPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	OnNewDocument();

	CDDATx::CEnclosedWisoFileIo fIn(*pFile, true);

	bool fOk = false;

	TRY {
		DeleteContents();
		SetModifiedFlag();

		fIn.Close(true);
		pFile->SeekToBegin();
		if (GetDDAT().LoadFrom(fIn, true, m_fDM2_EXTENDED_MAP)) {
			fOk = true; m_fLE = true;
		} else {
			fIn.Close(false);
			pFile->SeekToBegin();
			if (GetDDAT().LoadFrom(fIn, false, m_fDM2_EXTENDED_MAP)) {
				fOk = true; m_fLE = false;
			} else {
				ReleaseFile(pFile, TRUE);
				DeleteContents();

				CArchiveException e(CArchiveException::badIndex, lpszPathName);

				ReportSaveLoadException(lpszPathName, &e, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
				return false;
			}
		}
	} CATCH_ALL(e) {
		ReleaseFile(pFile, TRUE);
		return false;
	} END_CATCH_ALL

	ReleaseFile(pFile, TRUE);

	SetModifiedFlag(FALSE);
	GetDDAT().DeleteUndoHistory();
	GetDDAT().SetModified(FALSE);

	m_pEdCtx->DispatchDMUpdate(DPM_RELOAD, NULL);
	m_pEdCtx->DispatchDMUpdate(DPM_SELECTDMLV, NULL);

	return TRUE;
}

BOOL CExp00069Doc::OnSaveDocument(LPCTSTR lpszPathName) 
{
//	CNotSupportedException e;
//	ReportSaveLoadException(lpszPathName, &e, TRUE, AFX_IDS_NOT_SUPPORTED_EXCEPTION);
//	return FALSE;
	return CDocument::OnSaveDocument(lpszPathName);
}

void CExp00069Doc::OnEditWorldShift() 
{
	if (!m_pEdCtx) return;

	CDM2WorldShiftDlg wndDlg;
	int r = wndDlg.DoModal();
	if (r != IDOK) return;

	GetDDAT().WorldShift(wndDlg.m_nX, wndDlg.m_nY);

	m_pEdCtx->DispatchDMUpdate(DPM_CHANGED_LEVELDESC, NULL);
}

bool CExp00069Doc::SettleLoad(CMemFile &f)
{
	CDDATx::CEnclosedWisoFileIo fIo(f, true);

	GetDDAT().SetSuppression(true);
	bool res = GetDDAT().LoadFrom(fIo, true, false);
	GetDDAT().SetSuppression(false);

	if (res) {
		SetModifiedFlag(FALSE);
		GetDDAT().DeleteUndoHistory();
		GetDDAT().SetModified(FALSE);

		m_pEdCtx->DispatchDMUpdate(DPM_RELOAD, NULL);
		return true;
	}

	return false;
}

bool CExp00069Doc::SettleSave(CMemFile &f)
{
	CDDATx::CEnclosedWisoFileIo fIo(f, true);

	bool res = GetDDAT().SaveTo(fIo, true, false, false);

	if (res) {
		return true;
	}
	return false;
}

void CExp00069Doc::OnDebugMove2actu2()
{
	CWaitCursor wc;
	HRESULT hr;
	if (SUCCEEDED(hr = GetDDAT().Move2actu2())) {
		AfxMessageBox(IDS_MOVE2ACTU_DONE, MB_ICONINFORMATION);
	}
	else if (hr == E_OUTOFMEMORY) {
		AfxMessageBox(IDS_ERR_KILL_RECORD11, MB_ICONEXCLAMATION);
	}
}
