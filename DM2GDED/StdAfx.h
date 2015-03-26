// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#pragma warning(disable: 4786)

#if !defined(AFX_STDAFX_H__FE58B589_9F98_4084_8561_4772AFC3454E__INCLUDED_)
#define AFX_STDAFX_H__FE58B589_9F98_4084_8561_4772AFC3454E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxcview.h>
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
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
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__FE58B589_9F98_4084_8561_4772AFC3454E__INCLUDED_)
