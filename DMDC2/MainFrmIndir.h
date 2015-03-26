
#pragma once

#define ELVTF_ERASE_TILE_REC (0x00000001)

class CMainFrameInfluential
{
public:
	// 
	virtual UINT GetDMLv() = 0;
	// 
	virtual void SetDMLv(UINT n) = 0;
	// 
	virtual CSize GetLvSize() = 0;
	// 
	virtual void SetLvSize(CSize size) = 0;
	// 
	virtual UINT GetLvCnt() = 0;
	// 
	virtual void SetLvCnt(UINT n) = 0;
	// 
	virtual UINT GetEditLvTool() = 0;
	// 
	virtual void SetEditLvTool(UINT i) = 0;
	// 
	virtual UINT GetVwLv() = 0;
	// 
	virtual void SetVwLv(UINT n) = 0;
	// 
	virtual UINT GetEditToolF() = 0;

	// 
	static CMainFrameInfluential *Cast(CObject *p)
	{
		return dynamic_cast<CMainFrameInfluential*>(p);
	}
};

class CMainFrameDockSite
{
public:
	// 
	virtual CWnd *GetDockedSite(UINT nID) = 0;
	// 
	virtual BOOL ShowDockedSite(UINT nID, BOOL fShow) = 0;
};
