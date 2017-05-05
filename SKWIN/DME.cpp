//SPX: DME.cpp holds common code outside the main class
#include <StdAfx.h>

#include <DME.h>

using namespace DMEncyclopaedia;
using namespace DM2Internal;





const Ax3 Ax3::Invalid(Ax3::Frm(0xFFFF));






U8 ObjectID::DBType() const { // DBType == FakeDBType
	switch (RealDBType()) {
		default:
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

#if defined(DM2_EXTENDED_DATABASE) && (DM2_EXTENDED_DATABASE != 0)
		case db11:
		case db12:
		case db13:
#endif
		case dbActuator:
			return dbActuator; // it is an actuator!
	}
}

#if (DM2_EXTENDED_JSON == 1)
U8 ObjectID::RealDBType() const {
	if (IsMissile()) {
		// missile is >= 0xFF80, and it is always calculated as dbCloud.
		return dbCloud;
	}
	checkIndirectIndex();
	return (glbDataTypeAndIndexMap[w] >> 16) & 15;
}
#endif
