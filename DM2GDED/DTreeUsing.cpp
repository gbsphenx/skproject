
#include "StdAfx.h"
#include "DTreeUsing.h"

void CDTreeUsing::InitEdCtx(CGDEdCtx *pEdCtx)
{
	if (m_pEdCtx)
		m_pEdCtx->RemoveCB(this),
		m_pEdCtx->Release();
	if (m_pEdCtx = pEdCtx) {
		m_pEdCtx->AddCB(this),
		m_pEdCtx->AddRef(),
		m_pEdCtx->GetOpts(m_aEdOpt);

		m_ilspCnlang = CILSp::GetCnlang(m_aEdOpt.m_cnlang);
		m_ilspInlang = CILSp::GetInlang(m_aEdOpt.m_inlang);
	}
}

CString CDTreeUsing::FormatClass2Name(CGDEntry pos)
{
	if (m_pEdCtx != NULL) {
		switch (pos.c1()) {
		case 0x02: // forspx: SPX:Spell Definition
		case 0x19: // forspx: SPX:Creature AI
			{
				pos.c3(0x05);
				pos.c4(0x18);
				pos.c5(0x00);
				pos.c6(0x00);

				CGDAT &core = m_pEdCtx->GetGDAT();
				CString strText;
				if (core.GetDispTextAt(core.GetCls3ValueAt(pos), strText)) {
					return strText;
				}
				break;
			}
		// SPX add more categories just than Creatures
		case 0x00: // Technical Data
		case 0x08: // Dungeon Graphics (Wallsets)
		case 0x09: // Wall Ornates
		case 0x0A: // Floor Ornates
		case 0x0B: // Door Ornates
		case 0x0C: // Door Buttons
		case 0x0D: // Missiles
		case 0x0E: // Doors
		case 0x0F: // Creatures
		case 0x17: // Skullkeep and Weather (Environments)
		case 0x18: // Teleporters
			{
				pos.c3(0x05);
				pos.c4(0x00);
				pos.c5(0x00);
				pos.c6(0x00);
				m_ilspCnlang.Transform(pos);

				CGDAT &core = m_pEdCtx->GetGDAT();
				CString strText;
				if (core.GetDispTextAt(core.GetValueAt(pos), strText)) {
					return strText;
				}
				break;
			}
		case 0x16: // Heroes
			{
				pos.c3(0x05);
				pos.c4(0x18);
				pos.c5(0x00);
				pos.c6(0x00);
				m_ilspInlang.Transform(pos);	// SPX: Allow Champions to have multilanguage
				CGDAT &core = m_pEdCtx->GetGDAT();
				CString strText;
				if (core.GetDispTextAt(core.GetValueAt(pos), strText)) {
					return strText;
				}
				break;
			}
		case 0x10: // Weapons
		case 0x11: // Clothes
		case 0x12: // Scrolls
		case 0x13: // Potions
		case 0x14: // Chests
		case 0x15: // Miscellaneous
			{
				pos.c3(0x05);
				pos.c4(0x00);
				pos.c5(0x00);
				pos.c6(0x00);
				m_ilspInlang.Transform(pos);
				CGDAT &core = m_pEdCtx->GetGDAT();
				CString strText;
				if (core.GetDispTextAt(core.GetValueAt(pos), strText)) {
					return strText;
				}
				break;
			}
		}
	}
	return "";
}
