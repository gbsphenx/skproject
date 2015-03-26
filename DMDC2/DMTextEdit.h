
#pragma once

#include "SizeVuff.h"

enum {
	DMtcA	=  0,
	DMtcZ	= 25,
	DMtcSPC	= 26,
	DMtcDot	= 27,
	DMtcLF	= 28,
	DMtcESC1= 29,
	DMtcESC2= 30,
	DMtcEND	= 31,
};


class DMTextEdit_t
{
public:
	// 
	CByteArray vec;

	// 
	void Load(const SizeBuff &fvTextData);
	// 
	bool Save(SizeBuff &fvTextData);
	// 
	int AddStr(LPCSTR psz);
	// 
	CString GetStr(UINT x);
	// 
	void EnumKey(CWordArray &vec);

};

class DMTextEnc_t
{
public:
	// 
	BYTE *pDst;
	// 
	UINT xDst, cxDst;
	// 
	const BYTE *pSrc;
	// 
	UINT xSrc, cxSrc;
	// 
	bool capeof, accepteof;

	// 
	DMTextEnc_t() {
		capeof = true;
		accepteof = true;
	}
	// 
	bool Encode();
	// 
	bool Decode();
};
