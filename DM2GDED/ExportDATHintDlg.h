#if !defined(AFX_EXPORTDATHINTDLG_H__DF530386_BF97_4910_B6FD_6E2EDE7FEA0A__INCLUDED_)
#define AFX_EXPORTDATHINTDLG_H__DF530386_BF97_4910_B6FD_6E2EDE7FEA0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDATHintDlg.h : �w�b�_�[ �t�@�C��
//

#include <set>
#include "GDAT.h"
#include "SizeVuff.h"
#include "ExportDATComment.h"

class CExportDATFx
{
	// 
	struct FxEntry {
		// 
		CString strType;
		// 
		CString strFileName;
		// 
		bool fIsRaw;
		// 
		LONG nVal;
		// 
		SizeBuff fv;
	};
	// 
	typedef map<CGDEntry, FxEntry> FxMap;

	// 
	FxMap m_map;

	// 
	void FormatEntryName(CString &strText, const CGDEntry &pos);
	// 
	bool ExportEntry(CGDAT &rDAT, const CGDEntry &pos, FxEntry &rEntry);
	// 
	bool ExportRawData(LPCTSTR pszFileInto, const SizeBuff &fv);
	// 
	void FormatEntryDisp(CString &strText, const CGDEntry &pos);

public:
	// 
	void Add4Export(const list<CGDEntry> &listKey);
	// 
	BOOL ExportTo(CGDAT &rDAT, CExportDATComment &rComment, LPCTSTR pszDir);

protected:
	// 
	virtual void OnExportHintStep(UINT nCur, UINT nMax) = NULL;
	// 
	virtual void OnExportHintText(LPCTSTR psz) = NULL;

};

/////////////////////////////////////////////////////////////////////////////
// CExportDATHintDlg �_�C�A���O

class CExportDATHintDlg : public CDialog, public CExportDATFx
{
public:
	// 
	CExportDATHintDlg(CWnd* pParent = NULL);
	// 
	virtual void OnExportHintStep(UINT nCur, UINT nMax);
	// 
	virtual void OnExportHintText(LPCTSTR psz);

	// 
	BOOL ExportTo(CGDAT &rDAT, CExportDATComment &rComment, LPCTSTR pszDir);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CExportDATHintDlg)
	enum { IDD = IDD_EXPORT_HINT };
	CString	m_strCurFile;
	CString	m_strCurPercent;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CExportDATHintDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CExportDATHintDlg)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EXPORTDATHINTDLG_H__DF530386_BF97_4910_B6FD_6E2EDE7FEA0A__INCLUDED_)
