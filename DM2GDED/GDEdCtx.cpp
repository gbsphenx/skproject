// GDEdCtx.cpp: CGDEdCtx クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DM2GDED.h"
#include "GDEdCtx.h"
#include "SplitStr.h"
#include "OSP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace OSP;

namespace
{

CString TestAndJoinPath(CString strBase, CString strAdd)
{
	if (strAdd.Left(2).Compare(".\\") == 0)
		return OSP_JoinPath(strBase, strAdd);
	return strAdd;
}

};

//////////////////////////////////////////////////////////////////////
// CGDEdOpt

void CGDEdOpt::LoadFrom(LPCTSTR pszFile)
{
	CWinApp *pApp = GetApp();

	m_wvct = GetProfileIntFrom("DM2GDED", "word_value_contents_type", 0, pszFile);
	m_inlang = GetProfileIntFrom("DM2GDED", "item_name_lang", 0, pszFile);
	m_cnlang = GetProfileIntFrom("DM2GDED", "creature_name_lang", 0, pszFile);
}

void CGDEdOpt::SaveTo(LPCTSTR pszFile)
{
	SaveProfileIntTo("DM2GDED", "word_value_contents_type", m_wvct, pszFile);
	SaveProfileIntTo("DM2GDED", "item_name_lang", m_inlang, pszFile);
	SaveProfileIntTo("DM2GDED", "creature_name_lang", m_cnlang, pszFile);
}

//////////////////////////////////////////////////////////////////////
// CGDEdCtxInCB

void CGDEdCtxInCB::AddCB(CGDEdCtxCB *pSink)
{
	arrSink.Add(pSink);
}

void CGDEdCtxInCB::RemoveCB(CGDEdCtxCB *pSink)
{
	for (int i = 0, n = arrSink.GetSize(); i < n; i++) {
		if (arrSink[i] == pSink) {
			arrSink.RemoveAt(i), i--, n--;
		}
	}
}

void CGDEdCtxInCB::DispatchGDUpdate(UINT nHint, void *pHint)
{
	for (int i = 0, n = arrSink.GetSize(); i < n; i++) {
		arrSink[i]->OnGDUpdate(nHint, pHint);
	}
}

//////////////////////////////////////////////////////////////////////
// CGDEdCtx

#pragma warning(push)
#pragma warning(disable: 4355)

CGDEdCtx::CGDEdCtx()
: m_vxxxfactory(this)
{
	LoadFrom(GetCnfFilePath());
}

#pragma warning(pop)

CGDEdCtx::~CGDEdCtx()
{
	m_opts.SaveTo(GetCnfFilePath());
}

void CGDEdCtx::LoadFrom(LPCTSTR pszFileIn)
{
	m_opts.LoadFrom(pszFileIn);

	LoadDescFrom(pszFileIn);
}

void CGDEdCtx::LoadDescFrom(LPCTSTR pszFileIn)
{
	CString strDescFiles;

	m_dctx.RemoveAll();
	if (LoadProfileSectionFrom("DM2GDED.Desc", strDescFiles, pszFileIn)) {
		strDescFiles.Replace("\r\n", "\n");
		strDescFiles.Replace("\r", "\n");
		list<CString> listDescFile;
		SplitStr(strDescFiles, CString(_T("\n")), listDescFile);

		while (!listDescFile.empty()) {
			m_dctx.LoadFrom(m_vxxxfactory, TestAndJoinPath(OSP_GetDir(OSP_GetModuleFileName()), listDescFile.front()));

			listDescFile.pop_front();
		}
	}

	GetGDAT().LoadToPIPCtxFrom(NULL, true);
	if (LoadProfileSectionFrom("DM2GDED.PalDesc", strDescFiles, pszFileIn)) {
		strDescFiles.Replace("\r\n", "\n");
		strDescFiles.Replace("\r", "\n");
		list<CString> listDescFile;
		SplitStr(strDescFiles, CString(_T("\n")), listDescFile);

		while (!listDescFile.empty()) {
			GetGDAT().LoadToPIPCtxFrom(TestAndJoinPath(OSP_GetDir(OSP_GetModuleFileName()), listDescFile.front()), false);

			listDescFile.pop_front();
		}
	}

	CString strSKTextFile;
	m_scdesc.RemoveAll();
	if (LoadProfileStringFrom("DM2GDED", "command_text_desc_ini", strSKTextFile, "", pszFileIn)) {
		m_scdesc.LoadFrom(TestAndJoinPath(OSP_GetDir(OSP_GetModuleFileName()), strSKTextFile));
	}
}
