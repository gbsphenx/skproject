// DM2OrnateTreeDic.cpp: CDM2OrnateTreeDic クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exp00069.h"
#include "DM2OrnateTreeDic.h"
#include "CSVrw.h"
#include <set>
#include "SplitStr.h"
#include <map>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;

CDM2OrnateTreeDic CDM2OrnateTreeDic::aInstance[2];

bool CDM2OrnateTreeDic::Load(LPCTSTR psz)
{
	Close();
	CSVR_t csvr;
	if (!csvr.Open(psz)) return false;
	for (int i = 0; i < 256; i++) {
		if (!csvr.ReadNextLine()) continue;
		if (false
			|| !csvr.ReadNextToken(m_str[i][0])
			|| !csvr.ReadNextToken(m_str[i][1])
		) {
			MkClassify();
			return false;
		}
	}
	MkClassify();
	return true;
}

void CDM2OrnateTreeDic::Close()
{
	for (int i = 0; i < 256; i++) for (int j = 0; j < 2; j++) m_str[i][j].Empty();

	m_saTree.RemoveAll();
}

void CDM2OrnateTreeDic::MkClassify()
{
	typedef set<CString> S0;
	S0 s0;
	for (int i = 0; i < 256; i++) {
		CStringList strLevels;
		SplitStr(m_str[i][1], ',', strLevels);
		for (; !strLevels.IsEmpty(); ) {
			CString strLevel = strLevels.RemoveHead();
			CStringList strTokens;
			SplitStr(strLevel, '/', strTokens);
			CString strMix;
			for (; !strTokens.IsEmpty(); ) {
				CString strToken = strTokens.RemoveHead();
				if (!strMix.IsEmpty()) strMix += '/';
				strMix += strToken;
				s0.insert(strMix);
			}
		}
	}
	m_saTree.SetSize(s0.size());
	S0::iterator
		iterPos = s0.begin(),
		iterEnd = s0.end();
	UINT x;
	for (x = 0; iterPos != iterEnd; x++, iterPos++) {
		CString s = (*iterPos);
		if (s.IsEmpty()) continue;
		m_saTree[x] = s;
	}
}

void CDM2OrnateTreeDic::FindNames(LPCSTR pszClass, CUIntArray &sa)
{
	typedef multimap<CString, BYTE> mm;
	mm m;
	UINT i;
	for (i = 0; i < 256; i++) {
		CStringList ss;
		SplitStr(m_str[i][1], ',', ss);
		while (!ss.IsEmpty()) {
			if (ss.RemoveHead() == pszClass) {
				m.insert(mm::value_type(m_str[i][0], i));
				break;
			}
		}
	}
	mm::iterator iter = m.begin();
	while (iter != m.end()) {
		sa.Add(iter->second);
		iter++;
	}
}
