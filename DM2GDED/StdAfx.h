// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#pragma warning(disable: 4786)

#if !defined(AFX_STDAFX_H__FE58B589_9F98_4084_8561_4772AFC3454E__INCLUDED_)
#define AFX_STDAFX_H__FE58B589_9F98_4084_8561_4772AFC3454E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#include <afxcview.h>
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxadv.h>
#include <afxole.h>

#define _SCB_REPLACE_MINIFRAME
#include "sizecbar.h"
#include "scbarg.h"
#include "scbarcf.h"
#define baseCMyBar CSizingControlBarCF

#include <list>
#include <map>
#include <mmsystem.h>
#include <mmreg.h>
#include <msacm.h>
#include <algorithm>
//#include <shobjidl.h>
#include <comdef.h>
#include <set>
#include <io.h>

#include "HyperLink.h"

#define GRIDCONTROL_NO_TITLETIPS
#define GRIDCONTROL_NO_DRAGDROP
//#define GRIDCONTROL_NO_CLIPBOARD
#define GRIDCONTROL_NO_PRINTING

using namespace std;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__FE58B589_9F98_4084_8561_4772AFC3454E__INCLUDED_)
