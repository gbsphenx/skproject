// DM2SIIVCtx.h: CDM2SIIVCtx クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM2SIIVCTX_H__F125855A_33B3_41E4_8310_84F165929D41__INCLUDED_)
#define AFX_DM2SIIVCTX_H__F125855A_33B3_41E4_8310_84F165929D41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RCI.h"
#include "GDEdCtx.h"
#include "ExtBM.h"

#define DM2SIIVT_PCAT   (0U)
#define DM2SIIVT_AMIGA  (1U)
#define DM2SIIVT_PC9801 (2U)

enum {
	siivlD4L2,
	siivlD4L1,
	siivlD4R2,
	siivlD4R1,
	siivlD4C,
	siivlD3L2,
	siivlD3L1,
	siivlD3R2,
	siivlD3R1,
	siivlD3C,
	siivlD2L2,
	siivlD2L1,
	siivlD2R2,
	siivlD2R1,
	siivlD2C,
	siivlD1L1,
	siivlD1R1,
	siivlD1C,
	siivlD0L1,
	siivlD0R1,
	siivlD0C,
};

class CDM2SIIVCtxCB
{
public:
	// 
	virtual void OnSIIVCtxUpdate(UINT nHint, void *pHint) = NULL;
};

class CDM2SIIVCtxInCB
{
	// 
	CArray<CDM2SIIVCtxCB *, CDM2SIIVCtxCB *> arrSink;

public:
	// 
	~CDM2SIIVCtxInCB()
	{
		ASSERT(arrSink.GetSize() == 0);
	}
	// 
	void AddCB(CDM2SIIVCtxCB *pSink);
	// 
	void RemoveCB(CDM2SIIVCtxCB *pSink);

	// 
	void DispatchSIIVCtxUpdate(UINT nHint, void *pHint);
};

class CDM2SIIVImg
{
public:
	// 
	DMGHLiIMGM imgm;
	// 
	BYTE c1;
	// 
	BYTE fx, fy;
};

class CDM2SIIVScrnMap
{
public:
	// 
	BYTE bAtlas[5][5];

	// 
	CDM2SIIVScrnMap()
	{
		memset(bAtlas, 0, sizeof(bAtlas));
	}
};

class CDM2SIIVScrnCtx
{
	// 
	CExtBM m_bm;
	// 
	BYTE m_bScrn[200][320];
	// 
	int m_nDM2SIIVT;
	// 
	BYTE m_bScene;
	// 
	int m_iSceneKey;
	// 
	BYTE m_bFMTbl[5][256];

public:
	// 
	enum LPS {
		lpsLT,
		lpsMT,
		lpsRT,
		lpsLC,
		lpsMC,
		lpsRC,
		lpsLB,
		lpsMB,
		lpsRB,
	};

	// 
	bool InitScreen(int nDM2SIIVT, BYTE bScene, const DMGHLciDM2Pal &DM2Pal);
	// 
	bool RenderToBM();
	// 
	bool RenderScreenTo(CDC &dc, CRect rc);
	// 
	int GetDM2SIIVT() { return m_nDM2SIIVT; }
	// 
	int GetScene() { return m_bScene; }
	// 
	int GetSceneKey() { return m_iSceneKey; }

	// 
	bool DrawImageEntry(DMGHLiIMGM &imgm, LPS lps, CRect rc, UINT key, BYTE al, BYTE fm, bool fDrawInterfaceGraphics, int px, int py, int sx, int sy);
	// 
	void ClearScreen(BYTE cPix);

protected:
	// 
	bool DrawOrnateSceneImage(DMGHLiIMGM &imgm, BYTE key, BYTE fm, int px, int py, int sx, int sy);
	// 
	bool DrawSceneImage(DMGHLiIMGM &imgm, BYTE key, BYTE fm, LPS lps, int px, int py, int sx, int sy);
	// 
	void FillBlackHatch(CRect rc);
	// 
	inline void SetScrnPixel(int x, int y, BYTE c) { m_bScrn[y][x] = c; }
	// 
	inline BYTE GetScrnPixel(int x, int y) { return m_bScrn[y][x]; }

	// 
	virtual int Get_SceneKey() { return -1; }
	// 
	virtual bool Read_ScenePalette(BYTE bScene, RGBQUAD r4[16]) { return false; }
	// 
	virtual bool Read_InterfacePalette(RGBQUAD r4[16]) { return false; }
	// 
	virtual bool Read_FMTbl(BYTE iTbl, SizeBuff &fv) { return false; }
	// 
	virtual void OnInitScreenDone() { }
	// 
	virtual CPoint Layout(
		LPS lps,
		CRect rect,
		CSize size,
		CPoint point,
		CSize scale
		);
};

class CDM2SIIVScrnCtxPG : public CDM2SIIVScrnCtx
{
	// 
	UINT m_iTick;
	// 
	CDM2SIIVScrnMap m_map;
	// 
	bool (CDM2SIIVScrnCtxPG::*m_pfnReadScenePalette)(BYTE bScene, RGBQUAD rq[16]);
	// 
	RGBQUAD m_rqScene[16];

	// 
	bool ReadFromRawScenePalette(BYTE bScene, RGBQUAD rq[16])
	{
		return _Read_ScenePalette(bScene, rq);
	}
	// 
	bool ReadFromYourScenePalette(BYTE bScene, RGBQUAD rq[16])
	{
		memcpy(rq, m_rqScene, sizeof(m_rqScene));
		return true;
	}

public:
	// 
	CDM2SIIVScrnCtxPG()
	{
		m_iTick = 0;
	}
	// 
	bool RenderScene();
	// 
	void GetAtlas(CDM2SIIVScrnMap &map) { map = m_map; }
	// 
	void SetAtlas(const CDM2SIIVScrnMap &map) { m_map = map; }
	// 
	void Tick();
	// 
	bool InitScreen(int nDM2SIIVT, BYTE bScene, const DMGHLciDM2Pal &DM2Pal)
	{
		m_pfnReadScenePalette = &CDM2SIIVScrnCtxPG::ReadFromRawScenePalette;

		return CDM2SIIVScrnCtx::InitScreen(nDM2SIIVT, bScene, DM2Pal);
	}
	// 
	bool InitScreen(int nDM2SIIVT, const RGBQUAD rq[16], const DMGHLciDM2Pal &DM2Pal)
	{
		m_pfnReadScenePalette = &CDM2SIIVScrnCtxPG::ReadFromYourScenePalette;

		memcpy(m_rqScene, rq, sizeof(m_rqScene));

		return CDM2SIIVScrnCtx::InitScreen(nDM2SIIVT, -1, DM2Pal);
	}

protected:
	// 
	virtual bool ReadRawEntry(const CGDEntry &pos, SizeBuff &fv) = NULL;
	// 
	virtual bool ReadImageEntry(CGDEntry pos, DMGHLiIMGM &imgm) = NULL;
	// 
	virtual int GetEntryValue(const CGDEntry &pos) = NULL;

	// 
	virtual int Get_SceneKey()
	{
		return GetEntryValue(CGDEntry(0x08, GetScene(), 0x0B, 0x64, 0, 0));
	}
	// 
	virtual bool Read_ScenePalette(BYTE bScene, RGBQUAD rq[16])
	{
		return (this->*m_pfnReadScenePalette)(bScene, rq);
	}
	// 
	virtual bool Read_InterfacePalette(RGBQUAD rq[16])
	{
		return _Read_InterfacePalette(rq);
	}
	// 
	virtual bool Read_FMTbl(BYTE iTbl, SizeBuff &fv)
	{
		CGDEntry pos(0x08, GetScene(), 0x07, iTbl, 0x08, 0x00);
		if (!ReadRawEntry(pos, fv) || fv.GetSize() != 256)
			return false;
		return true;
	}

	// 
	virtual bool _Read_ScenePalette(BYTE bScene, RGBQUAD rq[16])
	{
		CGDEntry pos1(0x08, bScene, 0x09, 0x00, 0x04, 0x00);
		CGDEntry pos2(0x08, bScene, 0x09, 0x00, 0x00, 0x00);
		SizeBuff fv;
		if ((ReadRawEntry(pos1, fv) || ReadRawEntry(pos2, fv)) && fv.GetSize() == 64*6) {
			UI8 *p = fv.GetData();
			for (int i = 0; i < 16; i++) {
				rq[i].rgbRed  = p[4*i +1],
				rq[i].rgbGreen= p[4*i +2],
				rq[i].rgbBlue = p[4*i +3],
				rq[i].rgbReserved = 0;
			}
			return true;
		}
		return false;
	}
	// 
	virtual bool _Read_InterfacePalette(RGBQUAD rq[16])
	{
		CGDEntry pos1(0x01, 0x00, 0x09, 0x00, 0x04, 0xFF);
		CGDEntry pos2(0x01, 0x00, 0x0D, 0xFE, 0x00, 0x00);
		CGDEntry posPal(0x01, 0x00, 0x09, 0xFE, 0x08, 0x00);
		CGDEntry posPalBeta(0x01, 0x00, 0x07, 0xFE, 0x08, 0x00);
		SizeBuff fv;
		if ((ReadRawEntry(pos1, fv)) && fv.GetSize() == 64*1) {
			UI8 *p = fv.GetData();
			for (int i = 0; i < 16; i++) {
				rq[i].rgbRed  = p[4*i +1],
				rq[i].rgbGreen= p[4*i +2],
				rq[i].rgbBlue = p[4*i +3],
				rq[i].rgbReserved = 0;
			}
			return true;
		}
		if ((ReadRawEntry(pos2, fv)) && fv.GetSize() == 16) {
			SizeBuff fvPal;
			if ((ReadRawEntry(posPal, fvPal) || ReadRawEntry(posPalBeta, fvPal)) && fvPal.GetSize() == 1024) {
				UI8 *p = fv.GetData();
				for (int i = 0; i < 16; i++) {
					rq[i].rgbRed  = fvPal.GetData()[4*i +1],
					rq[i].rgbGreen= fvPal.GetData()[4*i +2],
					rq[i].rgbBlue = fvPal.GetData()[4*i +3],
					rq[i].rgbReserved = 0;
				}
				return true;
			}
		}
		return false;
	}
};

class CDM2SIIVCtx : public CRCI, public CDM2SIIVCtxInCB, protected CDM2SIIVScrnCtxPG
{
	// 
	typedef map<CGDEntry, CGDEntry> RealEntryMap;

	// 
	CGDEdCtx *m_pEdCtx;
	// 
	RealEntryMap m_map;

	// 
	void InitEdCtx(CGDEdCtx *pEdCtx);

public:
	// 
	CDM2SIIVCtx(CGDEdCtx *pEdCtx);
	// 
	virtual ~CDM2SIIVCtx();
	// 
	CDM2SIIVScrnCtxPG &GetScrnCtx() { return *this; }
	// 
	void ForceRefreshScene();
	// 
	void SceneTick();

protected:
	// 
	virtual bool ReadRawEntry(const CGDEntry &pos, SizeBuff &fv);
	// 
	virtual bool ReadImageEntry(CGDEntry pos, DMGHLiIMGM &imgm);
	// 
	virtual int GetEntryValue(const CGDEntry &pos);
	// 
	virtual void OnInitScreenDone();

	// 
	virtual bool _Read_ScenePalette(BYTE bScene, RGBQUAD rq[16]);
	// 
	virtual bool _Read_InterfacePalette(RGBQUAD rq[16]);

};

#endif // !defined(AFX_DM2SIIVCTX_H__F125855A_33B3_41E4_8310_84F165929D41__INCLUDED_)
