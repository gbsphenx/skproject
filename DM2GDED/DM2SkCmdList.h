// DM2SkCmdList.h: CDM2SkCmdList クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2SKCMDLIST_H__8BE2C84E_7C84_45EA_B4BC_A20B6EA90DF9__INCLUDED_)
#define AFX_DM2SKCMDLIST_H__8BE2C84E_7C84_45EA_B4BC_A20B6EA90DF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DM2SKCMD_MAX_CMD 17

enum {
	skcCM,
	skcSK,
	skcLV,
	skcBZ,
	skcTR,
	skcTA,
	skcEX,
	skcPB,
	skcDM,
	//
	skcAT,
	skcHN,
	skcNC,
	skcRP,
	//
	skcPA,
	skcSD,
	skcST,
	skcWH,
};

struct CDM2SkCM {
	// 
	int i;
	// 
	LPCSTR pszName;
	// 
	int iIcoRes;
};

class CDM2SkCMUtil
{
public:
	// 
	static LPCSTR GetNameOfId(int i);
};
extern const CDM2SkCM g_DM2SkCM[];

class CDM2SkPAUtil
{
public:
	// 
	static LPCSTR GetNameOfId(int i);
};
extern const CDM2SkCM g_DM2SkPA[];

class CDM2SkCmdList
{
	// 
	static bool ReadInt(LPCSTR &pszPos, LPCSTR pszEnd, int &x);

public:
	// 
	CString strName;
	// 
	int nVal[DM2SKCMD_MAX_CMD];

	// 
	CDM2SkCmdList();

	// 
	static LPCSTR GetName(UINT i);
	// 
	static int LookUpName(LPCSTR pszPos, LPCSTR pszEnd);

	// 
	bool Parse(LPCSTR pszPos);
	// 
	CString Mk();
};

class CDM2SkCmdDesc
{
	// 
	CString strDesc[DM2SKCMD_MAX_CMD];

public:
	// 
	void RemoveAll();
	// 
	void LoadFrom(LPCTSTR pszFileIn);
	// 
	LPCTSTR GetDesc(UINT i) const;

};

#endif // !defined(AFX_DM2SKCMDLIST_H__8BE2C84E_7C84_45EA_B4BC_A20B6EA90DF9__INCLUDED_)
