// DM2OrnateTreeDic.h: CDM2OrnateTreeDic クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2ORNATETREEDIC_H__AAA0A8B4_D6BE_4DEB_B460_789655E79EEE__INCLUDED_)
#define AFX_DM2ORNATETREEDIC_H__AAA0A8B4_D6BE_4DEB_B460_789655E79EEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDM2OrnateTreeDic  
{
	// 
	CString m_str[256][2];
	// 
	CStringArray m_saTree;

	// 
	static CDM2OrnateTreeDic aInstance[2];

	// 
	void MkClassify();

public:
	// 
	static CDM2OrnateTreeDic &Get1(BYTE i) { return aInstance[i & 1]; }

	// 
	bool Load(LPCTSTR psz);
	// 
	void Close();
	// 
	LPCSTR GetName(BYTE i) { return m_str[i][0]; }
	// 
	const CStringArray &GetTree() { return m_saTree; }
	// 
	void FindNames(LPCSTR pszClass, CUIntArray &sa);

};

#endif // !defined(AFX_DM2ORNATETREEDIC_H__AAA0A8B4_D6BE_4DEB_B460_789655E79EEE__INCLUDED_)
