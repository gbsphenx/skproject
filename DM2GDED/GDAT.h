// GDAT.h: CGDAT クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDAT_H__D6B0F7FF_DBEC_4DDA_9FF6_C1CD7AED94A7__INCLUDED_)
#define AFX_GDAT_H__D6B0F7FF_DBEC_4DDA_9FF6_C1CD7AED94A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DMGHLt.h"
#include "SizeVuff.h"
#include "DMGHLci.h"
#include "GDATDef.h"
#include "DM2PIPDescList.h"
#include "GDEntry.h"

class CGDAT_RRIf
{
public:
	// 
	virtual I32 GetValueAt(const CGDEntry &r) = NULL;
	// 
	virtual BOOL GetRawDataAt(UINT iIdx, SizeBuff &rBuff) = NULL;

};

class CGDAT : DMGHLciFDM2, public CGDAT_RRIf
{
	// 
	struct C6 : CGDEntry {
		// 
		C6(): CGDEntry() { }
		// 
		C6(const BYTE *p6)
		{
			memcpy(c, p6, 6);
		}
		// 
		C6(BYTE c1, BYTE c2, BYTE c3, BYTE c4, BYTE c5, BYTE c6)
		{
			c[0] = c1, c[1] = c2, c[2] = c3, c[3] = c4, c[4] = c5, c[5] = c6;
		}
	};
	// 
	struct C3 {
		// 
		CGDEntry f;
		// 
		BOOL fIsBin;
		// 
		UINT x;

		// 
		C3(): x(0) { }
		// 
		C3(CGDEntry f): f(f), x(0) { }
	};
	// 
	typedef map<CGDEntry, C3> C3Map;
	// 
	struct RFio;
	// 
	struct RawData {
		// 
		SizeBuff fv;
	};
	// 
	typedef list<RawData> RawDataList;
	// 
	typedef map<UINT, UINT> TrackChainMap;
	// 
	struct BM;
	// 
	struct WFio;
	// 
	typedef list<CGDEntry> ModifiedEntryList;
	// 
	struct OwnPal {
		// 
		BYTE iIdxes[16];
	};
	// 
	typedef map<UINT, CGDEntry> Idx2EntryMap;

	// 
	int m_nGDRPIC;
	// 
	bool m_fLE;
	// 
	bool m_fEN;
	// 
	bool m_fTextEnc;
	// 
	C3Map m_c6;
	// 
	RawDataList m_listRaw;
	// 
	CUIntArray m_listRawRef;
	// 
	TrackChainMap m_mapTrack;
	// 
	bool m_fModified;
	// 
	ModifiedEntryList m_listModEnt;
	// 
	Idx2EntryMap m_mapIdx2Entry;
	// 
	int m_nDM2TET;
	// 
	int m_nGDRSLT;
	// 
	int m_nGDRSDT;
	// 
	int m_nGDRIPT;
	// 
	bool m_fChainImageTrack;
	// 
	BYTE m_bMainScenePal[16];
	// 
	CDM2PIPDescList m_pipctx;
	// 
	UINT m_nGDOF;
	// 
	bool m_fRestrictPal;
	// 
	DMGHLciDM2Pal m_DM2Pal;

	// 
	C3 *FindC3Entry(CGDEntry f, BOOL fAdd);
	// 
	BOOL loadFrom(LPCTSTR pszFileIn, UINT nGDOF);
	// 
	RawData *GetRawDataAt(UINT iIdx);
	// 
	BOOL saveTo(LPCTSTR pszFileInto);
	// 
	void Unassoc(C3 &c3);
	// 
	BOOL EncodeText(LPCSTR pszText, SizeBuff &fv);
	// 
	void AddModifiedEnt(const CGDEntry &pos);
	// 
	bool IsImageTracked(UINT iIdx);
	// 
	bool GetOwnPalAt(CGDEntry pos, bool fUseHisOne, OwnPal &rOwnPal);
	// 
	bool GetSegaOwnPalAt(CGDEntry pos, bool fUseHisOne, OwnPal &rOwnPal);
	// 
	bool makeSure();
	// 
	bool makeUnsure();
	// 
	void SetMode(UINT nGDOF);
	// 
	bool Detective();
	// 
	RawData *FindRawDataAtNearestEntry(CGDEntry pos);
	// 
	int StrictImageTypeAtNearestEntry(CGDEntry pos);
	// 
	int StrictEffectSndAtNearestEntry(CGDEntry pos);
	// 
	void MkC3Map();

	// 
	HBITMAP GenImg4(DMGHLiIMGM &imgm);
	// 
	HBITMAP GenImg8(DMGHLiIMGM &imgm);
	// 
	bool MakeExpImgC4(DMGHLiIMGM &imgm, SizeBuff &fv);
	// 
	bool MakeExpImgC8(DMGHLiIMGM &imgm, SizeBuff &fv);

public:
	// 
	virtual ~CGDAT()
	{
		Close();
	}
	// 
	void Close();
	// 
	BOOL LoadFrom(LPCTSTR pszFileIn, UINT nGDOF);
	// 
	BOOL SaveTo(LPCTSTR pszFileInto);
	// 
	CGDEntry GetHeadPos();
	// 
	CGDEntry GetNextPos(const CGDEntry &r);
	// 
	CGDEntry GetLowerPos(const CGDEntry &r);
	// 
	virtual I32 GetValueAt(const CGDEntry &r);
	// 
	virtual I32 GetCls3ValueAt(const CGDEntry &r);
	// 
	HBITMAP GetImgAt(UINT iIdx);
	// 
	HBITMAP GetImgAt(UINT iIdx, DMGHLiIMGM &imgm);
	// 
	BOOL GetImgAt(const CGDEntry &pos, DMGHLiIMGM &imgm, HBITMAP *phbm);
	// 
	HBITMAP GetImgAt(const CGDEntry &pos, DMGHLiIMGM &imgm);
	// 
	HBITMAP GetImgAt(const CGDEntry &pos);
	// 
	virtual BOOL GetRawDataAt(UINT iIdx, SizeBuff &rBuff);
	// 
	BOOL GetTextAt(UINT iIdx, CString &strText);
	// 
	BOOL GetDispTextAt(UINT iIdx, CString &strText);
	// 
	BOOL SetRawDataFor(const CGDEntry &r, const SizeBuff &fv);
	// 
	BOOL EnumL4(list<CGDEntry> &rList, BYTE c1, BYTE c2);
	// 
	BOOL Enum(list<CGDEntry> &rList, const CGDEntry &lower, const CGDEntry &upper);
	// 
	BOOL Enum(list<CGDEntry> &rList);
	// 
	BOOL AddEntry(const CGDEntry &r, UINT iIdx, BOOL fIsBin);
	// 
	BOOL AddEntry(const CGDEntry &r, UINT iIdx);
	// 
	BOOL AddBinEntry(const CGDEntry &r, const SizeBuff &fv);
	// 
	BOOL DeleteEntry(const CGDEntry &r);
	// 
	BOOL AddTextEntry(const CGDEntry &r, LPCSTR pszText);
	// 
	BOOL SetTextFor(const CGDEntry &r, LPCSTR pszText);
	// 
	BOOL SetValueAt(const CGDEntry &r, UI32 nVal, BOOL fIsBin);
	// 
	void SetModified(bool fModified = true);
	// 
	bool IsModified();
	// 
	bool IsSnd3() { return m_nGDRSDT == GDRSDT_SND3; }
	// 
	bool IsSnd56() { return m_nGDRSDT == GDRSDT_SND5SND6; }
	// 
	bool IsSndPCM() { return (m_nGDRSDT == GDRSDT_SND7) || (m_nGDRSDT == GDRSDT_SND3) || (m_nGDRSDT == GDRSDT_SND5SND6) || (m_nGDRSDT == GDRSDT_SNDA) || (m_nGDRSDT == GDRSDT_SND9); }
	// 
	BOOL RenameEntry(const CGDEntry &rExist, const CGDEntry &rNew);
	// 
	void EnumModifiedEnt(list<CGDEntry> &rList);
	// 
	bool GrabNextModifiedEnt(CGDEntry &pos);
	// 
	BOOL GetRawImageDataAt(UINT iIdx, SizeBuff &rBuff, int &nGDRIDT);
	// 
	BOOL DeleteEntries(CGDEntryMatch &rPred);
	// 
	int GetDM2TET() { return m_nDM2TET; }
	// 
	BOOL GetExcTextAt(UINT iIdx, SizeBuff &rBuff, int &nGDRTDT);
	// 
	BOOL AddExcTextEntry(const CGDEntry &r, const SizeBuff &fv, int nGDRTDT);
	// 
	UI32 GetGDRSLT() { return m_nGDRSLT; }
	// 
	I32 GetSizeOfRawData(UINT iIdx);
	// 
	UI32 GetGDRSDT() { return m_nGDRSDT; }
	// 
	UI32 GetGDRIPT() { return m_nGDRIPT; }
	// 
	bool GetScenePalette(int nScene, int iDarkness, COLORREF clr[16]);
	// 
	bool GetScenePalette(int nScene, int iDarkness, RGBQUAD clr[16]);
	// 
	bool GetInterfacePalette(COLORREF clr[16]);
	// 
	bool GetInterfacePalette(RGBQUAD rq[16]);
	// 
	BOOL ReadRawEntry(const CGDEntry &pos, SizeBuff &fv);
	// 
	BOOL LoadToPIPCtxFrom(LPCSTR pszFileIn, bool fFirst);
	// 
	bool ReadRawColorPalette(const CGDEntry &pos, int iDarkness, COLORREF clr[16]);
	// 
	bool ReadRawColorPalette(const CGDEntry &pos, int iDarkness, RGBQUAD rq[16]);
	// 
	const CDM2PIPDescList &GetPIPCtx() const { return m_pipctx; }
	// 
	BOOL MakeBinEntry(const CGDEntry &r, const SizeBuff &fv);
	// 
	bool GetPicIPF(const CGDEntry &pos, DMGHLiPicIPF &ipf);
	// 
	bool SetPicIPF(const CGDEntry &pos, DMGHLiPicIPF ipf, bool fStrict = false);
	// 
	bool SetPicIPF(UINT iIdx, DMGHLiIPF ipf, bool fStrict = false);
	// 
	bool IsImage4bpp(const CGDEntry &pos);
	// 
	I32 GetRawDataCount();
	// 
	bool MakeSure();
	// 
	UINT GetGDOF();
	// 
	bool PutRawDataFor(UINT iIdx, const SizeBuff &fv);
	// 
	bool AppendRawData(const SizeBuff &fv);
	// 
	bool MakeUnsure();
	// 
	bool IsPalRestricted() { return m_fRestrictPal; }
	// 
	bool FindNearestEntry(CGDEntry &pos);
	// 
	void ReplicateSubclass(const CGDEntry &posFrom, const CGDEntry &posTo);
	// 
	bool IsEntryBin(const CGDEntry &pos);
	// 
	UINT CountEnts(const CGDEntry &lower, const CGDEntry &upper);
	// 
	bool Mark4x4ToImage(const CGDEntry &pos);
	// 
	void DeleteSubclass(BYTE c1, BYTE c2);
	// 
	bool ExportRange(LPCTSTR pszFileInto, const CGDEntry &lower, const CGDEntry &upper);
	// 
	bool Copyfrm(CGDAT &cores, const CGDEntry &lower, const CGDEntry &upper, bool copyAtts);
	// 
	const DMGHLciDM2Pal &GetDM2Pal();

public:
	// 
	virtual bool ReadEntry(int i, SizeBuff &rBuff);
	// 
	virtual bool IsLE() { return m_fLE; }
	// 
	virtual void Track(int iIdx, int nTracked[DMGHLi_MAX_TRACK]);
	// 
	virtual bool IsPalIndependent() { return m_nGDRIPT != GDRIPT_IMAGE; }
	// 
	virtual bool FindImagePalette(int iIdx, DMGHLciPAL &rPal);
	// 
	virtual bool IsFMTowns() { return m_nGDRPIC == GDRPIC_FMTOWNS; }
	// 
	virtual bool IsSega() { return m_nGDRPIC == GDRPIC_SEGA; }
	// 
	virtual bool IsC8U8Allowed() { return m_nGDRPIC == GDRPIC_C4C8; }
};

#endif // !defined(AFX_GDAT_H__D6B0F7FF_DBEC_4DDA_9FF6_C1CD7AED94A7__INCLUDED_)
