// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__9C5F7745_9B58_4280_8D88_B52F66EF3FBC__INCLUDED_)
#define AFX_STDAFX_H__9C5F7745_9B58_4280_8D88_B52F66EF3FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786)

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>
#include <map>
#include <afxadv.h>
#include <afxcview.h>
#include <afxole.h>
#include <set>
#include <list>
#include <afxsock.h>
#include <xutility>
#include <stack>

#define _SCB_REPLACE_MINIFRAME
#include "sizecbar.h"
#include "scbarg.h"
#include "scbarcf.h"
#define baseCMyBar CSizingControlBarCF

#if _MSC_VER <= 1200
#define _std_max _cpp_max
#define _std_min _cpp_min
#else
#define _std_max std::max
#define _std_min std::min
#endif

#include <atlbase.h>

using namespace std;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__9C5F7745_9B58_4280_8D88_B52F66EF3FBC__INCLUDED_)
