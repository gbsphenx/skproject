
#include "Stdafx.h"
#include "F2C.h"

#include "DTreeUsing.h"

void CF2C::InitEdCtx(CGDEdCtx *pEdCtx)
{
	if (m_pEdCtx)
		m_pEdCtx->Release();
	if (m_pEdCtx = pEdCtx) {
		m_pEdCtx->AddRef(),
		m_pEdCtx->GetOpts(m_aEdOpt);

		m_ilspCnlang = CILSp::GetCnlang(m_aEdOpt.m_cnlang);
		m_ilspInlang = CILSp::GetInlang(m_aEdOpt.m_inlang);
	}
}

CString CF2C::FormatClass2Name(CGDEntry pos)
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
		case 0x0F: // Creatures
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

CString CF2C::FormatMessage(int cls4) {
	CGDEntry pos;
	pos.c1(0x03);
	pos.c2(0x00);
	pos.c3(0x05);
	pos.c4(cls4);
	pos.c5(0x00);
	pos.c6(0x00);
	m_ilspInlang.Transform(pos);
	CGDAT &core = m_pEdCtx->GetGDAT();
	CString strText;
	if (core.GetDispTextAt(core.GetValueAt(pos), strText)) {
		return strText;
	}
	return "";
}
