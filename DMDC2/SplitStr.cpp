
#include "StdAfx.h"
#include "SplitStr.h"

using namespace std;

void SplitStr(CString tstr, TCHAR tc, CStringList &l)
{
	int f = 0;
	for (; ; ) {
		int t = tstr.Find(tc, f);
		if (t < 0) {
			l.AddTail((LPCTSTR)tstr + f);
			return;
		} else {
			l.AddTail(tstr.Mid(f, t - f));
			f = t + 1;
		}
	}
}

void SplitStr(CString tstr, TCHAR tc, CStringArray &l)
{
	int f = 0;
	for (; ; ) {
		int t = tstr.Find(tc, f);
		if (t < 0) {
			l.Add((LPCTSTR)tstr + f);
			return;
		} else {
			l.Add(tstr.Mid(f, t - f));
			f = t + 1;
		}
	}
}

void SplitStr(CString s, CString sToken, list<CString> &l)
{
	int v = 0, w;
	for (; ; ) {
		w = s.Find(sToken, v);
		if (w < 0) {
			l.push_back((LPCTSTR)s + v);
			return;
		} else {
			l.push_back(s.Mid(v, w - v));
			v = w + sToken.GetLength();
		}
	}
}
