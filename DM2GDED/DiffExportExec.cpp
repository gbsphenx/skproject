// DiffExportExec.cpp: CDiffExportExec クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DiffExportExec.h"
#include "OSP.h"
#include "CSVrw.h"
#include "InprogressDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace OSP;

bool CDiffExportExec::Exec(CString strFileOrg, CGDAT &rMod, CString strOutput)
{
	CInprogressDlg wndDlg;

	wndDlg.DoModeless(CWnd::GetActiveWindow());

	CInprogressSPH sph(wndDlg);

	sph.Init(3);

	// STG: 1/3
	sph.SetStageMaxStep(1);

	CGDAT aOrg;

	if (!aOrg.LoadFrom(strFileOrg, rMod.GetGDOF())) return false;

	if (!aOrg.MakeSure()) return false;
	if (!rMod.MakeSure()) return false;

	sph.StepStage();

	// STG: 2/3
	int n1 = aOrg.GetRawDataCount();
	int n2 = rMod.GetRawDataCount();

	sph.SetStageMaxStep(n2);

	CSVW_t csvw;
	if (!csvw.Open(OSP_JoinPath(strOutput, _T("Diff.csv"))))
		return false;

	SizeBuff fvOrg, fvMod;
	CString strFileName, strNo;

	int i = 0;

	for (; i < n1 && i < n2; i++) {
		if (!aOrg.GetRawDataAt(i, fvOrg)) return false;
		if (!rMod.GetRawDataAt(i, fvMod)) return false;

		if (fvOrg != fvMod) {
			strFileName.Format(_T(".\\%04d.bin"), i);

			TRY
				CFile fileInto(OSP_JoinPath(strOutput, strFileName), 0 |CFile::modeCreate |CFile::modeWrite);
				UINT cb = fvMod.GetSize();
				fileInto.Write(fvMod.GetData(), cb);
				fileInto.Close();
			CATCH_ALL(e)
				return false;
			END_CATCH_ALL

			strNo.Format(_T("%d"), i);

			csvw.WriteToken(_T("!"));
			csvw.WriteToken(strNo);
			csvw.WriteToken(strFileName);

			if (!csvw.WriteNextLine()) return false;
		}

		sph.Step();
	}

//	for (; i < n1; i++) { // delete more
//		strNo.Format(_T("%d"), i);
//
//		csvw.WriteToken(_T("-"));
//		csvw.WriteToken(strNo);
//		csvw.WriteToken(_T(""));
//
//		sph.Step();
//	}

	for (; i < n2; i++) { // add more
		if (!rMod.GetRawDataAt(i, fvMod)) return false;

		strFileName.Format(_T(".\\%04d.bin"), i);

		TRY
			CFile fileInto(OSP_JoinPath(strOutput, strFileName), 0 |CFile::modeCreate |CFile::modeWrite);
			UINT cb = fvMod.GetSize();
			fileInto.Write(fvMod.GetData(), cb);
			fileInto.Close();
		CATCH_ALL(e)
			return false;
		END_CATCH_ALL

		strNo.Format(_T("%d"), i);

		csvw.WriteToken(_T("+"));
		csvw.WriteToken(strNo);
		csvw.WriteToken(strFileName);

		sph.Step();
	}

	csvw.Close();

	CString strPathInf = OSP_JoinPath(strOutput, _T("Diff.ini"));

	CString strOpenFlag;
	strOpenFlag.Format("%lu", 0UL +rMod.GetGDOF());

	if (false
		|| !SaveProfileStringTo("DM2GDED.DiffExport", "list_csv", ".\\diff.csv", strPathInf)
		|| !SaveProfileStringTo("DM2GDED.DiffExport", "ver", "0", strPathInf)
		|| !SaveProfileStringTo("DM2GDED.DiffExport", "generator", "DM2GDED", strPathInf)
		|| !SaveProfileStringTo("DM2GDED.DiffExport", "open_flags", strOpenFlag, strPathInf)
	) {
		return false;
	}

	sph.StepStage();

	// STG: 3/3
	sph.SetStageMaxStep(1);

	aOrg.Close();

	sph.StepStage();

	// STG: END

	Sleep(1);

	return true;
}
