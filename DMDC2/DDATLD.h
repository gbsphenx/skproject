
#pragma once

struct LevelDesc_t
{
	// horz
	BYTE nShiftX;
	// vert
	BYTE nShiftY;
	// 
	WORD w2, w4;
	// 
	BYTE iLv;
	// vert, 1-32
	BYTE cxLv;
	// horz, 1-32
	BYTE cyLv;
	// 
	BYTE bWallOrnateIndices[15];
	// 
	BYTE nWallOrnateIndices;
	// 
	BYTE n10_4_7;
	// 
	BYTE bFloorOrnateIndices[15];
	// 
	BYTE nFloorOrnateIndices;
	// 
	BYTE n10_C_F;
	// 
	BYTE bSharedOrnateIndices[15];
	// 
	BYTE nSharedOrnateIndices;
	// 
	BYTE bCreatureIndices[15];
	// 
	BYTE nCreatureIndices;
	// 
	BYTE n12_8_B, n12_C_F, n14_0_3, n14_4_7, n14_8_B, n14_C_F;
	// 
	UINT nOffset;
	// 
	inline BYTE Width() const { return cxLv & 0x1f; }
	// 
	inline BYTE Height() const { return cyLv & 0x1f; }
	// 
	inline BYTE Depth() const { return iLv; }
	// 
	void Init()
	{
		nShiftX = 0; nShiftY = 0;
		w2 = 0; w4 = 0;
		iLv = 0; cxLv = 32; cyLv = 32;
		nWallOrnateIndices = 0;
		n10_4_7 = 0;
		nFloorOrnateIndices = 0;
		n10_C_F = 0;
		nSharedOrnateIndices = 0;
		nCreatureIndices = 0;
		n12_8_B = 0; n12_C_F = 0; n14_0_3 = 0; n14_4_7 = 0; n14_8_B = 0; n14_C_F = 0;
		nOffset = 0;
	}
	// 
	friend CArchive &operator <<(CArchive &ar, const LevelDesc_t &s)
	{
		ar	<< s.nShiftX << s.nShiftY
			<< s.w2 << s.w4
			<< s.iLv << s.cxLv << s.cyLv
			<< s.nWallOrnateIndices
			<< s.n10_4_7
			<< s.nFloorOrnateIndices
			<< s.n10_C_F
			<< s.nSharedOrnateIndices
			<< s.nCreatureIndices
			<< s.n12_8_B << s.n12_C_F << s.n14_0_3 << s.n14_4_7 << s.n14_8_B << s.n14_C_F;
		ar.Write(s.bWallOrnateIndices, 15);
		ar.Write(s.bFloorOrnateIndices, 15);
		ar.Write(s.bCreatureIndices, 15);
		ar.Write(s.bSharedOrnateIndices, 15);
		return ar;
	}
	// 
	friend CArchive &operator >>(CArchive &ar, LevelDesc_t &s)
	{
		ar	>> s.nShiftX >> s.nShiftY
			>> s.w2 >> s.w4
			>> s.iLv >> s.cxLv >> s.cyLv
			>> s.nWallOrnateIndices
			>> s.n10_4_7
			>> s.nFloorOrnateIndices
			>> s.n10_C_F
			>> s.nSharedOrnateIndices
			>> s.nCreatureIndices
			>> s.n12_8_B >> s.n12_C_F >> s.n14_0_3 >> s.n14_4_7 >> s.n14_8_B >> s.n14_C_F;
		if (false
			|| ar.Read(s.bWallOrnateIndices, 15) != 15
			|| ar.Read(s.bFloorOrnateIndices, 15) != 15
			|| ar.Read(s.bCreatureIndices, 15) != 15
			|| ar.Read(s.bSharedOrnateIndices, 15) != 15
		) {
			AfxThrowArchiveException(CArchiveException::endOfFile);
		}
		return ar;
	}
};
