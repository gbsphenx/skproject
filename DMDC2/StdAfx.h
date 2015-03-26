// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__9C5F7745_9B58_4280_8D88_B52F66EF3FBC__INCLUDED_)
#define AFX_STDAFX_H__9C5F7745_9B58_4280_8D88_B52F66EF3FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786)

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
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
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__9C5F7745_9B58_4280_8D88_B52F66EF3FBC__INCLUDED_)
