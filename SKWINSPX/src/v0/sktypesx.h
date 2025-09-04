#ifndef _SKTYPES_EXT_H_
#define _SKTYPES_EXT_H_

#pragma once

#include <sktypes.h>

//------------------------------------------------------------------------------

#if (DM2_EXTENDED_OBJECT_DEF == 0)
	// SPX: only struct for precise data
	struct ObjectID_Reference {
		unsigned short iID:10;
		unsigned short iDBType:4;
		unsigned short iDir:2;
	};
#elif (DM2_EXTENDED_OBJECT_DEF == 1)
	struct ObjectID_Reference {
		unsigned short iID:16;
		unsigned short iReserved:8;
		unsigned short iDBType:6;
		unsigned short iDir:2;
	};
#endif

	struct ObjectRefID {
		U16 idx:10;
		U16 db:4;
		U16 dir:2;
	};
	// 

#if (DM2_EXTENDED_OBJECT_DEF == 0) || !defined DM2_EXTENDED_OBJECT_DEF
	struct ObjectID {
		// FEDCBA98 76543210
		// -----------------
		// DDTTTTii iiiiiiii
		//
		//         DD -> direction. 0=North, 1=E, 2=S, 3=W
		//       TTTT -> record type. 0=door, 1=tele, 2=text, 3=actu, ...
		// iiiiiiiiii -> record index. 0 to 1023.
		//U16 w;
		union {
			U16 w;
			U16 w_00;	// V5
			struct ObjectRefID orif;
		};
		//U32 w32;	// SPX extension experimentation; not here like that, this shifts data then crashes

		ObjectID() { }
		ObjectID(const ObjectID &src): w(src.w) { }
		explicit ObjectID(const ObjectID &src, U16 withNewDir) {
			w = (withNewDir << 14) | (src.w & 0x3fff);
		}
		ObjectID(U16 recordLink): w(recordLink) { }
		ObjectID(U16 dir, U16 dbType, U16 index) {
			w = (dir << 14) | ((dbType & 15) << 10) | (index & 1023);
		}

		operator U16() const { return w; }
		

		void ClearDir() {
			w &= 0x3fff;
		}

		U8 Dir() const { return (U8)(w >> 14); }
		void Dir(U16 val) {
			val &= 3;
			w &= 0x3fff;
			w |= val << 14;
		}

		U16 DBIndex() const { return w & 0x03ff; }
		void DBIndex(U16 val) {
			val &= 1023;
			w &= 0xfc00;
			w |= val;
		}

		// DBType == FakeDBType ... fake DB type for expected sorting: dbDoor < dbTele < dbText < dbActu == db11 == db12 == db13 < dbCreature < ...
		U8 DBType() const;

		U8 RealDBType() const { return (U8)((w & 0x3c00) >> 10); }
		void RealDBType(U16 val) {
			val &= 15;
			w &= 0xc3ff;
			w |= val << 10;
		}

		ObjectID GetAsNorth() const { return w & 0x3fff; }

		static ObjectID MissileRecord(U16 index) { return ObjectID(0xff80 | index); }
		U16 MissileType() { return (w -0xff80)&0x7f; }
		bool IsMissile() const { return w >= 0xff80; }

		static ObjectID Raw(U16 w) { ObjectID rl; rl.w = w; return rl; };
		//static ObjectID Raw32(U32 w32) { ObjectID rl; rl.w32 = w32; return rl; };
		//static ObjectID Raw32(U32 w32) { ObjectID rl; rl.w = ((U16)w32 & 0xFFFF); return rl; };
	};
#else	// SPX: Experimental definition of 32 bits object pointer => NOT READY AND THIS CRASHS ..
#define OID_OBJECT_BITS		16
#define OID_SHIFT_RESERVED	OID_SHIFT_DB - 8
#define OID_SHIFT_DB		OID_SHIFT_DIR - 6
#define OID_SHIFT_DIR		OID_OBJECT_BITS - 2	
#define OID_MASK_DB_TYPE	0x3F	/// 0x3F for 64-DB // 0x0F in standard for 16-DB
#define OID_MASK_RESERVED	0xFF
#define OID_MASK_ID			0xFFFF

	typedef struct {
		U32 id:16;	// id
		U32 r:8;	// reserved
		U32 t:6;	// db type
		U32 d:2;	// direction
	} ref32b;

	union ObjectID {
		// FEDCBA98 76543210 FEDCBA98 76543210
		// -----------------------------------
		// DDTTTTTT rrrrrrrr iiiiiiii iiiiiiii
		//
		//         DD -> direction. 0=North, 1=E, 2=S, 3=W
		//     TTTTTT -> record type. 0=door, 1=tele, 2=text, 3=actu, ... (extended to 64)
		//   rrrrrrrr -> reserved, for other specific needs if required
		//   iiiiiiii -> 16-bits object index, up to 65 536
		//		  U32 -> w
		U32 w;
		
		//U32 id:16;	// id
		//U32 r:8;	// reserved
		//U32 t:6;	// db type
		//U32 d:2;	// direction
		ref32b rb;
		
		ObjectID() { }
		ObjectID(const ObjectID &src): w(src.w) { }
		explicit ObjectID(const ObjectID &src, U16 withNewDir) {
			//w = (withNewDir << 14) | (src.w & 0x3fff);
			//w = (withNewDir << OID_SHIFT_DIR) | ((t & 15) << OID_SHIFT_DB) | (src.w & OID_MASK_ID);
			rb.d = withNewDir;
			w = (rb.d << OID_SHIFT_DIR) | ((rb.t & OID_MASK_DB_TYPE) << OID_SHIFT_DB) | ((rb.r & OID_MASK_RESERVED) << OID_SHIFT_RESERVED) | (rb.id & OID_MASK_ID);
		}
		ObjectID(U32 recordLink): w(recordLink) { }
		ObjectID(U16 dir, U16 dbType, U16 index) {
			rb.id = index;
			rb.t = dbType;
			rb.d = dir;
			rb.r = 0;
			//w = (rb.d << OID_SHIFT_DIR) | ((rb.t & OID_MASK_DB_TYPE) << OID_SHIFT_DB) | ((rb.r & OID_MASK_RESERVED) << OID_SHIFT_RESERVED) | (rb.id & OID_MASK_ID);
		}

		operator U32() const { 
			return w; }
		

		void ClearDir() {
			rb.d = 0;
			//w = (rb.d << OID_SHIFT_DIR) | ((rb.t & OID_MASK_DB_TYPE) << OID_SHIFT_DB) | ((rb.r & OID_MASK_RESERVED) << OID_SHIFT_RESERVED) | (rb.id & OID_MASK_ID);
		}

		U8 Dir() const { return (U8)(rb.d); }
		void Dir(U16 val) {
			rb.d = val & 0x03;
			w = (rb.d << OID_SHIFT_DIR) | ((rb.t & OID_MASK_DB_TYPE) << OID_SHIFT_DB) | ((rb.r & OID_MASK_RESERVED) << OID_SHIFT_RESERVED) | (rb.id & OID_MASK_ID);
		}

		U16 DBIndex() const { return rb.t; }
		void DBIndex(U16 val) {
			rb.t = val & OID_MASK_DB_TYPE;
			//w = (rb.d << OID_SHIFT_DIR) | ((rb.t & OID_MASK_DB_TYPE) << OID_SHIFT_DB) | ((rb.r & OID_MASK_RESERVED) << OID_SHIFT_RESERVED) | (rb.id & OID_MASK_ID);
		}

		// DBType == FakeDBType ... fake DB type for expected sorting: dbDoor < dbTele < dbText < dbActu == db11 == db12 == db13 < dbCreature < ...
		U8 DBType() const;

		U8 RealDBType() const { return (U8)(rb.t); }
		void RealDBType(U16 val) {
			val &= OID_MASK_DB_TYPE;
			rb.t = val;
			//w = (rb.d << OID_SHIFT_DIR) | ((rb.t & OID_MASK_DB_TYPE) << OID_SHIFT_DB) | ((rb.r & OID_MASK_RESERVED) << OID_SHIFT_RESERVED) | (rb.id & OID_MASK_ID);
		}

		ObjectID GetAsNorth() const { return 0; }

		// -- SPX: TO BE REDEFINED
		static ObjectID MissileRecord(U16 index) { return ObjectID(0xff80 | index); }
		U16 MissileType() { return (w -0xff80)&0x7f; }
		bool IsMissile() const { return w >= 0xff80; }
		static ObjectID Raw(U32 w) { ObjectID rl; rl.w = w; return rl; };
	};
#endif // #ifndef DM2_EXTENDED_OBJECT_DEF


	// SPX: sk57c8 renamed LeaderPossession
	struct LeaderPossession { // 22 bytes
		ObjectID object;	// @0 w0
		U8 *pb2;						// @2 *pb2
		U8 b6[16];						// @6 b6[16]
	};


//------------------------------------------------------------------------------

#endif // _SKTYPES_EXT_H_