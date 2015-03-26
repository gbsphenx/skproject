// ImportDATFx.cpp: CImportDATFx クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "ImportDATFx.h"
#include "CSVrw.h"
#include "OSP.h"
#include "DM2ImageComp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace OSP;

namespace
{

bool Conv2EntryPos(LPCSTR pszName, CGDEntry &pos)
{
	UINT iPos[6];
	if (true
		&& strlen(pszName) == 12
		&& sscanf(pszName, "%2X%2X%2X%2X%2X%2X"
		, &iPos[0]
		, &iPos[1]
		, &iPos[2]
		, &iPos[3]
		, &iPos[4]
		, &iPos[5]
		) == 6
	) {
		pos = CGDEntry(iPos[0], iPos[1], iPos[2], iPos[3], iPos[4], iPos[5]);
		return true;
	}
	return false;
}

bool ReadFileInto(LPCSTR pszFileIn, SizeBuff &fv, UINT nMaxSize = 65536)
{
	for (; ; ) {
		if (!fv.Alloc(nMaxSize))
			break;
		FILE *f = fopen(pszFileIn, "rb");
		if (!f)
			break;
		for (; ; ) {
			int nRead = fread(fv.GetData(), 1, nMaxSize, f);
			if (nRead > 0) {
				if (!fv.Alloc(nRead))
					break;
				fclose(f);
				return true;
			}
			break;
		}
		fclose(f);
		break;
	}
	fv.Free();
	return false;
}

CString TestAndJoinPath(CString strBase, CString strAdd)
{
	if (strAdd.Left(2).Compare(".\\") == 0)
		return OSP_JoinPath(strBase, strAdd);
	return strAdd;
}

};

//////////////////////////////////////////////////////////////////////
// CImportDATFx

BOOL CImportDATFx::Load(LPCSTR pszFileIn)
{
	m_map.clear();

	CString strListFile;
	CString strExporterVer;
	if (false
		|| !LoadProfileStringFrom("DM2GDED.Export", "list_csv", strListFile, "", pszFileIn)
		|| !LoadProfileStringFrom("DM2GDED.Export", "exporter_ver", strExporterVer, "", pszFileIn)
		|| (strExporterVer != "0" && strExporterVer != "")
	) {
		return FALSE;
	}
	CString strBaseDir = OSP_GetDir(pszFileIn);
	CString strListFilePath = TestAndJoinPath(strBaseDir, strListFile);
	CSVR_t csvr;
	if (!csvr.Open(strListFilePath))
		return FALSE;
	while (csvr.ReadNextLine()) {
		CString strEntry, strType, strValue;
		if (true
			&& csvr.ReadNextToken(strEntry)
			&& csvr.ReadNextToken(strType)
			&& csvr.ReadNextToken(strValue)
		) {
			FxImpEntry fx;
			fx.nFXIT = 0;
			if (strType == "raw") fx.nFXIT = FXIT_RAW;
			else if (strType == "dm2_c4_le") fx.nFXIT = FXIT_DM2_C4_LE;
			else if (strType == "dm2_c8_le") fx.nFXIT = FXIT_DM2_C8_LE;
			else if (strType == "value") fx.nFXIT = FXIT_VALUE;
			else if (strType == "txt_en_enc") fx.nFXIT = FXIT_TEXT_EN_ENC;
			else if (strType == "txt_ja_enc") fx.nFXIT = FXIT_TEXT_JA_ENC;

			switch (fx.nFXIT) {
			case FXIT_RAW:
			case FXIT_DM2_C4_LE:
			case FXIT_DM2_C8_LE:
			case FXIT_TEXT_EN_ENC:
			case FXIT_TEXT_JA_ENC:
				{
					fx.strFile = OSP_JoinPath(strBaseDir, strValue);
					break;
				}
			case FXIT_VALUE:
				{
					fx.nVal = _ttoi(strValue);
					break;
				}
			}

			if (fx.nFXIT != 0) {
				CGDEntry pos;
				if (Conv2EntryPos(strEntry, pos)) {
					m_map[pos] = fx;
				}
			}
		}
	}
	return m_map.size() != 0;
}

int CImportDATFx::Import(CGDAT &rDAT)
{
	m_res.ResAddSubject("Import entries");

	FxImpEntryMap::iterator
		iterPos = m_map.begin(),
		iterEnd = m_map.end();
	int nCount = 0, nDone = 0;
	for (; iterPos != iterEnd; iterPos++) {
		const CGDEntry &pos = iterPos->first;
		FxImpEntry &rfx = iterPos->second;

		BOOL fRes = FALSE;

		switch (rfx.nFXIT) {
		case FXIT_RAW:
			{
				SizeBuff fv;
				if (ReadFileInto(rfx.strFile, fv)) {
					if (rDAT.AddBinEntry(pos, fv)) {
						fRes = TRUE;
					}
				}
				break;
			}
		case FXIT_VALUE:
			{
				if (rDAT.AddEntry(pos, rfx.nVal, FALSE)) {
					fRes = TRUE;
				}
				break;
			}
		case FXIT_DM2_C4_LE:
			{
				SizeBuff fv;
				if (ReadFileInto(rfx.strFile, fv)) {
					if (rDAT.IsLE()) {
						if (rDAT.AddBinEntry(pos, fv)) {
							fRes = TRUE;
						}
					} else {
						DMGHLiIMGM imgm;
						if (true
							&& CDM2ImageComp(true).DecompC4(fv, imgm, rDAT.GetDM2Pal())
							&& CDM2ImageComp(false).MakeC4Auto(imgm, fv)
						) {
							if (rDAT.AddBinEntry(pos, fv)) {
								fRes = TRUE;
							}
						}
					}
				}
				break;
			}
		case FXIT_DM2_C8_LE:
			{
				SizeBuff fv;
				if (!rDAT.IsC8U8Allowed()) {
					m_res.ResAddUnsup8();
					break;
				}
				if (ReadFileInto(rfx.strFile, fv)) {
					if (rDAT.IsLE()) {
						if (rDAT.AddBinEntry(pos, fv)) {
							fRes = TRUE;
						}
					} else {
						DMGHLiIMGM imgm;
						if (true
							&& CDM2ImageComp(true).DecompC8(fv, imgm)
							&& CDM2ImageComp(false).MakeC8Auto(imgm, fv)
						) {
							if (rDAT.AddBinEntry(pos, fv)) {
								fRes = TRUE;
							}
						}
					}
				}
				break;
			}
		case FXIT_TEXT_EN_ENC:
		case FXIT_TEXT_JA_ENC:
			{
				SizeBuff fv;
				if (ReadFileInto(rfx.strFile, fv)) {
					int nGDRTDT = GDRTDT_EN;
					switch (rfx.nFXIT) {
					case FXIT_TEXT_JA_ENC: nGDRTDT = GDRTDT_JP; break;
					}
					if (rDAT.AddExcTextEntry(pos, fv, nGDRTDT)) {
						fRes = TRUE;
					}
				}
				break;
			}
		}

		m_res.ResAddEntryImpRes(pos, fRes);

		nCount++;
		if (fRes) nDone++;
	}
	if (nDone == 0) return irERR;
	return (nDone == nCount) ? irOk : irPartial;
}

BOOL CImportDATFx::PreImport(CGDAT &rDAT, bool fOverwrite)
{
	// 
	class Test : public CGDEntryMatch
	{
		// 
		set<UINT> setClass;
		// 
		static UINT ToKey(const CGDEntry &pos) { return MAKEWORD(pos.c1(), pos.c2()); }

	public:
		// 
		void Add(const CGDEntry &pos) { setClass.insert(ToKey(pos)); }

		// 
		list<CGDEntry> listMarked;

		// 
		virtual bool operator ()(const CGDEntry &pos)
		{
			if (setClass.find(ToKey(pos)) != setClass.end()) {
				listMarked.push_back(pos);
				return true;
			}
			return false;
		}

	};

	if (fOverwrite) {
		// Delete all possible dup entries from db
		for (; ; ) {
			m_res.ResAddSubject("Exclude collision entries from database");

			FxImpEntryMap::iterator
				iterPos = m_map.begin(),
				iterEnd = m_map.end();
			Test aTest;
			for (; iterPos != iterEnd; iterPos++) aTest.Add(iterPos->first);
			rDAT.DeleteEntries(aTest);

			for (; !aTest.listMarked.empty(); aTest.listMarked.pop_front()) m_res.ResAddKillEntry(aTest.listMarked.front(), true);
			return TRUE;
		}
	} else {
		// Delete all possible dup entries from import contents
		for (; ; ) {
			m_res.ResAddSubject("Exclude collision entries from import contents");

			list<CGDEntry> listKey;
			rDAT.Enum(listKey);
			Test aTest;
			list<CGDEntry>::iterator
				iterPos = listKey.begin(),
				iterEnd = listKey.end();
			for (; iterPos != iterEnd; iterPos++) aTest.Add(*iterPos);

			{
				FxImpEntryMap::iterator
					iterPos = m_map.begin(),
					iterEnd = m_map.end();
				while (iterPos != iterEnd) {
					if (aTest(iterPos->first)) {
						m_res.ResAddKillEntry(iterPos->first, false);
						iterPos = m_map.erase(iterPos);
					} else {
						iterPos++;
					}
				}
			}
			return TRUE;
		}
	}
}
