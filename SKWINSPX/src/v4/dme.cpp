//SPX: DME.cpp holds common code outside the main class
#if defined (SKDLL_EXPORTS) || defined (LIBSKWINDLL_EXPORTS)
#include <StdAfx.h>
#endif // LIBSKWINDLL_EXPORTS

#include <dme.h>

using namespace DMEncyclopaedia;
using namespace DM2Internal;





const Ax3 Ax3::Invalid(Ax3::Frm(0xFFFF));






U8 ObjectID::DBType() const { // DBType == FakeDBType
	switch (RealDBType()) {
		case dbDoor:
		case dbTeleporter:
		case dbText:
		//
		case dbCreature:
		case dbWeapon:
		case dbCloth:
		case dbScroll:
		case dbPotion:
		case dbContainer:
		case dbMiscellaneous_item:
		//
		//
		//
		case dbMissile:
		case dbCloud:
			return RealDBType(); // return as is.
/*
#if defined(DM2_EXTENDED_DATABASE) && (DM2_EXTENDED_DATABASE != 0)
		case db11:
		case db12:
		case db13:
#endif
*/
		case dbActuator:
			return dbActuator; // it is an actuator!
		default:
			return dbNone; // SPX trick more for 32-ref
	}
}