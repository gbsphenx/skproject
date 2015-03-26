// ExportDATHintDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dm2gded.h"
#include "ExportDATHintDlg.h"
#include "OSP.h"
#include "CSVrw.h"
#include "DM2TextEncoder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace OSP;

/*
Type

0x00 --- ?
0x01 --- Raw (Image)
0x02 --- Raw (Sound)
0x03 --- Raw (Music)
0x04 --- Raw (?)
0x05 --- Raw (Message)
0x06 --- Raw (?)
0x07 --- Raw (?)
0x08 --- Raw (?)
0x09 --- Raw (?)
0x0A --- ?
0x0B --- Word value
0x0C --- Word value
0x0D --- Raw (?)
0x0E --- Raw (?)
0x0F --- ?

*/

/////////////////////////////////////////////////////////////////////////////
// CExportDATFx

void CExportDATFx::Add4Export(const list<CGDEntry> &listKey)
{
	list<CGDEntry>::const_iterator
		iterPos = listKey.begin(),
		iterEnd = listKey.end();
	for (; iterPos != iterEnd; iterPos++) {
		m_map[*iterPos];
	}
}

BOOL CExportDATFx::ExportTo(CGDAT &rDAT, CExportDATComment &rComment, LPCTSTR pszDir)
{
	CString strPathInf = OSP_JoinPath(pszDir, "DM2DATEx.ini");
	CString strPathCsv = OSP_JoinPath(pszDir, "DM2DATEx.csv");

	CSVW_t csvw;
	if (!csvw.Open(strPathCsv)) {
		return FALSE;
	}

	UINT iItem, nItems = m_map.size();
	OnExportHintStep(0, nItems);
	FxMap::iterator
		iterPos = m_map.begin(),
		iterEnd = m_map.end();
	for (iItem = 0; iterPos != iterEnd; iterPos++, iItem++) {
		const CGDEntry &pos = iterPos->first;
		FxEntry &rEntry = iterPos->second;

		CString strDisp;
		FormatEntryDisp(strDisp, pos);

		OnExportHintText(strDisp);

		if (!ExportEntry(rDAT, pos, rEntry)) {

		} else {
			CString strName;
			FormatEntryName(strName, pos);
			CString strText;
			if (rEntry.fIsRaw) {
				if (!ExportRawData(OSP_JoinPath(pszDir, strText = rEntry.strFileName), rEntry.fv)) {
					return FALSE;
				}
			} else {
				strText.Format("%ld", rEntry.nVal);
			}

			csvw.WriteToken(strName);
			csvw.WriteToken(rEntry.strType);
			csvw.WriteToken(strText);

			if (!csvw.WriteNextLine()) {
				return FALSE;
			}
		}

		OnExportHintStep(iItem + 1, nItems);
	}

	OnExportHintStep(-1, 0);
	OnExportHintText("Please wait for a while...");

	csvw.Close();

	if (false
		|| !SaveProfileStringTo("DM2GDED.Export", "list_csv", ".\\DM2DATEx.csv", strPathInf)
//		|| !SaveProfileStringTo("DM2GDED.Export", "memo_txt", ".\\MEMO.txt", strPathInf)
		|| !SaveProfileStringTo("DM2GDED.Export", "exporter_ver", "0", strPathInf)
		|| !rComment.Revert(strPathInf, false)
	) {
		return FALSE;
	}

	return TRUE;
}

bool CExportDATFx::ExportRawData(LPCTSTR pszFileInto, const SizeBuff &fv)
{
	FILE *f = fopen(pszFileInto, "wb");
	if (f) {
		if (fwrite(fv.GetData(), fv.GetSize(), 1, f) == 1) {
			fclose(f);
			return true;
		}
		fclose(f);
	}
	return false;
}

bool CExportDATFx::ExportEntry(CGDAT &rDAT, const CGDEntry &pos, FxEntry &rEntry)
{
	switch (pos.c3()) {
	case 0x01:
		{
			int nIdx = rDAT.GetValueAt(pos);
			rEntry.strFileName.Format("%04d.bin", nIdx);
			rEntry.strType = "raw";
			rEntry.fIsRaw = true;
			int nGDRIDT;
			if (!rDAT.GetRawImageDataAt(nIdx, rEntry.fv, nGDRIDT))
				return false;
			switch (nGDRIDT) {
			default: ASSERT(FALSE); break;
			case GDRIDT_C4: rEntry.strType = "dm2_c4_le"; break;
			case GDRIDT_C8: rEntry.strType = "dm2_c8_le"; break;
			case GDRIDT_U4: rEntry.strType = "dm2_u4_le"; break;
			case GDRIDT_U8: rEntry.strType = "dm2_u8_le"; break;
			}
			return true;
		}
	case 0x05:
		{
			CString strText;
			int nIdx = rDAT.GetValueAt(pos);
			int nGDRTDT;
			if (!rDAT.GetExcTextAt(nIdx, rEntry.fv, nGDRTDT))
				return false;
			rEntry.strFileName.Format("%04d.txt", nIdx);
			rEntry.fIsRaw = true;
			switch (nGDRTDT) {
			default: ASSERT(FALSE); break;
			case GDRTDT_EN: rEntry.strType = "txt_en_enc"; break;
			case GDRTDT_JP: rEntry.strType = "txt_ja_enc"; break;
			}
			return true;
		}
	case 0x02:
	case 0x03:

	case 0x04:
	case 0x06:
	case 0x07: // ANYRAW
	case 0x08:
	case 0x09:
	case 0x0D:
	case 0x0E:
		{
			int nIdx = rDAT.GetValueAt(pos);
			rEntry.strFileName.Format("%04d.bin", nIdx);
			rEntry.strType = "txt";
			rEntry.fIsRaw = true;
			if (!rDAT.GetRawDataAt(nIdx, rEntry.fv))
				return false;
			return true;
		}
	case 0x0B:
	case 0x0C:
		{
			rEntry.strType = "value";
			rEntry.fIsRaw = false;
			rEntry.nVal = rDAT.GetValueAt(pos);
			return true;
		}
	}
	return false;
}

void CExportDATFx::FormatEntryName(CString &strText, const CGDEntry &pos)
{
	strText.Format("%02X%02X%02X%02X%02X%02X"
		, 0U+pos.c1()
		, 0U+pos.c2()
		, 0U+pos.c3()
		, 0U+pos.c4()
		, 0U+pos.c5()
		, 0U+pos.c6()
		);
}

void CExportDATFx::FormatEntryDisp(CString &strText, const CGDEntry &pos)
{
	strText.Format("%02X %02X-%02X-%02X %02X %02X"
		, 0U+pos.c1()
		, 0U+pos.c2()
		, 0U+pos.c3()
		, 0U+pos.c4()
		, 0U+pos.c5()
		, 0U+pos.c6()
		);
}

/////////////////////////////////////////////////////////////////////////////
// CExportDATHintDlg ダイアログ

CExportDATHintDlg::CExportDATHintDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportDATHintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportDATHintDlg)
	m_strCurFile = _T("");
	m_strCurPercent = _T("");
	//}}AFX_DATA_INIT
}

void CExportDATHintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportDATHintDlg)
	DDX_Text(pDX, IDC_STATIC_CUR_FILE, m_strCurFile);
	DDX_Text(pDX, IDC_STATIC_CUR_PERCENT, m_strCurPercent);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExportDATHintDlg, CDialog)
	//{{AFX_MSG_MAP(CExportDATHintDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportDATHintDlg メッセージ ハンドラ

void CExportDATHintDlg::OnExportHintStep(UINT nCur, UINT nMax)
{
	if (!UpdateData()) return;

	if (nCur != -1) {
		m_strCurPercent.Format("%5d/%5d", nCur, nMax);
	} else {
		m_strCurPercent.Empty();
	}

	UpdateData(FALSE);
	UpdateWindow();
}

void CExportDATHintDlg::OnExportHintText(LPCTSTR psz)
{
	if (!UpdateData()) return;

	m_strCurFile = psz;

	UpdateData(FALSE);
	UpdateWindow();
}

BOOL CExportDATHintDlg::ExportTo(CGDAT &rDAT, CExportDATComment &rComment, LPCTSTR pszDir)
{
	if (Create(IDD, GetActiveWindow())) {
		ShowWindow(SW_SHOW);
		UpdateWindow();
		BOOL fRes = CExportDATFx::ExportTo(rDAT, rComment, pszDir);
		DestroyWindow();
		return fRes;
	}
	return FALSE;
}
