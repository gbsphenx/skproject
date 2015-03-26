// DMTarTrack.cpp: CDMTarTrack クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "exp00069.h"
#include "DMTarTrack.h"
#include "DM2NameDic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CDMTarTrack::Clear()
{
	m_sentry.clear();
	m_trackFrom = AtlasPos_t();
}

void CDMTarTrack::Track(CDDAT &rDD, AtlasPos_t ap, CDMTarTrackLev ttlv)
{
	m_pDDIf = &rDD;
	m_sentry.clear();
	m_trackFrom = ap;
	m_ttlv = ttlv;

	Track(ATP_t(ap), 0, m_ttlv.nDepth, TrackSrc());

	if (m_ttlv.nDepth != 0) {
		BackTrack(ATP_t(ap));
	}
}

bool CDMTarTrack::BackTrack(ATP_t ap)
{
	Atlas_t at;
	HRESULT hr;
	for (BYTE z=0; z<64; z++) {
		bool crossTrack = (z != ap.Z());
		if (!m_ttlv.fDeepBack && crossTrack)
			continue;
		if (FAILED(hr = m_pDDIf->GetMapAtlasAt(z, at)))
			continue;

		for (BYTE y=0; y<32; y++) {
			for (BYTE x=0; x<32; x++) {
				BYTE c = at.c[y][x];
				if ((c & 0x10) == 0) continue;
				bool fWall = CDDAT::IsTileWall(c);
				RN rn = at.r[y][x];
				while (rn != RN_END && rn != RN_NIL) {
					DMDBU rec;
					if (FAILED(hr = m_pDDIf->GetDBRecAt(DBIPos_t(rn), rec)))
						break;
					switch (rn.db()) {
					case 1: // teleporter
						{
							WORD w2 = rec.r1.w2;
							WORD w4 = rec.r1.w4;
							BYTE tz = (w4 / 0x100);
							BYTE ty = (w2) & 31;
							BYTE tx = (w2 / 32) & 31;

							if (ap.X() == tx && ap.Y() == ty && ap.Z() == tz) {
								Tar t;
								t.ttt = tttTele;
								t.ap = ap.T();

								m_sentry.insert(make_pair(AtlasPos_t(z, x, y), t));
							}
							break;
						}
					case 3: // actuator
						{
							WORD w2 = rec.r3.w2;
							WORD w6 = rec.r3.w6;
							const CDM2ActuHint &r = CDM2NameDic::GetInstance().GetActuHint(w2 & 127, fWall);
							if (r.a3t == a3tLocator) {
								BYTE tz = z;
								if (r.a1t == a1tMap) {
									tz = (w2 / 128) & 63;
								}
								BYTE tx = (w6 / 64 / 32) & 31;
								BYTE ty = (w6 / 64) & 31;

								if (ap.X() == tx && ap.Y() == ty && ap.Z() == tz) {
									Tar t;
									t.ttt = tttBack;
									t.ap = ap.T();

									m_sentry.insert(make_pair(AtlasPos_t(z, x, y), t));
								}
							}
							break;
						}
					}
					rn = rec.r0.next;
				}
			}
		}
	}
	return true;
}

BOOL CDMTarTrack::Track(ATP_t ap, int nDeep, int nLoop, TrackSrc ts)
{
	if (nLoop < 1) return TRUE;
	if (m_sentry.find(ap) != m_sentry.end()) return FALSE;

	AtlasBit_t ab;
	HRESULT hr;
	if (FAILED(hr = m_pDDIf->GetAtlasBitAt(ap.T(), ab)))
		return FALSE;
	bool fWall = CDDAT::IsTileWall(ab.c);
	RN rn = ab.r;
	if (rn != RN_NIL) {
		bool fExact = nDeep != 0 && m_ttlv.fNorm;
		while (rn != RN_END) {
			DMDBU rec;
			if (FAILED(hr = m_pDDIf->GetDBRecAt(DBIPos_t(rn), rec)))
				break;
			switch (rn.db()) {
			case dbTeleporter:
				{
					if (fExact && ts.ts == tsTrap) break;

					WORD w2 = rec.r1.w2;
					WORD w4 = rec.r1.w4;
					BYTE z = (w4 / 0x100);
					BYTE x = (w2) & 31;
					BYTE y = (w2 / 32) & 31;
					Tar t;
					t.ttt = tttTele;
					t.ap = AtlasPos_t(z, y, x);

					m_sentry.insert(make_pair(ap, t));

					Track(ATP_t(t.ap), nDeep +1, nLoop -1, TrackSrc(tsTele));
					break;
				}
			case dbActuator:
			case db11: case db12: case db13:
				{
					if (fExact && ts.ts != tsNo) if (ts.ts != tsTrap || (fWall && ap.room() != rn.room())) break;

					WORD w2 = rec.r3.w2;
					WORD w6 = rec.r3.w6;
					const CDM2ActuHint &r = CDM2NameDic::GetInstance().GetActuHint(w2 & 127, fWall);
					if (r.a3t == a3tLocator) {
						BYTE z = ap.Z();
						bool fOther = false;
						if (r.a1t == a1tMap) {
							z = (w2 / 128) & 63;
							fOther = true;
						}
						BYTE x = (w6 / 64 / 32) & 31;
						BYTE y = (w6 / 64) & 31;
						BYTE room = (w6 / 16) & 3;
						Tar t;
						t.ttt = tttFwd;
						t.ap = AtlasPos_t(z, x, y);

						m_sentry.insert(make_pair(ap, t));

						if (r.a3lt == a3ltTrap) Track(ATP_t(t.ap, room), nDeep +1, nLoop -1, TrackSrc(tsTrap));
					}
					break;
				}
			}
			rn = rec.r0.next;
		}
	}

	return TRUE;
}
