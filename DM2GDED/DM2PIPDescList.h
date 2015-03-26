// DM2PIPDescList.h: CDM2PIPDescList クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2PIPDESCLIST_H__961D9DF9_AF3D_4A98_A9CF_938FAA4E5902__INCLUDED_)
#define AFX_DM2PIPDESCLIST_H__961D9DF9_AF3D_4A98_A9CF_938FAA4E5902__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DM2BSwDomain.h"
#include "GDEntry.h"

typedef enum
{	piptsNo,
	piptsScene,
	piptsColorPal,
	piptsIf,
}	CDM2PIPTS;

class CDM2PIPDesc
{
public:

	// 
	CDM2PIPTS m_ts;
	// 
	int m_nScene;
	// 
	CGDEntry m_pos;
	// 
	int m_nColorKey;
	// 
	bool m_fDepend;
	// 
	int m_nColorKey2;

	// 
	CDM2PIPDesc()
	{
		m_ts = piptsNo;
		m_nScene = -1;
		m_pos = CGDEntry::ERR;
		m_nColorKey = -1;
		m_fDepend = false;
		m_nColorKey2 = -1;
	}
	// 
	bool Parse(CString *(pstr[]));
	// 
	bool Match(CGDEntry pos) const;

private:
	// 
	CDM2BSwDomain m_aDomain[3];

	// 
	bool ParseTS(LPCSTR psz);
};

class CDM2PIPDescList  
{
	// 
	typedef list<CDM2PIPDesc> List;
	// 
	List m_list;

public:
	// 
	void RemoveAll();
	// 
	bool LoadFrom(LPCTSTR pszFileIn);
	// 
	const CDM2PIPDesc *FindFirstMatch(const CGDEntry &pos) const;
};

#endif // !defined(AFX_DM2PIPDESCLIST_H__961D9DF9_AF3D_4A98_A9CF_938FAA4E5902__INCLUDED_)
