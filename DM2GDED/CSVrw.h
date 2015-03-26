// CSVrw.h: CSVW_t クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSVRW_H__97F4030D_6DBA_40C7_A197_42BCE21CB53E__INCLUDED_)
#define AFX_CSVRW_H__97F4030D_6DBA_40C7_A197_42BCE21CB53E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SizeVuff.h"

class CSVR_t
{
	// 
	SizeBuff fv;
	// 
	LPCSTR pszPos;
	// 
	LPCSTR pszEnd;
	// 
	LPCSTR pszLnPos, pszLnEnd;
	// 
	int iColumn;

	// 
	static bool readAsMark1(LPCSTR &pszPos, LPCSTR pszEnd)
	{
		if (pszPos < pszEnd && *pszPos == ',') {
			pszPos++;
			return true;
		}
		return false;
	}
	// 
	static bool readAsMark2(LPCSTR &pszPos, LPCSTR pszEnd)
	{
		if (pszPos + 1 < pszEnd && *(pszPos+0) == '\r' && *(pszPos+1) == '\n') {
			pszPos += 2;
			return true;
		}
		if (pszPos < pszEnd && (*(pszPos+0) == '\n' || *(pszPos+0) == '\r')) {
			pszPos++;
			return true;
		}
		return false;
	}
	// 
	static bool readAsMark3(LPCSTR &pszPos, LPCTSTR pszEnd)
	{
		if (pszPos < pszEnd && *pszPos == '\"') {
			pszPos++;
			return true;
		}
		return false;
	}
	// 
	static bool isMark1(LPCSTR pszPos, LPCSTR pszEnd)
	{
		return readAsMark1(pszPos, pszEnd);
	}
	// 
	static bool isMark2(LPCSTR pszPos, LPCSTR pszEnd)
	{
		return readAsMark2(pszPos, pszEnd);
	}
	// 
	static bool isMark3(LPCSTR pszPos, LPCSTR pszEnd)
	{
		return readAsMark3(pszPos, pszEnd);
	}
	// 
	BOOL read1Token(LPCSTR &pszPos, LPCSTR pszEnd, CString &sVal)
	{
		sVal.Empty();
		if (pszPos == pszEnd) return FALSE;
		if (isMark2(pszPos, pszEnd)) return FALSE;
		bool fEnclosed = readAsMark3(pszPos, pszEnd);
		if (!fEnclosed) {

		} else {
			while (pszPos < pszEnd) {
				if (readAsMark3(pszPos, pszEnd)) {
					LPCSTR pszNext1 = pszPos;
					if (readAsMark3(pszPos, pszEnd)) {
						for (; pszNext1 < pszPos; pszNext1++) {
							sVal += *pszNext1;
						}
					} else {
						break;
					}
				} else {
					LPCSTR pszNext = _tcsinc(pszPos);
					for (; pszPos < pszNext; pszPos++) {
						sVal += *pszPos;
					}
				}
			}
		}
		while (pszPos < pszEnd) {
			if (isMark2(pszPos, pszEnd)) {
				break;
			}
			if (readAsMark1(pszPos, pszEnd)) {
				break;
			}
			LPCSTR pszNext = _tcsinc(pszPos);
			for (; pszPos < pszNext; pszPos++) {
				sVal += *pszPos;
			}
		}
		return TRUE;
	}
public:
	// 
	CSVR_t(): pszPos(NULL), pszEnd(NULL), iColumn(0)
	{

	}
	// 
	~CSVR_t()
	{

	}

	// 
	void Close()
	{
		fv.Free();
		pszPos = pszEnd = NULL;
	}
	// 
	BOOL Open(LPCTSTR pszFileIn)
	{
		Close();
		FILE *f = _tfopen(pszFileIn, "rb");
		if (f != NULL) {
			fseek(f, 0, SEEK_END);
			int nLen = ftell(f);
			fseek(f, 0, SEEK_SET);
			if (nLen > 0) {
				if (fv.Alloc(nLen) && fread(fv.GetData(), nLen, 1, f) == 1) {
					fclose(f);
					pszLnPos = pszLnEnd = pszPos = (LPCSTR)fv.GetData();
					pszEnd = pszPos + nLen;
					return TRUE;
				}
			}
		}
		if (f) fclose(f);
		return FALSE;
	}
	// 
	BOOL ReadNextLine()
	{
		CString sVal;
		readAsMark2(pszLnEnd, pszEnd);
		pszLnPos = pszLnEnd;
		while (pszLnEnd < pszEnd) {
			if (!read1Token(pszLnEnd, pszEnd, sVal)) break;
		}
		if (pszLnPos == pszLnEnd)
			return FALSE;
		return TRUE;
	}
	// 
	BOOL ReadNextToken(CString &sRet)
	{
		return read1Token(pszLnPos, pszLnEnd, sRet);
	}
};

class CSVW_t  
{
	// 
	FILE *f;
	// 
	int iColumn;
public:
	// 
	CSVW_t(): f(NULL)
	{

	}
	// 
	~CSVW_t()
	{
		Close();
	}

	// 
	void Close()
	{
		if (this->f)
			fclose(this->f),
			this->f = NULL;
	}
	// 
	BOOL Open(LPCTSTR pszFileIn)
	{
		Close();
		if ((this->f = _tfopen(pszFileIn, "w+b")) == NULL)
			return FALSE;
		iColumn = 0;
		return TRUE;
	}
	// 
	void WriteToken(LPCSTR pszText)
	{
		LPCSTR pszPos = pszText;
		LPCSTR pszEnd = _tcschr(pszPos, 0);
		if (iColumn != 0) fputs(",", this->f);
		fputs("\"", this->f);
		while (pszPos < pszEnd) {
			TCHAR tc = _tcsnextc(pszPos);
			UINT n = _tcsinc(pszPos) - pszPos;
			switch (tc) {
			case '\"':
				fputs("\"\"", this->f);
				break;
			default:
				fwrite(pszPos, n, 1, this->f);
				break;
			}
			pszPos = _tcsinc(pszPos);
		}
		fputs("\"", this->f);

		iColumn++;
	}
	// 
	BOOL WriteNextLine()
	{
		if (fwrite("\r\n", 2, 1, this->f) != 1)
			return FALSE;
		iColumn = 0;
		return TRUE;
	}
};

#endif // !defined(AFX_CSVRW_H__97F4030D_6DBA_40C7_A197_42BCE21CB53E__INCLUDED_)
