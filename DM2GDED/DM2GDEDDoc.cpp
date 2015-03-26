// DM2GDEDDoc.cpp : CDM2GDEDDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "DM2GDED.h"

#include "DM2GDEDDoc.h"
#include "OpenFileTypeDlg.h"
#include "ExportDATFrame.h"
#include "ExportDATDoc.h"
#include "ImportQueryDlg.h"
#include "ImportDATFx.h"
#include "DM2EdPicFrame.h"
#include "DiffExportDlg.h"
#include "DiffExportExec.h"
#include "DiffImportDlg.h"
#include "DiffImportExec.h"
#include "DM2GDEDOPInfoDlg.h"
#include "DTreeUsing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDDoc

IMPLEMENT_DYNCREATE(CDM2GDEDDoc, CDocument)

BEGIN_MESSAGE_MAP(CDM2GDEDDoc, CDocument)
	//{{AFX_MSG_MAP(CDM2GDEDDoc)
	ON_COMMAND(ID_OPT_RELOAD_RULES, OnOptReloadRules)
	ON_COMMAND(ID_FILE_EXPORT_DIFF, OnFileExportDiff)
	ON_COMMAND(ID_FILE_IMPORT_DIFF, OnFileImportDiff)
	ON_COMMAND(ID_FILE_DISPLAY_OPI, OnFileDisplayOPI)
	ON_COMMAND(ID_TEST, OnTest)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_OPT_UWV_HEX, ID_OPT_LANG_FR, OnOptMisc)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPT_UWV_HEX, ID_OPT_LANG_FR, OnUpdateOptMisc)
//	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
//	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDDoc クラスの構築/消滅

CDM2GDEDDoc::CDM2GDEDDoc()
{
	m_pEdCtx = new CGDEdCtx();

	m_pEdCtx->GetOpts(m_aEdOpt);

	GetApp()->RegisterIdleCB(this);
}

CDM2GDEDDoc::~CDM2GDEDDoc()
{
	if (m_pEdCtx)
		m_pEdCtx->Release();

	GetApp()->RevokeIdleCB(this);
}

BOOL CDM2GDEDDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_pEdCtx->GetGDAT().Close();
	m_pEdCtx->GetGDAT().SetModified(false);

	m_nGDRSLT = -1;

	m_pEdCtx->DispatchGDUpdate(PM_V_RELOAD, NULL);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDDoc シリアライゼーション

void CDM2GDEDDoc::Serialize(CArchive& ar)
{
	AfxThrowNotSupportedException();
}

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDDoc クラスの診断

#ifdef _DEBUG
void CDM2GDEDDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDM2GDEDDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDM2GDEDDoc コマンド

BOOL CDM2GDEDDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	UINT nGDOF = GDFF_DETECTIVE;
	bool fDontAsk = GetApp()->GetUseDetective();
	if (!fDontAsk) {
		COpenFileTypeDlg wndDlg(lpszPathName, AfxGetMainWnd());
		int r = wndDlg.DoModal();
		if (r == IDOK) {
			nGDOF = wndDlg.GetGDOF();
		} else if (r == IDRETRY) {
			nGDOF;
		} else {
			return FALSE;
		}
	}
	CWaitCursor wc;
	if (!m_pEdCtx->GetGDAT().LoadFrom(lpszPathName, nGDOF)) {
		CArchiveException fe(CArchiveException::badIndex, lpszPathName);
		ReportSaveLoadException(lpszPathName, &fe, FALSE, 0);
		return FALSE;
	}
	m_nGDRSLT = m_pEdCtx->GetGDAT().GetGDRSLT();
	m_pEdCtx->GetGDAT().SetModified(false);
	m_pEdCtx->DispatchGDUpdate(PM_V_RELOAD, NULL);
	return TRUE;
}

BOOL CDM2GDEDDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	int r = AfxMessageBox(IDS_WARN_SAVE, MB_ICONEXCLAMATION|MB_YESNO);
	if (r != IDYES) {
		CUserException fe;
		ReportSaveLoadException(lpszPathName, &fe, TRUE, 0);
		return FALSE;
	}

	CWaitCursor wc;
	if (!m_pEdCtx->GetGDAT().SaveTo(lpszPathName)) {
		CArchiveException fe(CArchiveException::genericException, lpszPathName);
		ReportSaveLoadException(lpszPathName, &fe, TRUE, 0);
		return FALSE;
	}
	m_pEdCtx->GetGDAT().SetModified(false);
	return TRUE;
}

BOOL CDM2GDEDDoc::IsModified()
{
	if (!m_pEdCtx) return FALSE;
	return m_pEdCtx->GetGDAT().IsModified();
}

void CDM2GDEDDoc::SetModifiedFlag(BOOL bModified)
{
	if (!m_pEdCtx) return;
	m_pEdCtx->GetGDAT().SetModified(bModified ? true : false);
}

void CDM2GDEDDoc::OnOptMisc(UINT nID) 
{
	if (!m_pEdCtx) return;

	switch (nID) {
	case ID_OPT_UWV_HEX:		m_aEdOpt.m_wvct = wvctHex; break;
	case ID_OPT_UWV_SIGNED:		m_aEdOpt.m_wvct = wvctSigned; break;
	case ID_OPT_UWV_UNSIGNED:	m_aEdOpt.m_wvct = wvctUnsigned; break;
	case ID_OPT_UWV_FLAGS:		m_aEdOpt.m_wvct = wvctBinary; break;
	case ID_OPT_CLANG_NO:		m_aEdOpt.m_cnlang = cnlangNo; break;
	case ID_OPT_CLANG_EN:		m_aEdOpt.m_cnlang = cnlangEn; break;
	case ID_OPT_CLANG_EN_PLAIN:	m_aEdOpt.m_cnlang = cnlangEnPlain; break;
	case ID_OPT_LANG_NO:		m_aEdOpt.m_inlang = inlangNo; break;
	case ID_OPT_LANG_EN:		m_aEdOpt.m_inlang = inlangEn; break;
	case ID_OPT_LANG_EN_PLAIN:	m_aEdOpt.m_inlang = inlangEnPlain; break;
	case ID_OPT_LANG_JA:		m_aEdOpt.m_inlang = inlangJa; break;
	case ID_OPT_LANG_DE:		m_aEdOpt.m_inlang = inlangDe; break;
	case ID_OPT_LANG_FR:		m_aEdOpt.m_inlang = inlangFr; break;
	}

	m_pEdCtx->SetOpts(m_aEdOpt);
	m_pEdCtx->DispatchGDUpdate(PM_V_SET_OPT, NULL);
}

void CDM2GDEDDoc::OnUpdateOptMisc(CCmdUI* pCmdUI) 
{
	bool fRadio = false, fEnabled = true;
	switch (pCmdUI->m_nID) {
	case ID_OPT_UWV_HEX:		fRadio = (m_aEdOpt.m_wvct == wvctHex); break;
	case ID_OPT_UWV_SIGNED:		fRadio = (m_aEdOpt.m_wvct == wvctSigned); break;
	case ID_OPT_UWV_UNSIGNED:	fRadio = (m_aEdOpt.m_wvct == wvctUnsigned); break;
	case ID_OPT_UWV_FLAGS:		fRadio = (m_aEdOpt.m_wvct == wvctBinary); break;
	case ID_OPT_CLANG_NO:		fRadio = (m_aEdOpt.m_cnlang == cnlangNo); break;
	case ID_OPT_CLANG_EN:		fRadio = (m_aEdOpt.m_cnlang == cnlangEn); break;
	case ID_OPT_CLANG_EN_PLAIN:	fRadio = (m_aEdOpt.m_cnlang == cnlangEnPlain); break;
	case ID_OPT_LANG_NO:		fRadio = (m_aEdOpt.m_inlang == inlangNo); break;
	case ID_OPT_LANG_EN:		fRadio = (m_aEdOpt.m_inlang == inlangEn); break;
	case ID_OPT_LANG_EN_PLAIN:	fRadio = (m_aEdOpt.m_inlang == inlangEnPlain); break;
	case ID_OPT_LANG_JA:		fRadio = (m_aEdOpt.m_inlang == inlangJa); break;
	case ID_OPT_LANG_DE:		fRadio = (m_aEdOpt.m_inlang == inlangDe); break;
	case ID_OPT_LANG_FR:		fRadio = (m_aEdOpt.m_inlang == inlangFr); break;
	}
	switch (pCmdUI->m_nID) {
	case ID_OPT_CLANG_NO:		fEnabled = true; break;
	case ID_OPT_CLANG_EN:		fEnabled = (m_nGDRSLT & GDRSLT_C_EN) ? true : false; break;
	case ID_OPT_CLANG_EN_PLAIN:	fEnabled = (m_nGDRSLT & GDRSLT_C_EN_PLAIN) ? true : false; break;
	case ID_OPT_LANG_NO:		fEnabled = true; break;
	case ID_OPT_LANG_EN:		fEnabled = (m_nGDRSLT & GDRSLT_I_EN) ? true : false; break;
	case ID_OPT_LANG_EN_PLAIN:	fEnabled = (m_nGDRSLT & GDRSLT_I_EN_PLAIN) ? true : false; break;
	case ID_OPT_LANG_JA:		fEnabled = (m_nGDRSLT & GDRSLT_I_JP) ? true : false; break;
	case ID_OPT_LANG_DE:		fEnabled = (m_nGDRSLT & GDRSLT_I_DE) ? true : false; break;
	case ID_OPT_LANG_FR:		fEnabled = (m_nGDRSLT & GDRSLT_I_FR) ? true : false; break;
	}
	pCmdUI->SetRadio(fRadio);
	pCmdUI->Enable(fEnabled);
}

BOOL CDM2GDEDDoc::OnIdleNotify()
{
	if (!m_pEdCtx) return FALSE;

	CGDAT &core = m_pEdCtx->GetGDAT();
	CGDEntry pos;
	if (core.GrabNextModifiedEnt(pos)) {
		m_pEdCtx->DispatchGDUpdate(PM_V_UPDATE_ENTRY, &pos);
		return TRUE;
	}
	return FALSE;
}

void CDM2GDEDDoc::OnFileExport() 
{
#if 0
//	for (; ; ) {
//		CExportDATFrame *pFrame = NULL;
//		CExportDATDoc *pDoc = NULL;
//		if (!(pFrame = new CExportDATFrame())) break;
//		CCreateContext aCtx;
//		ZeroMemory(&aCtx, sizeof(aCtx));
//		if (!(pDoc = (CExportDATDoc *)(aCtx.m_pCurrentDoc = (CDocument *)RUNTIME_CLASS(CExportDATDoc)->CreateObject()))) break;
//		pDoc->InitEdCtx(m_pEdCtx);
//		if (!pFrame->LoadFrame(IDR_EXPORTDATFRAME, WS_OVERLAPPEDWINDOW, NULL, &aCtx)) break;
//		pFrame->InitialUpdateFrame(pDoc, TRUE);
//		pFrame->StartModal(AfxGetMainWnd());
//		return;
//	}
//
//	return;
#endif
}

void CDM2GDEDDoc::OnFileImport() 
{
#if 0
//	CImportQueryDlg wndDlg;
//	int r = wndDlg.DoModal();
//	if (r != IDOK) return;
//
//	CImportResDlg wndResDlg;
//
//	for (; ; ) {
//		CWaitCursor wc;
//		CImportDATFx aImp(wndResDlg);
//		if (!aImp.Load(wndDlg.m_strFileIn)) {
//			wndResDlg.ResAddText("Load failed."); break;
//		}
//		if (!aImp.PreImport(GetEdCtx(FALSE)->GetGDAT(), wndDlg.IfOverwrite())) {
//			break;
//		}
//		int r = aImp.Import(GetEdCtx(FALSE)->GetGDAT());
//		if (r == aImp.irERR) {
//			wndResDlg.ResAddText("Import failed.");
//			break;
//		}
//		wndResDlg.DoModal(LoadString(r == aImp.irOk ? IDS_STR_IMPORT_OK : IDS_STR_IMPORT_PART_OK));
//		return;
//	}
//
//	wndResDlg.DoModal(LoadString(IDS_STR_IMPORT_ERR));
//	return;
#endif
}

void CDM2GDEDDoc::OnOptReloadRules() 
{
	CWaitCursor wc;
	m_pEdCtx->LoadDescFrom(GetCnfFilePath());

	m_pEdCtx->DispatchGDUpdate(PM_V_REFRESH, NULL);
}

void CDM2GDEDDoc::OnCloseDocument() 
{
	CDM2EdPicFrame::OnCloseDocument(this);
	
	CDocument::OnCloseDocument();
}

void CDM2GDEDDoc::OnFileExportDiff() 
{
	CDiffExportDlg wndDlg(AfxGetMainWnd());

	wndDlg.m_strFileMod = GetPathName();

	while (true) {
		int r = wndDlg.DoModal();
		if (r == IDOK) {
			{
				CWaitCursor wc;
				CDiffExportExec v;
				if (v.Exec(wndDlg.m_strFileOrg, m_pEdCtx->GetGDAT(), wndDlg.m_strOutput)) {
					AfxMessageBox(IDS_STR_EXPORT_DIFF_DONE, 0 |MB_ICONINFORMATION);
					break;
				}
			}
			AfxMessageBox(IDS_ERR_EXPORT_DIFF, 0 |MB_ICONEXCLAMATION);
		} else {
			break;
		}
	}
}

void CDM2GDEDDoc::OnFileImportDiff() 
{
	CDiffImportDlg wndDlg(AfxGetMainWnd());

	wndDlg.m_strFileOrg = GetPathName();

	while (true) {
		int r = wndDlg.DoModal();
		if (r != IDOK) break;
		CWaitCursor wc;
		CDiffImportExec v;
		if (v.Exec(m_pEdCtx->GetGDAT(), wndDlg.m_strFileDiff)) {
			m_pEdCtx->DispatchGDUpdate(PM_V_RELOAD, NULL);
			AfxMessageBox(IDS_STR_IMPORT_DIFF_DONE, 0 |MB_ICONINFORMATION);
			break;
		}
		AfxMessageBox(IDS_ERR_IMPORT_DIFF, 0 |MB_ICONEXCLAMATION);
	}
}

void CDM2GDEDDoc::OnFileDisplayOPI() 
{
	CDM2GDEDOPInfoDlg wndDlg(CWnd::GetActiveWindow());

	wndDlg.m_strPath = GetPathName();
	wndDlg.SetGDOF(m_pEdCtx->GetGDAT().GetGDOF());

	wndDlg.DoModal();
}

void CDM2GDEDDoc::OnTest() 
{
	CGDAT &core = m_pEdCtx->GetGDAT();
#if 1
	list<CGDEntry> al;
	if (core.Enum(al)) {
		list<CGDEntry>::iterator iterPos = al.begin(), iterEnd = al.end();
		for (; iterPos != iterEnd; iterPos++) {
			CGDEntry pos = *iterPos;
			if (pos.c3() == 1) {
				I32 val = core.GetValueAt(pos);
				AfxTrace("%02X-%02X-%02X-%02X #%04u ", pos.c1(), pos.c2(), pos.c3(), pos.c4(), val);
				// ASSERT(val != 1471); // DEBUG for FM-towns version
				Sleep(5);
				HBITMAP hbm = core.GetImgAt(pos);
				DeleteObject(hbm);

				AfxTrace("%s\n", (hbm != NULL) ? "ok" : "X");
				Sleep(5);
			}
		}
	}
#endif
#if 0
	HBITMAP hbm = core.GetImgAt(CGDEntry(0x1A,0x80,0x01,0x00,0x00,0xFF));
	if (hbm != NULL) DeleteObject(hbm);
#endif
#if 0
	CString str;
	core.GetDispTextAt(core.GetValueAt(CGDEntry(0x1A,0x80,0x05,0x00,0x20,0xFF)), str);
#endif
}

