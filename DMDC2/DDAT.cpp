// DDAT.cpp: CDDAT クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exp00069.h"
#include "DDAT.h"
#include "DM2NameDic.h"

#include "Str2Clipbrd.h"
#include "DMRecFmt.h"
#include "SizeVuff.h"
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;

#define DDATUC_DELETE_RECDB	1U
//#define DDATUC_DELETE_UNASSOCSET	2U
#define DDATUC_SET_DBREC			3U
//#define DDATUC_INSERT_UNASSOCSET	4U
#define DDATUC_SET_ATLAS_BIT		5U
#define DDATUC_SET_ATLAS			6U
#define DDATUC_SET_LEVEL_DESC		7U
#define DDATUC_DELETE_MAP			8U
#define DDATUC_INSERT_MAP			9U
#define DDATUC_SET_ALL_MAP_DESC		10U
#define DDATUC_SAVE_MASK_OCCUPIED	11U

#define DDATAF_NEXT	1U
#define DDATAF_STOLEN	2U

#define UIF_NO				0x00000000
#define UIF_UNDO_RUNNING	0x00000001

DBIPos_t DBIPos_t::NIL = DBIPos_t::From(0xffff);
DBIPos_t DBIPos_t::END = DBIPos_t::From(0xfffe);

RN RN_NIL = RNco(0xffff);
RN RN_END = RNco(0xfffe);
RN RNff80 = RNco(0xff80);

//	CArchive &readByteArray(CArchive &ar, CByteArray &s)
//	{
//		DWORD n;
//		ar >> n;
//		s.SetSize(n);
//		if (ar.Read(s.GetData(), n) != n)
//			AfxThrowArchiveException(CArchiveException::endOfFile);
//		return ar;
//	}

//	CArchive &writeByteArray(CArchive &ar, const CByteArray &s)
//	{
//		ar << s.GetSize();
//		ar.Write(s.GetData(), s.GetSize());
//		return ar;
//	}

//	CArchive &readWordArray(CArchive &ar, CWordArray &s)
//	{
//		DWORD n;
//		ar >> n;
//		s.SetSize((n + 1) / sizeof(WORD));
//		DWORD cb = n;
//		if (ar.Read(s.GetData(), cb) != cb)
//			AfxThrowArchiveException(CArchiveException::endOfFile);
//		return ar;
//	}

//	CArchive &writeWordArray(CArchive &ar, const CWordArray &s)
//	{
//		DWORD cb = sizeof(WORD) * s.GetSize();
//		ar << cb;
//		ar.Write(s.GetData(), cb);
//		return ar;
//	}

//	CArchive &writeByteArrayAsIs(CArchive &ar, const CByteArray &s)
//	{
//		ar.Write(s.GetData(), s.GetSize());
//		return ar;
//	}

bool IsThisAsIs(const RN &rn)
{
	if (rn == RN_NIL || rn == RN_END || rn == RNff80)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////
// 

namespace
{
	// 
	bool fnReadRec0(CDDATx::CSerializedReader &fLoad, DMDBU &rec)
	{
		WORD w0 = fLoad.ReadWord();
		WORD w2 = fLoad.ReadWord();
		rec.r0.next = RNco((WORD)w0);
		rec.r0.w2 = w2;
		return !fLoad.HasError();
	}
	// 
	bool fnReadRec1(CDDATx::CSerializedReader &fLoad, DMDBU &rec)
	{
		WORD w0 = fLoad.ReadWord();
		WORD w2 = fLoad.ReadWord();
		WORD w4 = fLoad.ReadWord();
		rec.r1.next = RNco((WORD)w0);
		rec.r1.w2 = w2;
		rec.r1.w4 = w4;
		return !fLoad.HasError();
	}
	// 
	bool fnReadRec3(CDDATx::CSerializedReader &fLoad, DMDBU &rec)
	{
		WORD w0 = fLoad.ReadWord();
		WORD w2 = fLoad.ReadWord();
		WORD w4 = fLoad.ReadWord();
		WORD w6 = fLoad.ReadWord();
		rec.r3.next = RNco((WORD)w0);
		rec.r3.w2 = w2;
		rec.r3.w4 = w4;
		rec.r3.w6 = w6;
		return !fLoad.HasError();
	}
	// 
	bool fnReadRec4(CDDATx::CSerializedReader &fLoad, DMDBU &rec)
	{
		WORD w0 = fLoad.ReadWord();
		WORD w2 = fLoad.ReadWord();
		BYTE b4 = fLoad.ReadByte();
		BYTE b5 = fLoad.ReadByte();
		WORD w6[4];
		w6[0] = fLoad.ReadWord();
		w6[1] = fLoad.ReadWord();
		w6[2] = fLoad.ReadWord();
		w6[3] = fLoad.ReadWord();
		WORD w14 = fLoad.ReadWord();

		rec.r4.next = RNco((WORD)w0);
		rec.r4.stolen = RNco((WORD)w2);
		rec.r4.b4 = b4;
		rec.r4.b5 = b5;
		rec.r4.w6[0] = w6[0];
		rec.r4.w6[1] = w6[1];
		rec.r4.w6[2] = w6[2];
		rec.r4.w6[3] = w6[3];
		rec.r4.w14 = w14;
		return !fLoad.HasError();
	}
	// 
	bool fnReadRec4xmap(CDDATx::CSerializedReader &fLoad, DMDBU &rec)
	{
		WORD w0 = fLoad.ReadWord();
		WORD w2 = fLoad.ReadWord();
		BYTE b4 = fLoad.ReadByte();
		BYTE b5 = fLoad.ReadByte();
		WORD w6[4];
		w6[0] = fLoad.ReadWord();
		w6[1] = fLoad.ReadWord();
		w6[2] = fLoad.ReadWord();
		w6[3] = fLoad.ReadWord();
		WORD w14 = fLoad.ReadWord();
		WORD w16 = fLoad.ReadWord();
		WORD w18 = fLoad.ReadWord();

		rec.r4.next = RNco((WORD)w0);
		rec.r4.stolen = RNco((WORD)w2);
		rec.r4.b4 = b4;
		rec.r4.b5 = b5;
		rec.r4.w6[0] = w6[0];
		rec.r4.w6[1] = w6[1];
		rec.r4.w6[2] = w6[2];
		rec.r4.w6[3] = w6[3];
		rec.r4.w14 = w14;
		rec.r4xmap.w16 = w16;
		rec.r4xmap.w18 = w18;
		return !fLoad.HasError();
	}
	// 
	bool fnReadRec9(CDDATx::CSerializedReader &fLoad, DMDBU &rec)
	{
		WORD w0 = fLoad.ReadWord();
		WORD w2 = fLoad.ReadWord();
		WORD w4 = fLoad.ReadWord();
		BYTE b6[2];
		b6[0] = fLoad.ReadByte();
		b6[1] = fLoad.ReadByte();

		rec.r9.next = RNco((WORD)w0);
		rec.r9.stolen = RNco((WORD)w2);
		rec.r9.w4 = w4;
		rec.r9.b6[0] = b6[0];
		rec.r9.b6[1] = b6[1];
		return !fLoad.HasError();
	}
	// 
	bool fnReadRec9xmap(CDDATx::CSerializedReader &fLoad, DMDBU &rec)
	{
		WORD w0 = fLoad.ReadWord();
		WORD w2 = fLoad.ReadWord();
		WORD w4 = fLoad.ReadWord();
		BYTE b6[2];
		b6[0] = fLoad.ReadByte();
		b6[1] = fLoad.ReadByte();
		WORD w8 = fLoad.ReadWord();
		WORD w10 = fLoad.ReadWord();

		rec.r9.next = RNco((WORD)w0);
		rec.r9.stolen = RNco((WORD)w2);
		rec.r9.w4 = w4;
		rec.r9.b6[0] = b6[0];
		rec.r9.b6[1] = b6[1];
		rec.r9xmap.w8 = w8;
		rec.r9xmap.w10 = w10;
		return !fLoad.HasError();
	}
	// 
	bool fnReadRecB(CDDATx::CSerializedReader &fLoad, DMDBU &rec)
	{
		return false;
	}
	// 
	bool fnReadRecE(CDDATx::CSerializedReader &fLoad, DMDBU &rec)
	{
		WORD w0 = fLoad.ReadWord();
		WORD w2 = fLoad.ReadWord();
		BYTE b4[2];
		b4[0] = fLoad.ReadByte();
		b4[1] = fLoad.ReadByte();
		WORD w6 = fLoad.ReadWord();

		rec.r14.next = RNco((WORD)w0);
		rec.r14.stolen = RNco((WORD)w2);
		rec.r14.b4[0] = b4[0];
		rec.r14.b4[1] = b4[1];
		rec.r14.w6 = w6;
		return !fLoad.HasError();
	}
	// 
	bool fnReadRecExmap(CDDATx::CSerializedReader &fLoad, DMDBU &rec)
	{
		WORD w0 = fLoad.ReadWord();
		WORD w2 = fLoad.ReadWord();
		BYTE b4[2];
		b4[0] = fLoad.ReadByte();
		b4[1] = fLoad.ReadByte();
		WORD w6 = fLoad.ReadWord();
		WORD w8 = fLoad.ReadWord();
		WORD w10 = fLoad.ReadWord();

		rec.r14.next = RNco((WORD)w0);
		rec.r14.stolen = RNco((WORD)w2);
		rec.r14.b4[0] = b4[0];
		rec.r14.b4[1] = b4[1];
		rec.r14.w6 = w6;
		rec.r14xmap.w8 = w8;
		rec.r14xmap.w10 = w10;
		return !fLoad.HasError();
	}
};

namespace
{
	// 
	typedef bool (*fnWriteRec)(CDDATx::CSerializedWriter &fSave, const DMDBU &rec);
	// 
	bool fnWriteRec0(CDDATx::CSerializedWriter &fSave, const DMDBU &rec)
	{
		if (false
			|| !fSave.WriteWord(rec.r0.next.r.w)
			|| !fSave.WriteWord(rec.r0.w2)
		) {
			return false;
		}
		return true;
	}
	// 
	bool fnWriteRec1(CDDATx::CSerializedWriter &fSave, const DMDBU &rec)
	{
		if (false
			|| !fSave.WriteWord(rec.r1.next.r.w)
			|| !fSave.WriteWord(rec.r1.w2)
			|| !fSave.WriteWord(rec.r1.w4)
		) {
			return false;
		}
		return true;
	}
	// 
	bool fnWriteRec3(CDDATx::CSerializedWriter &fSave, const DMDBU &rec)
	{
		if (false
			|| !fSave.WriteWord(rec.r3.next.r.w)
			|| !fSave.WriteWord(rec.r3.w2)
			|| !fSave.WriteWord(rec.r3.w4)
			|| !fSave.WriteWord(rec.r3.w6)
		) {
			return false;
		}
		return true;
	}
	// 
	bool fnWriteRec4(CDDATx::CSerializedWriter &fSave, const DMDBU &rec)
	{
		if (false
			|| !fSave.WriteWord(rec.r4.next.r.w)
			|| !fSave.WriteWord(rec.r4.stolen.r.w)
			|| !fSave.WriteByte(rec.r4.b4)
			|| !fSave.WriteByte(rec.r4.b5)
			|| !fSave.WriteWord(rec.r4.w6[0])
			|| !fSave.WriteWord(rec.r4.w6[1])
			|| !fSave.WriteWord(rec.r4.w6[2])
			|| !fSave.WriteWord(rec.r4.w6[3])
			|| !fSave.WriteWord(rec.r4.w14)
		) {
			return false;
		}
		return true;
	}
	// 
	bool fnWriteRec4xmap(CDDATx::CSerializedWriter &fSave, const DMDBU &rec)
	{
		if (false
			|| !fSave.WriteWord(rec.r4.next.r.w)
			|| !fSave.WriteWord(rec.r4.stolen.r.w)
			|| !fSave.WriteByte(rec.r4.b4)
			|| !fSave.WriteByte(rec.r4.b5)
			|| !fSave.WriteWord(rec.r4.w6[0])
			|| !fSave.WriteWord(rec.r4.w6[1])
			|| !fSave.WriteWord(rec.r4.w6[2])
			|| !fSave.WriteWord(rec.r4.w6[3])
			|| !fSave.WriteWord(rec.r4.w14)
			|| !fSave.WriteWord(rec.r4xmap.w16)
			|| !fSave.WriteWord(rec.r4xmap.w18)
		) {
			return false;
		}
		return true;
	}
	// 
	bool fnWriteRec9(CDDATx::CSerializedWriter &fSave, const DMDBU &rec)
	{
		if (false
			|| !fSave.WriteWord(rec.r9.next.r.w)
			|| !fSave.WriteWord(rec.r9.stolen.r.w)
			|| !fSave.WriteWord(rec.r9.w4)
			|| !fSave.WriteByte(rec.r9.b6[0])
			|| !fSave.WriteByte(rec.r9.b6[1])
		) {
			return false;
		}
		return true;
	}
	// 
	bool fnWriteRec9xmap(CDDATx::CSerializedWriter &fSave, const DMDBU &rec)
	{
		if (false
			|| !fSave.WriteWord(rec.r9.next.r.w)
			|| !fSave.WriteWord(rec.r9.stolen.r.w)
			|| !fSave.WriteWord(rec.r9.w4)
			|| !fSave.WriteByte(rec.r9.b6[0])
			|| !fSave.WriteByte(rec.r9.b6[1])
			|| !fSave.WriteWord(rec.r9xmap.w8)
			|| !fSave.WriteWord(rec.r9xmap.w10)
		) {
			return false;
		}
		return true;
	}
	// 
	bool fnWriteRecB(CDDATx::CSerializedWriter &fSave, const DMDBU &rec)
	{
		return false;
	}
	// 
	bool fnWriteRecE(CDDATx::CSerializedWriter &fSave, const DMDBU &rec)
	{
		if (false
			|| !fSave.WriteWord(rec.r14.next.r.w)
			|| !fSave.WriteWord(rec.r14.stolen.r.w)
			|| !fSave.WriteByte(rec.r14.b4[0])
			|| !fSave.WriteByte(rec.r14.b4[1])
			|| !fSave.WriteWord(rec.r14.w6)
		) {
			return false;
		}
		return true;
	}
	// 
	bool fnWriteRecExmap(CDDATx::CSerializedWriter &fSave, const DMDBU &rec)
	{
		if (false
			|| !fSave.WriteWord(rec.r14.next.r.w)
			|| !fSave.WriteWord(rec.r14.stolen.r.w)
			|| !fSave.WriteByte(rec.r14.b4[0])
			|| !fSave.WriteByte(rec.r14.b4[1])
			|| !fSave.WriteWord(rec.r14.w6)
			|| !fSave.WriteWord(rec.r14xmap.w8)
			|| !fSave.WriteWord(rec.r14xmap.w10)
		) {
			return false;
		}
		return true;
	}
	// 
	fnWriteRec findWriteRec(BYTE i, bool fDM2_EXTENDED_MAP)
	{
		switch (i) {
		case  0: return fnWriteRec0;
		case  1: return fnWriteRec1;
		case  2: return fnWriteRec0;
		case  3: return fnWriteRec3;
		case  4: return fDM2_EXTENDED_MAP ? fnWriteRec4xmap : fnWriteRec4;
		case  5: return fnWriteRec0;
		case  6: return fnWriteRec0;
		case  7: return fnWriteRec0;
		case  8: return fnWriteRec0;
		case  9: return fDM2_EXTENDED_MAP ? fnWriteRec9xmap : fnWriteRec9;
		case 10: return fnWriteRec0;
		case 11:  return fnWriteRec3;
		case 12:  return fnWriteRec3;
		case 13:  return fnWriteRec3;
		case 14: return fDM2_EXTENDED_MAP ? fnWriteRecExmap : fnWriteRecE;
		case 15: return fnWriteRec0;
		}
		return fnWriteRecB;
	}
};

//////////////////////////////////////////////////////////////////////
// CDMRio

class CDMRio
{
	// 
	CFile &file;
	// 
	BOOL fAnti;
	// 
	DWORD nCheckSum;

	// 
	void swapTest(WORD &x)
	{
		if (fAnti) x = MAKEWORD(HIBYTE(x), LOBYTE(x));
	}

public:
	// 
	CDMRio(CFile &file, BOOL fAnti): file(file), fAnti(fAnti), nCheckSum(0) { }
	// 
	DWORD GetCheckSumAway()
	{
		DWORD r = nCheckSum;
		nCheckSum = 0;
		return r;
	}
	// 
	void SureRead(void *pv, UINT cb)
	{
		BYTE *pbPos = (BYTE *)pv;
		BYTE *pbEnd = pbPos + cb;

		BYTE *pb = pbPos;
		while (cb != 0) {
			UINT nRead = file.Read(pb, cb);
			if (nRead == 0) AfxThrowFileException(CFileException::endOfFile);
			pb += nRead;
			cb -= nRead;
		}

		for (; pbPos < pbEnd; pbPos++) nCheckSum += *pbPos;
	}
	// 
	UINT Read(void *pv, UINT cb)
	{
		BYTE *pbPos = (BYTE *)pv;
		BYTE *pbEnd;

		BYTE *pb = pbPos;
		while (cb != 0) {
			UINT nRead = file.Read(pb, cb);
			if (nRead == 0) break;
			pb += nRead;
			cb -= nRead;
		}

		pbEnd = pb;
		cb = pbEnd - pbPos;
		for (; pbPos < pbEnd; pbPos++) nCheckSum += *pbPos;
		return cb;
	}
	// 
	void ReadThru(UINT cb)
	{
		file.Seek(cb, CFile::current);
	}
	// 
	CDMRio &operator >>(BYTE &x)
	{
		return SureRead(&x, 1), *this;
	}
	// 
	CDMRio &operator >>(WORD &x)
	{
		return SureRead(&x, 2), swapTest(x), *this;
	}
	// 
	int ReadWord()
	{
		WORD z;
		if (Read(&z, 2) != 2) return -1;
		swapTest(z);
		return z;
	}
};

//////////////////////////////////////////////////////////////////////
// CDMWio

class CDMWio
{
	// 
	CArchive &file;
	// 
	BOOL fAnti;
	// 
	DWORD nCheckSum;

	// 
	void swapTest(WORD &x)
	{
		if (fAnti) x = MAKEWORD(HIBYTE(x), LOBYTE(x));
	}

public:
	// 
	CDMWio(CArchive &file, BOOL fAnti): file(file), fAnti(fAnti), nCheckSum(0) { }
	// 
	DWORD GetCheckSumAway()
	{
		DWORD r = nCheckSum;
		nCheckSum = 0;
		return r;
	}
	// 
	void SureWrite(const void *pv, UINT cb)
	{
		const BYTE *pbPos = (const BYTE*)pv;
		const BYTE *pbEnd = pbPos + cb;

		file.Write(pv, cb);

		for (; pbPos < pbEnd; pbPos++) nCheckSum += *pbPos;
	}
	// 
	CDMWio &operator <<(BYTE x)
	{
		SureWrite(&x, 1);
		return *this;
	}
	// 
	CDMWio &operator <<(WORD x)
	{
		swapTest(x);
		SureWrite(&x, 2);
		return *this;
	}
	// 
	void WriteRecord(UINT db, const DMDBU &rec)
	{
		CDMWio &ar = *this;
		switch (db) {
		case 0:
			ar	<< rec.r0.next.r.w
				<< rec.r0.w2;
			break;
		case 1:
			ar	<< rec.r1.next.r.w
				<< rec.r1.w2
				<< rec.r1.w4;
			break;
		case 2:
			ar	<< rec.r2.next.r.w
				<< rec.r2.w2;
			break;
		case 3:
			ar	<< rec.r3.next.r.w
				<< rec.r3.w2
				<< rec.r3.w4
				<< rec.r3.w6;
			break;
		case 4:
			ar	<< rec.r4.next.r.w
				<< rec.r4.stolen.r.w
				<< rec.r4.b4
				<< rec.r4.b5
				<< rec.r4.w6[0]
				<< rec.r4.w6[1]
				<< rec.r4.w6[2]
				<< rec.r4.w6[3]
				<< rec.r4.w14;
			break;
		case 5:
			ar	<< rec.r5.next.r.w
				<< rec.r5.w2;
			break;
		case 6:
			ar	<< rec.r6.next.r.w
				<< rec.r6.w2;
			break;
		case 7:
			ar	<< rec.r7.next.r.w
				<< rec.r7.w2;
			break;
		case 8:
			ar	<< rec.r8.next.r.w
				<< rec.r0.w2;
			break;
		case 9:
			ar	<< rec.r9.next.r.w
				<< rec.r9.stolen.r.w
				<< rec.r9.w4
				<< rec.r9.b6[0]
				<< rec.r9.b6[1];
			break;
		case 10:
			ar	<< rec.r10.next.r.w
				<< rec.r10.w2;
			break;
		case 14:
			ar	<< rec.r14.next.r.w
				<< rec.r14.stolen.r.w
				<< rec.r14.b4[0]
				<< rec.r14.b4[1]
				<< rec.r14.w6;
			break;
		case 15:
			ar	<< rec.r15.next.r.w
				<< rec.r15.w2;
			break;
		}
	}

};

//////////////////////////////////////////////////////////////////////
// CDDAT

CDDAT::CDDAT()
{
	for (int i = 0; i < 16; i++) m_nRec[i] = 0;
	m_nMaps = 0;
	m_fModified = FALSE;

	m_fNocs = false;
}

CDDAT::~CDDAT()
{

}

bool CDDAT::loadFrom(CDDATx::CSerializedReader &fIn, BOOL fLE, bool fDM2_EXTENDED_MAP)
{
	int RandomGraphicsSeed;
	int MapDataSize;
	int NumberOfLevels;
	int TextDataSize;
	int StartingPartyPosition;
	int ObjectListSize;
	int NumberOfRecords[16];

	CDDATx::CSerializedReaderWithChkSum fLoad(fIn);

	RandomGraphicsSeed = fLoad.ReadWord();
	MapDataSize = fDM2_EXTENDED_MAP ? fLoad.ReadDWord() : fLoad.ReadWord();
	NumberOfLevels = fLoad.ReadByte();
	fLoad.ReadByte();
	TextDataSize = fLoad.ReadWord();
	StartingPartyPosition = fLoad.ReadWord();
	ObjectListSize = fLoad.ReadWord();

	if (fLoad.HasError())
		return false;

	if (DDAT_MAX_MAPS < NumberOfLevels)
		return false;

	UINT i;
	for (i = 0; i < 16; i++) {
		if ((NumberOfRecords[i] = fLoad.ReadWord()) < 0)
			return false;
		switch (i) {
		case 0x0:
		case 0x1:
		case 0x2:
		case 0x3:
		case 0x4:
		case 0x5:
		case 0x6:
		case 0x7:
		case 0x8:
		case 0x9:
		case 0xA:
		case  0xB:
		case  0xC:
		case  0xD:
		case 0xE:
		case 0xF:
			if (NumberOfRecords[i] > DDAT_MAX_RECS)
				return false;
			break;
		default:
			if (NumberOfRecords[i] != 0)
				return false;
			break;
		}
	}

	LevelDesc_t ss[DDAT_MAX_MAPS];

	WORD NumberOfColumns = 0;

	// Level Descriptor
	{
		struct {
			int w0;
			int w2, w4;
			int b6, b7;
			int w8, w10, w12, w14;
		} t;

		for (int i = 0; i < NumberOfLevels; i++) {
			t. w0 = fDM2_EXTENDED_MAP ? fLoad.ReadDWord() : fLoad.ReadWord();
			t. w2 = fLoad.ReadWord();
			t. w4 = fLoad.ReadWord();
			t. b6 = fLoad.ReadByte();
			t. b7 = fLoad.ReadByte();
			t. w8 = fLoad.ReadWord();
			t.w10 = fLoad.ReadWord();
			t.w12 = fLoad.ReadWord();
			t.w14 = fLoad.ReadWord();
			if (fLoad.HasError())
				return false;
			int cy;
			ss[i].nOffset = t.w0;
			ss[i].w2 = t.w2;
			ss[i].w4 = t.w4;
			ss[i].nShiftX = t.b6;
			ss[i].nShiftY = t.b7;
			ss[i].cxLv = ((t.w8 >>11) & 0x1F) +1;
			ss[i].cyLv = cy = ((t.w8 >> 6) & 0x1F) +1;
			ss[i].iLv  = ((t.w8 >> 0) & 0x3F);
			ss[i].nWallOrnateIndices  = (t.w10 >> 0) & 0x0F;
			ss[i].n10_4_7             = (t.w10 >> 4) & 0x0F;
			ss[i].nFloorOrnateIndices = (t.w10 >> 8) & 0x0F;
			ss[i].n10_C_F             = (t.w10 >>12) & 0x0F;
			ss[i].nSharedOrnateIndices = (t.w12 >> 0) & 0x0F;
			ss[i].nCreatureIndices     = (t.w12 >> 4) & 0x0F;
			ss[i].n12_8_B              = (t.w12 >> 8) & 0x0F;
			ss[i].n12_C_F              = (t.w12 >>12) & 0x0F;
			ss[i].n14_0_3 = (t.w14 >> 0) & 0x0F;
			ss[i].n14_4_7 = (t.w14 >> 4) & 0x0F;
			ss[i].n14_8_B = (t.w14 >> 8) & 0x0F;
			ss[i].n14_C_F = (t.w14 >>12) & 0x0F;
			NumberOfColumns += cy;
		}
	}

	SizeBuff fvObjectList;
	SizeBuff fvTextData;
	SizeBuff fvColumnList;
	if (false
		|| !fvObjectList.Alloc(2 * ObjectListSize)
		|| !fvTextData.Alloc(2 * TextDataSize)
		|| !fvColumnList.Alloc(2 * NumberOfColumns)
	) {
		return false;
	}

	WORD *pwObjectList = (WORD *)fvObjectList.GetData();
	WORD *pwTextData = (WORD *)fvTextData.GetData();
	WORD *pwColumnList = (WORD *)fvColumnList.GetData();

	{
		int i;

		for (i = 0; i < NumberOfColumns; i++) {
			int x = fLoad.ReadWord();
			if (x < 0)
				return false;
			pwColumnList[i] = (WORD)x;
		}
		for (i = 0; i < ObjectListSize; i++) {
			int x = fLoad.ReadWord();
			if (x < 0)
				return false;
			pwObjectList[i] = (WORD)x;
		}
		for (i = 0; i < TextDataSize; i++) {
			int x = fLoad.ReadWord();
			if (x < 0)
				return false;
			pwTextData[i] = (WORD)x;
		}
	}

	AllMapDesc_t aAmd;
	aAmd.Init();
	aAmd.nRandomGraphicsSeed = RandomGraphicsSeed;
	aAmd.nStartX =   ((StartingPartyPosition >> 5)&0x1F);
	aAmd.nStartY =   ((StartingPartyPosition >> 0)&0x1F);
	aAmd.nStartDir = ((StartingPartyPosition >>10)&0x03);
	aAmd.nMinObjectListSize = ObjectListSize;
	aAmd.fvTextData = fvTextData;
	VERIFY(SUCCEEDED(SetAllMapDesc(aAmd)));

	{
		int i;
		int x;
		for (i = 0; i < 16; i++) {
			bool (*pfnReadRec)(CDDATx::CSerializedReader &fLoad, DMDBU &rec);
			switch (i) {
			case  0: pfnReadRec = fnReadRec0; break;
			case  1: pfnReadRec = fnReadRec1; break;
			case  2: pfnReadRec = fnReadRec0; break;
			case  3: pfnReadRec = fnReadRec3; break;
			case  4: pfnReadRec = fDM2_EXTENDED_MAP ? fnReadRec4xmap : fnReadRec4; break;
			case  5: pfnReadRec = fnReadRec0; break;
			case  6: pfnReadRec = fnReadRec0; break;
			case  7: pfnReadRec = fnReadRec0; break;
			case  8: pfnReadRec = fnReadRec0; break;
			case  9: pfnReadRec = fDM2_EXTENDED_MAP ? fnReadRec9xmap : fnReadRec9; break;
			case 10: pfnReadRec = fnReadRec0; break;
			case 11: pfnReadRec =  fnReadRec3; break;
			case 12: pfnReadRec =  fnReadRec3; break;
			case 13: pfnReadRec =  fnReadRec3; break;
			case 14: pfnReadRec = fDM2_EXTENDED_MAP ? fnReadRecExmap : fnReadRecE; break;
			case 15: pfnReadRec = fnReadRec0; break;
			}
			for (x = 0; x < NumberOfRecords[i]; x++) {
				DMDBU rec;
				if (!pfnReadRec(fLoad, rec))
					return false;
				HRESULT hr;
				VERIFY(SUCCEEDED(hr = addDBRecTo(i, rec, DBIPos_t())));
			}
		}
	}

	{
		UINT iCurOff = 0, iNewOff;
		UINT iMap;
		UINT iObj = 0;
		for (iMap = 0; iMap < (UINT)NumberOfLevels; iMap++) {
			LevelDesc_t &s = ss[iMap];

			iNewOff = s.nOffset;

			if (iNewOff < iCurOff)
				return false;

			while (iCurOff < iNewOff) {
				fLoad.ReadByte();
				iCurOff++;
			}
			if (fLoad.HasError())
				return false;

			Atlas_t a;
			BYTE x, y, cx = s.cxLv, cy = s.cyLv;
			for (y = 0; y < cy; y++) {
				for (x = 0; x < cx; x++) {
					int w = fLoad.ReadByte();
					if (fLoad.HasError())
						return false;
					RN rn = RN_NIL;
					BYTE c = (a.c[y][x] = w);
					if ((c & 0x10) == 0x10) {
						rn = RNco((WORD)pwObjectList[iObj]); iObj++;
						ASSERT(rn != RN_END && rn != RN_NIL);
					}
					a.r[y][x] = rn;
				}
			}
			BYTE iOff, nLen;

			for (iOff = 0, nLen = s.nCreatureIndices; iOff < nLen; iOff++) {
				s.bCreatureIndices[iOff] = fLoad.ReadByte();
				if (fLoad.HasError())
					return false;
			}
			for (iOff = 0, nLen = s.nWallOrnateIndices; iOff < nLen; iOff++) {
				s.bWallOrnateIndices[iOff] = fLoad.ReadByte();
				if (fLoad.HasError())
					return false;
			}
			for (iOff = 0, nLen = s.nFloorOrnateIndices; iOff < nLen; iOff++) {
				s.bFloorOrnateIndices[iOff] = fLoad.ReadByte();
				if (fLoad.HasError())
					return false;
			}
			for (iOff = 0, nLen = s.nSharedOrnateIndices; iOff < nLen; iOff++) {
				s.bSharedOrnateIndices[iOff] = fLoad.ReadByte();
				if (fLoad.HasError())
					return false;
			}
			UINT iMapNew;
			HRESULT hr;
			VERIFY(SUCCEEDED(hr = addEmptyMap(iMapNew)));
			VERIFY(SUCCEEDED(hr = setMapDescAt(iMapNew, s, UIF_NO)));
			VERIFY(SUCCEEDED(hr = setMapAtlasAt(iMapNew, a, UIF_NO)));

			iCurOff += cx * cy
				+ s.nCreatureIndices
				+ s.nSharedOrnateIndices
				+ s.nWallOrnateIndices
				+ s.nFloorOrnateIndices
				;
		}
	}

	if (!m_fNocs) {
		int nValue = fIn.ReadWord();
		if (!fIn.HasError() && nValue != (fLoad.nValue & 0xFFFF)) {
			int r = AfxMessageBox(IDS_ERR_INVALID_CHECK_SUM, 0 |MB_RETRYCANCEL |MB_ICONEXCLAMATION);
			if (r != IDRETRY)
				return false;
		}
	}

	return true;
}

bool CDDAT::saveTo(CDDATx::CSerializedWriter &fInto, BOOL fLE, BOOL fAddCheckSum, bool fDM2_EXTENDED_MAP)
{
	HRESULT hr;

	UINT nOffsets[DDAT_MAX_MAPS];
	UINT nObjList = 0, nColumns = 0, nObjListSize;
	UINT nMapData;

	const UINT nMaps = CntMap();

	{
		UINT iOffset = 0;
		for (UINT iMap = 0; iMap < nMaps; iMap++) {
			Map_t *m = NULL;
			VERIFY(SUCCEEDED(hr = getMapAt(iMap, m)));
			nOffsets[iMap] = iOffset;
			const LevelDesc_t &s = m->aDesc;
			const BYTE cx = s.cxLv, cy = s.cyLv;
			const Atlas_t &t = m->aAtlas;
			for (BYTE y = 0; y < cy; y++) {
				for (BYTE x = 0; x < cx; x++) {
					BYTE c = t.c[y][x];
					if (c & 0x10) {
						nObjList++;
					}
				}
			}

			nColumns += cy;

			iOffset += cx * cy
				+ s.nCreatureIndices
				+ s.nSharedOrnateIndices
				+ s.nWallOrnateIndices
				+ s.nFloorOrnateIndices
				;
		}

		nMapData = iOffset;
	}

	const AllMapDesc_t &a = m_aAllDesc;

	const UINT nMinObjList = a.nMinObjectListSize;

	nObjListSize = __max(nObjList, nMinObjList);

	SizeBuff fvMapData;
	SizeBuff fvObjList;
	SizeBuff fvColumns;

	if (false
		|| !fvMapData.Alloc(nMapData)
		|| !fvObjList.Alloc(2 * nObjListSize)
		|| !fvColumns.Alloc(2 * nColumns)
	) {
		return false;
	}

	CDDATx::CMemVuff mvMapData, mvObjList, mvColumns;

	mvMapData.fLE = true, mvMapData.pData = fvMapData.GetData(), mvMapData.iCur = 0, mvMapData.nMax = fvMapData.GetSize();
	mvObjList.fLE = true, mvObjList.pData = fvObjList.GetData(), mvObjList.iCur = 0, mvObjList.nMax = fvObjList.GetSize();
	mvColumns.fLE = true, mvColumns.pData = fvColumns.GetData(), mvColumns.iCur = 0, mvColumns.nMax = fvColumns.GetSize();

	{
		UINT iObjList = 0;

		for (UINT iMap = 0; iMap < nMaps; iMap++) {
			Map_t *m = NULL;
			VERIFY(SUCCEEDED(hr = getMapAt(iMap, m)));
			const LevelDesc_t &s = m->aDesc;
			const BYTE cx = s.cxLv, cy = s.cyLv;
			const Atlas_t &t = m->aAtlas;
			for (BYTE y = 0; y < cy; y++) {
				VERIFY(mvColumns.WriteWord(iObjList));
				for (BYTE x = 0; x < cx; x++) {
					BYTE c = t.c[y][x];
					if (c & 0x10) {
						iObjList++;
						VERIFY(mvObjList.WriteWord(t.r[y][x].r.w));
					}
					VERIFY(mvMapData.WriteByte(c));
				}
			}

			BYTE i, c;
			for (i = 0, c = s.nCreatureIndices    ; i < c; i++) VERIFY(mvMapData.WriteByte(s.bCreatureIndices[i]));
			for (i = 0, c = s.nWallOrnateIndices  ; i < c; i++) VERIFY(mvMapData.WriteByte(s.bWallOrnateIndices[i]));
			for (i = 0, c = s.nFloorOrnateIndices ; i < c; i++) VERIFY(mvMapData.WriteByte(s.bFloorOrnateIndices[i]));
			for (i = 0, c = s.nSharedOrnateIndices; i < c; i++) VERIFY(mvMapData.WriteByte(s.bSharedOrnateIndices[i]));
		}

		for (; iObjList < nObjListSize; iObjList++) VERIFY(mvObjList.WriteWord(0xFFFF));
	}

	CDDATx::CSerializedWriterWithChkSum fSave(fInto);

	UINT nTextData = a.fvTextData.GetSize() / 2;

	UINT nStartPartyPos = 0
		|((a.nStartX  & 0x1F) << 5)
		|((a.nStartY  & 0x1F) << 0)
		|((a.nStartDir& 0x03) <<10)
		;

	if (false
		|| !fSave.WriteWord(a.nRandomGraphicsSeed)
		|| !(fDM2_EXTENDED_MAP ? fSave.WriteDWord(nMapData) : fSave.WriteWord(nMapData))
		|| !fSave.WriteByte(nMaps)
		|| !fSave.WriteByte(0)
		|| !fSave.WriteWord(nTextData)
		|| !fSave.WriteWord(nStartPartyPos)
		|| !fSave.WriteWord(nObjListSize)
	) {
		return false;
	}

	UINT nRecords[16];

	{
		for (UINT i = 0; i < 16; i++) {
			if (!fSave.WriteWord(nRecords[i] = CntDBRecIn(i)))
				return false;
		}
	}

	{
		for (UINT iMap = 0; iMap < nMaps; iMap++) {
			Map_t *m = NULL;
			VERIFY(SUCCEEDED(hr = getMapAt(iMap, m)));
			const LevelDesc_t &s = m->aDesc;

			WORD w8 = 0
				|(((s.cxLv -1) & 0x1F) <<11)
				|(((s.cyLv -1) & 0x1F) << 6)
				|(((s.iLv    ) & 0x3F) << 0)
				;
			WORD w10 = 0
				|((s.nWallOrnateIndices  & 0x0F) << 0)
				|((s.n10_4_7             & 0x0F) << 4)
				|((s.nFloorOrnateIndices & 0x0F) << 8)
				|((s.n10_C_F             & 0x0F) <<12)
				;
			WORD w12 = 0
				|((s.nSharedOrnateIndices & 0x0F) << 0)
				|((s.nCreatureIndices     & 0x0F) << 4)
				|((s.n12_8_B              & 0x0F) << 8)
				|((s.n12_C_F              & 0x0F) <<12)
				;
			WORD w14 = 0
				|((s.n14_0_3 & 0x0F) << 0)
				|((s.n14_4_7 & 0x0F) << 4)
				|((s.n14_8_B & 0x0F) << 8)
				|((s.n14_C_F & 0x0F) <<12)
				;

			if (false
				|| !(fDM2_EXTENDED_MAP ? fSave.WriteDWord(nOffsets[iMap]) : fSave.WriteWord(nOffsets[iMap]))
				|| !fSave.WriteWord(s.w2)
				|| !fSave.WriteWord(s.w4)
				|| !fSave.WriteByte(s.nShiftX)
				|| !fSave.WriteByte(s.nShiftY)
				|| !fSave.WriteWord(w8)
				|| !fSave.WriteWord(w10)
				|| !fSave.WriteWord(w12)
				|| !fSave.WriteWord(w14)
			) {
				return false;
			}
		}
	}

	{
		const WORD *pwColumns = (const WORD *)mvColumns.pData;

		for (UINT i = 0; i < nColumns; i++, pwColumns++) {
			if (!fSave.WriteWord(*pwColumns))
				return false;
		}
	}

	{
		const WORD *pwObjList = (const WORD *)mvObjList.pData;

		for (UINT i = 0; i < nObjListSize; i++, pwObjList++) {
			if (!fSave.WriteWord(*pwObjList))
				return false;
		}
	}

	{
		const WORD *pwTextData = reinterpret_cast<const WORD *>(a.fvTextData.GetData());

		for (UINT i = 0; i < nTextData; i++, pwTextData++) {
			if (!fSave.WriteWord(*pwTextData))
				return false;
		}
	}

	{
		for (UINT i = 0; i < 16; i++) {
			fnWriteRec pfn = findWriteRec(i, fDM2_EXTENDED_MAP);
			for (UINT x = 0; x < nRecords[i]; x++) {
				DMDBU rec;
				VERIFY(SUCCEEDED(hr = getDBRecAt(DBIPos_t(i, x), rec)));
				if (!pfn(fSave, rec))
					return false;
			}
		}
	}

	{
		const BYTE *pDataPos = mvMapData.pData;
		const BYTE *pDataEnd = pDataPos + mvMapData.iCur;

		for (; pDataPos < pDataEnd; pDataPos++)
			if (!fSave.WriteByte(*pDataPos))
				return false;
	}

	if (fAddCheckSum) {
		if (!fInto.WriteWord(WORD(fSave.nValue & 0xFFFF)))
			return false;
	}

	return true;
}

bool CDDAT::LoadFrom(CDDATx::CSerializedReader &fLoad, BOOL fLE, bool fDM2_EXTENDED_MAP)
{
	TRY {
		deleteContents();

		CDDATUndoLock lock(*this, TRUE);
		//
		if (!loadFrom(fLoad, fLE, fDM2_EXTENDED_MAP)) return false;
		//
		lock.Unlock(TRUE);
		return true;
	} CATCH_ALL(e) {
		return false;
	} END_CATCH_ALL
}

bool CDDAT::SaveTo(CDDATx::CSerializedWriter &fInto, BOOL fLE, BOOL fAddCheckSum, bool fDM2_EXTENDED_MAP)
{
	TRY {
		if (!saveTo(fInto, fLE, fAddCheckSum, fDM2_EXTENDED_MAP)) return false;
		return true;
	} CATCH_ALL(e) {
		return false;
	} END_CATCH_ALL
}

HRESULT CDDAT::BTFromDBRec(const DBIPos_t &pos, std::list<AtlasPos_t> &l) const
{
	m_bt.Lookup(pos, l);
//	BTDB2AtlasMap::const_iterator
//		iterPos = m_mmapDB2Atlas.lower_bound(pos),
//		iterEnd = m_mmapDB2Atlas.upper_bound(pos);
//	while (iterPos != iterEnd) {
//		l.push_back(iterPos->second);
//		iterPos++;
//	}
	if (l.empty())
		return DMERR_NO_ITEM;
	return S_OK;
}

HRESULT CDDAT::BTFromDBRec(const DBIPos_t &pos, std::list<DBIPos_t> &l) const
{
	m_bt.Lookup(pos, l);
//	BTDB2DBMap::const_iterator
//		iterPos = m_mmapDB2DB.lower_bound(pos),
//		iterEnd = m_mmapDB2DB.upper_bound(pos);
//	while (iterPos != iterEnd) {
//		l.push_back(iterPos->second);
//		iterPos++;
//	}
	if (l.empty())
		return DMERR_NO_ITEM;
	return S_OK;
}

void CDDAT::Undo_OnUndo(CDDATUndoListItem *pItem)
{
	TRY
		pItem->f.SeekToBegin();
		CArchive ar(&pItem->f, CArchive::load);
		switch (pItem->wp) {
		case DDATUC_DELETE_RECDB:
			{
				DBIPos_t pos; ar >> pos;
				UINT iDB = pos.db();
				DMDBU rec;
				UINT i;
				for (i = pos.idx(); i < 1023; i++) {
					getRecAt(iDB, i + 1, rec);
					setRecAt(iDB, i, rec);
				}
				m_nRec[iDB]--;
				break;
			}
		case DDATUC_SET_DBREC:
			{
				DBIPos_t pos; ar >> pos;
				DMDBU rec; DMDBU::Ar(ar) >> rec;
				VERIFY(SUCCEEDED(setDBRecAt(pos, rec, UIF_UNDO_RUNNING)));
				break;
			}
		case DDATUC_SET_ATLAS_BIT:
			{
				AtlasPos_t pos; ar >> pos;
				AtlasBit_t bit; ar >> bit;

				VERIFY(SUCCEEDED(setAtlasBitAt(pos, bit, UIF_UNDO_RUNNING)));
				break;
			}
		case DDATUC_SET_ATLAS:
			{
				UINT iMapAt; ar >> iMapAt;
				Atlas_t ma;
				ar.Read(&ma, sizeof(Atlas_t));
				VERIFY(SUCCEEDED(setMapAtlasAt(iMapAt, ma, UIF_UNDO_RUNNING)));
				break;
			}
		case DDATUC_SET_LEVEL_DESC:
			{
				UINT iMapAt; ar >> iMapAt;
				LevelDesc_t d;
				ar >> d;
				VERIFY(SUCCEEDED(setMapDescAt(iMapAt, d, UIF_UNDO_RUNNING)));
				break;
			}
		case DDATUC_DELETE_MAP:
			{
				UINT iMapAt; ar >> iMapAt;
				VERIFY(SUCCEEDED(deleteMapAt(iMapAt, UIF_UNDO_RUNNING)));
				break;
			}
		case DDATUC_INSERT_MAP:
			{
				UINT iMapAt; ar >> iMapAt;
				Map_t m;
				ar >> m.aDesc;
				ar.Read(&m.aAtlas, sizeof(Atlas_t));
				VERIFY(SUCCEEDED(insertEmptyMapAt(iMapAt, UIF_UNDO_RUNNING)));
				VERIFY(SUCCEEDED(setMapDescAt(iMapAt, m.aDesc, UIF_UNDO_RUNNING)));
				VERIFY(SUCCEEDED(setMapAtlasAt(iMapAt, m.aAtlas, UIF_UNDO_RUNNING)));
				break;
			}
		case DDATUC_SET_ALL_MAP_DESC:
			{
				ar >> m_aAllDesc;
				break;
			}
		case DDATUC_SAVE_MASK_OCCUPIED:
			{
				ar.Read(&m_maskOccupied, sizeof(CDMDBRMask));
				break;
			}
		}
	CATCH_ALL(e)
#ifdef _DEBUG
		e->ReportError(MB_ICONINFORMATION);
#endif
	END_CATCH_ALL
}

HRESULT CDDAT::addDBRecTo(UINT iDB, const DMDBU &recAny, DBIPos_t &posNew)
{
	UINT iInsertBefore = DDAT_MAX_RECS;
	if (!insertToDBRecAnd_(iDB, iInsertBefore)) {
		//ASSERT(FALSE);
		return DMERR_NO_MORE_SPACE;
	}
	setRecAt(iDB, iInsertBefore, recAny);
	posNew = DBIPos_t(iDB, iInsertBefore);
	saveUndo_DeleteDBRecAt(posNew);
	intersectDBRecAndDBRec(posNew, recAny, 1);
	OnInsertDBRec(posNew, recAny), SetModified();

	m_maskOccupied.Set(iDB, iInsertBefore);
	return S_OK;
}

BOOL CDDAT::insertToDBRecAnd_(UINT iDB, UINT &iInsertBefore)
{
	if (iDB >= 16) return FALSE;
	UINT cb1 = sizeInDBRec(iDB);
	UINT nInDB = m_nRec[iDB];
	if (nInDB >= DDAT_MAX_RECS) return FALSE;
	iInsertBefore = __min(iInsertBefore, nInDB);
	m_nRec[iDB]++;
	DMDBU recDie;
	memset(&recDie, 0xff, sizeof(recDie));
	setRecAt(iDB, nInDB, recDie);
	if (nInDB != 0) {
		for (UINT i = nInDB; i != iInsertBefore; i--) {
			DBIPos_t p0(iDB, i-1), p1(iDB, i);
			DMDBU recMove;
			HRESULT hr;
			list<DBIPos_t> l1;
			list<AtlasPos_t> l2;
			VERIFY(SUCCEEDED(hr = BTFromDBRec(p0, l1)) || hr == DMERR_NO_ITEM);
			VERIFY(SUCCEEDED(hr = BTFromDBRec(p0, l2)) || hr == DMERR_NO_ITEM);
			VERIFY(SUCCEEDED(hr = getDBRecAt(p0, recMove)));
			VERIFY(SUCCEEDED(hr = setDBRecAt(p1, recMove, UIF_NO)));
			while (!l1.empty()) {
				DMDBU recDet;
				if (SUCCEEDED(hr = getDBRecAt(l1.front(), recDet))) {
					int n = 0;
					if (DBIPos_t(recDet.r0.next) == p0) {
						recDet.r0.next.idx(p1.idx());
						n++;
					} else {
						switch (l1.front().db()) {
						case 4: case 9: case 14:
							if (DBIPos_t(recDet.r4.stolen) == p0) {
								recDet.r4.stolen.idx(p1.idx());
								n++;
							}
							break;
						}
					}
					if (n) {
						VERIFY(SUCCEEDED(setDBRecAt(l1.front(), recDet, UIF_NO)));
					}
				}
				l1.pop_front();
			}
			while (!l2.empty()) {
				AtlasBit_t o;
				if (SUCCEEDED(hr = GetAtlasBitAt(l2.front(), o))) {
					_ASSERTE(DBIPos_t(o.r) == p0);
					o.r.idx(p1.idx());
					VERIFY(SUCCEEDED(hr = setAtlasBitAt(l2.front(), o, UIF_NO)));
				}
				l2.pop_front();
			}
		}
	}
	return TRUE;
}

UINT CDDAT::sizeInDBRec(UINT iDB)
{
	static const UINT Sizes[] = {
		sizeof(DMDB0),
		sizeof(DMDB1),
		sizeof(DMDB2),
		sizeof(DMDB3),
		sizeof(DMDB4),
		sizeof(DMDB5),
		sizeof(DMDB6),
		sizeof(DMDB7),
		sizeof(DMDB8),
		sizeof(DMDB9),
		sizeof(DMDB10),
		sizeof(DMDB11),
		sizeof(DMDB12),
		sizeof(DMDB13),
		sizeof(DMDB14),
		sizeof(DMDB15),
	};
	_ASSERTE(iDB < 16);
	return Sizes[iDB];
}

HRESULT CDDAT::GetDBRecAt(const DBIPos_t &pos, DMDBU &rec) const
{
	return getDBRecAt(pos, rec);
}

HRESULT CDDAT::getDBRecAt(const DBIPos_t &pos, DMDBU &rec) const
{
	if (CntDBRecIn(pos.db()) <= pos.idx()) {
		//ASSERT(FALSE);
		return DMERR_DBREC_OUT_OF_RANGE;
	}
	getRecAt(pos.db(), pos.idx(), rec);
	return S_OK;
}

HRESULT CDDAT::GetMapAtlasAt(UINT iMap, Atlas_t &atlas) const
{
	const Map_t *m = 0;
	HRESULT hr;
	if (FAILED(hr = getMapAt(iMap, m))) return hr;
	atlas = m->aAtlas;
	return S_OK;
}

HRESULT CDDAT::getMapAt(UINT iMap, const Map_t *&ppMap) const
{
	if (m_nMaps <= iMap) {
		//ASSERT(FALSE);
		return DMERR_MAP_OUT_OF_RANGE;
	}
	ppMap = (const Map_t *)(m_aMaps + iMap);
	return S_OK;
}

HRESULT CDDAT::getMapAt(UINT iMap, Map_t *&ppMap)
{
	if (m_nMaps <= iMap) {
		ASSERT(FALSE);
		return DMERR_MAP_OUT_OF_RANGE;
	}
	ppMap = &m_aMaps[iMap];
	return S_OK;
}

HRESULT CDDAT::setMapAtlasAt(UINT iMap, const Atlas_t &atlas, UINT nUIF)
{
	HRESULT hr;
	Map_t *m = 0;
	if (FAILED(hr = getMapAt(iMap, m))) return hr;
	if (!(nUIF & UIF_UNDO_RUNNING)) {
		saveUndo_SetAtlas(iMap, m->aAtlas);
	}
	UINT cx=m->aDesc.cxLv, cy = m->aDesc.cyLv;
	for (UINT y=0; y<cy; y++) {
		for (UINT x=0; x<cx; x++) {
			RN rn = m->aAtlas.r[y][x];
			if (rn != RN_NIL) {
				intersectAtlasBitAndDBRec(AtlasPos_t(iMap, x, y), rn, 1, 0);
			}
			m->aAtlas.c[y][x] = atlas.c[y][x];
			m->aAtlas.r[y][x] = rn = atlas.r[y][x];
			if (rn != RN_NIL) {
				intersectAtlasBitAndDBRec(AtlasPos_t(iMap, x, y), rn, 1, 1);
			}
		}
	}
	OnChangeAtlas(iMap), SetModified();
	return S_OK;
}

HRESULT CDDAT::setMapDescAt(UINT iMap, const LevelDesc_t &desc, UINT nUIF)
{
	Map_t *m;
	HRESULT hr;
	if (FAILED(hr = getMapAt(iMap, m))) return hr;
	if (desc.cxLv > 32 || desc.cyLv > 32) {
		ASSERT(FALSE);
		return DMERR_INVALID_MAP_SIZE;
	}
	if (!(nUIF & UIF_UNDO_RUNNING)) {
		saveUndo_SetAtlas(iMap, m->aAtlas);
		saveUndo_SetLevelDesc(iMap, m->aDesc);
	}
	UINT x, y;
	UINT cxOld = m->aDesc.cxLv, cyOld = m->aDesc.cyLv;
	UINT cxNew = desc.cxLv, cyNew = desc.cyLv;
	for (y=0; y<32; y++) {
		for (x=0; x<32; x++) {
			BOOL bInNew = (y<cyNew && x<cxNew);
			if (!bInNew) {
				if (x<cxOld && y<cyOld) {
					RN rn = m->aAtlas.r[y][x];
					if (rn != RN_NIL) {
						intersectAtlasBitAndDBRec(AtlasPos_t(iMap, x, y), rn, 1, 0);
					}
				}
				m->aAtlas.c[y][x] = 0x00;
				m->aAtlas.r[y][x] = RN_NIL;
			}
		}
	}
	m->aDesc = desc;
	OnChangeLevelDesc(iMap), SetModified();
	return S_OK;
}

HRESULT CDDAT::addEmptyMap(UINT &iMapAt)
{
	iMapAt = DDAT_MAX_MAPS;
	return insertEmptyMapAt(iMapAt, UIF_NO);
}

HRESULT CDDAT::insertEmptyMapAt(UINT &iMapAt, UINT nUIF)
{
	UINT nMaps = m_nMaps;
	if (DDAT_MAX_MAPS <= nMaps) {
		ASSERT(FALSE);
		return DMERR_MAP_CNT_AT_MAX;
	}
	iMapAt = __min(iMapAt, nMaps);
	map_insertAt(iMapAt);
	if (!(nUIF & UIF_UNDO_RUNNING)) {
		saveUndo_DeleteMap(iMapAt);
	}
	Map_t *m = 0;
	HRESULT hr;
	if (FAILED(hr = getMapAt(iMapAt, m))) return hr;
	m->aDesc.Init();
	m->aDesc.cxLv = 15;
	m->aDesc.cyLv = 15;
	m->aDesc.iLv = 6;
	UINT x, y;
	for (y=0; y<32; y++) {
		for (x=0; x<32; x++) {
			m->aAtlas.c[y][x] = 0x00;
			m->aAtlas.r[y][x] = RN_NIL;
		}
	}
	OnInsertMap(iMapAt), SetModified();
	return S_OK;
}

void CDDAT::deleteContents()
{
	UINT i;
	for (i=0; i<16; i++) m_nRec[i] = 0;
	m_nMaps = 0;
//	m_mmapDB2Atlas.clear();
//	m_mmapDB2DB.clear();
	m_bt.Clear();
	m_aAllDesc.Init();
	Undo_Clear();
	m_fModified = FALSE;
}

UINT CDDAT::CntDBRecIn(UINT iDB) const
{
	_ASSERTE(iDB < 16);
	return m_nRec[iDB];
}

UINT CDDAT::CntMap() const
{
	return m_nMaps;
}

HRESULT CDDAT::GetAtlasBitAt(const AtlasPos_t &pos, AtlasBit_t &one) const
{
	HRESULT hr;
	const Map_t *m = 0;
	if (FAILED(hr = getMapAt(pos.Z(), m))) return hr;
	UINT x = pos.X(), y = pos.Y();
	UINT cx = m->aDesc.cxLv, cy = m->aDesc.cyLv;
	if (cx <= x || cy <= y) {
		ASSERT(FALSE);
		return DMERR_ATLAS_RANGE_OUT_OF_RANGE;
	}
	one.c = m->aAtlas.c[y][x];
	one.r = m->aAtlas.r[y][x];
	return S_OK;
}

void CDDAT::DeleteContents()
{
	deleteContents();
}

void CDDAT::DeleteUndoHistory()
{
	Undo_Clear();
}

BOOL CDDAT::IsUndoable()
{
	return Undo_IsUndoable() != 0;
}

BOOL CDDAT::DoUndo()
{
	Undo_UndoOnce();
	return TRUE;
}

HRESULT CDDAT::GetMapDescAt(UINT iMap, LevelDesc_t &desc) const
{
	const Map_t *m = 0;
	HRESULT hr;
	if (FAILED(hr = getMapAt(iMap, m))) return hr;
	desc = m->aDesc;
	return S_OK;
}

HRESULT CDDAT::SetMapAtlasAt(UINT iMap, const Atlas_t &atlas)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = setMapAtlasAt(iMap, atlas, UIF_NO))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

UINT CDDAT::CntAtlasAssocAt(const AtlasPos_t &pos) const
{
	HRESULT hr;
	const Map_t *m = 0;
	if (FAILED(hr = getMapAt(pos.Z(), m))) return hr;
	RN rn = m->aAtlas.r[pos.Y()][pos.X()];
	UINT n = 0;
	hr = cntAtlasAssoc(rn, n, 0);
	if (FAILED(hr)) return (UINT)-1;
	return n;
}

HRESULT CDDAT::cntAtlasAssoc(RN rn, UINT &n, UINT nDepth) const
{
	if (nDepth >= 16) {
//		ASSERT(FALSE);
		return DMERR_STUCK;
	}
	for (; rn != RN_END && rn != RN_NIL; ) {
		bool fNext = false;
		DMDBU rec;
		HRESULT hr;
		if (!IsThisAsIs(rn)) {
			if (FAILED(hr = getDBRecAt(DBIPos_t(rn), rec))) return hr;
			fNext = true;
		}
		n++;
		RN rn2 = RN_NIL;
		switch (rn.db()) {
		case 4:
		case 9:
		case 14:
			rn2 = rec.r4.stolen; break;
		}
		if (rn2 != RN_NIL) {
			HRESULT hr;
			if (FAILED(hr = cntAtlasAssoc(rn2, n, nDepth+1))) return hr;
		}
		if (!fNext) break;
		rn = rec.r0.next;
	}
	return S_OK;
}

HRESULT CDDAT::SetAtlasBitAt(const AtlasPos_t &pos, const AtlasBit_t &one)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = setAtlasBitAt(pos, one, UIF_NO))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::setAtlasBitAt(const AtlasPos_t &pos, const AtlasBit_t &one, UINT nUIF)
{
	Map_t *m = 0;
	HRESULT hr;
	if (FAILED(hr = getMapAt(pos.Z(), m))) return hr;
	UINT x = pos.X(), y = pos.Y();
	if (m->aDesc.cxLv <= x || m->aDesc.cyLv <= y) {
		ASSERT(FALSE);
		return DMERR_ATLAS_RANGE_OUT_OF_RANGE;
	}
	AtlasBit_t o;
	o.c = m->aAtlas.c[y][x];
	o.r = m->aAtlas.r[y][x];
	BOOL bNewc = o.c != one.c;
	BOOL bNewr = o.r != one.r;

	RN rn = m->aAtlas.r[y][x];
	if (bNewr && rn != RN_NIL) {
		intersectAtlasBitAndDBRec(pos, rn, 1, 0);
	}
	if (!(nUIF & UIF_UNDO_RUNNING) && (bNewc || bNewr)) {
		saveUndo_SetAtlasBitAt(pos, o);
	}
	m->aAtlas.c[y][x] = one.c;
	m->aAtlas.r[y][x] = rn = one.r;
	if (bNewr && rn != RN_NIL) {
		intersectAtlasBitAndDBRec(pos, rn, 1, 1);
	}
	OnChangeAtlasBit(pos), SetModified();
	return S_OK;
}

HRESULT CDDAT::SetMapDescAt(UINT iMap, const LevelDesc_t &desc)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = setMapDescAt(iMap, desc, UIF_NO))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::AddEmptyMap(UINT &iMapAt)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = addEmptyMap(iMapAt))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::DeleteMapAt(UINT iMapAt)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = deleteMapAt(iMapAt, UIF_NO))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::deleteMapAt(UINT iMapAt, UINT nUIF)
{
	Map_t *m = 0;
	HRESULT hr;
	if (FAILED(hr = getMapAt(iMapAt, m))) {
		ASSERT(FALSE);
		return DMERR_MAP_OUT_OF_RANGE;
	}
	removeMapAssoc(iMapAt, *m);
	if (!(nUIF & UIF_UNDO_RUNNING)) {
		saveUndo_InsertMapAt(iMapAt, *m);
	}
	deleteMapAt_(iMapAt);
	OnDeleteMap(iMapAt), SetModified();
	return S_OK;
}

HRESULT CDDAT::deleteMapAt_(UINT iMapAt)
{
	map_removeAt(iMapAt);
	return S_OK;
}

HRESULT CDDAT::removeMapAssoc(UINT iMapAt, Map_t &m)
{
	UINT x, y, cx = m.aDesc.cxLv, cy = m.aDesc.cyLv;
	for (y=0; y<cy; y++) {
		for (x=0; x<cx; x++) {
			RN rn = m.aAtlas.r[y][x];
			if (rn != RN_NIL) {
				intersectAtlasBitAndDBRec(AtlasPos_t(iMapAt, x, y), rn, 1, 0);
			}
		}
	}
	return S_OK;
}

HRESULT CDDAT::removeDBRecAssoc(RN rn, BOOL bDeleteNext, BOOL bDeleteSub)
{
	if (IsThisAsIs(rn))
		return S_OK;
	return removeDBRecAssoc(DBIPos_t(rn), bDeleteNext, bDeleteSub);
}

HRESULT CDDAT::removeDBRecAssoc(DBIPos_t pos, BOOL bDeleteNext, BOOL bDeleteSub)
{
	DMDBU rec;
	HRESULT hr;
	if (FAILED(hr = getDBRecAt(pos, rec)))
		return hr;
	RN rn2 = rec.r0.next;
	RN rn3 = RN_NIL;
	saveUndo_SetDBRecAt(pos, rec);
	DMDBU reco = rec;
	if (bDeleteSub) {
		switch (pos.db()) {
		case 4: case 9: case 14: {
			intersectDBRecAndDBRec(pos, DBIPos_t(rec.r4.stolen), 0, 0);
			rn3 = rec.r4.stolen;
			rec.r4.stolen = RN_END;
			intersectDBRecAndDBRec(pos, DBIPos_t(rec.r4.stolen), 0, 1);
			break;
		}
		}
	}

	intersectDBRecAndDBRec(pos, DBIPos_t(rn2), 1, 0);
	rec.r0.next = RN_END; setRecAt(pos.db(), pos.idx(), rec);
	intersectDBRecAndDBRec(pos, DBIPos_t(rec.r0.next), 1, 1);

	if (bDeleteNext) {
		if (FAILED(hr = removeDBRecAssoc(rn2, bDeleteNext, TRUE)))
			return hr;
	}
	if (bDeleteSub) {
		if (FAILED(hr = removeDBRecAssoc(rn3, TRUE, bDeleteSub)))
			return hr;
	}
	OnChangeDBRec(pos, DDSRF_NEW_NEXT|DDSRF_NEW_STOLEN, reco, rec), SetModified();
	//if (!bDeleteNext && !IsThisAsIs(rn2))
	//	OnLinkedDBRec(DBIPos_t(rn2));
	//if (!bDeleteSub && !IsThisAsIs(rn3))
	//	OnLinkedDBRec(DBIPos_t(rn3));
	return S_OK;
}

void CDDAT::intersectAtlasBitAndDBRec(const AtlasPos_t &pos, RN rn, BOOL fNext, BOOL bSet)
{
	_ASSERTE(rn != RN_END && rn != RNff80);
	if (bSet) {
		m_bt.Insert(DBIPos_t(rn), CDMDBRBT(pos));
	} else {
		m_bt.RemoveValue(CDMDBRBT(pos));
	}
//	if (bSet) {
//		m_mmapDB2Atlas.insert(BTDB2AtlasMap::value_type(DBIPos_t(rn), pos));
//	} else {
//		BTDB2AtlasMap::iterator
//			iterPos = m_mmapDB2Atlas.lower_bound(DBIPos_t(rn)),
//			iterEnd = m_mmapDB2Atlas.upper_bound(DBIPos_t(rn));
//		while (iterPos != iterEnd) {
//			if (iterPos->second == pos) {
//				m_mmapDB2Atlas.erase(iterPos);
//				break;
//			}
//			iterPos++;
//		}
//	}
}

void CDDAT::intersectDBRecAndDBRec(const DBIPos_t &p, const DMDBU &rec, BOOL bSet)
{
	intersectDBRecAndDBRec(p, DBIPos_t(rec.r0.next), 1, bSet);
	switch (p.db()) {
	case 4:
	case 9:
	case 14:
		intersectDBRecAndDBRec(p, DBIPos_t(rec.r4.stolen), 0, bSet); break;
	}
}

void CDDAT::intersectDBRecAndDBRec(const DBIPos_t &p, const DBIPos_t &p2, BOOL fNext, BOOL bSet)
{
	if (bSet) {
		m_bt.Insert(p2, CDMDBRBT(p, fNext ? true : false));
	} else {
		m_bt.RemoveValue(CDMDBRBT(p, fNext ? true : false));
	}
//	if (bSet) {
//		m_mmapDB2DB.insert(BTDB2DBMap::value_type(p2, p));
//	} else {
//		BTDB2DBMap::iterator
//			iterPos = m_mmapDB2DB.lower_bound(p2),
//			iterEnd = m_mmapDB2DB.upper_bound(p2);
//		while (iterPos != iterEnd) {
//			if (iterPos->second == p) {
//				m_mmapDB2DB.erase(iterPos);
//				break;
//			}
//			iterPos++;
//		}
//	}
}

HRESULT CDDAT::BTFromDBRec(const DBIPos_t &pos, RN &rn, int &nLinked) const
{
	std::list<DBIPos_t> l1;
	std::list<AtlasPos_t> l2;
	HRESULT hr;
	BTFromDBRec(pos, l1);
	BTFromDBRec(pos, l2);
	nLinked = l1.size() + l2.size();
	rn = RN_NIL;
	if (nLinked == 1) {
		if (!l1.empty()) {
			DMDBU rec;
			if (FAILED(hr = getDBRecAt(l1.front(), rec)))
				return hr;
			if (DBIPos_t(rec.r0.next) == pos) {
				rn = rec.r0.next;
			} else {
				switch(l1.front().db()) {
				case 4:
				case 9:
				case 14:
					rn = rec.r4.stolen; break;
				}
				if (DBIPos_t(rn) != pos) {
					rn = RN_NIL;
				}
			}
		} else if (!l2.empty()) {
			AtlasBit_t a;
			if (FAILED(hr = GetAtlasBitAt(l2.front(), a)))
				return hr;
			rn = a.r;
		}
	}
	return S_OK;
}

HRESULT CDDAT::SetDBRecAt(const DBIPos_t &pos, const DMDBU &rec)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = setDBRecAt(pos, rec, UIF_NO))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::setDBRecAt(const DBIPos_t &pos, const DMDBU &rec, UINT nUIF)
{
	DMDBU recTar;
	HRESULT hr;
	if (FAILED(hr = getDBRecAt(pos, recTar)))
		return hr;
	DMDBU reco = recTar;
	if (!(nUIF & UIF_UNDO_RUNNING)) {
		saveUndo_SetDBRecAt(pos, recTar);
	}
	UINT nSRf = 0;
	if (rec.r0.next != recTar.r0.next) nSRf |= DDSRF_NEW_NEXT;
	RN rnStolen;
	switch (pos.db()) {
	case 4: case 9: case 14:
		if ((rnStolen = rec.r4.stolen) != recTar.r4.stolen) nSRf |= DDSRF_NEW_STOLEN;
	}
	//OnChangingDBRec(pos);
	intersectDBRecAndDBRec(pos, recTar, 0);
	setRecAt(pos.db(), pos.idx(), recTar = rec);
	intersectDBRecAndDBRec(pos, recTar, 1);
	OnChangeDBRec(pos, nSRf, reco, recTar), SetModified();
	//if ((nSRf & DDSRF_NEW_NEXT) && !IsThisAsIs(recTar.r0.next))
	//	OnLinkedDBRec(DBIPos_t(recTar.r0.next));
	//if ((nSRf & DDSRF_NEW_STOLEN) && !IsThisAsIs(rnStolen))
	//	OnLinkedDBRec(DBIPos_t(rnStolen));
	return S_OK;
}

HRESULT CDDAT::SetAllMapDesc(const AllMapDesc_t &amd)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = setAllMapDesc(amd))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::GetAllMapDesc(AllMapDesc_t &amd)
{
	amd = m_aAllDesc;
	SetModified();
	return S_OK;
}

HRESULT CDDAT::setAllMapDesc(const AllMapDesc_t &s)
{
	saveUndo_SetAllMapDesc(m_aAllDesc);
	m_aAllDesc = s;
	return S_OK;
}

HRESULT CDDAT::InsertDBRecAt(const DBIPos_t &posIdx, const DMDBU &rec)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = insertDBRecAt(posIdx, rec))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::insertDBRecAt(const DBIPos_t &posIdx, const DMDBU &rec)
{
	UINT iInsertBefore = posIdx.idx();
	HRESULT hr;
	UINT iDB = posIdx.db(), iItem = posIdx.idx(), nItems = CntDBRecIn(iDB);
	if (FAILED(hr = insertToDBRecAnd_(iDB, iInsertBefore)))
		return hr;
	if (iInsertBefore != posIdx.idx()) {
		ASSERT(FALSE);
		return DMERR_DBREC_OUT_OF_RANGE;
	}
	setRecAt(iDB, iInsertBefore, rec);
	saveUndo_DeleteDBRecAt(posIdx);
	OnInsertDBRec(posIdx, rec), SetModified();
	m_maskOccupied.Set(iDB, iInsertBefore);
	return S_OK;
}

HRESULT CDDAT::GetDetachedDBRecAt(const DBIPos_t &pos, DMDBU &rec) const
{
	return getDBRecAt(pos, rec);
}

HRESULT CDDAT::AddDBRecTo(UINT iDB, const DMDBU &rec, DBIPos_t &posNew)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = addDBRecTo(iDB, rec, posNew))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

void CDDAT::Dev()
{
#if 0
	BTDB2AtlasMap::iterator
		iterPos = m_mmapDB2Atlas.begin(),
		iterEnd = m_mmapDB2Atlas.end();
	TRACE0("DBI <= Atlas\n");
	for (; iterPos != iterEnd; iterPos++) {
		Sleep(1);
		TRACE3(
			"%04X [%2u,%10u]", 
			(UINT)iterPos->first.r.w,
			(UINT)iterPos->first.db(),
			(UINT)iterPos->first.idx());
		TRACE3(
			"<=[%3u,%2u,%2u]\n",
			(UINT)iterPos->second.Z(),
			(UINT)iterPos->second.X(),
			(UINT)iterPos->second.Y());
	}

	{
		BTDB2DBMap::iterator
			iterPos = m_mmapDB2DB.begin(),
			iterEnd = m_mmapDB2DB.end();
		TRACE0("DBI <= DBI\n");
		for (; iterPos != iterEnd; iterPos++) {
			Sleep(1);
			TRACE3(
				"%04X [%2u,%10u]", 
				(UINT)iterPos->first.r.w,
				(UINT)iterPos->first.db(),
				(UINT)iterPos->first.idx());
			TRACE3(
				"<= %04X [%2u,%10u]\n",
				(UINT)iterPos->second.r.w,
				(UINT)iterPos->second.db(),
				(UINT)iterPos->second.idx());
		}
	}
#elif 1 // DISPLAY DM1 text
	const WORD *pwPos = (const WORD *)m_aAllDesc.fvTextData.GetData();
	const WORD *pwEnd = pwPos + m_aAllDesc.fvTextData.GetSize() / 2;

	CByteArray aText;
	while (pwPos < pwEnd) {
		WORD w = *pwPos;
		BYTE c3 = (w >> 0) & 0x1F;
		BYTE c2 = (w >> 5) & 0x1F;
		BYTE c1 = (w >>10) & 0x1F;
		aText.Add(c1);
		aText.Add(c2);
		aText.Add(c3);
		pwPos++;
	}
	static char *cTbl[32] = {
		"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
		" ",".","!","!","!","!",
	};
	UINT iTextPos = 0;
	UINT iChar = 0, iCharPos = 0;
	UINT iOffset = 0;
	const BYTE *pbPos = aText.GetData(), *pbBegin = pbPos;
	const BYTE *pbEnd = pbPos + aText.GetSize();
	CString s, ss;
	while (pbPos < pbEnd) {
		switch (*pbPos) {
		case 28:
			s += ("_");
			pbPos++; iCharPos++;
			break;
		case 29:
			s += ("?");
			// ASSERT(FALSE);
			pbPos++;
			break;
		case 30:
			ASSERT(pbPos + 1 != pbEnd);
			switch (*(pbPos + 1)) {
			case 0:
				s += ("AND ");
				break;
			case 1:
				s += ("!");
				break;
			case 2:
				s += ("THE ");
				break;
			case 3:
				s += ("YOU ");
				break;
			default:
				ASSERT(FALSE);
				break;
			}
			pbPos++; pbPos++; iCharPos++;
			break;
		case 31:
			s += ("/\n");
			ss.Format("%5u,%04X,%04X,%s", iTextPos, iChar, iOffset/3*2*4, s); s = "";
			TRACE0(ss);
			pbPos++; iCharPos++;
			while ((pbPos - pbBegin) % 3) pbPos++;

			iChar = iCharPos;
			iTextPos++;
			iOffset = pbPos - pbBegin;
			break;
		default:
			s += (cTbl[*pbPos]);
			pbPos++; iCharPos++;
			break;
		}
	}
#elif 0
	UINT iMisc, nMisc = CntDBRecIn(10);
	for (iMisc = 0; iMisc < nMisc; iMisc++) {
		DMDBU rec;
		VERIFY(SUCCEEDED(getDBRecAt(DBIPos_t(10, iMisc), rec)));
		UINT iIdx = rec.r10.w2 & 0x7F;
		ASSERT(iIdx != 0x15); // {VI} KEY
	}

	UINT iMap, nMaps = CntMap();
	for (iMap = 0; iMap < nMaps; iMap++) {
		LevelDesc_t d;
		VERIFY(SUCCEEDED(GetMapDescAt(iMap, d)));
		{
			const BYTE *pbPos = d.arr10_0_3.GetData();
			const BYTE *pbEnd = pbPos + d.arr10_0_3.GetSize();
			for (; pbPos < pbEnd; pbPos++) ASSERT(*pbPos != 0x62);
		}
		{
			const BYTE *pbPos = d.arrWallDeco.GetData();
			const BYTE *pbEnd = pbPos + d.arrWallDeco.GetSize();
			for (; pbPos < pbEnd; pbPos++) ASSERT(*pbPos != 0x62);
		}
	}
#elif 0
	UINT i;
	for (i = 5; i <= 10; i++) {
		UINT x;
		for (x = 0; x < 128; x++) {
			DMDBU rec;
			rec.r0.next = RN_END;
			switch (i) {
			case 5:
			case 6:
			case 10:
				rec.r5.w2 = x |(1<<10)|(1<<11)|(1<<12)|(1<<13);
				break;
			case 8:
				rec.r8.w2 = 0x0100 * x |0x00FF;
				break;
			case 7:
				rec.r7.w2 = 0;
				break;
			case 9:
				rec.r9.stolen = RN_END;
				rec.r9.b6[0] = 0;
				rec.r9.b6[1] = 0;
				rec.r9.w4 = 0x0002 |(0x2000*(x % 4));
				break;
			}
			DBIPos_t pos;
			HRESULT hr;
			VERIFY(SUCCEEDED(hr = addDBRecTo(i, rec, pos)));
		}
	}
#elif 0
	list<BYTE> s;
	UINT i, n = CntDBRecIn(3);
	for (i = 0; i < n; i++) {
		DMDBU rec;
		VERIFY(SUCCEEDED(GetDBRecAt(DBIPos_t(3, i), rec)));
		BYTE x = (BYTE)(rec.r3.w2 & 0x7F);
		s.push_back(x);
	}
	s.sort();
	s.unique();
	while (!s.empty()) {
		TRACE1("&H%02X\n", (UINT)s.front());
		s.pop_front();
	}
#elif 0
	int nUsing[128];
	set<DWORD> m;

	for (int x = 0; x < 128; x++) nUsing[x] = 0;

	int iMap, nMaps = CntMap();
	for (iMap = 0; iMap < nMaps; iMap++) {
		HRESULT hr;
		try {
			Atlas_t a;
			LevelDesc_t s;
			if (FAILED(hr = GetMapAtlasAt(iMap, a))) throw int();
			if (FAILED(hr = GetMapDescAt(iMap, s))) throw int();
			int cx = s.cxLv;
			int cy = s.cyLv;
			for (int y = 0; y < cy; y++) {
				for (int x = 0; x < cx; x++) {
					RN r = a.r[y][x];
					BYTE c = a.c[y][x];
					BYTE t = c / 0x20;
					bool fWall = (t == 0);
					if (r != RN_NIL) {
						ASSERT(t == 0 || t == 1 || t == 2 || t == 4 || t == 5);
						while (r != RN_END) {
							ASSERT(r != RN_NIL);
							DMDBU rec;
							if (FAILED(GetDBRecAt(DBIPos_t(r), rec))) throw int();
							switch (r.db()) {
							case 3:
								{
									ASSERT(t == 0 || t == 1 || t == 2 || t == 5);

									BYTE k = rec.r3.w2 & 0x7F;
									nUsing[k] |= fWall ? 2 : 1;

									m.insert(k * 0x10000UL + rec.r3.w2 / 0x80 + (fWall ? 0x1000000UL : 0));
								}
							}
							r = rec.r0.next;
						}
					}
				}
			}
		} catch (int) {
			ASSERT(FALSE);
		}
	}
	for (int z = 0; z < 128; z++) {
		LPCTSTR psz = "";
		LPCSTR psz4[] = {"", " /F", "W/ ", "W/F"};
		int t[2] = {
			distance(m.lower_bound(0x1000000UL | z * 0x10000UL), m.upper_bound(0x1000000UL | z * 0x10000UL | 0xFFFF)),
			distance(m.lower_bound(0x0000000UL | z * 0x10000UL), m.upper_bound(0x0000000UL | z * 0x10000UL | 0xFFFF)),
		};
		if (nUsing[z]) AfxTrace("&H%02X, %s, Wv:%3d, Fv:%3d\n", z, psz4[nUsing[z] & 3], t[0], t[1]);
		Sleep(1);
	}
#elif 0
	for (int i = 0; i < CntDBRecIn(4); i++) {
		DMDBU rec;
		HRESULT hr;
		VERIFY(SUCCEEDED(hr = GetDBRecAt(DBIPos_t(4, i), rec)));
		if (rec.r4.b4 == 0x19) {
			rec.r4.b4 = 0x22;
			VERIFY(SUCCEEDED(hr = SetDBRecAt(DBIPos_t(4, i), rec)));
		}
	}
#elif 0
	CDDATUndoLock lock(*this, TRUE);
	Atlas_t a;
	HRESULT hr;
	VERIFY(SUCCEEDED(hr = GetMapAtlasAt(0, a)));

	for (int fy = 0; fy < 32; fy++)
		for (int fx = 0; fx < 32; fx++)
			a.c[fy][fx] = 0x20,
			a.r[fy][fx] = RN_NIL;

	for (int vy = 0; vy < 4; vy++) {
		int y = 3 * vy + 1;
		for (int vx = 0; vx < 16; vx++) {
			int x = vx;

			int index = x + 16 * vy;
			{
				DBIPos_t pos;
				DMDBU rec;
				ZeroMemory(&rec, sizeof(rec));
				rec.r2.next = RN_END;
				rec.r2.w2 = 0
					| 1
					| ((0x19) << 3)
					| ((index & 0x0001) ?     2 : 0)
					| ((index & 0x0002) ?  2048 : 0)
					| ((index & 0x0004) ?  4096 : 0)
					| ((index & 0x0008) ?  8192 : 0)
					| ((index & 0x0010) ? 16384 : 0)
					| ((index & 0x0020) ? 32768 : 0)
					;
				VERIFY(SUCCEEDED(hr = AddDBRecTo(2, rec, pos)));
				a.r[x][y] = RNco(2, pos.idx(), 2);
				a.c[x][y] = 0x10;
			}
		}
	}
	VERIFY(SUCCEEDED(hr = SetMapAtlasAt(0, a)));
	lock.Unlock(TRUE);
#elif 0
	UINT i, n = CntDBRecIn(2);
	for (i = 0; i < n; i++) {
		HRESULT hr;
		VERIFY(SUCCEEDED(hr = GetDBRecAt(DBIPos_t(2, i), rec)));
		WORD w2 = rec.r2.w2;
	}
#elif 0
	set<DWORD> m;

	union {
		struct {
			UINT idx:10;
			UINT wall:1;
			UINT type:7;
		} t;
		DWORD x;
	} v;

	v.x = 0;

	int iMap, nMaps = CntMap();
	for (iMap = 0; iMap < nMaps; iMap++) {
		HRESULT hr;
		try {
			Atlas_t a;
			LevelDesc_t s;
			if (FAILED(hr = GetMapAtlasAt(iMap, a))) throw int();
			if (FAILED(hr = GetMapDescAt(iMap, s))) throw int();
			int cx = s.cxLv;
			int cy = s.cyLv;
			for (int y = 0; y < cy; y++) {
				for (int x = 0; x < cx; x++) {
					RN r = a.r[y][x];
					BYTE c = a.c[y][x];
					BYTE t = c / 0x20;
					bool fWall = (t == 0);
					if (r != RN_NIL) {
						ASSERT(t == 0 || t == 1 || t == 2 || t == 4 || t == 5);
						while (r != RN_END) {
							ASSERT(r != RN_NIL);
							DMDBU rec;
							if (FAILED(GetDBRecAt(DBIPos_t(r), rec))) throw int();
							switch (r.db()) {
							case 3:
								{
									ASSERT(t == 0 || t == 1 || t == 2 || t == 5);

									BYTE k = rec.r3.w2 & 0x7F;

									v.t.idx = r.idx();
									v.t.wall = fWall ? 1 : 0;
									v.t.type = k;

									m.insert(v.x);
								}
							}
							r = rec.r0.next;
						}
					}
				}
			}
		} catch (int) {
			ASSERT(FALSE);
		}
	}
	for (int z = 0; z < 128; z++) {
		LPCTSTR psz = "";
		LPCSTR psz4[] = {" /F", "W/ "};
		for (int w = 0; w < 2; w++) {
			v.t.idx = 0;
			v.t.wall = w;
			v.t.type = z;
			DWORD v0 = v.x;
			v.t.idx = -1;
			DWORD v1 = v.x;
			set<DWORD>::iterator
				iterPos = m.lower_bound(v0),
				iterEnd = m.upper_bound(v1);
			if (iterPos != iterEnd) {
				CString s, ss;
				s.Format("%02X:%s:", z, psz4[w]);
				int i = 0;
				for (; iterPos != iterEnd; iterPos++, i++) {
					v.x = *iterPos;
					if (i != 0) s += ",";
					ss.Format("#%04u", v.t.idx);
					s += ss;
				}
				TRACE1("%s\n", (LPCSTR)s);
				Sleep(1);
			}
		}
	}
#elif 0
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 3; x++) {
			DMDBU rec[2];
			ZeroMemory(rec, sizeof(rec));
			//
			BYTE t;
			switch (x) {
			case 0: t = 0x20; break;
			case 1: t = 0x45; break;
			case 2: t = 0x3D; break;
			}

			BYTE fSw[4] = {0,0,1,1};
			BYTE fI [4] = {0,0,0,0};
			BYTE fAA[4] = {2,2,2,2};

			rec[0].r3.w2 =
			rec[1].r3.w2 = t;
			rec[0].r3.w4 = 0x0080 |(fSw[y] ? 0x004 : 0x000) |((fAA[y] & 3) << 3) |(fI[y] ? 0x020 : 0x000);
			rec[1].r3.w4 = 0x0080 |(fSw[y] ? 0x004 : 0x000) |((fAA[y] & 3) << 3) |(fI[y] ? 0x020 : 0x000);
			rec[0].r3.w6 = 0x0010 |((1+x)<<11) |((0)<< 6);
			rec[1].r3.w6 = 0x0010 |((1+x)<<11) |((4)<< 6);
			//
			HRESULT hr;
			DBIPos_t pos;
			rec[1].r3.next = RN_END;
			VERIFY(SUCCEEDED(hr = AddDBRecTo(3, rec[1], pos)));
			rec[0].r3.next = RNco(pos.db(), pos.idx(), 1);
			VERIFY(SUCCEEDED(hr = AddDBRecTo(3, rec[0], pos)));
			AtlasBit_t ab;
			VERIFY(SUCCEEDED(hr = GetAtlasBitAt(AtlasPos_t(1, 1+x, 7+y), ab)));
			ab.c |= 0x10;
			ab.r = RNco(pos.db(), pos.idx(), 1);
			VERIFY(SUCCEEDED(hr = SetAtlasBitAt(AtlasPos_t(1, 1+x, 7+y), ab)));
		}
	}
#elif 0
	CDDATUndoLock lock(*this, TRUE);
	Atlas_t a;
	HRESULT hr;
	VERIFY(SUCCEEDED(hr = GetMapAtlasAt(1, a)));
	int ys[] = {1, 2, 3, 4};
	int rs[] = {0, 0, 0, 0};
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 32; x++) {
			int v = 0x80 +y*32 +x;
			a.c[x][ys[y]] |= 0x10;
			{
				DBIPos_t pos;
				DMDBU rec;
				ZeroMemory(&rec, sizeof(rec));
				rec.r2.w2 = (43 << 3)
					|((v & 0x01) ? (1<< 0) : 0)
					|((v & 0x02) ? (1<< 1) : 0)
					|((v & 0x04) ? (1<< 2) : 0)
					|((v & 0x08) ? (1<<11) : 0)
					|((v & 0x10) ? (1<<12) : 0)
					|((v & 0x20) ? (1<<13) : 0)
					|((v & 0x40) ? (1<<14) : 0)
					|((v & 0x80) ? (1<<15) : 0)
					;
				rec.r2.next = RN_END;
				VERIFY(SUCCEEDED(hr = AddDBRecToEmpty(2, rec, pos)));
				a.r[x][ys[y]] = RNco(pos.db(), pos.idx(), rs[y]);
			}
		}
	}
	VERIFY(SUCCEEDED(hr = SetMapAtlasAt(1, a)));
	lock.Unlock(TRUE);
#elif 0
	UINT iMap, nMaps = CntMap();
	CString strText;
	for (iMap = 0; iMap < nMaps; iMap++) {
		HRESULT hr;
		Map_t *p;
		if (FAILED(hr = getMapAt(iMap, p)))
			return;
		Atlas_t &a = p->aAtlas;
		LevelDesc_t &s = p->aDesc;
		BYTE x, y, cx = s.cxLv, cy = s.cyLv;
		for (y = 0; y < cy; y++) {
			for (x = 0; x < cx; x++) {
				BYTE c = a.c[y][x];
				RN rn = a.r[y][x];
				CString str;
				str.Format("[%2d,%2d,%2d]%02X\r\n", x, y, iMap, 0+c);
				if (c & 0x10) {
					walkTest(str, rn, 1);
				}
				strText += str;
			}
		}
	}
	CStr2Clipbrd().SetClipboardTextData2(strText);
#elif 0
	UINT i;
	for (i = 0; i < CntDBRecIn(2); i++) {
		DMDBU rec;
		getRecAt(2, i, rec);
		int n = 0;
		RN rn;
		BTFromDBRec(DBIPos_t(2, i), rn, n);
		if (n == 0) continue;

		TRACE2("TEXT(%4u) %s\n", i, computeDM1Text(rec.r2.w2 / 8));
		Sleep(10);
	}
	for (i = 0; i < CntDBRecIn(7); i++) {
		DMDBU rec;
		getRecAt(7, i, rec);
		int n = 0;
		RN rn;
		BTFromDBRec(DBIPos_t(7, i), rn, n);
		if (n == 0) continue;

		getRecAt(2, rec.r7.w2 & 1023, rec);

		TRACE2("SCRO(%4u) %s\n", i, computeDM1Text(rec.r2.w2 / 8));
		Sleep(10);
	}
#elif 1

#endif
}

HRESULT CDDAT::RemoveDBRecAssoc(const DBIPos_t &posIdx, BOOL bDeleteNext, BOOL bDeleteSub)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = removeDBRecAssoc(posIdx, bDeleteNext, bDeleteSub))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::addDBRecToEmpty(UINT iDB, const DMDBU &rec, DBIPos_t &posNew)
{
	UINT i = 0, n = CntDBRecIn(iDB);
	for (; i < n; i++) {
		if (m_maskOccupied.IsSet(iDB, i))
			continue;
		DBIPos_t pos(iDB, i);
		list<AtlasPos_t> v;
		m_bt.Lookup(pos, v);
		int nUsed = v.size();
		if (nUsed == 0) {
			list<DBIPos_t> v;
			m_bt.Lookup(pos, v);
			for (; !v.empty(); v.pop_front()) {
				_ASSERTE(v.front() != DBIPos_t::END);
				if (v.front() != DBIPos_t::NIL) nUsed++;
			}
		}
		if (nUsed == 0) {
			HRESULT hr;
			if (FAILED(hr = setDBRecAt(pos, rec, UIF_NO)))
				return hr;
			posNew = pos;
			m_maskOccupied.Set(iDB, i);
			return S_OK;
		}
	}
	return addDBRecTo(iDB, rec, posNew);
}

HRESULT CDDAT::AddDBRecToEmpty(UINT iDB, const DMDBU &rec, DBIPos_t &posNew)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = addDBRecToEmpty(iDB, rec, posNew))) {
		if (iDB == 3) {
			if (hr == DMERR_NO_MORE_SPACE) {
				hr = addDBRecToEmpty(11, rec, posNew);

				if (hr == DMERR_NO_MORE_SPACE) {
					hr = addDBRecToEmpty(12, rec, posNew);

					if (hr == DMERR_NO_MORE_SPACE) {
						hr = addDBRecToEmpty(13, rec, posNew);
					}
				}
			}
		}
	}
	if (FAILED(hr))
		return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::RemoveDBRecAt(const DBIPos_t &posIdx)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = removeDBRecFrom(posIdx.db(), posIdx.idx()))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::removeDBRecFrom(UINT iDB, UINT iRemoveAt)
{
	if (iDB >= 16) return DMERR_DBREC_OUT_OF_RANGE;
	UINT cb1 = sizeInDBRec(iDB);
	UINT nInDB = m_nRec[iDB];
	if (nInDB > DDAT_MAX_RECS) return DMERR_DBREC_OUT_OF_RANGE;
	UINT iIdx = iRemoveAt;
	if (nInDB <= iIdx) return DMERR_DBREC_OUT_OF_RANGE;
	if (nInDB != 0) {
		UINT i;
		for (i = iIdx; i < nInDB-1; i++) {
			DBIPos_t p0(iDB, i+1), p1(iDB, i);
			DMDBU recMove;
			HRESULT hr;
			list<DBIPos_t> l1;
			list<AtlasPos_t> l2;
			VERIFY(SUCCEEDED(hr = BTFromDBRec(p0, l1)) || hr == DMERR_NO_ITEM);
			VERIFY(SUCCEEDED(hr = BTFromDBRec(p0, l2)) || hr == DMERR_NO_ITEM);
			VERIFY(SUCCEEDED(hr = getDBRecAt(p0, recMove)));
			VERIFY(SUCCEEDED(hr = setDBRecAt(p1, recMove, UIF_NO)));
			while (!l1.empty()) {
				DMDBU recDet;
				if (SUCCEEDED(hr = GetDetachedDBRecAt(l1.front(), recDet))) {
					int n = 0;
					if (DBIPos_t(recDet.r0.next) == p0) {
						recDet.r0.next.idx(p1.idx());
						n++;
					} else {
						switch (l1.front().db()) {
						case 4: case 9: case 14:
							if (DBIPos_t(recDet.r4.stolen) == p0) {
								recDet.r4.stolen.idx(p1.idx());
								n++;
							}
							break;
						}
					}
					if (n) {
						VERIFY(SUCCEEDED(setDBRecAt(l1.front(), recDet, UIF_NO)));
					}
				}
				l1.pop_front();
			}
			while (!l2.empty()) {
				AtlasBit_t o;
				if (SUCCEEDED(hr = GetAtlasBitAt(l2.front(), o))) {
					_ASSERTE(DBIPos_t(o.r) == p0);
					o.r.idx(p1.idx());
					VERIFY(SUCCEEDED(hr = setAtlasBitAt(l2.front(), o, UIF_NO)));
				}
				l2.pop_front();
			}
		}
		DMDBU recDie;
		memset(&recDie, 0xff, sizeof(recDie));
		HRESULT hr;
		VERIFY(SUCCEEDED(hr = setDBRecAt(DBIPos_t(iDB, i), recDie, UIF_NO)));
	}
	m_nRec[iDB] = nInDB-1;
	return TRUE;
}

HRESULT CDDAT::SetMapCnt(UINT nMaps)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = setMapCnt(nMaps))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::setMapCnt(UINT nMaps)
{
	HRESULT hr;
	nMaps = _MIN(nMaps, 64U);
	UINT nMapsNow = CntMap();
	if (nMaps < nMapsNow) {
		while (nMaps != nMapsNow) {
			if (FAILED(hr = deleteMapAt(nMapsNow - 1, UIF_NO)))
				return hr;
			nMapsNow--;
		}
	} else if (nMaps > nMapsNow) {
		while (nMapsNow != nMaps) {
			UINT i;
			if (FAILED(hr = addEmptyMap(i)))
				return hr;
			nMapsNow++;
		}
	}
	return S_OK;
}

void CDDAT::getRecAt(UINT db, UINT idx, DMDBU &rec) const
{
	db = db % 16; idx = idx % DDAT_MAX_RECS;
	switch (db) {
	case 0: rec.r0 = m_sRec[idx].r0; break;
	case 1: rec.r1 = m_sRec[idx].r1; break;
	case 2: rec.r2 = m_sRec[idx].r2; break;
	case 3: rec.r3 = m_sRec[idx].r3; break;
	case 4: rec.r4 = m_sRec[idx].r4; break;
	case 5: rec.r5 = m_sRec[idx].r5; break;
	case 6: rec.r6 = m_sRec[idx].r6; break;
	case 7: rec.r7 = m_sRec[idx].r7; break;
	case 8: rec.r8 = m_sRec[idx].r8; break;
	case 9: rec.r9 = m_sRec[idx].r9; break;
	case 10: rec.r10 = m_sRec[idx].r10; break;
	case 11: rec.r11 = m_sRec[idx].r11; break;
	case 12: rec.r12 = m_sRec[idx].r12; break;
	case 13: rec.r13 = m_sRec[idx].r13; break;
	case 14: rec.r14 = m_sRec[idx].r14; break;
	case 15: rec.r15 = m_sRec[idx].r15; break;
	}
}

void CDDAT::setRecAt(UINT db, UINT idx, const DMDBU &rec)
{
	db = db % 16; idx = idx % DDAT_MAX_RECS;
	switch (db) {
	case 0: m_sRec[idx].r0 = rec.r0; break;
	case 1: m_sRec[idx].r1 = rec.r1; break;
	case 2: m_sRec[idx].r2 = rec.r2; break;
	case 3: m_sRec[idx].r3 = rec.r3; break;
	case 4: m_sRec[idx].r4 = rec.r4; break;
	case 5: m_sRec[idx].r5 = rec.r5; break;
	case 6: m_sRec[idx].r6 = rec.r6; break;
	case 7: m_sRec[idx].r7 = rec.r7; break;
	case 8: m_sRec[idx].r8 = rec.r8; break;
	case 9: m_sRec[idx].r9 = rec.r9; break;
	case 10: m_sRec[idx].r10 = rec.r10; break;
	case 11: m_sRec[idx].r11 = rec.r11; break;
	case 12: m_sRec[idx].r12 = rec.r12; break;
	case 13: m_sRec[idx].r13 = rec.r13; break;
	case 14: m_sRec[idx].r14 = rec.r14; break;
	case 15: m_sRec[idx].r15 = rec.r15; break;
	}
}

BOOL CDDAT::GetModified() const
{
	return m_fModified;
}

void CDDAT::SetModified(BOOL fModified)
{
	m_fModified = fModified;
}

HRESULT CDDAT::removeTailDBRec(UINT iDB)
{
	UINT nRec = CntDBRecIn(iDB);
	if (nRec < 1) return DMERR_DBREC_OUT_OF_RANGE;
	return removeDBRecFrom(iDB, nRec - 1);
}

HRESULT CDDAT::Optimize()
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = optimize())) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::optimize()
{
	OptimizeTester_t tester;
	UINT iMap, nMaps = CntMap();

	tester.Init();

	for (iMap = 0; iMap < nMaps; iMap++) {
		HRESULT hr;
		LevelDesc_t m;
		Atlas_t g;
		if (FAILED(hr = GetMapDescAt(iMap, m))) return hr;
		if (FAILED(hr = GetMapAtlasAt(iMap, g))) return hr;

		UINT cx = m.cxLv, cy = m.cyLv;

		for (UINT y = 0; y < cy; y++) {
			for (UINT x = 0; x < cx; x++) {
				if (g.c[y][x] & 0x10) {
					trackTarget(tester, g.r[y][x]);
				}
			}
		}
	}
	UINT z;
	for (z = 0; z < 16; z++) {
		tester.Optz(z, CntDBRecIn(z));
	}

	for (iMap = 0; iMap < nMaps; iMap++) {
		HRESULT hr;
		LevelDesc_t m;
		Atlas_t g;
		if (FAILED(hr = GetMapDescAt(iMap, m))) return hr;
		if (FAILED(hr = GetMapAtlasAt(iMap, g))) return hr;

		UINT cx = m.cxLv, cy = m.cyLv;

		int s = 0;
		for (UINT y = 0; y < cy; y++) {
			for (UINT x = 0; x < cx; x++) {
				if (g.c[y][x] & 0x10) {
					if (g.r[y][x] == RN_NIL || g.r[y][x] == RN_END) {
						// for BUG FIX
						g.r[y][x] = RN_NIL;
						g.c[y][x] &= ~0x10;
						s++;
					} else {
						int iz = g.r[y][x].db();
						int ix = g.r[y][x].idx();
						int ixNew = tester.TrackNewPos(iz, ix);
						if (ixNew != ix) {
							if (ixNew < 0) {
								g.r[y][x] = RN_NIL;
								g.c[y][x] &= ~0x10;
								s++;
							} else {
								g.r[y][x].idx(ixNew);
								s++;
							}
#ifdef _DEBUG
							//TCHAR tc[256];
							//_stprintf(tc, "1:[%2u,%2u,%2u]->(%2u,%4u)->%4d\n", iMap, x, y, iz, ix, ixNew); TRACE0(tc); Sleep(1);
#endif
						}
					}
				}
			}
		}

		if (s != 0) {
			if (FAILED(hr = setMapAtlasAt(iMap, g, 0))) return hr;
		}
	}
	for (z = 0; z < 16; z++) {
		UINT x;
		UINT cx = CntDBRecIn(z);
		for (x = 0; x < cx; x++) {
			int s = 0;
			int xNew = tester.TrackNewPos(z, x);
			if (xNew < 0) {

			} else {
				HRESULT hr;
				DMDBU rec;
				if (FAILED(hr = getDBRecAt(DBIPos_t(z, x), rec))) return hr;

				if (!IsThisAsIs(rec.r0.next)) {
					int iz = rec.r0.next.db();
					int ix = rec.r0.next.idx();
					int ixNew = tester.TrackNewPos(iz, ix);
					if (ixNew != ix) {
						if (ixNew < 0) {
							// for BUG FIX
							rec.r0.next = RN_END;
							s++;
						} else {
							rec.r0.next.idx(ixNew);
							s++;
						}
					}
#ifdef _DEBUG
					//TCHAR tc[256];
					//_stprintf(tc, "2:{%2u,%4u}->%4d:(%2u,%4u)->%4d\n", z, x, xNew, iz, ix, ixNew); TRACE0(tc); Sleep(1);
#endif
				} else if (rec.r0.next == RN_NIL) {
					// for BUG FIX
					rec.r0.next = RN_END;
					s++;
				}

				switch (z) {
#if _DDAT_OPTS_DM1SPECIFIC
				case 2:
					{
						int iz = 2;
						int ix = rec.r2.w2 % DDAT_MAX_RECS;
						int ixNew = tester.TrackNewPos(iz, ix);
						if (ixNew != ix) {
							if (ixNew < 0) {
								ASSERT(FALSE);
								ixNew = 0;
							} else {

							}
							s++;
							rec.r2.w2 -= rec.r2.w2 & 1023;
							rec.r2.w2 |= ixNew;
						}
					}
					break;
#endif
				case 4:
				case 9:
				case 14:
					if (!IsThisAsIs(rec.r4.stolen)) {
						int iz = rec.r4.stolen.db();
						int ix = rec.r4.stolen.idx();
						int ixNew = tester.TrackNewPos(iz, ix);
						if (ixNew != ix) {
							if (ixNew < 0) {
								rec.r4.stolen = RN_END;
								s++;
							} else {
								rec.r4.stolen.idx(ixNew);
								s++;
							}
						}
#ifdef _DEBUG
						//TCHAR tc[256];
						//_stprintf(tc, "3:{%2u,%4u}->%4d:(%2u,%4u)->%4d\n", z, x, xNew, iz, ix, ixNew); TRACE0(tc); Sleep(1);
#endif
					} else if (rec.r4.stolen == RN_NIL) {
						// for BUG FIX
						rec.r4.stolen = RN_END;
						s++;
					}
					break;
				}

				if (FAILED(hr = setDBRecAt(DBIPos_t(z, xNew), rec, 0))) return hr;
			}
		}
	}
	for (z = 0; z < 16; z++) {
		UINT x = 0;
		UINT c = CntDBRecIn(z) - tester.GetNewTblLen(z);
		for (; x < c; x++) {
			HRESULT hr;
			if (FAILED(hr = removeTailDBRec(z))) return hr;
		}
	}

	return S_OK;
}

HRESULT CDDAT::trackTarget(OptimizeTester_t &tester, RN rn)
{
	if (IsThisAsIs(rn)) return S_OK;
	if (tester.IsMarkedAsUsing(rn.db(), rn.idx())) return S_OK;

	HRESULT hr;
	DMDBU rec;
	if (FAILED(hr = getDBRecAt(DBIPos_t(rn), rec))) return hr;

	tester.MarkAsUsing(rn.db(), rn.idx());

	int nCount = 0, nDone = 0;

	nCount++, nDone += SUCCEEDED(hr = trackTarget(tester, rec.r0.next)) ? 1 : 0;

	switch (rn.db()) {
#if _DDAT_OPTS_DM1SPECIFIC
	case 2:
		nCount++, nDone += SUCCEEDED(hr = trackTarget(tester, RNco(2, rec.r2.w2 % DDAT_MAX_RECS, 0))) ? 1 : 0;
		break;
#endif
	case 4:
	case 9:
	case 14:
		nCount++, nDone += SUCCEEDED(hr = trackTarget(tester, rec.r4.stolen)) ? 1 : 0;
		break;
	}

	return (nCount == nDone) ? S_OK : S_FALSE;
}

HRESULT CDDAT::OptimizeFlatten()
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = optimizeFlatten())) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::optimizeFlatten()
{
	UINT iMap, nMaps = CntMap();

	CDMDBRMask mask;

	HRESULT hr;
	for (iMap = 0; iMap < nMaps; iMap++) {
		LevelDesc_t d;
		Atlas_t g;
		if (FAILED(hr = GetMapDescAt(iMap, d))) return hr;
		if (FAILED(hr = GetMapAtlasAt(iMap, g))) return hr;

		int nMod = 0;
		UINT cx = d.cxLv, cy = d.cyLv;
		for (UINT y = 0; y < cy; y++) {
			for (UINT x = 0; x < cx; x++) {
				RN rn = g.r[y][x];
				if (g.c[y][x] & 0x10 && !IsThisAsIs(rn)) {
					if (mask.IsSet(rn)) {
						if (FAILED(hr = copyRecords(rn, rn, WalkRecord_t(mask))))
							return hr;
						g.r[y][x] = rn;
						nMod++;
					} else {
						if (FAILED(hr = walkRecords(rn, WalkRecord_t(mask))))
							return hr;
					}
				}
			}
		}

		if (nMod != 0) {
			if (FAILED(hr = setMapAtlasAt(iMap, g, UIF_NO))) return hr;
		}
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// OptimizeTester_t

void OptimizeTester_t::Optz(UINT z, UINT cx)
{
	UINT x1;
	for (x1 = 0; x1 < DDAT_MAX_RECS; x1++) iOld2New[z][x1] = -1;

	UINT x2 = 0;
	for (x1 = 0; x1 < cx; x1++) {
		if (IsMarkedAsUsing(z, x1)) {
			iOld2New[z][x1] = x2;
			x2++;
		}
	}
	nTblRec[z] = x2;

#ifdef _DEBUG
	//for (x1 = 0; x1 < cx; x1++) {
	//	TCHAR tc[256];
	//	_stprintf(tc, "M:(%2u,%4u)->(%2u,%4d)\n", z, x1, z, iOld2New[z][x1]); TRACE0(tc); Sleep(1);
	//}
#endif
}

//////////////////////////////////////////////////////////////////////
// CDDATWork

#include "Exp00069Doc.h"

bool CDDATWork::LoadFrom(CDDATx::CSerializedReader &fLoad, BOOL fLE, bool fDM2_EXTENDED_MAP)
{
	TRY {
		StayUpdate(TRUE);
		if (!CDDAT::LoadFrom(fLoad, fLE, fDM2_EXTENDED_MAP)) return false;
		StayUpdate(FALSE);
		return true;
	} CATCH_ALL(e) {
		StayUpdate(FALSE);
		THROW_LAST();
		return false;
	} END_CATCH_ALL
}

void CDDATWork::OnChangeAtlasBit(const AtlasPos_t &pos)
{
	setModified();

	if (ifStayUpdate()) return;

	m_lpNMTo->UpdateAllViews(NULL, DPM_CHANGED_ATLASBIT, (CObject *)&pos);
}

void CDDATWork::OnChangeLevelDesc(UINT iMapAt)
{
	setModified();

	if (ifStayUpdate()) return;

	ChangeLevelDesc_t notify;
	notify.iMapAt = iMapAt;

	m_lpNMTo->UpdateAllViews(NULL, DPM_CHANGED_LEVELDESC, (CObject *)&notify);
}

void CDDATWork::OnChangeAtlas(UINT iMapAt)
{
	setModified();

	if (ifStayUpdate()) return;

	RepaintAtlas_t notify;
	notify.iMapAt = iMapAt;

	m_lpNMTo->UpdateAllViews(NULL, DPM_CHANGED_ATLAS, (CObject *)&notify);
}

void CDDATWork::OnDeleteMap(UINT iMapAt)
{
	setModified();

	if (ifStayUpdate()) return;

	DeleteMap_t notify;
	notify.iMapAt = iMapAt;

	m_lpNMTo->UpdateAllViews(NULL, DPM_DELETED_MAP, (CObject *)&notify);
}

void CDDATWork::OnChangeDBRec(const DBIPos_t &pos, UINT nSRf)
{
	setModified();

	if (ifStayUpdate()) return;

	ChangeDBRec_t notify;
	notify.pos = pos;
	notify.nSRf = nSRf;

	m_lpNMTo->UpdateAllViews(NULL, DPM_CHANGED_DBITEM, (CObject *)&notify);
}

void CDDATWork::OnInsertMap(UINT iMapAt)
{
	setModified();

	if (ifStayUpdate()) return;

	InsertMap_t notify;
	notify.iMapAt = iMapAt;

	m_lpNMTo->UpdateAllViews(NULL, DPM_INSERTED_MAP, (CObject *)&notify);
}

void CDDATWork::OnInsertDBRec(const DBIPos_t &pos)
{
	setModified();

	if (ifStayUpdate()) return;

	InsertDBRec_t notify;
	notify.pos = pos;
	m_lpNMTo->UpdateAllViews(NULL, DPM_INSERTED_DBITEM, (CObject *)&notify);
}

void CDDATWork::OnDeleteDBRec(const DBIPos_t &pos)
{
	setModified();

	if (ifStayUpdate()) return;

	InsertDBRec_t notify;
	notify.pos = pos;
	m_lpNMTo->UpdateAllViews(NULL, DPM_DELETED_DBITEM, (CObject *)&notify);
}

void CDDATWork::OnChangingDBRec(const DBIPos_t &pos)
{
	setModified();

	if (ifStayUpdate()) return;

	ChangeDBRec_t notify;
	notify.pos = pos;

	m_lpNMTo->UpdateAllViews(NULL, DPM_CHANGING_DBITEM, (CObject *)&notify);
}

void CDDATWork::OnLinkedDBRec(const DBIPos_t &pos)
{
	setModified();

	if (ifStayUpdate()) return;

	ChangeDBRec_t notify;
	notify.pos = pos;

	m_lpNMTo->UpdateAllViews(NULL, DPM_LINKED_DBITEM, (CObject *)&notify);
}

HRESULT CDDAT::saveUndo_SetDBRecAt(const DBIPos_t &pos, const DMDBU &rec)
{
	CDDATUndoListItem *pItem = new CDDATUndoListItem();
	pItem->wp = DDATUC_SET_DBREC;
	pItem->lp = 0;
	CArchive ar(&pItem->f, CArchive::store);
	ar << pos;
	DMDBU::Ar(ar) << rec;
	ar.Close();
	AddUndoItem(pItem);

	return S_OK;
}

HRESULT CDDAT::saveUndo_DeleteDBRecAt(const DBIPos_t &pos)
{
	CDDATUndoListItem *pItem = new CDDATUndoListItem();
	pItem->wp = DDATUC_DELETE_RECDB;
	pItem->lp = 0;
	CArchive ar(&pItem->f, CArchive::store);
	ar << pos;
	ar.Close();
	AddUndoItem(pItem);

	return S_OK;
}

HRESULT CDDAT::saveUndo_SetAtlasBitAt(const AtlasPos_t &p, AtlasBit_t &bit)
{
	CDDATUndoListItem *pItem = new CDDATUndoListItem();
	pItem->wp = DDATUC_SET_ATLAS_BIT;
	pItem->lp = 0;
	CArchive ar(&pItem->f, CArchive::store);
	ar << p << bit;
	ar.Close();
	AddUndoItem(pItem);

	return S_OK;
}

HRESULT CDDAT::saveUndo_SetAtlas(UINT iMap, Atlas_t &at)
{
	CDDATUndoListItem *pItem = new CDDATUndoListItem();
	pItem->wp = DDATUC_SET_ATLAS;
	pItem->lp = 0;
	CArchive ar(&pItem->f, CArchive::store);
	ar << iMap;
	ar.Write(&at, sizeof(Atlas_t));
	ar.Close();
	AddUndoItem(pItem);

	return S_OK;
}

HRESULT CDDAT::saveUndo_SetLevelDesc(UINT iMap, LevelDesc_t &ld)
{
	CDDATUndoListItem *pItem = new CDDATUndoListItem();
	pItem->wp = DDATUC_SET_LEVEL_DESC;
	pItem->lp = 0;
	CArchive ar(&pItem->f, CArchive::store);
	ar << iMap << ld;
	ar.Close();
	AddUndoItem(pItem);

	return S_OK;
}

HRESULT CDDAT::saveUndo_DeleteMap(UINT iMap)
{
	CDDATUndoListItem *pItem = new CDDATUndoListItem();
	pItem->wp = DDATUC_DELETE_MAP;
	pItem->lp = 0;
	CArchive ar(&pItem->f, CArchive::store);
	ar << iMap;
	ar.Close();
	AddUndoItem(pItem);

	return S_OK;
}

HRESULT CDDAT::saveUndo_InsertMapAt(UINT iMap, Map_t &m)
{
	CDDATUndoListItem *pItem = new CDDATUndoListItem();
	pItem->wp = DDATUC_INSERT_MAP;
	pItem->lp = 0;
	CArchive ar(&pItem->f, CArchive::store);
	ar << iMap;
	ar << m.aDesc;
	ar.Write(&m.aAtlas, sizeof(m.aAtlas));

	ar.Close();
	AddUndoItem(pItem);

	return S_OK;
}

HRESULT CDDAT::saveUndo_SetAllMapDesc(const AllMapDesc_t &s)
{
	CDDATUndoListItem *pItem = new CDDATUndoListItem();
	pItem->wp = DDATUC_SET_ALL_MAP_DESC;
	pItem->lp = 0;
	CArchive ar(&pItem->f, CArchive::store);
	ar << s;
	ar.Close();
	AddUndoItem(pItem);

	return S_OK;
}

void CDDAT::Undo_OnUndoStacked()
{
	m_maskOccupied = CDMDBRMask();
}

HRESULT CDDAT::ScanOrnate(UINT iMap, CDM2OrnateIdxMig &oim)
{
#if 1
	HRESULT hr;
	LevelDesc_t m;
	if (FAILED(hr = GetMapDescAt(iMap, m))) return hr;

	oim.LoadFrom(m);

	return S_OK;
#elif 0
	HRESULT hr;
	LevelDesc_t m;
	if (FAILED(hr = GetMapDescAt(iMap, m))) return hr;

	BYTE i;
	BYTE nShared = m.nSharedOrnateIndices, nWall = m.nWallOrnateIndices, nFloor = m.nFloorOrnateIndices;

	CByteArray v;

	for (i = 0; i < nWall; i++)
		v.Add(m.bWallOrnateIndices[i]);
	for (i = 0; i < nFloor; i++)
		v.Add(m.bFloorOrnateIndices[i]);
	for (i = 0; i < nShared; i++)
		v.Add(m.bSharedOrnateIndices[i]);

	BYTE x = 0;

	oim = CDM2OrnateIdxMig();

	for (i = 0; i < nWall; i++, x++)
		oim.m_wall.Add(v[x]);
	for (i = 0; i < nFloor; i++, x++)
		oim.m_floor.Add(v[x]);
	for (i = 0; i < nShared; i++, x++)
		oim.m_door.Add(v[x]);

	oim.m_iDoor[0] = (m.w2 & 0x0080) ? m.n14_8_B : -1;
	oim.m_iDoor[1] = (m.w2 & 0x0100) ? m.n14_C_F : -1;

	return S_OK;
#else
	HRESULT hr;
	LevelDesc_t m;
	Atlas_t g;
	if (FAILED(hr = GetMapDescAt(iMap, m))) return hr;
	if (FAILED(hr = GetMapAtlasAt(iMap, g))) return hr;

	UINT x, y, cx = m.cxLv, cy = m.cyLv;

	ScanOrante_t so;

	for (y = 0; y < cy; y++) {
		for (x = 0; x < cx; x++) {
			RN rn = g.r[y][x];
			BYTE t = g.c[y][x] / 0x20;
			switch (t) {
			case 0:
			case 1:
			case 4:
			case 5:
				{
					bool fWall = (t == 0);
					if (rn != RN_NIL) scanRecord4Ornate(rn, fWall, so);
					break;
				}
			}
		}
	}

	CByteArray v;

	BYTE i;
	BYTE nShared = m.nSharedOrnateIndices, nWall = m.nWallOrnateIndices, nFloor = m.nFloorOrnateIndices;
	BYTE n = nShared + nWall + nFloor;

	for (i = 0; i < nWall; i++)
		v.Add(m.bWallOrnateIndices[i]);
	for (i = 0; i < nFloor; i++)
		v.Add(m.bFloorOrnateIndices[i]);
	for (i = 0; i < nShared; i++)
		v.Add(m.bSharedOrnateIndices[i]);

	v.SetSize(48);

	oim = CDM2OrnateIdxMig();

	for (i = 0; i < 16; i++) {
		if (so.nWallUsed & (1 << i))
			oim.m_wall.Add(v[i]);
		if (so.nFloorUsed & (1 << i))
			oim.m_floor.Add(v[n - nShared - nFloor + i]);
	}
	for (i = 0; i < 8; i++) {
		if (so.nDoorUsed & (1 << i))
			oim.m_door.Add(v[n - nShared + i]);
	}

	return S_OK;
#endif
}

///	HRESULT CDDAT::scanRecord4Ornate(RN rn, bool fWall, ScanOrante_t &so)
///	{
///		HRESULT hr;
///		while (rn != RN_END) {
///			DMDBU rec;
///			if (FAILED(hr = GetDBRecAt(DBIPos_t(rn), rec))) return hr;
///			switch (rn.db()) {
///			case 0:
///				{
///					if (!fWall) {
///						BYTE i = (rec.r0.w2 / 0x0002) & 7;
///						if (i != 0) {
///							so.nDoorUsed |= 1 << (i - 1);
///						}
///					}
///					break;
///				}
///			case 3:
///				{
///					BYTE i = (rec.r3.w4 / 0x1000) & 15;
///					if (i != 0) {
///						(DWORD &)(fWall ? so.nWallUsed : so.nFloorUsed) |= 1 << (i - 1);
///					}
///					break;
///				}
///			}
///			rn = rec.r0.next;
///		}
///		return S_OK;
///	}

HRESULT CDDAT::UpdateOrnate(UINT iMap, const CDM2OrnateIdxMig &oim)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = updateOrnate(iMap, oim))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::updateOrnate(UINT iMap, const CDM2OrnateIdxMig &oim)
{
#if 1
	HRESULT hr;
	LevelDesc_t m;
	if (FAILED(hr = GetMapDescAt(iMap, m))) return hr;

	oim.SaveTo(m);

	if (FAILED(hr = setMapDescAt(iMap, m, UIF_NO))) return hr;

	return S_OK;
#else
	HRESULT hr;
	LevelDesc_t m;
	if (FAILED(hr = GetMapDescAt(iMap, m))) return hr;

	BYTE i;
	BYTE nShared = oim.m_door.Size(), nWall = oim.m_wall.Size(), nFloor = oim.m_floor.Size();

	CByteArray v;

	for (i = 0; i < nWall; i++)
		v.Add(oim.m_wall[i]);
	for (i = 0; i < nFloor; i++)
		v.Add(oim.m_floor[i]);
	for (i = 0; i < nShared; i++)
		v.Add(oim.m_door[i]);

	BYTE x = 0;

	m.nWallOrnateIndices = nWall;
	m.nFloorOrnateIndices = nFloor;
	m.nSharedOrnateIndices = nShared;

	for (i = 0; i < nWall; i++, x++)
		m.bWallOrnateIndices[i] = v[x];
	for (i = 0; i < nFloor; i++, x++)
		m.bFloorOrnateIndices[i] = v[x];
	for (i = 0; i < nShared; i++, x++)
		m.bSharedOrnateIndices[i] = v[x];

	if (oim.m_iDoor[0] < 0) {
		m.w2 &=~0x0080;
	} else {
		m.w2 |= 0x0080; m.n14_8_B = oim.m_iDoor[0];
	}

	if (oim.m_iDoor[1] < 0) {
		m.w2 &=~0x0100;
	} else {
		m.w2 |= 0x0100; m.n14_C_F = oim.m_iDoor[1];
	}

	if (FAILED(hr = setMapDescAt(iMap, m, UIF_NO))) return hr;

	return S_OK;
#endif
}

bool CDDAT::IsTileWall(BYTE c)
{
	c /= 0x20;
	return (c == 0 || c == 6);
}

HRESULT CDDAT::RotateMapAtlas(UINT iMap, int vx, int vy)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = rotateMapAtlas(iMap, vx, vy))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::copyRecords(RN rn, RN &rn2, WalkRecord_t &wr)
{
	if (rn == RN_NIL) {
		rn2 = rn;
		return S_OK;
	}

	if (wr.maskLoc.IsSet(rn)) {
		rn2 = RN_END;
		return S_OK;
	}
	wr.maskLoc.Set(rn);

	HRESULT hr;
	BYTE db = rn.db();
	DMDBU rec;
	if (FAILED(hr = getDBRecAt(DBIPos_t(db, rn.idx()), rec)))
		return hr;
	wr.mask.Set(rn);
	switch (db) {
	case 4:
	case 9:
	case 14:
		{
			if (rec.r4.stolen != RN_END) {
				if (FAILED(hr = copyRecords(rec.r4.stolen, rec.r4.stolen, wr)))
					return hr;
			}
			break;
		}
	}
	if (rec.r0.next != RN_END) {
		if (FAILED(hr = copyRecords(rec.r0.next, rec.r0.next, wr)))
			return hr;
	}
	DBIPos_t pos;
	if (FAILED(hr = addDBRecToEmpty(db, rec, pos)))
		return hr;
	rn2 = RNco(db, pos.idx(), rn.room());
	return S_OK;
}

HRESULT CDDAT::rotateMapAtlas(UINT iTar, int vx, int vy)
{
	HRESULT hr;

	LevelDesc_t ts;
	if (FAILED(hr = GetMapDescAt(iTar, ts)))
		return hr;
	Atlas_t ta;
	if (FAILED(hr = GetMapAtlasAt(iTar, ta)))
		return hr;

	BYTE tcx = ts.cxLv;
	BYTE tcy = ts.cyLv;

	UINT iMap, nMaps = CntMap();
	for (iMap = 0; iMap < nMaps; iMap++) {
		LevelDesc_t s;
		if (FAILED(hr = GetMapDescAt(iMap, s)))
			return hr;
		Atlas_t a;
		if (FAILED(hr = GetMapAtlasAt(iMap, a)))
			return hr;

		BYTE cx = s.cxLv;
		BYTE cy = s.cyLv;
		for (BYTE y = 0; y < cy; y++) {
			for (BYTE x = 0; x < cx; x++) {
				RN rn = a.r[y][x];
				if (rn != RN_NIL) {
					ASSERT(rn != RN_END);
					while (rn != RN_END) {
						DMDBU rec;
						if (FAILED(hr = getDBRecAt(DBIPos_t(rn), rec)))
							return hr;
						bool fMod = false;
						bool fIsWall = CDDAT::IsTileWall(a.c[y][x]);
						bool fHere = (iMap == iTar);
						switch (rn.db()) {
						case 1: // TELE
							{
								BYTE iTarMe = (rec.r1.w4 >> 8) & 63;
								if (iTar != iTarMe) break;
								int tx = ((rec.r1.w2 / 0x20) & 31) + vx;
								int ty = ((rec.r1.w2       ) & 31) + vy;
								while (tx < 0) tx += tcx;
								while (ty < 0) ty += tcy;
								while (tcx <= tx) tx -= tcx;
								while (tcy <= ty) ty -= tcy;
								rec.r1.w2 &=~(31 * 0x20 | 31);
								rec.r1.w2 |= 0
									|(tx * 0x20)
									|(ty       )
									;
								fMod = true;
								break;
							}
						case 3: // ACTU
							{
								BYTE t = rec.r3.w2 & 0x7F;
								const CDM2ActuHint &r = CDM2NameDic::GetInstance().GetActuHint(t, fIsWall);
								if (r.a3t != a3tLocator) break;
								bool fCross = (r.a1t == a1tMap);
								if (!fHere && !fCross) break;
								if (fCross) {
									BYTE t = (rec.r3.w2 / 128) & 63;
									if (t != iTar) break;
								}
								int tx = ((rec.r3.w6 / 0x0800) & 31) + vx;
								int ty = ((rec.r3.w6 / 0x0040) & 31) + vy;
								while (tx < 0) tx += tcx;
								while (ty < 0) ty += tcy;
								while (tcx <= tx) tx -= tcx;
								while (tcy <= ty) ty -= tcy;
								rec.r3.w6 &=~(31 * 0x0800 | 31 * 0x0040);
								rec.r3.w6 |= 0
									|(tx * 0x0800)
									|(ty * 0x0040)
									;
								fMod = true;
								break;
							}
						}
						if (fMod) {
							if (FAILED(hr = setDBRecAt(DBIPos_t(rn), rec, UIF_NO)))
								return hr;
						}
						rn = rec.r0.next;
					}
				}
			}
		}
	}

	{
		LevelDesc_t &s = ts;
		Atlas_t &a = ta;

		iMap = iTar;

		//
		BYTE cx = s.cxLv;
		BYTE cy = s.cyLv;

		// vx
		while (vx > 0) {
			for (BYTE y = 0; y < cy; y++) {
				for (BYTE x = 0; x < cx - 1; x++) {
					char x1 = cx - x - 1;
					char x2 = cx - x - 2;
					if (x1 < 0) x1 += cx;
					if (x2 < 0) x2 += cx;
					ASSERT(0 <= x1 && x1 < cx);
					ASSERT(0 <= x2 && x2 < cx);
					swap(a.c[y][x1], a.c[y][x2]);
					swap(a.r[y][x1], a.r[y][x2]);
				}
			}
			vx--;
		}
		while (vx < 0) {
			for (BYTE y = 0; y < cy; y++) {
				for (BYTE x = 0; x < cx - 1; x++) {
					char x1 = x;
					char x2 = x + 1;
					if (cx <= x1) x1 -= cx;
					if (cx <= x2) x2 -= cx;
					ASSERT(0 <= x1 && x1 < cx);
					ASSERT(0 <= x2 && x2 < cx);
					swap(a.c[y][x1], a.c[y][x2]);
					swap(a.r[y][x1], a.r[y][x2]);
				}
			}
			vx++;
		}

		// vy
		while (vy > 0) {
			for (BYTE x = 0; x < cx; x++) {
				for (BYTE y = 0; y < cy - 1; y++) {
					char y1 = cy - y - 1;
					char y2 = cy - y - 2;
					if (y1 < 0) y1 += cy;
					if (y2 < 0) y2 += cy;
					ASSERT(0 <= y1 && y1 < cy);
					ASSERT(0 <= y2 && y2 < cy);
					swap(a.c[y1][x], a.c[y2][x]);
					swap(a.r[y1][x], a.r[y2][x]);
				}
			}
			vy--;
		}
		while (vy < 0) {
			for (BYTE x = 0; x < cx; x++) {
				for (BYTE y = 0; y < cy - 1; y++) {
					char y1 = y;
					char y2 = y + 1;
					if (cy <= y1) y1 -= cy;
					if (cy <= y2) y2 -= cy;
					ASSERT(0 <= y1 && y1 < cy);
					ASSERT(0 <= y2 && y2 < cy);
					swap(a.c[y1][x], a.c[y2][x]);
					swap(a.r[y1][x], a.r[y2][x]);
				}
			}
			vy++;
		}

		//
		if (FAILED(hr = setMapAtlasAt(iMap, a, UIF_NO)))
			return hr;
	}

	return S_OK;
}

HRESULT CDDAT::WorldShift(char vx, char vy)
{
	CDDATUndoLock lock(*this, TRUE);
	//
	HRESULT hr;
	if (FAILED(hr = worldShift(vx, vy))) return hr;
	//
	lock.Unlock(TRUE);
	return S_OK;
}

HRESULT CDDAT::worldShift(char vx, char vy)
{
	HRESULT hr;
	UINT iMap, nMaps = CntMap();
	for (iMap = 0; iMap < nMaps; iMap++) {
		LevelDesc_t s;
		if (FAILED(hr = GetMapDescAt(iMap, s)))
			return hr;
		//
		s.nShiftX += vx;
		s.nShiftY += vy;
		//
		if (FAILED(hr = setMapDescAt(iMap, s, UIF_NO)))
			return hr;
	}

	return S_OK;
}

HRESULT CDDAT::walkRecords(RN rn, WalkRecord_t &wr)
{
	ASSERT(rn != RN_NIL);

	HRESULT hr;
	while (rn != RN_END) {
		if (wr.maskLoc.IsSet(rn))
			break;

		DMDBU rec;
		if (FAILED(hr = getDBRecAt(DBIPos_t(rn), rec)))
			return hr;
		wr.maskLoc.Set(rn);
		wr.mask.Set(rn);
		switch (rn.db()) {
		case 4:
		case 9:
		case 14:
			{
				RN rnAnother = rec.r4.stolen;
				ASSERT(rnAnother != RN_NIL);
				if (rnAnother != RN_END) {
					if (wr.mask.IsSet(rnAnother)) {
						if (FAILED(hr = copyRecords(rnAnother, rnAnother, wr)))
							return hr;
						rec.r4.stolen = rnAnother;
						if (FAILED(hr = setDBRecAt(DBIPos_t(rn), rec, UIF_NO)))
							return hr;
					} else {
						if (FAILED(hr = walkRecords(rnAnother, wr)))
							return hr;
					}
				}
				break;
			}
		}

		{
			RN rnAnother = rec.r0.next;
			ASSERT(rnAnother != RN_NIL);
			if (wr.mask.IsSet(rnAnother)) {
				if (FAILED(hr = copyRecords(rnAnother, rnAnother, wr)))
					return hr;
				rec.r4.next = rnAnother;
				if (FAILED(hr = setDBRecAt(DBIPos_t(rn), rec, UIF_NO)))
					return hr;
			}
		}

		rn = rec.r0.next;
	}
	return S_OK;
}

void CDDAT::walkTest(CString &str, RN rn, int iIndent)
{
	while (rn != RN_END) {
		DMDBU rec;
		HRESULT hr;
		if (FAILED(hr = getDBRecAt(DBIPos_t(rn), rec)))
			return;
		for (int x = 0; x < iIndent; x++) str += '\t';
		str += CDMRecFmt::Format(rn.db(), rn.room(), rec, true);
		str += "\r\n";

		switch (rn.db()) {
		case 4:
		case 9:
		case 14:
			{
				walkTest(str, rec.r4.stolen, 1 + iIndent);
			}
		}

		rn = rec.r0.next;
	}
}

void CDDAT::map_insertAt(UINT i)
{
	UINT x;
	for (x = m_nMaps; i < x; x--) {
		m_aMaps[x] = m_aMaps[x -1];
	}
	m_aMaps[i] = Map_t();
	m_nMaps++;
}

void CDDAT::map_removeAt(UINT i)
{
	UINT x;
	for (x = i; x < m_nMaps - 1; x++) {
		m_aMaps[x] = m_aMaps[x + 1];
	}
	m_aMaps[m_nMaps - 1] = Map_t();
	m_nMaps--;
}

CString CDDAT::computeDM1Text(UINT i)
{
	const WORD *pwBody = (const WORD *)m_aAllDesc.fvTextData.GetData();
	UINT size = m_aAllDesc.fvTextData.GetSize() / 2;
	if (size < i)
		return "(Overflow)";
	pwBody += i;
	CString s;
	bool escape = false;
	while (true) {
		WORD w = *pwBody;
		BYTE nibbles[] = {
			(w >> 10) & 0x1F,
			(w >>  5) & 0x1F,
			(w >>  0) & 0x1F,
		};
		for (int x = 0; x < 3; x++) {
			BYTE nibble = nibbles[x];

			static char *cTbl[32] = {
				"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
				" ",".","!","!","!","!",
			};
			if (escape) {
				switch (nibble) {
				case 0:
					s += ("AND ");
					break;
				case 1:
					s += ("!");
					break;
				case 2:
					s += ("THE ");
					break;
				case 3:
					s += ("YOU ");
					break;
				default:
					ASSERT(FALSE);
					break;
				}
				escape = false;
			}
			else {
				switch (nibble) {
				case 28:
					s += ("_");
					break;
				case 29:
					ASSERT(false);
					break;
				case 30:
					escape = true;
					break;
				case 31:
					return s;
				default:
					s += cTbl[nibble];
					break;
				}
			}
		}
		pwBody++;
	}
}

HRESULT CDDAT::Move2actu2() {
	int cnt11 = CntDBRecIn(11);
	if (cnt11 != 0) {
		return E_OUTOFMEMORY;
	}

	{
		UINT db = 3;
		UINT iMisc, nMisc = CntDBRecIn(db);
		for (iMisc = 0; iMisc < nMisc; iMisc++) {
			DMDBU rec;
			VERIFY(SUCCEEDED(getDBRecAt(DBIPos_t(db, iMisc), rec)));

			DBIPos_t pos;
			VERIFY(SUCCEEDED(addDBRecTo(11, rec, pos)));
			ASSERT(pos.idx() == iMisc);
		}
	}

	int iMap, nMaps = CntMap();
	for (iMap = 0; iMap < nMaps; iMap++) {
		HRESULT hr;
		{
			Atlas_t a;
			LevelDesc_t s;
			if (FAILED(hr = GetMapAtlasAt(iMap, a))) throw int();
			if (FAILED(hr = GetMapDescAt(iMap, s))) throw int();
			int cx = s.cxLv;
			int cy = s.cyLv;
			bool any = false;
			for (int y = 0; y < cy; y++) {
				for (int x = 0; x < cx; x++) {
					RN r = a.r[y][x];
					BYTE c = a.c[y][x];

					if (r.db() == 3) {
						r.db(11);
						a.r[y][x] = r;
						any = true;
					}
				}
			}
			if (any) {
				VERIFY(SUCCEEDED(hr = SetMapAtlasAt(iMap, a)));
			}
		}
	}

	for (UINT db = 0; db < 16; db++) {
		UINT iMisc, nMisc = CntDBRecIn(db);
		for (iMisc = 0; iMisc < nMisc; iMisc++) {
			DMDBU rec;
			VERIFY(SUCCEEDED(getDBRecAt(DBIPos_t(db, iMisc), rec)));

			bool any = false;

			if (rec.r0.next.db() == 3) {
				rec.r0.next.db(11);
				any = true;
			}

			if (any) {
				VERIFY(SUCCEEDED(setDBRecAt(DBIPos_t(db, iMisc), rec, UIF_NO)));
			}
		}
	}

	{
		UINT db = 3;
		UINT iMisc, nMisc = CntDBRecIn(db);
		for (iMisc = 0; iMisc < nMisc; iMisc++) {
			VERIFY(SUCCEEDED(removeDBRecFrom(db, nMisc -iMisc -1)));
		}
	}

	return S_OK;
}

HRESULT CDDAT::CheckAllLevelDesc() {
	HRESULT hr;
	int iMap, nMaps = CntMap();
	std::set<ATP_t> dropHere;
	for (iMap = 0; iMap < nMaps; iMap++) {
		Map_t *p = NULL;
		if (SUCCEEDED(hr = getMapAt(iMap, p))) {
			for (int iVert = 0; iVert < p->aDesc.cxLv; iVert++) {
				for (int iHorz = 0; iHorz < p->aDesc.cyLv; iHorz++) {
					BYTE c = p->aAtlas.c[iHorz][iVert];
					switch (c >> 5) {
						case 2: // pit
							int px = p->aDesc.nShiftX + iHorz;
							int py = p->aDesc.nShiftY + iVert;
							dropHere.insert(ATP_t((BYTE)(p->aDesc.Depth() + 1), (BYTE)px, (BYTE)py, 0));
							break;
					}
				}
			}
		}
	}
	for (iMap = 0; iMap < nMaps; iMap++) {
		HRESULT hr;
		Atlas_t a;
		LevelDesc_t s;
		if (FAILED(hr = GetMapAtlasAt(iMap, a))) return hr;
		if (FAILED(hr = GetMapDescAt(iMap, s))) return hr;
		bool save = false;
		if (s.Depth() == 0) continue;
		int cx = s.cxLv;
		int cy = s.cyLv;
		for (int iHorz = 0; iHorz < cy; iHorz++) {
			for (int iVert = 0; iVert < cx; iVert++) {
				RN r = a.r[iHorz][iVert];
				BYTE c = a.c[iHorz][iVert];
				bool wall = false, floor = false;

				int ax = s.nShiftX + iHorz;
				int ay = s.nShiftY + iVert;
				if (dropHere.find(ATP_t((BYTE)s.Depth(), (BYTE)ax, (BYTE)ay, 0)) != dropHere.end()) {
					if ((s.w2 & 1) != 1) {
						s.w2 |= 1;
						save = true;
					}
				}

				switch (c >> 5) {
					case 0: // wall
						wall = true;
						break;
					case 1: // floor
						floor = true;
						break;
					case 2: // pit
						floor = true;
						if ((s.w2 & 2) != 2) {
							s.w2 |= 2;
							save = true;
						}
						break;
					case 3: // stairs
						if (c & 4) {
							// up
							if ((s.w2 & 8) != 8) {
								s.w2 |= 8;
								save = true;
							}
						}
						else {
							// down
							if ((s.w2 & 16) != 16) {
								s.w2 |= 16;
								save = true;
							}
						}
						break;
					case 5: // tele
						floor = true;
						if (c & 4) {
							if ((s.w2 & 32) != 32) {
								s.w2 |= 32;
								save = true;
							}
						}
						break;
				}

				RN rn = r;
				while (rn != RN_END && rn != RN_NIL) {
					DMDBU rec;
					if (FAILED(hr = getDBRecAt(DBIPos_t(rn), rec))) return hr;
					if (rn.db() == dbCreature) {
						BYTE cty = rec.r4.b4;

						bool has = false;
						BYTE t;
						for (t=0; t<s.nCreatureIndices; t++) {
							if (s.bCreatureIndices[t] == cty)
								has = true;
						}
						if (!has && t < 15) {
							s.bCreatureIndices[t] = cty;
							s.nCreatureIndices = t + 1;
							save = true;
						}
					}
					rn = rec.r0.next;
				}
			}
		}
		if (save) {
			VERIFY(SUCCEEDED(hr = SetMapDescAt(iMap, s)));
		}
	}
	return S_OK;
}
