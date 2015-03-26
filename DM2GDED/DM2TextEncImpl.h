
#ifndef _DM2TEXTENCIMPL_H_
#define _DM2TEXTENCIMPL_H_

#include "DM2TextEncoder.h"

typedef struct _DM2TextTbl
{	// 
	BYTE c;
	// 
	LPCSTR pszText;

}	_DM2TextTbl;

class CDM2TextSearchTri
{
	// 
	struct ELEM {
		// 
		WORD v;
		// 
		bool IsOk() { return v != (WORD)-1; }

		// 
		ELEM(): v(-1) { }
		// 
		explicit ELEM(WORD v): v(v) { }
	};
	// 
	struct TREE {
		// 
		ELEM v;
		// 
		TREE *p[16];

		// 
		TREE()
		{
			for (int i = 0; i < 16; i++) p[i] = NULL;
		}
		// 
		~TREE()
		{
			for (int i = 0; i < 16; i++) if (p[i]) delete p[i];
		}
	};

	// 
	TREE *m_pTree;

	// 
	bool Add(LPCSTR pszPos, WORD v) { return Add(pszPos, strchr(pszPos, 0), v); }
	// 
	bool Add(LPCSTR pszPos, LPCSTR pszEnd, WORD v);
	// 
	static TREE *WalkTree(TREE *pTree, BYTE c, bool fAdd);

public:
	// 
	CDM2TextSearchTri(LPCSTR *pEn0);
	// 
	CDM2TextSearchTri(LPCSTR *pJp0, LPCSTR *pJp1);
	// 
	~CDM2TextSearchTri();
	// 
	bool Match(LPCSTR &pszPos, LPCSTR pszEnd, UINT &v);
};

class CDM2TextEncoderBase : public CDM2TextEncoder
{
public:
	// 
	struct WIo {
		// 
		virtual bool WriteByte(BYTE x) = NULL;
		// 
		virtual bool WriteString(const char *psz) = NULL;
	};
	// 
	struct MemWIo : WIo {
		// 
		MemWIo(BYTE *pbBegin, BYTE *pbEnd): pbBegin(pbBegin), pbPos(pbBegin), pbEnd(pbEnd) { }
		// 
		virtual bool WriteByte(BYTE x)
		{
			if (pbPos < pbEnd) {
				*pbPos = x; pbPos++;
				return true;
			}
			return false;
		}
		// 
		virtual bool WriteString(const char *psz)
		{
			int n = strlen(psz);
			if (pbPos + n <= pbEnd) {
				memcpy(pbPos, psz, n);
				pbPos += n;
				return true;
			}
			return false;
		}
		// 
		UINT GetPosition() { return pbPos - pbBegin; }

	private:
		// 
		BYTE *pbBegin, *pbPos, *pbEnd;
	};
	// 
	struct TEncBase {
		// 
		virtual BYTE Encode(BYTE x, bool fStep) = NULL;
	};
	// 
	struct TEncNo : TEncBase {
		// 
		virtual BYTE Encode(BYTE x, bool fStep) { return x; }
	};
	// 
	struct TEncDM2 : TEncBase {
		// 
		TEncDM2(): iChr(0) { }
		// 
		virtual BYTE Encode(BYTE x, bool fStep)
		{
			x = (x ^ 0xFF) - iChr;
			if (fStep) iChr++;
			return x;
		}

	private:
		// 
		BYTE iChr;
	};
	// 
	struct TDecBase {
		// 
		virtual BYTE Decode(BYTE x, bool fStep) = NULL;
	};
	// 
	struct TDecNo : TDecBase {
		// 
		virtual BYTE Decode(BYTE x, bool fStep) { return x; }
	};
	// 
	struct TDecDM2 : TDecBase {
		// 
		TDecDM2(): iChr(0) { }
		// 
		virtual BYTE Decode(BYTE x, bool fStep)
		{
			x = (x + iChr) ^ 0xFF;
			if (fStep) iChr++;
			return x;
		}

	private:
		// 
		BYTE iChr;
	};

	// 
	bool Encode(LPCSTR pszPos, LPCSTR pszEnd, WIo &wio, TEncBase &rTE);
	// 
	bool Encode(LPCSTR pszPos, LPCSTR pszEnd, SizeBuff &fv, TEncBase &rTE);
	// 
	bool Decode(LPCSTR pszPos, LPCSTR pszEnd, WIo &wio, TDecBase &rTD);
	// 
	bool Decode(LPCSTR pszPos, LPCSTR pszEnd, CString &strText, TDecBase &rTD);

	// 
	virtual CDM2TextSearchTri &GetSearchTri() = NULL;
	// 
	virtual LPCSTR *GetDecodeTbl1() = NULL;
	// 
	virtual LPCSTR *GetDecodeTbl2() = NULL;
};

class CDM2TextEncoderEn : public CDM2TextEncoderBase
{
	// 
	bool m_fPlain;

public:
	// 
	CDM2TextEncoderEn(bool fPlain);

	// 
	virtual bool Encode(LPCSTR pszPos, LPCSTR pszEnd, SizeBuff &fv);
	// 
	virtual bool Decode(LPCSTR pszPos, LPCSTR pszEnd, CString &strText);

protected:
	// 
	virtual CDM2TextSearchTri &GetSearchTri();
	// 
	virtual LPCSTR *GetDecodeTbl1();
	// 
	virtual LPCSTR *GetDecodeTbl2();

};

class CDM2TextEncoderJa : public CDM2TextEncoderBase
{
public:
	// 
	CDM2TextEncoderJa();

	// 
	virtual bool Encode(LPCSTR pszPos, LPCSTR pszEnd, SizeBuff &fv);
	// 
	virtual bool Decode(LPCSTR pszPos, LPCSTR pszEnd, CString &strText);

protected:
	// 
	virtual CDM2TextSearchTri &GetSearchTri();
	// 
	virtual LPCSTR *GetDecodeTbl1();
	// 
	virtual LPCSTR *GetDecodeTbl2();

};

class CDM2TextEncoderJa_PSV : public CDM2TextEncoderJa
{
protected:
	// 
	virtual LPCSTR *GetDecodeTbl1();
	// 
	virtual LPCSTR *GetDecodeTbl2();

};

#endif // #ifndef _DM2TEXTENCIMPL_H_
