#include "addon.h"
#include "driver.h" // DRVW_mousevisicnt
#include "dm2global.h"
#include "dm2data.h"
#include "mdata.h"
#include "src/c_alloc.h"
#include "src/c_dballoc.h"
#include "src/c_mouse.h"
#include "src/c_gfx_pal.h"
#include "src/c_gfx_stretch.h"
#include "src/c_gfx_misc.h"
#include "src/c_gfx_blit.h"
#include "src/c_gfx_str.h"
#include "src/c_gfx_fill.h"
#include "src/c_gfx_decode.h"
#include "src/c_savegame.h"
#include "calls.h"



#include "../skwin/defines.h"

#include "dm2debug.h"


// SPX: Name of skill
const char* getSkillName(ui8 skill)
{
	static char text[128] = {0};
	if (skill == SKILL_FIGHTER_GLOBAL) return "FIGHTER";
	if (skill == SKILL_NINJA_GLOBAL) return "NINJA";
	if (skill == SKILL_PRIEST_GLOBAL) return "PRIEST";
	if (skill == SKILL_WIZARD_GLOBAL) return "WIZARD";
	if (skill == SKILL_FIGHTER_SWING) return "(F1)SWING";
	if (skill == SKILL_FIGHTER_THRUST) return "(F2)THRUST";
	if (skill == SKILL_FIGHTER_BLUNT) return "(F3)BLUNT";
	if (skill == SKILL_FIGHTER_PARRY) return "(F4)PARRY";
	if (skill == SKILL_NINJA_STEAL) return "(N1)STEAL";
	if (skill == SKILL_NINJA_FIGHT) return "(N2)FIGHT";
	if (skill == SKILL_NINJA_THROW) return "(N3)THROW";
	if (skill == SKILL_NINJA_SHOOT) return "(N4)SHOOT";
	if (skill == SKILL_PRIEST_IDENTIFY) return "(P1)IDENTIFY";
	if (skill == SKILL_PRIEST_HEAL) return "(P2)HEAL";
	if (skill == SKILL_PRIEST_INFLUENCE) return "(P3)INFLUENCE";
	if (skill == SKILL_PRIEST_DEFEND) return "(P4)DEFEND";
	if (skill == SKILL_WIZARD_FIRE) return "(W1)FIRE";
	if (skill == SKILL_WIZARD_AIR) return "(W2)AIR";
	if (skill == SKILL_WIZARD_EARTH) return "(W3)EARTH";
	if (skill == SKILL_WIZARD_WATER) return "(W4)WATER";
	if (text[0] != 0)
		return text;
	return "UNKNOWN";
}
