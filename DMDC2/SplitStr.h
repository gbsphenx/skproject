
#ifndef _SPLITSTR_H_
#define _SPLITSTR_H_

#include <afx.h>
#include <list>

extern
void SplitStr(CString tstr, TCHAR tc, CStringList &l);
extern
void SplitStr(CString tstr, TCHAR tc, CStringArray &l);
extern
void SplitStr(CString s, CString sToken, std::list<CString> &l);

template<typename T> void SplitStr(T wstr, T wstrDiv, std::list<T> &l)
{
	size_t p = 0, n, m = wstrDiv.size();
	while (true) {
		if ((n = wstr.find(wstrDiv, p)) == wstr.npos) {
			l.push_back(wstr.substr(p));
			break;
		} else {
			l.push_back(wstr.substr(p, n - p));
			p = n + m;
		}
	}
}

#endif // #ifndef _SPLITSTR_H_
