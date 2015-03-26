// DiffImportExec.cpp: CDiffImportExec クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DiffImportExec.h"
#include "CSVrw.h"
#include "OSP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace OSP;

namespace
{

bool ReadInt(LPCSTR psz, int &x)
{
	LPSTR pszEnd = NULL;
	x = strtol(psz, &pszEnd, 10);
	if (pszEnd[0] != 0) return false;
	return true;
}

bool ReadFileFrom(LPCTSTR pszFileIn, SizeBuff &fv)
{
	CFile fileIn;
	if (!fileIn.Open(pszFileIn, 0 |CFile::modeRead))
		return false;
	int n = fileIn.GetLength();
	if (!fv.Alloc(n) || fileIn.Read(fv.GetData(), n) != n)
		return false;
	return true;
}

CString TestAndJoinPath(CString strBase, CString strAdd)
{
	if (strAdd.Left(2).Compare(".\\") == 0)
		return OSP_JoinPath(strBase, strAdd);
	return strAdd;
}

};

bool CDiffImportExec::Exec(CGDAT &rOrg, CString strPathInf)
{
	CString strCsvFileName, strVer, strOpenFlag;

	if (!rOrg.MakeSure()) return false;

	int nOpenFlag;
	if (false
		|| !LoadProfileStringFrom("DM2GDED.DiffExport", "list_csv", strCsvFileName, _T(""), strPathInf)
		|| !LoadProfileStringFrom("DM2GDED.DiffExport", "ver", strVer, _T(""), strPathInf)
		|| !LoadProfileStringFrom("DM2GDED.DiffExport", "open_flags", strOpenFlag, _T(""), strPathInf)
		|| strVer != _T("0")
		|| !ReadInt(strOpenFlag, nOpenFlag)
	) {
		return false;
	}
	if (nOpenFlag != rOrg.GetGDOF()) {
		AfxMessageBox(IDS_ERR_ANOTHER_OPEN_FLAGS, 0 |MB_ICONEXCLAMATION);
		return false;
	}
	CString strBaseDir = OSP_GetDir(strPathInf);
	CString strCsvPathName = TestAndJoinPath(strBaseDir, strCsvFileName);

	CSVR_t csvr;
	if (!csvr.Open(strCsvPathName))
		return false;

	int nCnt = rOrg.GetRawDataCount();

	SizeBuff fv;

	while (csvr.ReadNextLine()) {
		CString str[3];
		int iIdx;
		if (false
			|| !csvr.ReadNextToken(str[0])
			|| !csvr.ReadNextToken(str[1])
			|| !csvr.ReadNextToken(str[2])
			|| str[0].GetLength() != 1
			|| !ReadInt(str[1], iIdx)
		) {
			continue;
		}
		TCHAR tc = str[0].GetAt(0);
		switch (tc) {
		case '!':
		case '+':
			{
				if (!ReadFileFrom(TestAndJoinPath(strBaseDir, str[2]), fv))
					continue;
				if (tc == '!') {
					VERIFY(rOrg.PutRawDataFor(iIdx, fv));
				} else if (tc == '+' && nCnt == iIdx) {
					VERIFY(rOrg.AppendRawData(fv));
					nCnt++;
				}
				break;
			}
		}
	}

	if (!rOrg.MakeUnsure()) return false;

	return true;
}
