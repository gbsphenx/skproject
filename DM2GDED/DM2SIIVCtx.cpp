// DM2SIIVCtx.cpp: CDM2SIIVCtx クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dm2gded.h"
#include "DM2SIIVCtx.h"
#include "CompatDC.h"
#include "DM2Palo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// 224,136
// 448,272

namespace {

static const COLORREF g_clrDM2PC9801[16] = {
	RGB(  0,  0,  0), //  0
	RGB( 80, 80, 48), //  1
	RGB(  0, 32,143), //  2
	RGB(  0,  0,255), //  3
	RGB(112, 16,159), //  4
	RGB(  0,175, 16), //  5
	RGB(  0,255,  0), //  6
	RGB( 16,255,255), //  7
	RGB(159,  0,  0), //  8
	RGB(255, 16,  0), //  9
	RGB(223, 80,255), // 10
	RGB(175,159, 16), // 11
	RGB(255,255, 16), // 12
	RGB(255,223, 96), // 13
	RGB(175,175,175), // 14
	RGB(255,255,255), // 15
};

static const RGBQUAD g_clrDM2PC9801RGBq[16] = {
	  0,  0,  0,  0, //  0
	 48, 80, 80,  0, //  1
	143, 32,  0,  0, //  2
	255,  0,  0,  0, //  3
	159, 16,112,  0, //  4
	 16,175,  0,  0, //  5
	  0,255,  0,  0, //  6
	255,255, 16,  0, //  7
	  0,  0,159,  0, //  8
	  0, 16,255,  0, //  9
	255, 80,223,  0, // 10
	 16,159,175,  0, // 11
	 16,255,255,  0, // 12
	 96,223,255,  0, // 13
	175,175,175,  0, // 14
	255,255,255,  0, // 15
};

static const BYTE g_tblDitherPC9801[256][4] = {
 0, 0, 0, 0, //   0
 0, 0, 0, 0, //   1
 0, 0, 0, 0, //   2
 0, 0, 1, 0, //   3
 0, 0, 1, 0, //   4
 0, 0, 8, 0, //   5
 0, 8, 1, 0, //   6
 0, 1, 1, 8, //   7
 0, 8,11, 0, //   8
 5, 8, 8, 8, //   9
 5, 9, 1, 8, //  10
 0, 8, 8,13, //  11
13, 8,11, 8, //  12
 2, 9,12,11, //  13
 0, 9,12,13, //  14
13, 9,12,11, //  15
 0, 0, 0, 0, //  16
 0, 0, 0, 0, //  17
 0, 0, 0, 0, //  18
 0, 0, 0, 0, //  19
 0, 0, 1, 0, //  20
 0, 2, 1, 0, //  21
 0, 0, 4, 0, //  22
 0, 4, 1, 0, //  23
 0, 4, 1, 4, //  24
 0, 0,10, 0, //  25
 0, 4,10, 0, //  26
 4,10, 1, 4, //  27
 0,10,10, 2, //  28
 4,10,10, 4, //  29
 4,10,10,10, //  30
10,10,10,10, //  31
 0, 0, 0, 0, //  32
 0, 0, 0, 0, //  33
 0, 0, 0, 0, //  34
 0, 0, 0, 0, //  35
 0, 0, 0, 0, //  36
 0, 0, 2, 0, //  37
 0, 0, 2, 0, //  38
 0, 0, 2, 0, //  39
 0, 2, 2, 0, //  40
 0, 0, 3, 0, //  41
 0, 2, 3, 0, //  42
 2, 2, 3, 2, //  43
 0, 3, 3, 2, //  44
 2, 3, 3, 2, //  45
 2, 3, 3, 3, //  46
 3, 3, 3, 3, //  47
 0, 0, 0, 0, //  48
 0, 0, 0, 0, //  49
 0, 0, 0, 0, //  50
 0, 0, 0, 0, //  51
 0, 0, 2, 0, //  52
 0, 2, 1, 0, //  53
 0, 2, 1, 2, //  54
 2, 2, 1, 2, //  55
 0, 5, 3, 0, //  56
 2, 3, 1, 5, //  57
 0, 2, 7, 2, //  58
 0, 3, 7, 5, //  59
 2, 7, 7, 2, //  60
 5, 7, 7, 3, //  61
 2, 7, 7, 7, //  62
 7, 7, 7, 7, //  63
 0, 0, 0, 0, //  64
 0, 0, 0, 0, //  65
 0, 0, 1, 0, //  66
 0, 0, 1, 0, //  67
 0, 1, 1, 0, //  68
 1, 1, 1, 1, //  69
 2, 8, 8, 5, //  70
 0,11,11, 2, //  71
 5, 4, 9, 5, //  72
 5, 8, 4,13, //  73
 2,13,12, 2, //  74
 4,12,12,14, //  75
11,12,15,11, //  76
 4,12,15,12, //  77
11,15,15,12, //  78
13,15,15,13, //  79
 0, 0, 0, 0, //  80
 0, 0, 0, 0, //  81
 0, 0, 0, 0, //  82
 0, 0, 1, 0, //  83
 0, 0, 1, 0, //  84
 0, 1, 1, 0, //  85
 0, 5, 1, 0, //  86
 0, 1, 1, 5, //  87
 2, 5, 8, 5, //  88
 4, 1, 1, 6, //  89
 0, 6, 1,14, //  90
 5,14, 6, 4, //  91
 5, 5,15, 5, //  92
13, 7, 1, 6, //  93
14, 7,12, 6, //  94
 6,15,15, 6, //  95
 0, 0, 0, 0, //  96
 0, 0, 0, 0, //  97
 0, 0, 0, 0, //  98
 0, 0, 0, 0, //  99
 0, 0, 1, 0, // 100
 0, 0, 1, 0, // 101
 0, 0, 5, 0, // 102
 0, 5, 1, 0, // 103
 0, 5, 5, 0, // 104
 0, 0, 6, 0, // 105
 0, 2, 6, 0, // 106
 2, 5, 5, 5, // 107
 2, 5, 6, 2, // 108
 5, 7, 1, 5, // 109
 5, 6, 7, 8, // 110
 5, 6, 7,14, // 111
 0, 0, 0, 0, // 112
 0, 0, 0, 0, // 113
 0, 0, 0, 0, // 114
 0, 0, 0, 0, // 115
 0, 0, 0, 0, // 116
 0, 0, 0, 0, // 117
 0, 0, 1, 0, // 118
 0, 0, 5, 0, // 119
 0, 0, 5, 0, // 120
 0, 5, 1, 0, // 121
 0, 5, 5, 0, // 122
 0, 5, 5, 5, // 123
 0, 5, 6, 5, // 124
 5, 6, 1, 6, // 125
 5, 6, 6, 6, // 126
 6, 6, 6, 6, // 127
 0, 0, 0, 0, // 128
 0, 0, 0, 0, // 129
 0, 0, 1, 0, // 130
 0, 0, 1, 0, // 131
 0, 1, 1, 0, // 132
 0, 0,11, 0, // 133
 0, 1, 1,11, // 134
 0, 8,11, 5, // 135
 0, 9, 1, 6, // 136
 0, 6, 9,11, // 137
 5,11,12, 8, // 138
 6,12, 1, 9, // 139
11,12, 1,12, // 140
 0,12,12,12, // 141
11,12,12,12, // 142
12,12,12,12, // 143
 0, 0, 0, 0, // 144
 0, 0, 0, 0, // 145
 0, 0, 0, 0, // 146
 0, 0, 1, 0, // 147
 0, 1, 1, 0, // 148
 0, 8, 1, 0, // 149
 8, 1, 1, 1, // 150
 0,11, 1, 8, // 151
 0, 9, 1, 5, // 152
 0,11, 9, 5, // 153
 0, 8,12, 8, // 154
 6, 9, 1, 9, // 155
 0, 9,12,11, // 156
 5, 9,12, 9, // 157
13, 9,12, 8, // 158
 9,12,12, 9, // 159
 0, 0, 0, 0, // 160
 0, 0, 0, 0, // 161
 0, 0, 0, 0, // 162
 0, 0, 1, 0, // 163
 0, 0, 8, 0, // 164
 0, 8, 1, 0, // 165
 0, 1, 1, 8, // 166
 8, 1, 1, 8, // 167
 0, 8,11, 8, // 168
 0,11, 9, 0, // 169
 5, 8, 9, 8, // 170
 5, 9, 1, 9, // 171
 5, 9, 9,11, // 172
11, 9, 9,11, // 173
 8, 9,12, 9, // 174
 9, 9,12, 9, // 175
 0, 0, 0, 0, // 176
 0, 0, 0, 0, // 177
 0, 0, 0, 0, // 178
 0, 0, 1, 0, // 179
 0, 0, 8, 0, // 180
 0, 0, 8, 0, // 181
 0, 8, 8, 0, // 182
 0, 8, 1, 8, // 183
 0, 8, 8, 8, // 184
 8, 8, 8, 8, // 185
 0, 8, 9, 8, // 186
 8, 8, 9, 8, // 187
 8, 9, 1, 9, // 188
 0, 9, 9, 9, // 189
 8, 9, 9, 9, // 190
 9, 9, 9, 9, // 191
 0, 0, 0, 0, // 192
 0, 0, 0, 0, // 193
 0, 0, 0, 0, // 194
 0, 0, 0, 0, // 195
 0, 0, 0, 0, // 196
 0, 0, 0, 0, // 197
 0, 0, 1, 0, // 198
 0, 0, 1, 0, // 199
 0, 0, 8, 0, // 200
 0, 0, 8, 0, // 201
 0, 8, 1, 0, // 202
 0, 1, 1, 8, // 203
 0, 8, 1, 8, // 204
 8, 1, 1, 8, // 205
 0, 8,11, 8, // 206
 8,11, 1, 8, // 207
 0, 0, 0, 0, // 208
 0, 0, 0, 0, // 209
 0, 0, 0, 0, // 210
 0, 0, 1, 0, // 211
 0, 1, 1, 0, // 212
 0, 8, 1, 0, // 213
 8, 1, 1, 1, // 214
 0, 8,11, 2, // 215
 2, 9, 1, 5, // 216
 8,11,11, 4, // 217
 5,14, 9, 8, // 218
 2, 9,12, 4, // 219
13, 9, 1,14, // 220
 5, 9,15, 9, // 221
13, 9,15,11, // 222
13, 9,15,13, // 223
 0, 0, 0, 0, // 224
 0, 0, 0, 0, // 225
 0, 0, 0, 0, // 226
 0, 0, 1, 0, // 227
 0, 0, 1, 0, // 228
 0, 1, 1, 0, // 229
 0, 1, 1, 1, // 230
 0, 2,11, 0, // 231
 0,11, 1, 2, // 232
14, 1, 1, 1, // 233
 2,14, 1,11, // 234
 8,10, 1, 6, // 235
 5,11, 3,13, // 236
11, 3,12,14, // 237
11,14,15,14, // 238
13, 7,10,13, // 239
 0, 0, 0, 0, // 240
 0, 0, 0, 0, // 241
 0, 0, 0, 0, // 242
 0, 0, 1, 0, // 243
 0, 0, 1, 0, // 244
 0, 1, 1, 0, // 245
 0, 1, 1, 2, // 246
 2, 1, 1, 1, // 247
 0,14, 1, 0, // 248
 2, 1, 1,14, // 249
 2,14,14,11, // 250
14,12, 1, 3, // 251
 0,14,15,14, // 252
14, 9,15, 7, // 253
15,15, 1,15, // 254
15,15,15,15, // 255
};

UI32 R4(UI32 i)
{
	UI32 r = i & 3;
	if (r)
		return i + 4 - r;
	return i;
}

};

//////////////////////////////////////////////////////////////////////
// CDM2SIIVCtx

CDM2SIIVCtx::CDM2SIIVCtx(CGDEdCtx *pEdCtx): m_pEdCtx(NULL)
{
	InitEdCtx(pEdCtx);
}

CDM2SIIVCtx::~CDM2SIIVCtx()
{
	InitEdCtx(NULL);
}

void CDM2SIIVCtx::InitEdCtx(CGDEdCtx *pEdCtx)
{
	if (m_pEdCtx)
		m_pEdCtx->Release();
	if (m_pEdCtx = pEdCtx)
		m_pEdCtx->AddRef();
}

bool CDM2SIIVCtx::ReadRawEntry(const CGDEntry &pos, SizeBuff &fv)
{
	CGDAT &core = m_pEdCtx->GetGDAT();

	if (core.GetRawDataAt(core.GetValueAt(pos), fv))
		return true;
	return false;
}

bool CDM2SIIVCtx::ReadImageEntry(CGDEntry pos, DMGHLiIMGM &imgm)
{
	CGDAT &core = m_pEdCtx->GetGDAT();

	RealEntryMap::iterator
		iterPos = m_map.find(pos),
		iterEnd = m_map.end();
	if (iterPos != iterEnd) {
		pos = iterPos->second;
	}

	int nVal = core.GetValueAt(pos);
	if (nVal < 0)
		return false;

	CExtBM bm;
	HBITMAP hbm;
	bm.Attach(hbm = core.GetImgAt(nVal, imgm));
	if (hbm != NULL) {
		return true;
	}
	return false;
}

int CDM2SIIVCtx::GetEntryValue(const CGDEntry &pos)
{
	CGDAT &core = m_pEdCtx->GetGDAT();

	return core.GetValueAt(pos);
}

void CDM2SIIVCtx::ForceRefreshScene()
{
	VERIFY(GetScrnCtx().RenderScene());
	VERIFY(GetScrnCtx().RenderToBM());
}

void CDM2SIIVCtx::SceneTick()
{
	GetScrnCtx().Tick();

	ForceRefreshScene();
}

void CDM2SIIVCtx::OnInitScreenDone()
{
	CGDAT &core = m_pEdCtx->GetGDAT();

	m_map.clear();

	BYTE bTbl[][2] = {
		{0x08, (BYTE)GetScene()},
		{0x01, 0x03},
	};

	for (int i = 0; i < 2; i++) {
		list<CGDEntry> listEnts;
		core.EnumL4(listEnts, bTbl[i][0], bTbl[i][1]);

		for (; listEnts.size() != 0; listEnts.pop_front()) {
			CGDEntry pos = listEnts.front();
			if (pos.c3() == 1) {
				pos.c5(0);
				pos.c6(0);

				m_map[pos] = listEnts.front();
			}
		}
	}
}

bool CDM2SIIVCtx::_Read_ScenePalette(BYTE bScene, RGBQUAD rq[16])
{
	CGDAT &core = m_pEdCtx->GetGDAT();

	return core.GetScenePalette(bScene, 0, rq);
}

bool CDM2SIIVCtx::_Read_InterfacePalette(RGBQUAD rq[16])
{
	CGDAT &core = m_pEdCtx->GetGDAT();

	return core.GetInterfacePalette(rq);
}

//////////////////////////////////////////////////////////////////////
// CDM2SIIVCtxInCB

void CDM2SIIVCtxInCB::AddCB(CDM2SIIVCtxCB *pSink)
{
	arrSink.Add(pSink);
}

void CDM2SIIVCtxInCB::RemoveCB(CDM2SIIVCtxCB *pSink)
{
	for (int i = 0, n = arrSink.GetSize(); i < n; i++) {
		if (arrSink[i] == pSink) {
			arrSink.RemoveAt(i), i--, n--;
		}
	}
}

void CDM2SIIVCtxInCB::DispatchSIIVCtxUpdate(UINT nHint, void *pHint)
{
	for (int i = 0, n = arrSink.GetSize(); i < n; i++) {
		arrSink[i]->OnSIIVCtxUpdate(nHint, pHint);
	}
}

//////////////////////////////////////////////////////////////////////
// CDM2SIIVScrnCtx

bool CDM2SIIVScrnCtx::InitScreen(int nDM2SIIVT, BYTE bScene, const DMGHLciDM2Pal &DM2Pal)
{
	m_bm.DeleteObject();

	CSize size(640, 400);
	switch (nDM2SIIVT) {
	case DM2SIIVT_PCAT:
		{
			RGBQUAD r4[256];
			for (int x = 0; x < 256; x++)
				CDM2Palo::Conv(DM2Pal.clrs[x], r4[x]);
			if (!m_bm.Create(size, 8, r4, 256))
				return false;
			break;
		}
	case DM2SIIVT_PC9801:
		{
			if (!m_bm.Create(size, 4, g_clrDM2PC9801RGBq, 16))
				return false;
			break;
		}
	case DM2SIIVT_AMIGA:
		{
			BYTE bScenez = bScene;
			if (bScenez == 0xFF)
				bScenez = 0x00;
			SizeBuff fv;
			RGBQUAD rq[32];
			if (!Read_ScenePalette(bScenez, &rq[0]))
				return false;
			if (!Read_InterfacePalette(&rq[16]))
				return false;
			if (!m_bm.Create(size, 8, rq, 32))
				return false;
			break;
		}
	}
	for (int iFM = 0; iFM < 5; iFM++) {
		SizeBuff fv;
		if (Read_FMTbl(iFM, fv)) {
			memcpy(m_bFMTbl[iFM], fv.GetData(), 256);
		} else {
			for (int i = 0; i < 256; i++) m_bFMTbl[iFM][i] = (BYTE)i;
		}
	}

	m_bScene = bScene;
	m_nDM2SIIVT = nDM2SIIVT;

	m_iSceneKey = Get_SceneKey();

	OnInitScreenDone();
	return true;
}

bool CDM2SIIVScrnCtx::RenderToBM()
{
	BITMAP bm;
	BYTE *pbVert;
	if (!m_bm.GetBitmap(bm) || !(pbVert = m_bm.GetPointer()))
		return false;
	UINT nPitch = R4(bm.bmWidthBytes);
	switch (m_nDM2SIIVT) {
	case DM2SIIVT_PCAT:
		{
			for (int y = 0; y < 200; y++) {
				BYTE *pbHorz = pbVert;
				for (int x = 0; x < 320; x++, pbHorz += 2) {
					BYTE c = GetScrnPixel(x, 200 -1 -y);
					pbHorz[0] = c;
					pbHorz[1] = c;
					pbHorz[nPitch +0] = c;
					pbHorz[nPitch +1] = c;
				}
				pbVert += nPitch + nPitch;
			}
			break;
		}
	case DM2SIIVT_PC9801:
		{
			for (int y = 0; y < 200; y++) {
				BYTE *pbHorz = pbVert;
				for (int x = 0; x < 320; x++, pbHorz ++) {
					BYTE c = GetScrnPixel(x, 200 -1 -y);
					const BYTE *pbMark = g_tblDitherPC9801[c];
					pbHorz[0] = 0
						|(pbMark[2] << 4)
						|(pbMark[3]     )
						;
					pbHorz[nPitch] = 0
						|(pbMark[0] << 4)
						|(pbMark[1]     )
						;
				}
				pbVert += nPitch + nPitch;
			}
			break;
		}
	case DM2SIIVT_AMIGA:
		{
			for (int y = 0; y < 200; y++) {
				BYTE *pbHorz = pbVert;
				for (int x = 0; x < 320; x++, pbHorz += 2) {
					BYTE c = GetScrnPixel(x, 200 -1 -y);
					pbHorz[0] =
					pbHorz[1] = 
					pbHorz[0 +nPitch] =
					pbHorz[1 +nPitch] = c & 0x1F;
				}
				pbVert += nPitch + nPitch;
			}
			break;
		}
	}
	return true;
}

bool CDM2SIIVScrnCtx::DrawOrnateSceneImage(DMGHLiIMGM &imgm, BYTE key, BYTE fm, int px, int py, int sx, int sy)
{
	static const CRect rc(0, 40, 0+224, 40+136);

	return DrawImageEntry(imgm, lpsMC, rc, key, 0, fm, false, rc.left + px, rc.top + py, sx, sy);
}

bool CDM2SIIVScrnCtx::DrawSceneImage(DMGHLiIMGM &imgm, BYTE key, BYTE fm, LPS lps, int px, int py, int sx, int sy)
{
	static const CRect rc(0, 40, 0+224, 40+136);

	return DrawImageEntry(imgm, lps, rc, key, 0, fm, false, rc.left + px, rc.top + py, sx, sy);
}

bool CDM2SIIVScrnCtx::DrawImageEntry(DMGHLiIMGM &imgm, LPS lps, CRect rc, UINT key, BYTE al, BYTE fm, bool fUseSys, int px, int py, int sx, int sy)
{
	int cx = imgm.cx;
	int cy = imgm.cy;

	rc.left = __max(0, rc.left);
	rc.right = __min(320, rc.right);
	rc.top = __max(0, rc.top);
	rc.bottom = __min(200, rc.bottom);

	CPoint point = Layout(lps, rc, CSize(cx, cy), CPoint(px, py), CSize(sx, sy));

	px = point.x;
	py = point.y;

	if (al & 0x01) {
		if (sx > 0 && (cx & 1) == 0) px = ((px   ) & (~1));
		if (sx < 0 && (cx & 1) == 0) px = ((px -1) & (~1)) + 1;
	}
	if (al & 0x02) {
		if (sx > 0) px = ((px +1) & (~1));
		if (sx < 0) px = ((px   ) & (~1)) + 1;
	}

	BYTE *pPixTbl = m_bFMTbl[fm];

	switch (m_nDM2SIIVT) {
	case DM2SIIVT_PCAT:
	case DM2SIIVT_PC9801:
		{
			if (imgm.fIs8bpp) {
				for (int y = 0; y < cy; y++) {
					for (int x = 0; x < cx; x++) {
						UINT ty = py + y * sy;
						UINT tx = px + x * sx;
						if (rc.left <= tx && tx < rc.right && rc.top <= ty && ty < rc.bottom) {
							BYTE cPix = imgm.GetPixel(x, y);
							if (key != cPix) SetScrnPixel(tx, ty, pPixTbl[cPix]);
						}
					}
				}
			} else {
				for (int y = 0; y < cy; y++) {
					for (int x = 0; x < cx; x++) {
						UINT ty = py + y * sy;
						UINT tx = px + x * sx;
						if (rc.left <= tx && tx < rc.right && rc.top <= ty && ty < rc.bottom) {
							BYTE cIdx = imgm.GetPixel(x, y) & 15;
							if (key != cIdx) {
								BYTE cPix = imgm.cPal.p.f.index[cIdx];
								SetScrnPixel(tx, ty, pPixTbl[cPix]);
							}
						}
					}
				}
			}
			break;
		}
	case DM2SIIVT_AMIGA:
		{
			if (imgm.fIs8bpp) {
				return false;
			} else {
				BYTE nBase = fUseSys ? 0x10 : 0x00;
				for (int y = 0; y < cy; y++) {
					for (int x = 0; x < cx; x++) {
						UINT ty = py + y * sy;
						UINT tx = px + x * sx;
						if (rc.left <= tx && tx < rc.right && rc.top <= ty && ty < rc.bottom) {
							BYTE cPix = imgm.GetPixel(x, y) & 15;
							if (key != cPix) SetScrnPixel(tx, ty, nBase|cPix);
						}
					}
				}
			}
			break;
		}
	}
	return true;
}

void CDM2SIIVScrnCtx::ClearScreen(BYTE cPix)
{
	for (int y = 0; y < 200; y++)
		for (int x = 0; x < 320; x++)
			SetScrnPixel(x, y, 0);
}

bool CDM2SIIVScrnCtx::RenderScreenTo(CDC &dc, CRect rc)
{
	if (!(HGDIOBJ)m_bm) return false;

	CCompatDC dcMem(&dc);
	dcMem.SaveDC();
	dcMem.SelectObject(&m_bm);
	dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.RestoreDC(-1);

	return true;
}

void CDM2SIIVScrnCtx::FillBlackHatch(CRect rc)
{
	rc.left = __max(0, rc.left);
	rc.top = __max(0, rc.top);
	rc.right = __min(320, rc.right);
	rc.bottom = __min(200, rc.bottom);

	for (int y = rc.top; y < rc.bottom; y++) {
		for (int x = rc.left; x < rc.right; x++) {
			int t = (x & 1) + (y & 1);
			if (t & 1) {
				SetScrnPixel(x, y, 0);
			}
		}
	}
}

CPoint CDM2SIIVScrnCtx::Layout(
	LPS lps,
	CRect rect,
	CSize size,
	CPoint point,
	CSize scale
	)
{
	int f = 0;
	enum {
		fHL=0x00, // horz. left
		fHC=0x01, // horz. center
		fHR=0x02, // horz. right
		fHm=0x03, // horz. bit mask
		fVT=0x00, // vert. top
		fVC=0x04, // vert. center
		fVB=0x08, // vert. bottom
		fVm=0x0C, // vert. bit mask
	};
	switch (lps) {
	default:
		ASSERT(FALSE);
		return point;
	case lpsLT: f |= 0 |fHL |fVT; break;
	case lpsMT: f |= 0 |fHC |fVT; break;
	case lpsRT: f |= 0 |fHR |fVT; break;
	case lpsLC: f |= 0 |fHL |fVC; break;
	case lpsMC: f |= 0 |fHC |fVC; break;
	case lpsRC: f |= 0 |fHR |fVC; break;
	case lpsLB: f |= 0 |fHL |fVB; break;
	case lpsMB: f |= 0 |fHC |fVB; break;
	case lpsRB: f |= 0 |fHR |fVB; break;
	}
	int cx = size.cx * scale.cx;
	int cy = size.cy * scale.cy;
	int rx = (cx < 0) ? 0 : cx;
	int ry = (cy < 0) ? 0 : cy;
	CPoint pt;
	switch (f & fHm) {
	case fHL: pt.x = point.x; break;
	case fHC: pt.x = point.x - (cx) / 2; break;
	case fHR: pt.x = point.x - rx; break;
	}
	switch (f & fVm) {
	case fVT: pt.y = point.y; break;
	case fVC: pt.y = point.y - (cy) / 2; break;
	case fVB: pt.y = point.y - ry; break;
	}
	return pt;
}

//////////////////////////////////////////////////////////////////////
// CDM2SIIVScrnCtxPG

void CDM2SIIVScrnCtxPG::Tick()
{
	m_iTick++;
}

bool CDM2SIIVScrnCtxPG::RenderScene()
{
	ClearScreen(0);

	DMGHLiIMGM imgm;

	{
		CGDEntry pos(0x01, 0x03, 0x01, 0, 0, 0);
		// [02] [06] [04]
		// [0C] [0A] [08]

		// 29x23

		static const CRect rcScreen(0, 0, 320, 200);

		typedef struct Tbl {
			BYTE c4;
			int tx, ty;
		} Tbl;
		static const Tbl tbl[6] = {
			{0x02, 229, 129 },
			{0x06, 260, 129 },
			{0x04, 291, 129 },
			{0x0C, 229, 153 },
			{0x0A, 260, 153 },
			{0x08, 291, 153 },
		};

		for (int i = 0; i < 6; i++) {
			pos.c4(tbl[i].c4);
			if (ReadImageEntry(pos, imgm)) {
				DrawImageEntry(imgm, lpsLT, rcScreen, -1, 0, 0, true, tbl[i].tx, tbl[i].ty, 1, 1);
			}
		}
	}

	{
		// 08 XX-01-00 XX XX: Lower
		// 08 XX-01-01 XX XX: Upper
		// 08 XX-01-23 XX XX: D1-L2
		// 08 XX-01-24 XX XX: D1-L2
		// 08 XX-01-25 XX XX: D1-C (56,32)
		// 08 XX-01-26 XX XX: D1-L1
		// 08 XX-01-27 XX XX: D1-L1

		// 08 XX-01-32 XX XX: D4-C

		BYTE bScene = GetScene();
		int iSceneKey = GetSceneKey();

		switch (bScene) {
		case 0:
			{
#if 0
				CGDEntry pos(0x0A, 0x28, 0x01, 0, 0, 0);

				int iV = 10 * (m_iTick %4);

				// D2-C
				if (!m_map.bAtlas[2][2]) {
					pos.c4(0x0E +iV);
					if (ReadImageEntry(pos, imgm)) {
						DrawOrnateSceneImage(imgm, iSceneKey,0,(224/2),(136/2) +30,-1, 1);
					}
				}

				// D1-L1
				if (!m_map.bAtlas[3][1]) {
					pos.c4(0x0D +iV);
					if (ReadImageEntry(pos, imgm)) {
						DrawOrnateSceneImage(imgm, iSceneKey,0,(81/2) -2,136 -(87/2/2) +8, 1, 1);
					}
				}
				// D1-R1
				if (!m_map.bAtlas[3][3]) {
					pos.c4(0x0D +iV);
					if (ReadImageEntry(pos, imgm)) {
						DrawOrnateSceneImage(imgm, iSceneKey,0,224 -(81/2) -1,136 -(87/2/2) +8,-1, 1);
					}
				}
				// D1-C
				if (!m_map.bAtlas[3][2]) {
					pos.c4(0x0C +iV);
					if (ReadImageEntry(pos, imgm)) {
						DrawOrnateSceneImage(imgm, iSceneKey,0,(224/2) -1,136 -(24/2) -16,-1, 1);
					}
				}

				// D0-L1
				if (!m_map.bAtlas[4][1]) {
					pos.c4(0x0B +iV);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey,0,(41/2) -1,136 -(22/2) +1, 1, 1);
					}
				}
				// D0-R1
				if (!m_map.bAtlas[4][3]) {
					pos.c4(0x0B +iV);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey,0,224 -(41/2),136 -(22/2) +1,-1, 1);
					}
				}
				// D0-C
				if (!m_map.bAtlas[4][2]) {
					pos.c4(0x0A +iV);
					if (ReadImageEntry(pos, imgm)) {
						DrawOrnateSceneImage(imgm, iSceneKey,0,(224/2),136 -10,+1, 1);
					}
				}
#endif
				break;
			}
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			{
				CGDEntry pos(0x08, bScene, 0x01, 0, 0, 0);
#if 0

#elif 1 // Sequenced

#define LOC2ATLAS(Y,X) ((X)|8*(Y))
				typedef struct Tbl
				{	int iAtlas;
					int fm;
					int iImage;
					LPS lps;
					int px;
					int py;
					int sx;
					int sy;
				}	Tbl;

				Tbl _tbl[] = {
					{-1            ,0,0x01,lpsMT,111,  0        ,-1, 1}, //Upper
					{-1            ,0,0x00,lpsMB,112,136        , 1, 1}, //Lower

//					{LOC2ATLAS(0,0),4,0x32,lpsMC,     20, 45, 1, 1}, //D4-L2
//					{LOC2ATLAS(0,1),4,0x32,lpsMC,     66, 45, 1, 1}, //D4-L1
//					{LOC2ATLAS(0,4),4,0x32,lpsMC,223 -20, 45, 1, 1}, //D4-R2
//					{LOC2ATLAS(0,3),4,0x32,lpsMC,223 -66, 45,-1, 1}, //D4-R1
//					{LOC2ATLAS(0,2),4,0x32,lpsMC,    111, 45, 1, 1}, //D4-C

//					{LOC2ATLAS(1,0),3,0x30,lpsLT,      0, 25, 1, 1}, //D3-L2
//					{LOC2ATLAS(1,1),3,0x2E,lpsMT,     49, 25, 1, 1}, //D3-L1
//					{LOC2ATLAS(1,4),3,0x31,lpsRT,223  -0, 25,-1, 1}, //D3-R2
//					{LOC2ATLAS(1,3),3,0x2F,lpsMT,223 -49, 25,-1, 1}, //D3-R1
//					{LOC2ATLAS(1,2),3,0x2D,lpsMT,112    , 25, 1, 1}, //D3-C

//					{LOC2ATLAS(2,0),2,0x2B,lpsLC,  0    , 51, 1, 1}, //D2-L2
//					{LOC2ATLAS(2,1),2,0x29,lpsLC,  0    , 56, 1, 1}, //D2-L1
//					{LOC2ATLAS(2,4),2,0x2C,lpsRC,223    , 51,-1, 1}, //D2-R2
//					{LOC2ATLAS(2,3),2,0x2A,lpsRC,223    , 56,-1, 1}, //D2-R1
					{LOC2ATLAS(2,2),2,0x28,lpsMC,112    , 55, 1, 1}, //D2-C

//					{LOC2ATLAS(3,1),1,0x26,lpsLC,  0    , 65, 1, 1}, //D1-L1
//					{LOC2ATLAS(3,3),1,0x27,lpsRC,223    , 65,-1, 1}, //D1-R1
//					{LOC2ATLAS(3,2),1,0x25,lpsMC,112    , 65, 1, 1}, //D1-C

//					{LOC2ATLAS(4,1),0,0x23,lpsLC,  0    , 68, 1, 1}, //D0-L1
//					{LOC2ATLAS(4,3),0,0x24,lpsRC,223    , 68,-1, 1}, //D0-L1
				};
				int nTbl = sizeof(_tbl) / sizeof(Tbl);
				Tbl *pTblPos = _tbl;
				Tbl *pTblEnd = _tbl + nTbl;

				for (; pTblPos != pTblEnd; pTblPos++) {
					pos.c4(pTblPos->iImage);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, pTblPos->fm, pTblPos->lps, pTblPos->px, pTblPos->py, pTblPos->sx, pTblPos->sy);
					}
				}

#elif 1	// Scene:3 (4, 4)

				//TRACE0("// Upper\n"); // Upper
				pos.c4(0x01);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey,0,112,(31/2),-1, 1);
				}
				//TRACE0("// Lower\n"); // Lower
				pos.c4(0x00);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey,0,112,136 -(76/2), 1, 1);
				}

				if (m_map.bAtlas[0][0]) {
					//TRACE0("// D4-L2\n"); // D4-L2
					pos.c4(0x32);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 4, 112-94 +2, 56-11,+1,+1);
					}
				}
				if (m_map.bAtlas[0][1]) {
					//TRACE0("// D4-L1\n"); // D4-L1
					pos.c4(0x32);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 4, 112-47 +1, 56-11,+1,+1);
					}
				}
				if (m_map.bAtlas[0][4]) {
					//TRACE0("// D4-R2\n"); // D4-R2
					pos.c4(0x32);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 4, 112-3 +94, 56-11,+1,+1);
					}
				}
				if (m_map.bAtlas[0][3]) {
					//TRACE0("// D4-R1\n"); // D4-R1
					pos.c4(0x32);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 4, 112-2 +47, 56-11,-1,+1);
					}
				}
				if (m_map.bAtlas[0][2]) {
					//TRACE0("// D4-C\n"); // D4-C
					pos.c4(0x32);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 4, 112 -1   , 56-11,+1,+1);
					}
				}

				if (m_map.bAtlas[1][0]) {
					//TRACE0("// D3-L2\n"); // D3-L2
					pos.c4(0x30);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 3,  22, 56 -7,+1,+1);
					}
				}
				if (m_map.bAtlas[1][1]) {
					//TRACE0("// D3-L1\n"); // D3-L1
					pos.c4(0x2E);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 3, 112 -73 +10, 56 -7,+1,+1);
					}
				}
				if (m_map.bAtlas[1][4]) {
					//TRACE0("// D3-R2\n"); // D3-R2
					pos.c4(0x31);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 3, 202 -1, 56 -7,-1,+1);
					}
				}
				if (m_map.bAtlas[1][3]) {
					//TRACE0("// D3-R1\n"); // D3-R1
					pos.c4(0x2F);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 3, 112 +73 -11, 56 -7,-1,+1);
					}
				}
				if (m_map.bAtlas[1][2]) {
					//TRACE0("// D3-C\n"); // D3-C
					pos.c4(0x2D);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 3, 112 -1, 56 -7,+1,+1);
					}
				}

				if (m_map.bAtlas[2][0]) {
					//TRACE0("// D2-L2\n"); // D2-L2
					pos.c4(0x2B);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 2,   4   , 56 -5,+1,+1);
					}
				}
				if (m_map.bAtlas[2][1]) {
					//TRACE0("// D2-L1\n"); // D2-L1
					pos.c4(0x29);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 2, 39   , 56 -2,+1,+1);
					}
				}
				if (m_map.bAtlas[2][4]) {
					//TRACE0("// D2-R2\n"); // D2-R2
					pos.c4(0x2C);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 2,220 -1, 56 -5,-1,+1);
					}
				}
				if (m_map.bAtlas[2][3]) {
					//TRACE0("// D2-R1\n"); // D2-R1
					pos.c4(0x2A);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 2,185 -1, 56 -1,-1,+1);
					}
				}
				if (m_map.bAtlas[2][2]) {
					//TRACE0("// D2-C\n"); // D2-C
					pos.c4(0x28);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 2,(224/2), 56   ,+1,+1);
					}
				}

				if (m_map.bAtlas[3][1]) {
					//TRACE0("// D1-L\n"); // D1-L1
					pos.c4(0x26);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 1,  30      +1, 64 +0, 1, 1);
					}
				}
				if (m_map.bAtlas[3][3]) {
					//TRACE0("// D1-R\n"); // D1-R1
					pos.c4(0x27);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 1, 194      -2, 64 +0,-1, 1);
					}
				}
				if (m_map.bAtlas[3][2]) {
					//TRACE0("// D1-C\n"); // D1-C
					pos.c4(0x25);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 1, 112, 64, 1, 1);
					}
				}

				if (m_map.bAtlas[4][1]) {
					//TRACE0("// D0-L1\n"); // D0-L1
					pos.c4(0x23);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 0,(35/2)   , 68, 1, 1);
					}
				}
				if (m_map.bAtlas[4][3]) {
					//TRACE0("// D0-R1\n"); // D0-R1
					pos.c4(0x24);
					if (ReadImageEntry(pos, imgm)) {
						DrawSceneImage(imgm, iSceneKey, 0,224 -(35/2), 68,-1, 1);
					}
				}

#elif 0	// Scene:1

				// Upper
				pos.c4(0x01);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 111, imgm.cy/2, 1, 1);
				}
				// Lower
				pos.c4(0x00);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 111      +1, 136 -1 -imgm.cy/2, 1, 1);
				}
				// D1-L
				pos.c4(0x26);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey,  30        , 64 +0, 1, 1);
				}
				// D1-R
				pos.c4(0x27);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 194      -1, 64 +0,-1, 1);
				}
				// D1-C
				pos.c4(0x25);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 111      +1, 64 +1, 1, 1);
				}
#elif 0	// Scene:2

				// Upper
				pos.c4(0x01);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 111, imgm.cy/2, 1, 1);
				}
				// Lower
				pos.c4(0x00);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 111      +1, 136 -imgm.cy/2, 1, 1);
				}
				// D1-L
				pos.c4(0x26);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey,  38        , 64 +1, 1, 1);
				}
				// D1-R
				pos.c4(0x27);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 186      -1, 64 +1,-1, 1);
				}
				// D1-C
				pos.c4(0x25);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 111      +2, 64 +1, 1, 1);
				}
#elif 0	// Scene:3 (4, 3)

				//TRACE0("// Upper\n"); // Upper
				pos.c4(0x01);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 0, 111 +1, imgm.cy/2,+1, 1);
				}
				//TRACE0("// Lower\n"); // Lower
				pos.c4(0x00);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 0, 111        , 136   -imgm.cy/2,-1, 1);
				}

				//TRACE0("// D4-L2\n"); // D4-L2
				pos.c4(0x32);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 4, 112-94 +2, 56-11,-1,+1);
				}
				//TRACE0("// D4-L1\n"); // D4-L1
				pos.c4(0x32);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 4, 112-47 +1, 56-11,-1,+1);
				}
				//TRACE0("// D4-R2\n"); // D4-R2
				pos.c4(0x32);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 4, 112-3 +94, 56-11,-1,+1);
				}
				//TRACE0("// D4-R1\n"); // D4-R1
				pos.c4(0x32);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 4, 112-2 +47, 56-11,+1,+1);
				}
				//TRACE0("// D4-C\n"); // D4-C
				pos.c4(0x32);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 4, 112 -1   , 56-11,-1,+1);
				}

				//TRACE0("// D3-L2\n"); // D3-L2
				pos.c4(0x31);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 3,  22, 56 -7,+1,+1);
				}
				//TRACE0("// D3-L1\n"); // D3-L1
				pos.c4(0x2F);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 3, 112 -73 +10, 56 -7,+1,+1);
				}
				//TRACE0("// D3-R2\n"); // D3-R2
				pos.c4(0x30);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 3, 202 -1, 56 -7,-1,+1);
				}
				//TRACE0("// D3-R1\n"); // D3-R1
				pos.c4(0x2E);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 3, 112 +73 -11, 56 -7,-1,+1);
				}
				//TRACE0("// D3-C\n"); // D3-C
				pos.c4(0x2D);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 3, 112 -1, 56 -7,+1,+1);
				}

				//TRACE0("// D2-L2\n"); // D2-L2
				pos.c4(0x2C);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 2,   4   , 56 -5,+1,+1);
				}
				//TRACE0("// D2-L1\n"); // D2-L1
				pos.c4(0x2A);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 2, 39   , 56 -1,+1,+1);
				}
				//TRACE0("// D2-R2\n"); // D2-R2
				pos.c4(0x2B);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 2,220 -1, 56 -5,-1,+1);
				}
				//TRACE0("// D2-R1\n"); // D2-R1
				pos.c4(0x29);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 2,185 -1, 56 -2,-1,+1);
				}
				//TRACE0("// D2-C\n"); // D2-C
				pos.c4(0x28);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 2,112 -1, 56   ,+1,+1);
				}

				//TRACE0("// D1-L\n"); // D1-L1
				pos.c4(0x27);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 1,  30      +1, 64 +0, 1, 1);
				}
				//TRACE0("// D1-R\n"); // D1-R1
				pos.c4(0x26);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 1, 194      -2, 64 +0,-1, 1);
				}
				//TRACE0("// D1-C\n"); // D1-C
				pos.c4(0x25);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 1, 111      +1, 64 +0, 1, 1);
				}

				//TRACE0("// D0-L1\n"); // D0-L1
				pos.c4(0x24);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 0,  17   , 68, 1, 1);
				}
				//TRACE0("// D0-R1\n"); // D0-R1
				pos.c4(0x23);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 0, 207 -1, 68,-1, 1);
				}

#else	// Scene:4

				// Upper
				pos.c4(0x01);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 111, imgm.cy/2,-1, 1);
				}
				// Lower
				pos.c4(0x00);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 111      +1, 136   -imgm.cy/2, 1, 1);
				}

				// D1-L
				pos.c4(0x26);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey,  30      +1, 64 +0, 1, 1);
				}
				// D1-R
				pos.c4(0x27);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 194      -2, 64 +0,-1, 1);
				}
				// D1-C
				pos.c4(0x25);
				if (ReadImageEntry(pos, imgm)) {
					DrawSceneImage(imgm, iSceneKey, 111        , 64 +0, 1, 1);
				}
#endif
				break;
			}
		}

	}

	FillBlackHatch(CRect(229, 129, 640, 176));

	return true;
}
