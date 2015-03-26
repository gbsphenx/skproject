// DMTarTrack.h: CDMTarTrack クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMTARTRACK_H__D8262E2C_CAD5_4266_A10A_510B4987A3A9__INCLUDED_)
#define AFX_DMTARTRACK_H__D8262E2C_CAD5_4266_A10A_510B4987A3A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DDAT.h"
#include "DMDBRMask.h"

class CDMTarTrackLev
{
public:
	// 
	int nDepth;
	// 
	bool fNorm;
	// 
	bool fBack;
	// 
	bool fDeepBack;

	// 
	CDMTarTrackLev()
	{
		nDepth = 1;
		fNorm = false;
		fBack = true;
		fDeepBack = true;
	}
};

enum {
	tttFwd,
	tttBack,
	tttTele,
	tttCross,
};

class CDMTarTrack  
{
	// 
	CDDAT *m_pDDIf;
	// 
	CDMTarTrackLev m_ttlv;

public:
	// 
	struct Tar {
		// 
		BYTE ttt;
		// 
		AtlasPos_t ap;
	};
	// 
	typedef multimap<ATP_t, Tar> Sentry;
	// 
	Sentry m_sentry;
	// 
	AtlasPos_t m_trackFrom;

	// 
	void Track(CDDAT &rDD, AtlasPos_t ap, CDMTarTrackLev ttlv);
	// 
	void Clear();
	// 
	bool HasAny() const { return !m_sentry.empty(); }

public:
	// 
	enum {
		tsNo,
		tsTrap,
		tsTele,
	};
	// 
	struct TrackSrc {
		// 
		int ts;

		// 
		TrackSrc(): ts(tsNo) { }
		// 
		TrackSrc(int ts): ts(ts) { }
	};

private:
	// 
	BOOL Track(ATP_t ap, int nDeep, int nLoop, TrackSrc ts);
	// 
	bool BackTrack(ATP_t ap);
};

#endif // !defined(AFX_DMTARTRACK_H__D8262E2C_CAD5_4266_A10A_510B4987A3A9__INCLUDED_)
