#ifndef _SK_DM_ENCYCLOPEDIA_
#define _SK_DM_ENCYCLOPEDIA_

//------------------------------------------------------------------------------

#pragma once

#include <skver.h>
#include <sktypes.h>
#include <skdefine.h>

//#include <skparam.h> // SPX: allow control of DM1 vs DM2 behaviour

//#if !defined (SKDOSV5) && defined (_MSC_VER)
#if defined (_MSC_VER)
//#include <winatl.h>	// ATLASSERT
#include <atlbase.h>	// ATLASSERT
#endif // _MSC_VER

#ifdef __LINUX__
#include <stdint.h> // for xintx_t
#endif // __LINUX__

// Check Linux 64 compilation env
#if defined(__LP64__) || defined (_LP64)
#define __LINUX64__
typedef int64_t i64;
typedef uint64_t U64;
typedef uint64_t X64;
#endif //

/*
// Signed (i)nteger
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

// (U)nsigned integer
typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;

// Sign indetermined integer. Either signed or unsigned. Treat as unsigned in default.
typedef uint8_t X8;
typedef uint16_t X16;
typedef uint32_t X32;
*/

// 64-bit OS support. It is not functional yet. However it is just remark for future portability.
#ifdef _Wp64
typedef i32 __w64 i32ptr;
typedef U32 __w64 U32ptr;
typedef X32 __w64 X32ptr;
#elif defined(__LINUX64__)
typedef i64 i32ptr;
typedef U64 U32ptr;
typedef X64 X32ptr;
#else
typedef i32 i32ptr;
typedef U32 U32ptr;
typedef X32 X32ptr;
#endif

#pragma pack(push, 1)

#if DM2_EXTENDED_MODE == 1
	#define MEM_EMS_MB 32
#else
	#define MEM_EMS_MB 16
#endif // SPX EXT MODE

#define MAX_CRAM (1024U*640)		// Conventional memory: 640KiB
#define MAX_CEMS (1024U*1024*MEM_EMS_MB)	// EMS memory space: 6MiB or more
#define MAX_VRAM (1024U*64)		// VRAM memory space: 64 KiB

namespace DM2Internal {
	// 
	struct Ax3 { // 2 bytes
		U16 w0;

		// fedcba98 76543210
		// 00000011 ???????? 0003
		// ???????? 11100000 00e0
		// 11111100 00011111 fc1f

		U16 GetX() const { return (w0)&31; }
		U16 GetY() const { return (w0 >> 5)&31; }
		U16 GetMap() const { return (w0 >> 10)&63; }

		void SetX(U16 val);
		void SetY(U16 val);
		void SetMap(U16 val);

		static const Ax3 Invalid;

		bool Is0() const { return w0 == 0; }
		bool IsInvalid() const { return w0 == (U16)-1; }
		bool IsValid() const { return !IsInvalid(); }

		Ax3() { }

		static Ax3 Frm(U16 w) {
			Ax3 a;
			a.w0 = w;
			return a;
		}

#ifdef _MSC_VER
		__declspec(property(get = GetX)) U16 X;
		__declspec(property(get = GetY)) U16 Y;
		__declspec(property(get = GetMap)) U16 Z;
#endif
	};
};

namespace DMEncyclopaedia {

	// visit DM&csb Encyclopaedia
	// http://dmweb.free.fr/?q=node/217

	// 
	struct File_header {
		U16 w0; //  Random seed used to display random 
		U16 cbMapData; // Size of global map data in byt
		U8 nMaps; // Number of maps (in Dungeon Mast;
		U8 b5; // # Unused, padding
		U16 cwTextData; // Text data size in words (17
		U16 w8; // Starting party position (mus
		U16 cwListSize; // Object list size in wor
		U16 nRecords[16]; // Number of objects of each type:

		U16 StartPartyPosX() const { return (w8)&0x1f; }
		U16 StartPartyPosY() const { return (w8 >> 5)&0x1f; }

		// w0 = w0
		// w2 = cbMapData
		// b4 = nMaps
		// b5 = b5
		// w6 = cwTextData
		// w8 = w8
		// w10 = cwListSize
		// w12 = nRecords[0]
		// w14 = nRecords[1]
		// w16 = nRecords[2]
		// w18 = nRecords[3]
		// w20 = nRecords[4]
		// w22 = nRecords[5]
		// w24 = nRecords[6]
		// w26 = nRecords[7]
		// w28 = nRecords[8]
		// w30 = nRecords[9]
		// w32 = nRecords[10]
		// w34 = nRecords[11]
		// w36 = nRecords[12]
		// w38 = nRecords[13]
		// w40 = nRecords[14]
		// w42 = nRecords[15]
	};

	// Added this structure for more precision on Map_definitions without replacing its variable definition .. yet
	struct Map_definition_info
	{
		unsigned short mapOffset;
		char unused1[4];
		unsigned char xOffset; // apparently up to 255
		unsigned char yOffset;
		unsigned short level:4; // position of the level in the level stack. 0 is top.
		unsigned short unused2:2;
		unsigned short xDim:5; // values 0x00 to 0x1F give dimensions 1 to 32.
		unsigned short yDim:5;
		unsigned char nWalls:4;
		unsigned char rWalls:4;
		unsigned char nFloors:4;	
		unsigned char rFloors:4;	
		unsigned char nOrnates:4;	
		unsigned char nMonsters:4;
		unsigned char unknown1:4;		
		unsigned char depth:4; // determinates the default level of generated monsters.	
		unsigned char unknown2:4;
		unsigned char tileset:4;
		unsigned char door1:4;
		unsigned char door2:4;
	};

	// 
	struct Map_definitions {
		U16 w0; // w0: Offset of map data i
		U16 bGfxFlags; // w2: unused in Dungeon Master and Chaos Strikes Back. Some values in Dungeon Master II.
		U16 w4; // w4: Unused
		U8 b6; // b6: Map offset x
		U8 b7; // b7: Map offset y
		U16 w8; // w8: Map size
		U16 w10; // w10: Number of graphics
		U16 w12; // w12: 1 U16:
		U16 w14; // w14: 1 U16: Door graphics.

		U16 Offset() const { return w0; }

		U16 UseUpperPitfall() const { return (bGfxFlags&1); }
		U16 UseLowerPitfall() const { return (bGfxFlags >> 1)&1; }
		U16 w2_2_2() const { return (bGfxFlags >> 2)&1; }
		U16 UseUpStaircase() const { return (bGfxFlags >> 3)&1; }
		U16 UseDownStaircase() const { return (bGfxFlags >> 4)&1; }
		U16 UseTeleporter() const { return (bGfxFlags >> 5)&1; }
		U16 w2_6_6() const { return (bGfxFlags >> 6)&1; }
		U16 UseDoor0() const { return (bGfxFlags >> 7)&1; }
		U16 UseDoor1() const { return (bGfxFlags >> 8)&1; }
		U8 b2_7_7() const { return (U8)((bGfxFlags >> 7)&1); }
		U8 b3_0_0() const { return (U8)((bGfxFlags >> 8)&1); }

		U8 MapOffsetX() const { return b6; }
		U8 MapOffsetY() const { return b7; }

		U8 Column() const { return ((w8 >> 6)&31)+1; }
		U8 Row() const { return ((w8 >> 11)&31)+1; }
		U16 Level() const { return w8&63; }				// altitude of map
		U8 RawColumn() const { return ((w8 >> 6)&31); }
		U8 RawRow() const { return ((w8 >> 11)&31); }
		U8 WallGraphics() const { return (U8)(w10 & 15); }
		U8 WallGraphicsRandomDecorations() const { return (U8)((w10 >> 4)&15); }
		U8 FloorGraphics() const { return (U8)((w10 >> 8)&15); }
		U8 CreaturesTypes() const { return (U8)((w12 >> 4)&15); }
		U8 DoorDecorationGraphics() const { return (U8)(w12 & 15); }
		U8 Difficulty() const { return (U8)((w12 >> 12)&0x0F); }
		U8 MapGraphicsStyle() const { return (w14 >> 4)&15; }
		U8 DoorType0() const { return (U8)((w14 >> 8)&15); }
		U8 DoorType1() const { return (U8)((w14 >>12)); }
	};

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


	// SPX: alternate door structure with precision before merging
	struct Door_Info
	{
		ObjectID next;
		unsigned short type:1;				// 1
		unsigned short ornate:4;			// 2-5
		unsigned short verticalOpening:1;	// 6
		unsigned short button:1;			// 7
		unsigned short destroyable:1;		// 8
		unsigned short bashable:1;			// 9
		unsigned short bit09:1;				// 10
		unsigned short bit10:1;				// 11
		unsigned short buttonstate:1;		// 12	released / pushed
		unsigned short bit12:1;				// 13
		unsigned short bit13:1;				// 14
// the next 2 bits are unused in DM2 -- I use them for extended mode
		unsigned short unused:1;
		unsigned short locked:1;
/*
		unsigned short unused:2;
		unsigned short bit13:1;				// 14
		unsigned short bit12:1;				// 13
		unsigned short buttonstate:1;		// 12	released / pushed
		unsigned short bit10:1;				// 11
		unsigned short bit09:1;				// 10
		unsigned short bashable:1;			// 9
		unsigned short destroyable:1;		// 8
		unsigned short button:1;			// 7
		unsigned short verticalOpening:1;	// 6
		unsigned short ornate:4;			// 2-5
		unsigned short type:1;				// 1
*/
	};

	// 
	struct Door {
		ObjectID next; // w0 Next object ID.
		U16 w2; // Door attributes.

		U8 Button() const { return (U8)((w2 >> 6)&1); }
		U8 DoorType() const { return (U8)(w2 & 1); }
		void w2_b_b(U16 val) {
			val &= 1;
			w2 &= 0xf7ff;
			w2 |= val << 11;
		}
		U16 ButtonState() const { return (w2 >> 11)&1; }	// w2_b_b()
		U16 OpeningDir() const { return (w2 >> 5)&1; }
		U8 DoorBit09() const { return (U8)((w2 >> 9)&1); }		// b3_1_1() Bit09
		void DoorBit09(U8 val) {
			val &= 1;
			w2 &= 0xfdff;
			w2 |= val << 9;
		}
		// Bit10 : door is moving or not (opening or closing)
		U8 DoorBit10() const { return (U8)((w2 >>10)&1); }		// b3_2_2()
		void DoorBit10(U8 val) {
			val &= 1;
			w2 &= 0xfbff;
			w2 |= val << 10;
		}
		void SetButtonState(U8 val) {	// b3_3_3 : 0 = release / 1 = pushed in
			val &= 1;
			w2 &= 0xf7ff;
			w2 |= val << 11;
		}
		U8 DestroyablebyFireball() const { return (w2 >> 7)&1; }
		U8 DoorBit12() const { return (U8)((w2 >> 12)&1); }	// b3_4_4()
		U8 DoorBit13() const { return (U8)((w2 >> 13)&1); }	// b3_5_5()
		U8 DoorBit13C() const { return (U8)((w2 >>13)&1); }		// w2_d_d() // same as 13
		void DoorBit13(U16 nibble) {	// 13B takes U8 instead ?
			nibble &= 1;
			w2 &= 0xDFFF;
			w2 |= nibble << 5 << 8;
		}
		void DoorBit13B(U8 val) {
			val &= 1;
			w2 &= 0xDFFF;
			w2 |= val << 13;
		}

		U8 BashablebyChopping() const { return (w2 >> 8)&1; }
		void DoorBit12(U8 val) {	// b3_4_4
			w2 &= 0xefff;
			val &= 1;
			w2 |= U16(val) << 12;
		}
		U16 OrnateIndex() const { return (w2>>1)&15; }
		U8 b2_5_5() const { return (U8)((w2 >> 5)&1); }	// Why this and OpeningDir func ?
	};
	// 
	struct Teleporter {
		ObjectID w0; // Next object ID.
		U16 w2; // Teleporter attributes
		U16 w4; // Destination map

		U8 DestinationX() const { return (w2 >> 0)&31; } // M!2,0,1F
		U8 DestinationY() const { return (w2 >> 5)&31; } // M!2,5,1F
		U8 DestinationMap() const { return (w4 >> 8)&255; } // M!4,8,00FF
		U8 Scope() const { return (U8)((w2 >> 13)&3); } // M!3,5,03
		U8 Sound() const { return (U8)((w2 >> 15)&1); } // M!3,7,01
		U8 Rotation() const { return (U8)((w2 >> 10)&3); } // M!3,2,03
		U8 RotationType() const { return (U8)(w2 >> 12)&1; } // M!3,4,01
		U16 w4_0_0() const { return w4 & 1; } // M!4,0,01
		U8 b4_1_2() const { return U8((w4 >> 1)&3); } // M!4,1,03
		U8 b5_0_7() const { return U8(w4 >> 8); } // M!5,0,FF

		// SPX : set
		void DestinationX(U8 val) { w2 = 0; };
		void DestinationY(U8 val) { w2 = 0; };
		void DestinationMap(U8 val) { w4 = ((val)<<8)&0xFF + (w4 & 0xFF); };
		void Scope(U8 val) { w2 = 0; };
		void Sound(U8 val) { w2 = 0; };
		void Rotation(U8 val) { w2 = 0; };
		void RotationType(U8 val) { w2 = 0; };

	};
	// 
	enum { // teleporter scope
		scopeItems = 0,
		scopeCreatures = 1,
		scopeItemsParty = 2,
		scopeEverything = 3,
	};
	// 

	// SPX: Add simple actuator structure to handle faster "Text" converted to Simple Actuator
	struct SimpleActuator { 
		ObjectID w0; // Next object ID
		unsigned short	activated:1;	// And for Text Visibility
		unsigned short 	mode:2;			// 0 = text / 1 = active decoration / 2 = complex
		unsigned short	graphism:8;	//2^7 = 128 (range: 0-7F)
		unsigned short	extendedUsage:5;	// 0 for still image, etc ...
	};

	struct Text {
		ObjectID w0; // Next object ID.
		U16 w2; // Referred text in text data.

		U8 TextMode() const { return (w2 >> 1)&3; } // M!2,1,03
		U16 TextIndex() const { return (w2 >> 3)&0x1fff; } // M!2,3,1FFF
		U8 SimpleTextExtUsage() const { return (TextIndex() >> 8)&0x1f; } // M!3,3,1F
		U8 TextVisibility() const { return (U8)(w2 & 0x0001); } // M!2,1,01
		// SPX: DM2 => bit 1 set = visible // DM1 => bit 1 set = invisible (deactivated)
		//U8 TextVisibility() {
		//	U8 iVisibilityFlag = (U8)(w2 & 0x0001);
		//	//if (SkCodeParam::bDM1Mode)
		//	//	return !iVisibilityFlag;
		//	return iVisibilityFlag;
		//}

		void TextVisibility(U16 val) {
			val &= 0x0001;
			//if (SkCodeParam::bDM1Mode)
			//	val = !val; // invert value 0 <-> 1 because DM1 visibility means INVISIBLE = 0
			w2 &= 0xfffe;
			w2 |= val;
		}
		U16 OrnateIndex() const { return (w2 >> 3)&0x1fff&0xff; }
		U16 w2_c_f() const { return (TextIndex() >> 9)&0xf; } // compare with sk1bf9.w2() ?
		U16 w2_8_b() const { return (TextIndex() >> 5)&0xf; }
		U16 w2_3_7() const { return (TextIndex())&0x001f; }
	};
	// 
	struct Actuator {
		ObjectID w0; // Next object ID.
		U16 w2;
		U16 w4;
		U16 w6; // This U16 has different meaning depending on the action target type (Remote or Local).

		U8 ActuatorType() const { return ((U8)w2) & 0x7F; }
		U16 ActuatorData() const { return (w2 >> 7)&0x01ff; }
		void ActuatorData(U16 val) {
			val &= 0x01ff;
			w2 &= 0x007f;
			val <<= 7;
			w2 |= val;
		}
		U16 BitfieldCurrent() const { return (w2>>7)&0x0F ; }; // SPX: DM1 compatibility for act5 trigger with bitfield
		U16 BitfieldTarget() const { return (w2>>11)&0x0F; }; // SPX: DM1 compatibility for act5 trigger with bitfield
		U16 Direction() const { return (w6 >> 4)&3; } // M!6,4,03
		U8 Ycoord() const { return (w6 >>11)&0x1f; } // M!7,3,1F
		U8 Xcoord() const { return (w6 >> 6)&0x1f; } // M!6,6,1F
		U16 GraphicNumber() const { return (w4 >> 12)&0x000f; } // M!4,12,0F
		U16 IsDisabled() const { return (w4 >> 11)&1; } // SPX: DM1 compatibility "is inactive""
		void Disable() { w4 |= 0x0800;	}	// SPX: DM1 compatibility
		void DisableToNoneType() { w2 &= 0xFF80; } // SPX: DM1 compatibility
		U16 ActuatorToggler() const { return (w4 >> 11)&1; } // SPX: DM1 compatibility "is active" / "act/gfx toggler"
		U16 Delay() const { return (w4 >> 7)&15; } // M!4,7,0F				// bits 7 to 10
		U8 SoundEffect() const { return (w4 >> 6)&1; } // M!4,6,01			// bit 6
		U8 RevertEffect() const { return (w4 >> 5)&1; } // M!4,5,01			// bit 5, belonging to effect
		U8 ActionType() const { return (w4 >> 3)&3; } // M!4,3,03			// bits 3 to 5
		U8 OnceOnlyActuator() const { 
			return (w4 >> 2)&1;					// bit 2 
		} // M!4,2,01
		void OnceOnlyActuator(U16 val) {
			val &= 1;
			w4 &= 0xfffb;
			w4 |= (val << 2);
		}
		U16 ActiveStatus() const { return (w4)&1; } // M!4,0,0001	// w4_0_0()	// bit 0 (unused in DM1)
		void ActiveStatus(U16 val) {
			val &= 1;
			w4 &= 0xfffe;
			w4 |= val;
		}
		U8 b4_0_0() const { return (U8)((w4)&1); } // M!4,0,01
		void b4_0_0(U8 val) {
			val &= 1;
			w4 &= 0xfffe;
			w4 |= val;
		}
		U16 ShopItemPoolNo() const { return Delay(); }
		U16 w6_4_f() const { return (w6 >> 4)&0xfff; }

		// SPX for handling modification over actuator, mostly used for dungeon conversion / creation
		void SetTarget(U16 tm, U16 tx, U16 ty) {
			tx = tx & 0x1F;
			ty = ty & 0x1F;
			w6 = w6 & 0x001F; // leave only 5 last bits
			w6 = w6 + (ty<<11) + (tx<<6);
		}
		void GraphicNumber(U16 val) {
			w4 = (w4 & 0x0FFF) + ((val+1 & 0x0F)<<12);
		}
		void SoundEffect(U16 val) {
			w4 = (w4 & 0xFFDF) + ((val & 0x01)<<6);
		}
		void ActuatorType(U8 type) {
			w2 = (w2 & 0xFF80) + type;
		}
		void ActionType(U8 action) {
			w4 = (w4 & 0xFFF7) + (action<<3);	// U8 ActionType() const { return (w4 >> 3)&3; } // M!4,3,03			// bits 3 to 5
		}
		void Delay(U16 delay) {
			w4 = (w4 & 0xF87F) + ((delay & 0x0F)<<7);	// bits 7 to 10
		}
	};
	// 

	struct Creature {
		ObjectID w0; // Next object ID.
		ObjectID possession; // w2 Next possession object ID. Although not recommended, it is possible to
		U8 b4; // Creature type. Her
		U8 iID; // Ingame-Internal Creature Index (was in DM1 Position of each c) // where &sk4ebe[b5]
		U16 hp1; // w6 Hit points of creature 1
		U16 iAnimSeq; // w8 Hit points of creature 2 // sk1c9a02c3::w0		--> DM2 anim sequence
		U16 iAnimFrame; // w10 Hit points of creature 3 // sk1c9a02c3::w2		--> DM2 anim frame (within sequence)
		U16 w12; // w12 Hit points of creature 4
		U8 b14; // 
		U8 b15;

		ObjectID GetPossessionObject() { return possession; }
		void SetPossessionObject(ObjectID val) { possession = val; }

		U8 CreatureType() const { return b4; }
		void CreatureType(U8 val) { b4 = val; }

		void HP1(U16 val) { hp1 = val; }
		U16 HP1() const{ return hp1; }

		void HP3(U16 val) { iAnimFrame = val; }

		U16 TriggerX() const { // M!C,0,1F
			return w12 & 0x001f;
		}
		U16 TriggerY() const { // M!C,5,1F
			return (w12 >> 5) & 0x001f;
		}
		U16 TriggerMap() const { // M!D,2,3F
			return (w12 >> 10) & 0x003f;
		}

		//SPX: HP4_0_4 renamed SetTriggerXPos
		void SetTriggerXPos(U16 val) { // ondie trigger x-pos
			val &= 0x001f;
			w12 &= 0xffe0;
			w12 |= val;
		}
		//SPX: HP4_5_9 renamed SetTriggerYPos
		void SetTriggerYPos(U16 val) { // ondie trigger y-pos
			val &= 0x001f;
			w12 &= 0xfc1f; // 1111 1100 0001 1111
			w12 |= val << 5;
		}
		//SPX: HP4_A_F renamed SetTriggerMap
		void SetTriggerMap(U16 val) { // ondie trigger map#
			val &= 0x003f;
			w12 &= 0x03ff;
			w12 |= val << 10;
		}
		U8 b14_7_7() const { return (b14 >> 7)&1; }
		U8 b15_2_2() const { return (b15 >> 2)&1; }
		void b15_2_2(U8 val) {
			val &= 1;
			b15 &= 0xfb;
			b15 |= val << 2;
		};
		U8 InternalID() const { return iID; } // (b5_0_7) xx of _4976_4ebe[xx]. -1 if not yet alloced.
		void SetInternalID(U8 val) { iID = val; } // b5_0_7
		U8 b15_0_1() const { return (b15 & 3); } // creature dir?
		void b15_0_1(U16 val) {
			val &= 3;
			b15 &= 0xfc;
			b15 |= (U8)val;
		}
		U8 b7_3_7() const { return U8(hp1 >> 11)&31; }
		U16 w6_6_a() const { return (hp1 >> 6)&31; }
		U16 w10_6_6() const { return (iAnimFrame >> 6)&1; }
		void w10_f_f(U16 val) {
			val &= 1;
			iAnimFrame &= 0x7fff;
			iAnimFrame |= val << 15;
		}
		void w10_d_d(U16 val) {
			val &= 1;
			iAnimFrame &= 0xdfff;
			iAnimFrame |= val << 13;
		}
		U16 w10_c_c() const { return (iAnimFrame>>12)&1; }
		U16 w10_3_3() const { return (iAnimFrame>>3)&1; }
		U16 w10_d_d() const { return (iAnimFrame>>13)&1; }
		U16 w10_7_7() const { return (iAnimFrame>>7)&1; }
		void w10_7_7(U16 val) {
			val &= 1;
			iAnimFrame &= 0xff7f;
			iAnimFrame |= val << 7;
		}
	};

	// 
#ifndef DM2_EXTENDED_MODE
	struct Weapon {
		ObjectID w0; // Next object ID.
		U16 w2; // Item description

		U8 Charges() const { return (U8)((w2 >> 10) & 0x0F); }
		void Charges(U16 newval) { w2 = (w2 & 0xc3ff) | ((newval & 0x0F) << 10); }
		U8 ItemType() const { return (U8)(w2 & 0x007F); }
		void ItemType(U16 val) {
			w2 = (U8)((w2 & 0xff80) | (val & 0x007f));
		}
		U8 Important() const { return (U8)((w2 >> 7) & 0x01); }
	};
#else
	struct Weapon {
		ObjectID w0; // Next object ID.
		U16 w2; // Item description
		// SPX (info taken from CSBWin)
		//	bits 0-6 : type	=> 128
		//	bit 7 : undiscardable
		//	bit 8 : cursed
		//	bit 9 : poisoned
		//	bits 10-13 : charges
		//	bit 14 : broken
		//	bit 15 : torch type

		U8 Charges() const { return (U8)((w2 >> 10) & 0x0F); }
		void Charges(U16 newval) { w2 = (w2 & 0xc3ff) | ((newval & 0x0F) << 10); }
		U8 ItemType() const { return (U8)(w2 & 0x007F); }
		void ItemType(U16 val) {
			w2 = (U8)((w2 & 0xff80) | (val & 0x007f));
		}
		U8 Important() const { return (U8)((w2 >> 7) & 0x01); }

		U8 Cursed() const { return (U8)((w2 >> 8) & 0x01); }
		void Cursed(U16 val) {
			w2 = (U8)((w2 & 0x0100) | (val & 0x01)<<8); }
		U8 Poisoned() const { return (U8)((w2 >> 9) & 0x01); }
		void Poisoned(U16 val) {
			w2 = (U8)((w2 & 0x0100) | (val & 0x01)<<9); }
		U8 Broken() const { return (U8)((w2 >> 14) & 0x01); }
		void Broken(U16 val) {
			w2 = (U8)((w2 & 0x0100) | (val & 0x01)<<14); }
	};
#endif // DM2_EXTENDED_MODE
	// 
	struct Cloth {
		ObjectID w0; // Next object ID.
		U16 w2; // Item attributes

		U8 Charges() const { return (U8)((w2 >> 9) & 0x0F); }
		void Charges(U16 newval) { w2 = (w2 & 0xe1ff) | ((newval & 0x0F) << 9); }
		U8 ItemType() const { return (U8)(w2 & 0x007F); }
		void ItemType(U16 val) {
			w2 = (U8)((w2 & 0xff80) | (val & 0x007f));
		}
		U8 Important() const { return (U8)((w2 >> 7) & 0x01); }
	};
	// 
	struct Scroll {
		ObjectID w0; // Next object ID.
		U16 w2; // Scroll attributes

		U16 w2_a_f() const { return (w2 >> 10)&0x3f; }
		U16 ReferredText() const { return w2 & 0x3ff; }

		U8 ItemType() const { return (U8)(w2 & 0x007F); }	// SPX: added ItemType like for other item categories
	};
	// 
	struct Potion {
		ObjectID w0; // Next object ID.
		U16 w2; // Item attributes.

		U8 PotionType() const { return (U8)((w2 >> 8) & 0x007F); }
		void PotionType(U16 val) {
			val &= 0x007f;
			w2 &= 0x80ff;
			w2 |= (val) << 8;
		}
		U8 PotionPower() const { return (U8)(w2 & 0x00ff); }
		void PotionPower(U16 val) {
			val &= 0x00ff;
			w2 &= 0xff00;
			w2 |= val;
		}
		U8 VisiblePower() const { return (U8)((w2 >> 15) & 0x0001); }
		void VisiblePower(U8 val) {
			val &= 1;
			w2 &= 0x7fff;
			w2 |= val << 15;
		}
	};
	// 
	struct Container {
		ObjectID w0; // Next object ID.
		ObjectID w2; // Next contained object ID
		U8 b4;
		U8 b5;
		U16 w6; // Unused

		ObjectID GetContainedObject() { return w2; }
		void SetContainedObject(ObjectID val) { w2 = val; }

		U16 IsOpened() const { return (b4 & 1); }
		void IsOpened(U16 val) {
			val &= 1;
			b4 &= 0xfe;
			b4 |= U8(val);
		}
		U8 ContainerType() const { return (b4 >> 1)&3; }
		void ContainerType(U8 val) {
			val &= 3;
			b4 &= 0xf9;
			b4 |= val << 1;
		}
		void ContainerFullType(U16 val) {
			b4 = (U8)((b4 & 0xf9) | (((val >> 3) & 0x0003) << 1));
			b5 = (U8)((b5 & 0x1f) |  ((val       & 0x0007) << 5));
		}
		U8 b5_5_7() const { return (b5 >> 5)&7; }
		void w6_0_f(U16 val) { w6 = val; }

		//U8 b7_2_7() const { return U8((w6 >> 10)&0x3f); } // =GetDestMap

		U16 GetDestX() const { return ((w6     )&0x1f); }
		U16 GetDestY() const { return ((w6 >> 5)&0x1f); }
		U16 GetDestMap() const { return ((w6 >>10)&0x3f); }

		DM2Internal::Ax3 GetDest() const {
			return DM2Internal::Ax3::Frm(w6);
		}

		void SetDestX(U16 val) {
			val &= 0x001f;
			w6 &= 0xffe0;
			w6 |= val;
		}
		void SetDestY(U16 val) {
			val &= 0x001f;
			w6 &= 0xfc1f;
			w6 |= val << 5;
		}
		void SetDestMap(U16 val) {
			val &= 0x003f;
			w6 &= 0x03ff;
			w6 |= val << 10;
		}
		void b7_2_2(U16 val) {
			val &= 0x0001;
			w6 &= 0xfbff;
			w6 |= val << 10;
		}

		U16 b7_2_2() const { return (w6 >> 10)&1; }
	};
	// 
	struct Miscellaneous_item {
		ObjectID w0; // ext object ID.
		U16 w2; // Item attributes

		U8 Who() const { return (U8)((w2 >> 14) & 0x03); }
		U8 Compass() const { return (U8)((w2 >> 14) & 0x03); }
		void Compass(U16 newval) { w2 = (w2 & 0x3fff) | ((newval & 0x03) << 14); }
		void Bone(U16 val) {
			val &= 3;
			w2 &= 0x3fff;
			w2 |= val << 14;
		}
		U8 ItemType() const { return (U8)(w2 & 0x007F); } // M!2,0,7F
		void ItemType(U8 val) { // M!2,0,7F
			val &= 0x7f;
			w2 &= 0xff80;
			w2 |= val;
		}
		U8 Charge() const { return (U8)((w2 >> 8)&0x3f); } // M!3,0,3F
		void Charge(U8 val) { // M!3,0,3F
			val &= 0x3f;
			w2 &= 0xc0ff;
			w2 |= val << 8;
		}
		U8 Important() const { return (U8)((w2 >> 7) & 0x01); }
		void Important(U8 val) {
			val &= 1;
			w2 &= 0xff7f;
			w2 |= val << 7;
		}
	};
	// 
	struct Missile {
		ObjectID w0; // : Next object ID.
		ObjectID w2; // Missile object (dagger, firebal, ...)
		U8 b4_; //  Energy remaining
		U8 b5_; // Energy remaining 2
		U16 w6; //  Timer index

		ObjectID GetMissileObject() const { return w2; }
		void SetMissileObject(ObjectID val) { w2 = val; }

		U8 EnergyRemaining() const { return b4_; }
		U8 b5_4_7() const { return b5_ >> 4; }
		U8 EnergyRemaining2() const { return b5_; }

		void EnergyRemaining(U8 val) { b4_ = val; }
		void EnergyRemaining2(U8 val) { b5_ = val; }

		U16 TimerIndex() const { return w6; }
		void TimerIndex(U16 val) { w6 = val; }

		U16 GetX() const { return (w4())&0x1f; }
		U16 GetY() const { return (w4()>>5)&0x1f; }
		U16 GetMap() const { return (w4()>>10)&0x3f; }

		U16 w4() const { return b4_ | (b5_ << 8); }
		void w4(U16 val) {
			b4_ = U8(val);
			b5_ = U8(val >> 8);
		}

		void SetX(U16 val) {
			val &= 0x001f;
			U16 w = w4();
			w &= 0xffe0;
			w |= val;
			w4(w);
		}
		void SetY(U16 val) {
			val &= 0x001f;
			U16 w = w4();
			w &= 0xfc1f;
			w |= val << 5;
			w4(w);
		}
		void SetMap(U16 val) {
			val &= 0x003f;
			U16 w = w4();
			w &= 0x03ff;
			w |= val << 10;
			w4(w);
		}
		void b6_0_0(U16 val) {
			val &= 0x0001;
			w6 &= 0xfffe;
			w6 |= val;
		}
	};
	// 
	struct Cloud {
		ObjectID w0; // Next object ID.
		U16 w2;

		U8 CloudType() const { return (U8)(w2 & 0x007F); }
		void CloudType(U8 val) {
			val &= 0x7f;
			w2 &= 0xff80;
			w2 |= val;
		}
		U8 b2_7_7() const { return (U8)((w2 >> 7)&1); }
		void b2_7_7(U8 val) {
			val &= 1;
			w2 &= 0xff7f;
			w2 |= val << 7;
		}
		U8 b3_0_f() const { return (U8)((w2 >> 8)&0xff); }
		void b3_0_f(U8 val) {
			val &= 0xff;
			w2 &= 0x00ff;
			w2 |= val << 8;
		}
		U8 b3_5_f() const { return (U8)((w2 >> 5)&0xff); }
	};
	// 
	struct GenericRecord {
		ObjectID w0; // Next object ID.

		inline Door *castToDoor() { return reinterpret_cast<Door *>(this); }
		inline Teleporter *castToTeleporter() { return reinterpret_cast<Teleporter *>(this); }
		inline Text *castToText() { return reinterpret_cast<Text *>(this); }
		inline SimpleActuator *castToSimpleActuator() { return reinterpret_cast<SimpleActuator *>(this); } // SPX
		inline Actuator *castToActuator() { return reinterpret_cast<Actuator *>(this); }
		inline Creature *castToCreature() { return reinterpret_cast<Creature *>(this); }
		inline Weapon *castToWeapon() { return reinterpret_cast<Weapon *>(this); }
		inline Cloth *castToCloth() { return reinterpret_cast<Cloth *>(this); }
		inline Scroll *castToScroll() { return reinterpret_cast<Scroll *>(this); }
		inline Potion *castToPotion() { return reinterpret_cast<Potion *>(this); }
		inline Container *castToContainer() { return reinterpret_cast<Container *>(this); }
		inline Miscellaneous_item *castToMisc() { return reinterpret_cast<Miscellaneous_item *>(this); }
		inline Missile *castToMissile() { return reinterpret_cast<Missile *>(this); }
		inline Cloud *castToCloud() { return reinterpret_cast<Cloud *>(this); }
	};
	// 
	struct GenericContainerRecord : GenericRecord {
		ObjectID possession;	//w2

		ObjectID GetSubObject() { return possession; }
		void SetSubObject(ObjectID val) { possession = val; }
	};

	// 
	enum dbIndex {
		dbNone = -1,			// SPX special
		dbDoor = 0,				// 0
		dbTeleporter,			// 1
		dbText,					// 2
		dbActuator,				// 3
		dbCreature,				// 4
		dbWeapon,				// 5
		dbCloth,				// 6
		dbScroll,				// 7
		dbPotion,				// 8
		dbContainer,			// 9
		dbMiscellaneous_item,	//10
		db11,
		db12,
		db13,
		dbMissile,				//14
		dbCloud,				//15
			dbMax = 16,
	};
	// 
	enum tileTypeIndex {
		ttWall = 0,		// 0
		ttFloor,		// 1
		ttPit,			// 2
		ttStairs,		// 3
		ttDoor,			// 4
		ttTeleporter,	// 5
		ttTrickWall,	// 6
		ttMapExit,		// 7
	};
	// 
	// SPX: moved to defines.h in order to keep a single code along skwindos which shares defines.h
	/*
	enum dtIndex {
		dtImage = 1,
		dtSnd = 2,
		dt04 = 4,
		dtText = 5,
		dt06 = 6,
		dt07 = 7,
		dt08 = 8, // image chain table
		dt09 = 9,
		dtWordValue = 11,
		dtImageOffset = 12,
		dt0d = 13,
	};*/
	// 
	enum picfmt {
		pfC4 =      0x01, // IMG3
		pfC8 = 0x80|0x01, // IMG9
		pfU4 =      0x00, // IMG6+SuffixPalette. Uncompressed4bpp
		pfU8 = 0x80|0x00, // ...?                Uncompressed8bpp
	};

};

namespace DM2Internal {
	// 
	struct TELE_inf { // 5 bytes
		U8 b0; // @0 // cross_scene_test;
		U8 b1; // @1 // cross_scene_test_of_teleporter_target;
		U8 b2; // @2 // target_x_pos;
		U8 b3; // @3 // target_y_pos;
		U8 b4; // @4 // target_z;     // MAP number

		U8 cross_scene_test() const { return b0; }
		U8 cross_scene_test_of_teleporter_target() const { return b1; }
		U8 target_x_pos() const { return b2; }
		U8 target_y_pos() const { return b3; }
		U8 target_z() const { return b4; }
	};
	// 
	struct RawEntry {
		U8 cls2;
		U8 cls4;
		U16 data;
	};
	// 
	struct GraphicFileEntry {
		U8 cls1, cls2, cls3, cls4, cls5, cls6;
		U16 data;
	};
	// 
/* -- replace this by sksave_header_asc
	struct sksave_header { // 42 bytes
		U16 bp4c;	// bp4c, usually 1
		U8  sSavegameName[34];	// bp4a
		U16 bp26;	// bp26
		U16 bp24;	// bp24
		U16 bp22;	// bp22
	};
*/

	// 
	struct sksave_header_asc { // 42 bytes
		U16 wTimerFlag;		// w0
		U8  sSavegameName[34];	// b2
		U16 w36;		// w36
		U16 w38;		// w38
		U16 w40;		// w40
	};
	// structure of data used in savegame.dat
	struct skload_table_60 { // 56 bytes
		U32 dwGameTick;
		U32 dwRandomSeed;
		U16 wChampionsCount;	// player cnt
		U16 wPlayerPosX;	// column
		U16 wPlayerPosY;	// row
		U16 wPlayerDir;	// dir
		U16 wPlayerMap;	// map#
		U16 wChampionLeader;
		U16 wTimersCount;
		U32 dw22;
		U32 dw26;
		U16 w30;
		U16 wPlayerThrowCounter;
		U16 w34;
		U8 b36;
		U8 b37;
		U8 b38;
		U8 b39;
		U16 wRainFlagSomething;
		U8 bRainAmbientLightModifier;
		U8 bRainDirection;
		U8 bRainStrength;
		U8 bRainLevelForSky;
		U8 bRainLevelForGround;
		U8 bRainMultiplicator;
		U16 wRainStormController;
		U8 bRainRelated3;
		U8 bRainRelated2;
		U32 dwRainSpecialNextTick;
	};
	// 
	struct shelf_memory {
		U32 val;

		shelf_memory(): val(U32(-1)) { }

		static shelf_memory FromSizeOnUnloaded(U16 size) {
			shelf_memory ps;
			ps.val = size | 0x80000000;
			return ps;
		}
		static shelf_memory From_cram(U32 offset) {
			ATLASSERT(offset < 0x200000);
			shelf_memory ps;
			ps.val = offset;
			return ps;
		}
		static shelf_memory From_cems(U32 offset) {
			ATLASSERT(offset < MAX_CEMS);
			shelf_memory ps;
			ps.val = offset +0x200000;
			return ps;
		}
		static shelf_memory From_raw_ems(U32ptr val) {
			shelf_memory ps;
			ATLASSERT(0x200000 <= val && val < 0x200000 +MAX_CEMS);
			ps.val = (U32)val;
			return ps;
		}

		// SPX: added sizeof CEMS to allow dynamic allocation of CEMS (32 MB / 64 MB)
		static U32 SizeOf_cems() {
			return (U32) MAX_CEMS;
		}

		U16 SizeIfAbsent() const {
			ATLASSERT(AtEMS());
			return (U16)(val & 0xffff);
		}

		bool Absent() const { return (val & 0x80000000) != 0; }
		bool Present() const { return (val & 0x80000000) == 0; }

		bool AtRAM() const{ return (val < 0x00200000); }
		bool AtEMS() const{ return (val >= 0x00200000); }

		inline bool operator <(const shelf_memory &r) const { return val < r.val; }
		inline bool operator >(const shelf_memory &r) const { return val > r.val; }
		inline bool operator <=(const shelf_memory &r) const { return val <= r.val; }
		inline bool operator >=(const shelf_memory &r) const { return val >= r.val; }
		inline bool operator ==(const shelf_memory &r) const { return val == r.val; }
		inline bool operator !=(const shelf_memory &r) const { return val != r.val; }

		inline shelf_memory operator +(U32 val) { return shelf_memory(this->val +val); }
		inline shelf_memory operator +=(U32 val) { return this->val += val, *this; }
		inline shelf_memory operator -(U32 val) { return shelf_memory(this->val -val); }
		inline shelf_memory operator -=(U32 val) { return this->val -= val, *this; }

	protected:
		shelf_memory(U32 val): val(val) { }

	};
	// 
	struct SRECT { // sized-point rectangle
		i16 x, y, cx, cy;
	};
	// 
	// struct blitx1 { // blitx1 = skxxxf
	// 	U8 b0;  // @0
	// 	U8 b1;  // @1
	// 	U16 x1; // @2
	// 	U16 y1; // @4
	// 	U16 x2; // @6
	// 	U16 y2; // @8
	// 	U8 b10; // @10
	// 	U8 b11; // @11
	// };
	// 
	struct mement {		// 18 bytes
		i32 _dw0;		// @0 // length. negative if it directs from lower to upper. it contains this header size.
		X16 _w4;		// @4
		X16 _w6;		// @6
		X16 _w8;		// @8 // mement index for chain image?
		X16 _w10;		// @10 // raw data index? (for chain image?)
		X16 _w12;		// @12 // bpp ?
		X16 _w14;		// @14 // width ?
		X16 _w16;		// @16 // height

		i32 dw0() const { return _dw0; }
		void dw0(i32 val) { _dw0 = val; }

		X16 w4() const { return _w4; }
		X16 w6() const { return _w6; }
		X16 w8() const { return _w8; }
		X16 w10() const { return _w10; }
		void w4(X16 val) { _w4 = val; }
		void w6(X16 val) { _w6 = val; }
		void w8(X16 val) { _w8 = val; }
		void w10(X16 val) { _w10 = val; }
		void w12(X16 val) { _w12 = val; }
		void w14(X16 val) { _w14 = val; }
		void w16(X16 val) { _w16 = val; }

		void pv4(mement *pv) {
			U32ptr val = reinterpret_cast<U32ptr>(pv);
			_w4 = U16(val);
			_w6 = U16(val >> 16);
		}
		void pv8(mement *pv) {
			U32ptr val = reinterpret_cast<U32ptr>(pv);
			_w8 = U16(val);
			_w10 = U16(val >> 16);
		}
		mement *pv4() { return reinterpret_cast<mement *>(_w4 | (_w6 << 16)); }
		mement *pv8() { return reinterpret_cast<mement *>(_w8 | (_w10 << 16)); }
	};
	// 
	struct IMG3 {	// 10 bytes
		i16 cx;		// @0
		i16 cy;		// @2
		i16 w4;		// @4

		U16 Width() const { return cx & 0x3FF; }
		U16 Height() const { return cy & 0x3FF; }
		i16 OffsetX() const { return cx >> 10; }
		i16 OffsetY() const { return cy >> 10; }

#if DM2_EXTENDED_MODE
		DMEncyclopaedia::picfmt Getpf() const {
			if (OffsetY() == 31) {
				return DMEncyclopaedia::pfC8;
			}
			else if (OffsetY() == -32) {
				if (w4 == 8) {
					return DMEncyclopaedia::pfU8;
				}
				else if (w4 == 4) {
					return DMEncyclopaedia::pfU4;
				}
			}
			return DMEncyclopaedia::pfC4;
		}
		U16 GetBitsCount() const {
			switch (Getpf()) {
			case DMEncyclopaedia::pfC8:
			case DMEncyclopaedia::pfU8:
				return 8;
			default:
				return 4;
			}
		}
		i32 GetImageSize() const {
			switch (Getpf()) {
			case DMEncyclopaedia::pfC8:
			case DMEncyclopaedia::pfU8:
				// 8bpp
				return Width() * Height();
			default:
				// 4bpp
				return (((Width() +1) & 0xFFFE) >> 1) * Height();
			}
		}
#endif
	};
	// 
	struct testen {		// 10 bytes
		i16 w0;		// @0
		i16 w2;		// @2
	};
	// 
	struct sk5d0c {	// 4 bytes // image chain table
		i16 w0;	// @0
		i16 w2;	// @2
	};
	// SPX: sk5c20 Timer structure ..
	struct Timer {		// 10 bytes
		U32 dw00;	// @0
		U8 ttype;		// @4	b04
		U8 actor;	// @5	// b5 player index or creature type or other ...
		U16 value;		// @6	// w6 => position x & y	// SPX: don't it take object ID ? or spell power ? it can also be an object! (door) ..
		U16 w8;		// @8

		X16 GetMap() const { return X16(dw00 >> 24); }
		void SetMap(U16 val) {
			val &= 0x00ff;
			dw00 &= 0x00ffffff;
			dw00 |= val << 24;
		}
		U32 GetTick() const { return dw00 & 0x00ffffff; }
		void SetTick(U32 val) {
			val &= 0x00ffffff;
			dw00 &= 0xff000000;
			dw00 |= val;
		}
		void TimerType(U8 val) {
			ttype = val;
		}
		U8 TimerType() const {
			return ttype;
		}
		U8 Xcoord() const {
			return U8(value & 0xff);
		}
		U8 Ycoord() const {
			return U8(value >> 8);
		}
		void Xcoord(U16 val) {
			val &= 0x00ff;
			value &= 0xff00;
			value |= val;
		}
		void Ycoord(U8 val) {
			val &= 0x00ff;
			value &= 0x00ff;
			value |= val << 8;
		}

		void XcoordB(U8 val) { value &= 0xff00; value |= val; }	// (b6_0_f)
		U8 XcoordB() const { return U8(value); }	// (b6_0_f) SPX what difference with Xcoord ?
		void b6_0_4(U16 val) {
			val &= 0x001f;
			value &= 0xffe0;
			value |= val;
		}
		U8 b6_0_4() const { return (value&0x1f); }
		void w6_5_9(U16 val) {
			val &= 0x001f;
			value &= 0xfc1f;
			value |= (val << 5);
		}
		U16 w6_5_9() const { return (value >> 5)&0x1f; }
		U8 b7_0_4() const { return U8((value >> 8)&0x1f); }
		void YcoordB(U8 val) { value &= 0x00ff; value |= val << 8; }	// (b7_0_f)
		U8 YcoordB() const { return U8(value >> 8); }	// (b7_0_f) SPX what difference with Ycoord ?
		X16 w6_a_b() const { return (value >> 10)&3; }
		void w6_a_b(U16 val) {
			val &= 0x0003;
			value &= 0xf3ff;
			value |= (val << 10);
		}
		// spx: this was previously named "direction". I renamed it since it has a larger scope
		void Value2(U16 val) { // b8_0_f(U16 val) for tty04: dir / can also hold a map id
			val &= 0x00ff;
			w8 &= 0xff00;
			w8 |= val;
		}
		U8 Value2() const { return U8(w8); }	// b8_0_f()
		U8 Direction() const { return (ActionType() >> 2)&3; }	// b9_2_3
		void Direction(U8 val) {	// b9_2_3
			val &= 3;
			w8 &= 0xf3ff;
			w8 |= (val << 10);
		}
		// w0_0_f is used to locate next unused timer entry. w0_0_f points next free entry.
		// 0xffff if no more avail.
		void w0_0_f(U16 val) { dw00 &= 0xffff0000; dw00 |= val; }
		U16 w0_0_f() const { return (U16)(dw00 & 0x0000ffff); }
		U8 ActionType() const { return (U8)(w8 >> 8); }	// b9_0_f()
		void ActionType(U8 val) { // b9_0_f(U8 val) for tty04: actionType
			w8 &= 0x00ff;
			w8 |= (val << 8);
		}
		U8 b9_4_7() const { return (w8>>12)&15; }
		DMEncyclopaedia::ObjectID *pv8_0_f() { return reinterpret_cast<DMEncyclopaedia::ObjectID *>(&w8); }
		DMEncyclopaedia::ObjectID id6() const { return value; }
		void id6(DMEncyclopaedia::ObjectID val) { value = val; }
		void id8(DMEncyclopaedia::ObjectID val) { w8 = val; }
		DMEncyclopaedia::ObjectID id8() const { return w8; }
		U8 b9_0_0() const { return (U8)((w8>>8)&1); }
		void b9_0_0(U8 val) {
			val &= 1;
			w8 &= 0xfeff;
			w8 |= val << 8;
		}

		void w8_0_4(U16 val) {
			val &= 0x1f;
			w8 &= 0xffe0;
			w8 |= val;
		}
		U16 w8_0_4() const { return (w8)&31; }
		void w8_5_9(U16 val) {
			val &= 0x1f;
			w8 &= 0xfc1f;
			w8 |= val << 5;
		}
		U16 w8_5_9() const { return (w8>>5)&31; }
		void w8_c_f(U16 val) {
			val &= 0x0f;
			w8 &= 0x0fff;
			w8 |= val << 12;
		}
		void w8_a_b(U16 val) { // could be the direction
			val &= 0x03;
			w8 &= 0xf3ff;
			w8 |= val << 10;
		}
	};
	// SPX sk53b4 replaced by Champion
	struct Champion {		// 261 bytes
		U8 firstName[8];		// @0 // b0[8] first name
		U8 lastName[16];		// @8 // b8[16] family name

		X8 b24_;			// @24
		X8 b25_;			// @25
		X8 b26_;			// @26
		X8 b27_;			// @27

		U8 absoluteDirection;			// @28 // b28_ absolute player dir (ignore squad dir). 0=North, 1=E, 2=S, 3=E
		U8 squadPosition;			// @29 // b29_ player position. 0=TL, 1=TR, 2=BR, 3=BL

		U8 playerDir() const { return absoluteDirection; }
		void playerDir(U8 val) {
			val &= 3;
			absoluteDirection = val;
		}

		U8 playerPos() const { return squadPosition; }
		void playerPos(U8 val) {
			val &= 3;
			squadPosition = val;
		}

		U16 w30() const { return ((U16)PoisonValue << 8) | runesCount; }	// SPX: What the ... poison & runes together ?!?

		U8 runesCount;			// @30 // b30

		U8 RuneCnt() const { return runesCount; }
		void RuneCnt(U8 val) { runesCount = val; }

		U8 PoisonValue; // b31;			// @31 // not 0 if poisoned
		U8 handCommand[2];		// @32 // b32[2] holds cm command

		U8 spelledRunes[4];		// @34	// b34_

		U8 *GetRunes() { return spelledRunes; }

		X8 b38_;			// @38
		X8 b39_;			// @39

		U8 direction;				// @40	// b40
		U8 herob41;				// @41	// b41
		U8 handCooldown[2];		// @42	// b42[2]
		U8 herob44;			// @44	// b44

		X8 b45_;			// @45

		U16 timerIndex;			// @46 // w46 timer index?
        X16 damageSuffered;			// @48 // w48 current damage?
		U16 heroFlag;			// @50 // w50 update mask: 0x8000=all?
									// 0x3000=wearing boosting attribute item
									// 0x2000=wearing boosting skill item
									// 0x1000=weight calculated ?
									// 0x0800=gained a level ? (temp flag?) temporary bonus max attrib????

		U16 bodyFlag;			// @52	// w52 SPX Flags of body part ??

		i16 curHP() const { return HPCurrent; }
		void curHP(i16 val) { HPCurrent = val; }
		i16 maxHP() const { return HPMax; }
		void maxHP(i16 val) { HPMax = val; }

		i16 curStamina() const { return staminaCurrent; }
		void curStamina(i16 val) { staminaCurrent = val; }
		i16 maxStamina() const { return staminaMax; }
		void maxStamina(i16 val) { staminaMax = val; }

		i16 curMP() const { return manaCurrent; }
		void curMP(i16 val) { manaCurrent = val; }
		i16 maxMP() const { return manaMax; }
		void maxMP(i16 val) { manaMax = val; }

		i16 HPCurrent;		// @54 // w54_ cur hp
		U16 HPMax;			// @56 // w56 max hp
		U16 staminaCurrent;			// @58 // w58 cur stamina x10
		U16 staminaMax;			// @60 // w60 max stamina x10
		U16 manaCurrent;			// @62 // w62 cur mana
		U16 manaMax;			// @64 // w64 max mana
		i8 handDefenseClass[2];			// @66	// b66[2] SPX: has something to do with armor change?
		U16 food;			// @68 // w68 food
		U16 water;			// @70 // w70 water

		i16 curFood() const { return food; }
		void curFood(U16 val) { food = val; }
		i16 curWater() const { return water; }
		void curWater(U16 val) { water = val; }

		U8 attributes[7][2];	//b72[7][2];	// @72

		//U8 w72;			// @72 // cur luck
		//U8 w73;			// @73 // max luck
		//U8 w74;			// @74 // cur str
		//U8 w75;			// @75 // max str
		//U8 w76;			// @76 // cur dex
		//U8 w77;			// @77 // max dex
		//U8 w78;			// @78 // cur wiz
		//U8 w79;			// @79 // max wiz
		//U8 w80;			// @80 // cur vit
		//U8 w81;			// @81 // max vit
		//U8 w82;			// @82 // cur anti-magic
		//U8 w83;			// @83 // max anti-magic
		//U8 w84;			// @84 // cur anti-fire
		//U8 w85;			// @85 // max anti-fire

		U8 curLuck() const { return attributes[0][0]; }   //72 48
		U8 maxLuck() const { return attributes[0][1]; }   //73 49
		U8 curStr() const { return attributes[1][0]; }   //74 4a
		U8 maxStr() const { return attributes[1][1]; }   //75 4b
		U8 curDex() const { return attributes[2][0]; }   //76 4c
		U8 maxDex() const { return attributes[2][1]; }   //77 4d
		U8 curWiz() const { return attributes[3][0]; }   //78 4e
		U8 maxWiz() const { return attributes[3][1]; }   //79 4f
		U8 curVit() const { return attributes[4][0]; }   //80 50
		U8 maxVit() const { return attributes[4][1]; }   //81 51
		U8 curAntiM() const { return attributes[5][0]; } //82 52
		U8 maxAntiM() const { return attributes[5][1]; } //83 53
		U8 curAntiF() const { return attributes[6][0]; } //84 54
		U8 maxAntiF() const { return attributes[6][1]; } //85 55
		void curStr(U8 val) { attributes[1][0] = val; }
		void maxStr(U8 val) { attributes[1][1] = val; }
		void curDex(U8 val) { attributes[2][0] = val; }
		void maxDex(U8 val) { attributes[2][1] = val; }
		void curWiz(U8 val) { attributes[3][0] = val; }
		void maxWiz(U8 val) { attributes[3][1] = val; }
		void curVit(U8 val) { attributes[4][0] = val; }
		void maxVit(U8 val) { attributes[4][1] = val; }
		void curAntiM(U8 val) { attributes[5][0] = val; }
		void maxAntiM(U8 val) { attributes[5][1] = val; }
		void curAntiF(U8 val) { attributes[6][0] = val; }
		void maxAntiF(U8 val) { attributes[6][1] = val; }

		i8 attributesEnhanced[7];				// @86 // enhanced value of attr (from str to antif)

		i32 skills[20];		// @93 // dw93[20]
		// 0 to 3 ... fighter, ninja, wiz, priest
		// 4 to 19 ... fighter x4, ninja x4, wiz x4, priest x4

		i8 skillBonus[20];		// @173 //  b173[20]

		//               +--+--+--+--+--+--+--+--+
		//       +--+    |14|15|16|17|18|19|20|21|
		//  +--+ | 2| +--+--+--+--+--+--+--+--+--+
		//  |10| +--+ |13|22|23|24|25|26|27|28|29|
		//  +--+      +--+--+--+--+--+--+--+--+--+
		//       +--+
		//  +--+ | 3| +--+
		//  | 0| +--+ | 1|
		//  +--+      +--+
		//       +--+
		//  +--+ | 4| +--+--+
		//  |11| +--+ |12| 7|
		//  +--+      +--+--+
		//  | 6| +--+ | 8| 9|
		//  +--+ | 5| +--+--+
		//       +--+

		DMEncyclopaedia::ObjectID inventory[INVENTORY_MAX_SLOT];	// @193 // w193[30] player posessions

		DMEncyclopaedia::ObjectID Possess(U16 no) const { return DMEncyclopaedia::ObjectID(inventory[no]); }
		void Possess(U16 no, DMEncyclopaedia::ObjectID recordLink) { inventory[no] = recordLink; }

		U8 HeroType() const { return heroType; }
		void HeroType(U8 val) { heroType = val; }

		U16 curWeight() { return weight; }
		void curWeight(U16 val) { weight = val; }

		U16 weight;		// @253 // w253 weight x10
		U8 heroType;			// @255 // b255 hero no (in GDAT)
		U8 enchantmentAura;			// @256	// b256 protection enchantment number
		i16 enchantmentPower;			// @257	// w257

		i8 walkSpeed;			// @259	// b259	SPX: 0 by default, the higher the better
#if (XDMX_EXTENDED_FEATURES == 0)
		X8 b260_;			// @260	// b260_ SPX: looks unused ? ... missing code ?
#elif (XDMX_EXTENDED_FEATURES == 1)
		X8 PlagueValue;	// SPX: custom value
#endif
	};
	// 
	struct GlobalEffects {		// sk57e4 6 bytes
		i16 Light;				// @0 w0
		X8 Invisibility;		// b2 used by invisibility
		X8 FreezeCounter;		// b3 value used by freeze life command
		X8 AuraOfSpeed;		// b4 used by aura of speed
		X8 UnusedCounter;				// b5 SPX: In present code this one is never used ?? or is code missing ?
#if (XDM1_EXTENDED_SEETHRUWALLS == 1)	// SPX: Extended mode, add SeeThruWalls from DM1
		X8 SeeThruWalls;
#endif
	};
//	// 
//	struct sk3a150696 {
//		U32 bp16;		// @0
//		U8 bp12;			// @4
//		U8 bp11;			// @5
//		U16 bp10;		// @6
//		U8 bp0e;			// @7
//	};
	// 

	// 
	struct PreEntry {
		i32 dw0;
		U16 w4;
	};

	// SPX: structure for converting DM1 item to DM2 in activation list (item used by actuator)
	struct DM1ItemConv {
		U8 iItemDB;	// DM2 corresponding category
		U8 iItemID;	// ID within DB
	};

	// SPX: sk03a2 renamed AIDefinition
	struct AIDefinition { // 36 bytes
		U16 w0AIFlags;		// @0 // flags
		U8 ArmorClass;	// U8 b2;		// @2	// armor/defense
		i8 b3;			// @3
		U16 BaseHP;	// (U16 w4) @4 // initial HP	 
		U8 AttackStrength;			// b6 @6	// attack strength
		U8 PoisonDamage;			// b7 @7	// poison
		U8 Defense;			// b8 @8	// defense (255 = undestroyable)
		X8 b9x;			// b9 @9 // 0x40: pit ghost
		// SPX: some details
	/* 
	// Rightmost
	unsigned char bSplashAttack:1;	// Can attack several champions everywhere
	unsigned char flag6:1;			// 0, not used.
	unsigned char bFlying:1;		// Flying, OK
	unsigned char flag4:1;			// Minions, Ghost, Bats, Dragoth, Chest and Skeleton ?
	unsigned char flag3:1;			// ? Minions have it
	unsigned char flag2:1;			// ?
	unsigned char bInvisible:1;		// Only Pit Ghost has it
	unsigned char flag0:1;			// Not used, always 0
	// Leftmost
	*/
		X16 w10;		// @10
		X16 w12;		// @12
		X16 AttacksSpells;		// (X16 w14) @14	// Attack and Spells commands (checked at ^19F0:0BD4)
		X16 w16;		// @16
		X16 w18;		// @18		// Can switch triggers, I think
		U16 w20;		// @20
		U16 w22;		// @22
		U16 w24;		// @24		// Resistance & Fire resistance & Poison resistance & ??
		X16 w26;		// @26		

		U8 b28;			// @28
		U8 Weight;		// U8 b29 @29		// Weight = Push resistance (255 = can't be pushed/moved)
		U16 wc30;		// @30	if 0x0800 : then it can turn missiles
		U16 w32;		// @32

		i8 b34;			// @34
		i8 b35;		// @35	// Cloud size when dying ? 0/default = 0x6E / 1 => 0xBE / 2 => 0xFF

		U16 IsStaticObject() const { return (w0AIFlags)&1; } // w0_0_0() 1 = sk1c9a02c3 within each Creature record.
												// this one is set for general static objects (non creature)
												// i.e. 0 = living creature
//		U16 w0_1_1() const { return (w0AIFlags >> 1)&1; } // 1 = reflector?
		U16 w0_2_2() const { return (w0AIFlags >> 2)&1; }	// w0_2_2()	?? for static objects
		U16 w0_3_3() const { return (w0AIFlags >> 3)&1; }	// w0_3_3()	spectres and ghosts have it
		U16 w0_4_4() const { return (w0AIFlags >> 4)&1; }	// w0_4_4() spectres ghosts + vexirks have it
		U16 w0_5_5() const { return (w0AIFlags >> 5)&1; } // w0_5_5() 1 = non-material 
		U16 w0_6_7() const { return (w0AIFlags >> 6)&3; }	// w0_6_7() for all creatures & objects - worms and glops have both bits.

		U16 PushWhenMoving() const { return (w0AIFlags >> 8)&1; }	// w0_8_8() move and push back anything on target (spiked wall)
		U16 AbsorbsMissile() const { return (w0AIFlags >> 9)&1; }	// w0_9_9() most creatures have this
		U16 w0_a_a() const { return (w0AIFlags >> 10)&1; }	// w0_a_a() related to invisibility?? (ghosts + dragoth)
		U16 w0_b_b() const { return (w0AIFlags >> 11)&1; }	// w0_b_b() related to light ??? (worm, glop, treant, rocky .. bats + minions)
		U16 w0_c_c() const { return (w0AIFlags >> 12)&1; }	// w0_c_c() dragoth only !! (for end game ??)
		// 0_d_d for pit ghost + void door
		U16 w0_e_e() const { return (w0AIFlags >> 14)&1; } // w0_e_e() 1 = can stay on all maps => all minions have this, because they travel between maps
		U16 w0_f_f() const { return (w0AIFlags >> 15)&1; } // w0_f_f() 0 = block use of rope?! (all flying and non material ..)


		U16 w10_2_2() const { return (w10 >> 2)&1; }
		U16 w20_c_f() const { return w20 >> 12; } // view scope len
		U16 GetPoisonResistance() const { return (w24 >> 8)&15; } // w24_8_b
		U16 w22_8_b() const { return (w22 >> 8)&15; }	// xp gained by champion when attacked by creature
		U16 w24_c_c() const { return (w24 >> 12)&1; }	// head damage immune : resistance to door and non-fireball explosion??? only for mummy and flying chest ??
		U16 GetFireResistance() const { return (w24 >> 4)&15; }	// (w24_4_7) would be fire resistance?
		U16 w30_0_3() const { return (wc30)&15; }	// related to static objects ? can be invisible ?
		U16 w30_11_11() const { return (wc30 >> 11)&1; }
		U16 w22_4_7() const { return (w22>>4)&15; }
		U16 w22_c_f() const { return (w22>>12); }
		U16 w22_0_3() const { return (w22)&15; }
		
		U8 CanActivatePitTeleport() const { return (b9x>>6)&1; }	// (b9_6_6) 0x40 : would be pit ghost : operate pit and/or teleporter
		U8 b9_5_5() const { return (b9x>>5)&1; }	// 0x20	// only "void door" and spiked wall
		U8 bConsumeHP() const { return (b9x>>4)&1; }	// 0x10	// (b9_4_4) attacks, scouts and guard minions (not all types) -> depleting HP from casting spells

		U8 GetShootStrength() const { return (AttackStrength >> 2); }	// b6_2_7() : attack strength / 4, used for shooting strength
		U16 w16_4_7() const { return (w16 >> 4)&15; } // anti confuse factor. 15=confuse disabled.
	};
	// 
	enum AllocmemFlags {
		afDefault = 0,	// upper first
		afUseUpper = 1,	// upper first
		afUseLower = 2,	// lower first
		afZeroMem = 0x8000,
#define afDefault 0
	};
	// 
	struct CreatureInfoData { // sk4ebe 34 bytes
		i16 w0_;			// @0 // index of creature db
		U16 w2_;		// @2 // timerid
		X8 b4;			// @4
		X8 b5_;			// @5
		X8 b6_;			// @6	// spx: compared to 1/4 of gametick ????
		U8 b7;			// @7	// spx: can this be related to target champion ?? (high speculation)
		X8 b8_;			// @8 // const base index. _4976_5fde[w8 + w10] // sk1c9a02c3::w0
		X8 b9_;			// @9 //
		X8 b10_;		// @10 // variant index. // sk1c9a02c3::w2 // internal animation frame/tick ?
		X8 b11_;		// @11 // 
		Ax3 w12;		// @12
		i16 w14;		// @14 // anim frame
		i16 w16;		// @16
		i8 x;			// @18 // b18 _4976_3672[xx][]  , b18 is xx
		i8 y;			// @19 // b19 _4976_3672[xx][yy], b19 is yy
		U16 w20;		// @20 // recent damage for creature
		i8 b22;			// @22
		U8 Command2;	// @23 // U8 b23 // pre creature action pattern? ccmXX
		Ax3 w24;		// @24 // creature's target, a point to the target.
		i8 Command;		// @26 // i8 b26 // creature action pattern? ccmXX
		i8 b27;			// @27 // i8 b27 // direction ?
		U8 b28;			// @28
		i8 b29;			// @29	// next direction ??
		i8 ItemToThrow;			// @30	// b30	ItemToThrow or CreatureToSummon
		i8 b31;			// @31	// 1 or 2 while jumping ?
		i8 b32;			// @32
		U8 b33;			// @33	// result of proceed_ccm ?

		U8 b30_6_6() const { return (ItemToThrow>>6)&1; }

		U16 TimerIndex() const { return w2_; }
		void TimerIndex(U16 val) {
			w2_ = val;
		}

		i16 CreatureIndex() const { return w0_; }
		void CreatureIndex(i16 val) {
			w0_ = val;
		}
	};
	// 
	// SPX: sk5fda renamed CreatureCommandAnimation
	struct CreatureCommandAnimation { // 4 bytes
		//U16 w0, w2;
		U16 ccmReference;
		U16 animSeqOffset;
	};
	// 
	// SPX: sk5fde renamed CreatureAnimationFrame
	struct CreatureAnimationFrame { // 6 bytes
		U16 w0;	// @0
		U16 w2;	// @2
		U8 b4;		// @4
		X8 sound;		// @5 // b5 / cls4 of effect sound

		U16 w2_9_9() const { return (w2 >> 9)&1; }		// .... ...X .... ....	this would check end of sequence if set to 1
		U16 w2_8_8() const { return (w2 >> 8)&1; }		// .... .... X... ....
		U16 w2_a_f() const { return (w2 >>10)&0x3f; }	// XXXX XXX. .... ....
		U16 w2_0_3() const { return (w2)&0xf; }			// .... .... .... XXXX
		U16 w2_4_7() const { return (w2>>4)&0xf; }		// .... .... XXXX ....
		U8 b4_0_2() const { return b4&7; }				// .... .XXX
		U8 b4_3_4() const { return (b4>>3)&3; }				// ...X X... /// was { return b4&7; }, which seems wrong against func name, I changed it
	};

	// SPX: V5 Animation FC 00 00 data
	struct CreatureAnimationFrameInfoFC_V5 {
		U8 sound;
		U8 seqnext;
		U8 b2;
		U8 b3;
	};
	// SPX: V5 Animation FD 00 00 data
	struct CreatureAnimationFrameInfoFD_V5 {
		U8 img[4];	// 4 images ID : back, left, front, right
		U8 b4;
		U8 iScaleGroup;
		U8 b6;
		U8 b7;
	};
	// 
	struct sk04bf17ac { // 6 bytes
		U16 w0, w2, w4;
	};
//	// 
//	struct sk4bda { // size?
//		U8 b0;		// @0
//		U8 b1;		// @1
//		U8 b2;		// @2
//		U8 b4;		// @4
//		U8 b5;		// @5
//		U8 b8;		// @8
//		U8 b9;		// @9
//		U8 b11;		// @11
//		U8 b12;		// @12
//		U8 b13;		// @13
//		U8 b15;		// @15
//	};
	// 
	struct skxxx1 { // 8 bytes
		U8 *pb0;		// @0
		U16 w4;		// @4
		U16 w6;		// @6
	};
	// 
	struct skxxx2 { // 9 bytes
		SRECT rc;
		U8 b8;
	};
	// 
	struct RectTable { // 10 bytes (or more)
		RectTable *pb0;
		U16 w4;
		U16 w6;
		U8 b8;
		U8 b9;
	};
	//
	struct sk4b84 { // 8 bytes
		i16 w0;
		i16 w2;
		i16 w4;
		i16 w6;
	};
	// 
	struct sk00fe { // 8 bytes
		U16 w0;
		U16 w2;
		U16 w4;
		U16 w6;
	};
	// 
	struct sk3914 { // 4 bytes
		i16 w0;		// @0
		i16 w2;		// @2
	};
	//
	struct sk38f4 { // 4 bytes
		i16 w0;		// @0
		i16 w2;		// @2
	};
	// 
	struct skxxx3 { // 10 bytes
		U32 bp0a;	// @0 // tick for message invokation
		U8 bp06;		// @4 // ?
		U8 bp05;		// @5 // message type: 1=SET, 2=TOGGLE, 3=CLEAR
		U8 bp04;		// @6 // xpos
		U8 bp03;		// @7 // ypos
		U8 bp02;		// @8 // dir
		U8 bp01;		// @9 // actionType
	};
	// 
	struct sk5f06 { // 7 bytes
		U16 w0;		// @0
		U8 category;		// @2 // b2 cls1
		U8 index;		// @3 // b3 cls2
		U8 entry;		// @4 // b4 cls4
		U16 w5;		// @5
	};
	// SPX: sk5f0e
	struct SoundEntryInfo { // 12 bytes
		X16 w0;		// @0 // 1 if using
		X8 category;		// @2 // b2 cls1	= GDAT category
		X8 index;		// @3 // b3 cls2	= index ?
		X8 entry;		// @4 // b4 cls4	= entry = #sound
		i8 b5;		// @5 // map#
		X8 b6;		// @6 // xpos
		X8 b7;		// @7 // ypos
		X16 w8;		// @8
		X16 w10;	// @10
	};
	// 
	struct sk5f0a { // 16 bytes
		sk5f0a *pv0;	// @0
		U16 w4;		// @4	// size of sound		< 65536
		X16 w6;			// @6	// playback frequency	= 5500
		sk5f0a *pv8;	// @8

		shelf_memory ps0;
	};
	// SPX: sk5efe renamed SoundStructure
	struct SoundStructure { // 12 bytes
		sk5f0a *pv0;	// @0
		U8 b4;		// @4
		U8 b5;		// @5
		i8 b6;		// @6 // dist x
		i8 b7;		// @7 // dist y
		U8 b8;		// @8 // volume
		U8 b9;		// @9 // SPX: left volume ?
		U8 b10;		// @10 // SPX: right volume ?
		U8 b11;		// @11
	};
	// 
/* not used anywhere
	struct sk4ba8 { // 8 bytes

	};
*/


	// there seems to be some correspondance between SkImage and sk5cfc_image
	struct SkImage {	// size=14?
		i16 w0_;		// @0
		i16 w2_;		// @2
		U16 w4_;			// @4
		U16 w6_;			// @6

		i16 Xoffset() const { return w0_ >> 10; }
		i16 Yoffset() const { return w2_ >> 10; }
		U16 BitsPixel() const { return w4_; }
		U16 AllocLower() const { return w6_; }
	};
	// 
	struct sk5cfc_image { // 14 bytes / 18 bytes 64-bits
		sk5cfc_image *pv0;	// @0
		X16 w4;			// @4
		X16 w6;			// @6
		X16 w8;			// @8		// bmp structure : resolution + bits
		X16 width;		// @10		// w10 width
		X16 height;		// @12		// w12 height
	};
	// SPX new structure instead of SkImage to handle FREE_PICT_ENTRY as 32 or 64 bits
	struct SkImageDealloc {
		sk5cfc_image *pv0;	// @0
		U16 w4_;			// @4 / @8
		U16 w6_;			// @6 / @10
		U16 BitsPixel() const { return w4_; }
		U16 AllocLower() const { return w6_; }
	};
	// 
	struct sk5cfc_root {	// 4 bytes
		sk5cfc_image	*pv0;			// @0
	};
	// 

	struct sk5d00 { // 28 bytes / 40 bytes 64-bits
		sk5d00 *pv0;	// @0
		U32 dw4;		// @4	// avail size after this->dw28
		U32ptr dw8;		// @8	// (void *) or (shelf_memory)
		i32 dw12;		// @12	// copy of dw4?
		U16 w16;		// @16	// poolflag: 0x800=Is4EMS
		U16 w18;		// @18	// allocflag?: 1=if used
		U32ptr dw20;	// @20
		X32 dw24;		// @24

		bool Is4EMS() const { return (w16 & 0x800) != 0; }
	};

	// 
	struct sk3f6c { // ? bytes
		U16 w0;	// index. use with _4976_5d08[w0]
		SRECT rc2;
		//  w2 = w2.x
		//  w4 = w2.y
		//  w6 = w2.cx
		//  w8 = w2.cy
		U16 w10;
		SRECT w12[5];
	};
	// 
	// SPX: skxxx5 renamed Picture
	struct Picture { // 24 bytes
		U8 *pb0;		// @0 // pic bits
		U16 w4;		// @4 // 4=Use (b8,b9,b11), 8=Use w12 for cacheIndex
		U16 w6;		// @6 // raw data index
		U8 b8;		// @8 // cls1
		U8 b9;		// @9 // cls2
		U8 b10;		// @10 // cls3
		U8 b11;		// @11 // cls4
		i16 w12;	// @12
		U16 w14;		// @14
		U16 w16;		// @16
		i16 width;	// @18 // (i16 w18) width
		i16 height;	// @20 // (i16 w20) height
		i16 w22;	// @20 // bpp
	};
	// 
	// SPX: skxxx4 renamed ExtendedPicture
	struct ExtendedPicture : Picture { // 314 bytes
	//	U8 *pb0;		// @0
	//	U16 w4;		// @4
	//	U16 w6;		// @6
	//	U8 b8;		// @8 // cls1
	//	U8 b9;		// @9 // cls2
	//	U8 b10;		// @10 // cls3
	//	U8 b11;		// @11 // cls4
	//	i16 w12;	// @12
	//	U16 w14;		// @14
	//	U16 w16;		// @16
	//	i16 w18;	// @18
	//	i16 w20;	// @20
		U16 rectNo;		// @24 // (U16 w24) rectno
		U16 w26;		// @26
		i16 w28;	// @28 // x-offset
		i16 w30;	// @30 // y-offset
		i16 w32;	// @32 // x?
		i16 w34;	// @34 // y?
		SRECT rc36;		// @36
		U8 *pb44;	// @44 // picture buffer
		i16 colorKeyPassThrough;		// @48 // (i16 w48) color key
		U16 mirrorFlip;		// @50 // (w50) mirror flip
		U16 w52;		// @52 // horz stretch factor? (unstretch is 64)
		U16 w54;		// @54 // vert stretch factor? (unstretch is 64)
		U16 w56;		// @56 // palentcnt
		U8 b58[256];	// @58 // local palette 16 colors
	};
	// SPX: about colorkey2 (w48), there can be these values : -1, -2, -3! what meaning?

	// Digging deeper into the data files:
	// http://www.dungeon-master.com/forum/viewtopic.php?t=24695

	// The description of CommandNumber and CommandMeaning are supplied by Suule!

	// 
	enum CommandNumber {
		CnSK, // 0
		CnLV, // 1
		CnCM, // 2
		CnBZ, // 3
		CnTR, // 4
		CnST, // 5
		CnPA, // 6
		CnTA, // 7
		CnNC, // 8
		CnEX, // 9
		CnPB, // 10
		CnDM, // 11
		CnMS, // 12
		CnSD, // 13
 		CnRP, // 14
		CnHN, // 15
		CnAT, // 16
		CnWH, // 17
	};
	//
	// SPX: I put the DM1 command list in correspondance. All DM1 "physical" attacks have been replaced by generic command (4 and 8)
	// Therefore, many just do not match anymore old commands
	// New commands would rather use ID than old DM1 ones.
	enum CommandMeaning {
								// 0 = N (none)
		CmBlock=1,				// 1 = BLOCK
		CmInvisibility=2,		// 2 = CHOP
		CmLaunchMissile=3,		// 3 = X (unused)
		CmPhysicalDamage4=4,	// 4 = BLOW HORN
		CmConfuse=5,			// 5 = FLIP
		CmDarkness=6,			// 6 = PUNCH
		CmSpellReflection=7,	// 7 = KICK
		CmPhysicalDamage8=8,	// 8 = WAR CRY
		CmAuraOfSpeed=9,		// 9 = STAB
		CmUseRope=10,			// 10 = CLIMB DOWN
		CmFreezeLife=11,		// 11 = FREEZE LIFE
		CmAuraOfDex=12,			// 12 = HIT
		CmAuraOfWiz=13,			// 13 = SWING
		CmAuraOfVit=14,			// 14 = STAB
		CmAuraOfStr=15,			// 15 = THRUST
		CmConsume=16,			// 16 = JAB
		CmPouch=17,				// 17 = PARRY
								// 18 = HACK
								// 19 = BERZERK
								// 20 = FIREBALL
								// 21 = DISPELL
								// 22 = CONFUSE
								// 23 = LIGHTNING
								// 24 = DISRUPT
								// 25 = MELEE
								// 26 = X
		CmInvoke=27,			// 27 = INVOKE
								// 28 = SLASH
								// 29 = CLEAVE
								// 30 = BASH
								// 31 = STUN
		CmLaunchProjectile=32,	// 32 = SHOOT
		CmSpellshield=33,		// 33 = SPELLSHIELD
		CmFireshield=34,		// 34 = FIRESHIELD
		CmShield=35,			// 35 = FLUXCAGE
		CmHealing=36,			// 36 = HEAL
								// 37 = CALM
		CmLight=38,				// 38 = LIGHT
		CmLongLight=39,			// 39 = WINDOW
								// 40 = SPIT
								// 41 = BRANDISH
		CmThrow=42,				// 42 = THROW
		CmFuse=43,				// 43 = FUSE
		CmMark=44,
		CmCallCarry=45,
		CmCallFetch=46,
		CmCallScout=47,
		CmKillMinion=48,
		CmAttackMinion=49,
		CmGuardMinion=50,
		CmUHaulMinion=51,
		CmTeleport=54,
	};

	// 
	struct sk16ed { // 7 bytes
		U16 w0;		// @0
		U16 w2;		// @2
		U16 w4;		// @4
		U8 b6;		// @6
	};
	// 
	struct MouseState { // 6 bytes
		U16 Button;		// w0_ // @0
		U16 XPos;		// w2_ // @2
		U16 YPos;		// w4_ // @4

		void MouseButton(U16 val) { Button = val; }
		void MouseX(U16 val) { XPos = val; }
		void MouseY(U16 val) { YPos = val; }
		U16 MouseButton() const { return Button; }
		U16 MouseX() const { return XPos; }
		U16 MouseY() const { return YPos; }
	};
	// 
	struct sk1891 { // 4 bytes
		U8 b0;		// @0
		U8 b1;		// @1	// game ended ? or used to compare with right panel
		U16 w2;		// @2

		U8 b0_0_6() const { return b0 & 0x7f; }
		U8 b0_7_7() const { return (b0 >> 7)&1; }
	};
	// 
	struct sk5358 { // 4 bytes
		U8 category;		// @0 // b0 cls1
		U8 index;		// @1 // b1 cls2
		U8 entry;		// @2 // b2 cls4
		U8 b3;		// @3
	};
	// SPX: skxxx6 renamed ExtendedTileInfo
	struct ExtendedTileInfo { // 16 bytes
		U16 w0;							// @0 // (w0) tiletype with extended value (i.e. door tile is 17, not 4!)
		U16 w2;							// @2 // (w2) tileval?
		U16 w4;							// @4 // (w4) tile record
		U16 interwalls[4];			// manage wall faces independently
		U16 tfoi[4];						// @6 // (w6) ornate index in GDAT // bit0-9: ornate index, bit10-15: frame index
											// (w6[0] serving as opened door status : open = 0 .... 4 = closed
											// SPX: renamed tfoi (tile face ornate index)
											// in case of door, tfoi[1] contains RL of door
		U16 xvalue;					// @14 // (w14) actuator value , portrait id, record link ..
// SPX EXTENDED		


		DMEncyclopaedia::ObjectID id4() const { return w4; }
	};
	// SPX: sk1c9a02c3 actually holds the same type of data contained within a creature records data, starting at byte 8.
	// Depending on creatures type, these data will come from direct creature data or some other table.
	struct sk1c9a02c3 { // 4 bytes
		U16 iAnimSeq;			// w0 @0	// Creature::w8		-- current animation sequence
		U16 iAnimInfo;			// w2 @2	// Creature::w10	-- current frame number in animation

		U16 w2_e_e() const { return (iAnimInfo>>14)&1; }
		U16 w2_d_d() const { return (iAnimInfo>>13)&1; }
		U16 w2_0_5() const { return (iAnimInfo)&0x3f; }	// get number of possible frames
	};
	// 
	struct sk0cea;
	struct skxxx7 { // 12 bytes
		sk0cea *pv0;	// @0
		SRECT rc4;		// @4
	};
//	// 
//	struct skxxx8 { // 10? bytes
//		U16 w0;		// @0
//		U8 b2;			// @2
//		U8 b3;		// @3
//		U8 b4;			// @4
//		U8 b5;			// @5
//		skxxx7 *pv6;	// @6
//
//		U8 b0_0_7() const { return U8(w0); }
//	};
	// 
	struct sk0cea { // 10 bytes
		X8 b0_;
		X8 b1_;
		X8 b2_;
		X8 b3_;
		X8 b4_;
		X8 b5_;
		skxxx7 *pv6_;

		skxxx7 *pv6() const { return pv6_; }
		void pv6(skxxx7 *pv) { pv6_ = pv; }

		X8 b0() const { return b0_; }
		X8 b2() const { return b2_; }
		X8 b3_0_3() const { return (b3_)&15; }
		X8 b3_4_4() const { return (b3_ >> 4)&1; }
		X8 b3_5_5() const { return (b3_ >> 5)&1; }
		X8 b3_6_6() const { return (b3_ >> 6)&1; }
		X8 b3_7_7() const { return (b3_ >> 7)&1; }
		X8 b4() const { return b4_; }
		X8 b5() const { return b5_; }

		X16 w0() const { return (b0_) | (b1_ << 8); }
		X16 w0_0_d() const { return w0() & 0x3fff; }
		X16 w0_f_f() const { return (w0() >> 15)&1; }

		void b3_4_4(U8 val) {
			val &= 1;
			b3_ &= 0xef;
			b3_ |= val << 4;
		}
		void b3_5_5(U8 val) {
			val &= 1;
			b3_ &= 0xdf;
			b3_ |= val << 5;
		}
		void b3_6_6(U8 val) {
			val &= 1;
			b3_ &= 0xbf;
			b3_ |= val << 6;
		}
		void b3_7_7(U8 val) {
			val &= 1;
			b3_ &= 0x7f;
			b3_ |= val << 7;
		}
	};
	// 
	struct sk13a4 { // 4 bytes
		U16 w0;		// @0
		U16 w2;		// @2
	};
	// 
	struct sk0d9e { // 6 bytes
		U16 w0;		// @0
		U16 w2;		// @2
		U16 w4;		// @4
	};
	// SPX: sk4e4e renamed MousePosition
	struct MousePosition { // 14 bytes
		U16 XPos;		// @0 // w0 mouse x-pos
		U16 YPos;		// @2 // w2 mouse y-pos
		U16 event;		// @4 // rectno?
		SRECT rc6;		// @6
		// rc6.x // @6
		// rc6.y // @8
		// rc6.cx // @10
		// rc6.cy // @12
	};
	// 
	struct ItemStatBarPos { // ItemStatBarPos 4 bytes
		U16 pos[2];	// w0[2] index of stat bar
	};
	// 
	struct sk3b74 { // 8 bytes
		U16 w0;	// @0
		U8 b2;	// @2
		U8 b3;	// @3
		U8 b4;	// @4
		U8 b5;	// @5
		U16 w6;	// @6

		U8 b5_0_0() const { return b5&1; }
		U8 b5_1_1() const { return b5&2; }
	};
	// SPX: sk57c8 renamed LeaderPossession
	struct LeaderPossession { // 22 bytes
		DMEncyclopaedia::ObjectID object;	// @0 w0
		U8 *pb2;						// @2 *pb2
		U8 b6[16];						// @6 b6[16]
	};
	// 
	// SPX: sk3e22 replaced by SpellDefinition
#if XDM2_EXTENDED_SPELLS_TAB == 0
	struct SpellDefinition { // 8 bytes
		U32 symbols;				// dw0 @0	//	4 Symbols (00 - 03)
		U8 difficulty;			// b4 @4	// Spell difficulty
		U8 requiredSkill;		// b5 @5	// Skill used
		U16 w6;					// w6 @6	// Spell type / cast

		U16 w6_a_f() const { return (w6 >> 10)&0x3f; }
		// SPX: w6_4_9 replaced by SpellCastIndex : retrieve spell index to cast (depend on spell type)
		// <!> limited to 64 possible value (not enough compared to 254 possible creatures to be summoned for example)
		U8 SpellCastIndex() const { return U8((w6 >> 4)&0x3f); }
	};
#elif XDM2_EXTENDED_SPELLS_TAB == 1
	struct SpellDefinition { // 8 bytes
		U32 symbols;				// dw0 @0	//	4 Symbols (00 - 03)
		U8 difficulty;
		U8 requiredSkill;
		U16 w6;					// w6 @6	// Spell type / cast
		U8 spellValue;

		U16 w6_a_f() const { return (w6 >> 10)&0x3f; }
		U8 SpellCastIndex() const { return spellValue; }
	};
#endif // DM2_EXTENDED_MODE
	// 
	enum SpellSymbol {
		s1LO = 0x60,	// 60
		s1UM,			// 61
		s1ON,			// 62
		s1EE,			// 63
		s1PAL,			// 64
		s1MON,			// 65

		s2YA = 0x66,	// 66
		s2VI,			// 67
		s2OH,			// 68
		s2FUL,			// 69
		s2DES,			// 6a
		s2ZO,			// 6b

		s3VEN = 0x6C,	// 6c
		s3EW,			// 6d
		s3KATH,			// 6e
		s3IR,			// 6f
		s3BRO,			// 70
		s3GOR,			// 71

		s4KU = 0x72,	// 72
		s4ROS,			// 73
		s4DAIN,			// 74
		s4NETA,			// 75
		s4RA,			// 76
		s4SAR,			// 77
	};
	// 
	struct sk536e { // 6 bytes
		U16 w0;							// @0
		U8 b2;							// @2
		U8 b3;							// @3
		DMEncyclopaedia::ObjectID id4;	// @4
	};
	// 
	struct sk4d1a { // 12 bytes
		SRECT rc0;						// @0
		DMEncyclopaedia::ObjectID w8;	// @8
		U8 b10;							// @10
		U8 b11;							// @11
	};
	// SPX: sk5a80 renamed CellTileDetail
	struct CellTileDetail { // 18 bytes
		i8 posx;			// @0 // b0 x-pos
		i8 posy;			// @1 // b1 y-pos
		ExtendedTileInfo xsrd;		// @2 // x2 summary room data
	};
	// 
	struct sk4be2 { // 2 bytes
		U8 b0;			// @0
		U8 b1;			// @1
	};
	// 
	struct sk4bde { // 9 bytes
		U8 b0;			// @0
		U8 *pv1;		// @1
		U8 *pv5;		// @5
	};

	// 
	struct sk1bf9 { // 14 bytes
		i8 b0;		// @0
		X8 b1;		// @1
		X16 w2;		// @2
		X16 w4;		// @4
		X16 w6;		// @6
		i8 b8;		// @8 // max eye distance?
		X8 b9;		// @9
		i8 b10;		// @10
		i8 b11;		// @11
		i8 b12;		// @12
		X8 b13;		// @13	// 0 = end of table

		U8 b6_0_f() const { return U8(w6); }
	};
	// 
	struct skxxx9 { // 22 bytes
		i8 b0;			// @0 // eye distance?
		i8 b1;			// @1
		Ax3 w2;			// @2
		Ax3 w4;			// @4 // where to go?
		i8 b6;			// @6 // non zero if need to find player, or have info to go player.
		i8 b7;			// @7 // reach-to-player confirm pattern#
		U16 w8;			// @8
		U16 w10;		// @10
		Ax3 w12;		// @12
		U8 b14;			// @14
		X8 b15;			// @15
		U8 b16;			// @16
		X8 b17;			// @17
		X32ptr dw18;	// @18

		sk1bf9 *pv18() {
			return reinterpret_cast<sk1bf9 *>(dw18);
		}
		void pv18(sk1bf9 *ss) {
			dw18 = reinterpret_cast<X32ptr>(ss);
		}
		U8 b18() const { return U8(dw18); }
		void b18(U8 val) {
			dw18 &= 0xffffff00;
			dw18 |= val;
		}
		U8 b19() const { return U8(dw18 >> 8); }
		void b19(U8 val) {
			dw18 &= 0xffff00ff;
			dw18 |= val << 8;
		}
	};
	// 
	struct skxxxa { // 4 bytes
		i8 b0_;			// @0
		i8 b1_;			// @1
		i8 b2_;			// @2
		i8 b3_;			// @3

		void setX(i8 val) { b0_ = val; }
		i8 getX() const { return b0_; }
		void setY(i8 val) { b1_ = val; }
		i8 getY() const { return b1_; }
		void setMap(i8 val) { b2_ = val; }
		i8 getMap() const { return b2_; }

		void setDist(i8 val) { b3_ = val; } // square distance
		i8 getDist() const { return b3_; }  // max=127
	};
//	// 
//	struct sk4eca { // ? bytes // sk4eca==AIDefinition
//		U16 w0;			// @0
//		X8 b2;			// @2
//
//		U16 w20;		// @20
//
//		U16 w0_6_7() const { return (w0 >> 6)&3; }
//		U16 w20_c_f() const { return w20 >> 12; }
//	};
	// SPX: looks to me this is a 'distance' map from player position
	struct DistMapTile { // 4 bytes
		U8 distance;	// b0 @0
		U8 dir;			// b1 @1 // dir? 0,1,2,3,4=up,5=down,255=me
		Ax3 w2;			// @2 // next one step to the target
	};
	// 
	struct WalkPath { // 2 bytes
		U16 w0;			// @0

		U8 getDir() const { return U8(w0&7); }
		void setDir(U16 val) {
			val &= 7;
			w0 &= 0xfff8;
			w0 |= val;
		}

		void setX(U16 val) {
			val &= 31;
			w0 &= 0xff07;
			w0 |= val << 3;
		}

		U8 getY() const { return U8((w0>>8)&31); }
		void setY(U16 val) {
			val &= 31;
			w0 &= 0xe0ff;
			w0 |= val << 8;
		}

		U8 getMapCross() const { return U8((w0>>13)&1); }
		void setMapCross(U16 val) {
			val &= 1;
			w0 &= 0xdfff;
			w0 |= val << 13;
		}
	};
//	// 
//	struct sk4ece { // ? bytes // sk4ece==sk4ebe
//		X8 b4;			// @4
//
//		Ax3 w12;		// @12
//
//		Ax3 w24;		// @24
//		X8 b26;			// @26
//		X8 b27;			// @27
//		X8 b28;			// @28
//		X8 b29;			// @29
//
//		X8 b32;			// @32
//
//		X8 b30;			// @30
//	};
	//
	struct SkEnt4 { // 4 bytes
		U8 b0;
		U8 b1;
		U8 b2;
		U8 b3;

		U8 cls1() const { return b0; } // main cls
		U8 cls2() const { return b1; } // sub cls
		U8 cls3() const { return b2; } // type
		U8 cls4() const { return b3; } // item

		void cls1(U8 val) { b0 = val; }
		void cls2(U8 val) { b1 = val; }
		void cls3(U8 val) { b2 = val; }
		void cls4(U8 val) { b3 = val; }
	};
	// 
	struct SkLoadEnt { // 6 bytes
		X16 w0_;	// @0
		SkEnt4 x2;

		void w0_f_f(U16 val) {
			val &= 1;
			w0_ &= 0x7fff;
			w0_ |= val << 15;
		}
		void w0(U16 val) {
			w0_ = val;
		}
		U16 w0() const {
			return w0_;
		}
		U16 w0_0_e() const {
			return w0_ & 0x7fffU;
		}

		void MarkIncluded() { w0_f_f(0); }
		void MarkExcluded() { w0_f_f(1); }
		void MarkContinuous() { w0_ = 1; }
		void MarkContinuousExcluded() { w0(0x8001); }
	};
//	// 
//	struct skxxxe { // ? bytes // skxxxe == RawEntry
//		X16 w0;		// @0
//		X16 w2;		// @2
//
//		U8 b0() const { return U8(w0); }
//		U8 b1() const { return U8(w0 >> 8); }
//	};
	// 
	struct SkEntIter {	// ? bytes
		X16 w0;		// @0 // enum init flag: 1=first num(need init), 0=second or later enum, 0x8000=ranged
		SkLoadEnt x2;	// @2 bp5a // single, or from
		SkLoadEnt x8;	// @8 bp54 // to
		RawEntry *pv14;	// @14 bp4e // found raw ent
		U8 b18_;	// @18 // cls1 current
		U8 b19_;	// @19 // cls1 base
		U8 b20_;	// @20 // cls3(type) current
		U8 b21_;	// @21 // cls3 base
		X16 w22;	// @22 // cls2 table index
		X16 w24;	// @24 // found raw entry index (pv14)
		U16 w26;	// @26 // cls3 table index

		void cls1cur(U8 val) {
			b18_ = val;
		}
		void cls1base(U8 val) {
			b19_ = val;
		}
		void cls3cur(U8 val) {
			b20_ = val;
		}
		void cls3base(U8 val) {
			b21_ = val;
		}

		U8 cls1cur() const {
			return b18_;
		}
		U8 cls1base() const {
			return b19_;
		}
		U8 cls3cur() const {
			return b20_;
		}
		U8 cls3base() const {
			return b21_;
		}
	};
	// 
	struct skxxxf { // 10 bytes
		X8 b0;	// @0 // table?
		X8 b1;	// @1 // char index
		X16 w2;	// @2 // cell x-cnt  // x1
		X16 w4;	// @4 // cell y-cnt  // y1
		X16 w6;	// @6 // cell width  // x2
		X16 w8;	// @8 // cell height // y2
	};
	// 
	struct tiamat {
		// tiamat is a representation of memory address pointer.
		// often tiamat is used in the source code instead of U8*
		// because DM2 MS-DOS version often allocates an offset to EMS memory to U8* variables.
		// it is ok as system design. however VisualC warns me that its conversion loses pointer precision!

		// tiamat locates 1 of:
		// ... address in main memory (cram[])
		// ... address in EMS memory (cems[])
		// ... invalid address

		U32 val;

		tiamat(): val(U32(-1)) { }
		tiamat(const tiamat &s): val(s.val) { }
		explicit tiamat(shelf_memory ps) {
			ATLASSERT(ps.Present());
			val = ps.val;
		}

		bool IsntZero() const {
			return val != 0;
		}
		bool IsZero() const {
			return val == 0;
		}

		static tiamat zero() {
			tiamat v;
			v.val = 0;
			return v;
		}
		static tiamat Raw(U32 val) {
			tiamat v;
			v.val = val;
			return v;
		}

		U8 Area() const {
			if (val < MAX_CRAM) // SPX: should be evaluated against skWinApp
				return 0;
			if (val - 0x200000 < MAX_CEMS)
				return 1;
			if (val == (U32)-1)
				return 2;

			return 3;
		}

		static U32 Size(const tiamat &x, const tiamat &y) {
			ATLASSERT(x.Area() == y.Area());
			return U32(x.val -y.val);
		}

		static tiamat From_cems(U32 val) {
			tiamat v(Raw(val +0x200000));
			return v;
		}

		void operator -=(U32 xx) {
			tiamat v(Raw(val -xx));
			ATLASSERT(v.Area() == Area());
			val = v.val;
		}

		tiamat operator -(U32 xx) const {
			tiamat v(Raw(val -xx));
			ATLASSERT(v.Area() == Area());
			return v;
		}
		tiamat operator +(U32 xx) const {
			tiamat v(Raw(val +xx));
			ATLASSERT(v.Area() == Area());
			return v;
		}

		tiamat operator -(const tiamat &y) const {
			const tiamat &x = *this;
			ATLASSERT(x.Area() == y.Area());
			tiamat v = Raw(this->val - y.val);
			ATLASSERT(v.Area() == y.Area());
			return v;
		}

		bool operator ==(const tiamat &y) const {
			const tiamat &x = *this;
			ATLASSERT(x.Area() == y.Area());
			return x.val == y.val;
		}
		bool operator !=(const tiamat &y) const {
			const tiamat &x = *this;
			ATLASSERT(x.Area() == y.Area());
			return x.val != y.val;
		}
		bool operator <(const tiamat &y) const {
			const tiamat &x = *this;
			ATLASSERT(x.Area() == y.Area());
			return x.val < y.val;
		}
		bool operator <=(const tiamat &y) const {
			const tiamat &x = *this;
			ATLASSERT(x.Area() == y.Area());
			return x.val <= y.val;
		}
		bool operator >(const tiamat &y) const {
			const tiamat &x = *this;
			ATLASSERT(x.Area() == y.Area());
			return x.val < y.val;
		}
		bool operator >=(const tiamat &y) const {
			const tiamat &x = *this;
			ATLASSERT(x.Area() == y.Area());
			return x.val >= y.val;
		}
	};
	// 
	struct sk5d12 {	// 14 bytes?
		tiamat t0;	// @0 // ptr to pool
		tiamat t4;	// @4 // (poolBuff -poolSize)
		tiamat t8;	// @8 // ?
		X16 w12;	// @12 // poolflags

		U16 Is4EMS() const { return (w12 >> 11)&1; }
	};
	// SPX: sk5c2a renamed DistantEnvironment
	struct DistantEnvironment { // 10 bytes
		X8 envImg;		// b0	: for sky image, distant thunder and wet ground
		X8 cmFW;			// b1 env command FW
		X16 cmCD;			// w2 env command CD
		X16 w4;
		X16 w6;
		U8 b8;
		U8 b9;
	};
	// 
	struct skfh4 { // 4 bytes
		U16 w0; // 1 U16: File Signature (8005h)
		U16 w2; // 1 U16: Number of items in file.
	};
	// 
	struct skxxxe { // 4 bytes
		U8 b0; // cls2?
		U8 b1; // cls4?
		U16 w2; // rawindex?
	};
	// 
	// SPX: sk5cb6 renamed 
	struct GDATEntries { // ? bytes (max 44 bytes)
		X16 *pw0;		// cls1-to-cls2
		X16 *pw4;		// cls2-to-cls3?
		RawEntry *pv8;
		U16 w12;		// cnt cls1
		X16 w14;		// total cnt of cls2
		X16 w16;
		X16 w18;
		X16 w20;
		X16 w22[7];
		X8 b36[5];
	};
	// 
	struct sk514e { // 14 bytes // 514e-515b
		U8 b0;			// @0 // _4976_514e // max len
		U8 b1;			// @1 // _4976_514f // cur pos: _4976_515c[b0 - b1]
		X8 b2;			// @2 // _4976_5150
		X8 b3;			// @3 // _4976_5151
		X16 w4;			// @4 // _4976_5152
		X16 w6;			// @6 // _4976_5154
		Ax3 w8;			// @8 // _4976_5156
		sk1bf9 *pv10;	// @10 // _4976_5158

		U8 b4() const { return U8(w4); }
		U8 b6() const { return U8(w4); }
	};
	// xactr (x-action result)
	enum {
		xactrNeedReset = -1,// 0xff: _4976_3672[xx], xx needs to be re-computed.
		xactrYes = -2,      // 0xfe: select _4976_3672[xx][yy].b1()
		xactrNo  = -3,      // 0xfd: select _4976_3672[xx][yy].b2()
		xactrAgain = -4,    // 0xfc: proceed again?

		// from b1() or b2():
		xactrNext1 = -5, // 0xfb: _4976_3672[xx][yy += 1]
		xactrPrev1 = -6, // 0xfa: _4976_3672[xx][yy -= 1]
		xactrRetry = -7, // 0xf9: proceed again?
		xactrNext2 = -8, // 0xf8: _4976_3672[xx][yy += 2]
	};
#ifdef __DJGPP__
	// 
	struct sk3672 { // 7 bytes
		i32 b0_[7];

		i8 b0() const { return b0_[0]; } // xact
		i8 b1() const { return b0_[1]; } // if yes, xactrNext1, xactrPrev1, xactrRetry, xactrNext2, or b1(),b2() may be yy of _4976_3672[xx][yy] if positive number given.
		i8 b2() const { return b0_[2]; } // if no,  same as above.
		i8 b3() const { return b0_[3]; } // parm1 for XACT_xx
		i8 b4() const { return b0_[4]; } // parm2 for XACT_xx
		U8 b5() const { return b0_[5]; } // func# [0..16]
		U8 b6() const { return b0_[6]; } // func param 2
	};
#else
	// 
	struct sk3672 { // 7 bytes
		X8 b0_[7];

		i8 b0() const { return b0_[0]; } // xact
		i8 b1() const { return b0_[1]; } // if yes, xactrNext1, xactrPrev1, xactrRetry, xactrNext2, or b1(),b2() may be yy of _4976_3672[xx][yy] if positive number given.
		i8 b2() const { return b0_[2]; } // if no,  same as above.
		i8 b3() const { return b0_[3]; } // parm1 for XACT_xx
		i8 b4() const { return b0_[4]; } // parm2 for XACT_xx
		U8 b5() const { return b0_[5]; } // func# [0..16]
		U8 b6() const { return b0_[6]; } // func param 2
	};
#endif
	// 
	struct skxxxh { // 7 bytes
		i8 b0;			// @0
		i8 b1;			// @1 // if positive pass on 100.0/b1 percent. if negative skip on 100.0/b1 percent.
		sk1bf9 *pv2;	// @2
		X8 b6;			// @6	// 0 = end of table
	};
	// 
	struct sk4efe { // 6 bytes
		X16 w0;			// @0
		skxxxh *pv2;	// @2
	};
	// 
	struct skxxxg { // 4 bytes?
		i8 b0;		// @0
		X8 b1;		// @1
		X8 b2;		// @2
		X8 b3;		// @3
	};
	// 
	struct sk4f04 { // 32 bytes
		sk1bf9 *pv0;	// @0
		X8 b4;			// @4
		i8 b5;			// @5
		X8 b6;			// @6
		X8 b7;			// @7
		skxxxg x8[6];	// @8
	};
	// 
	struct skxxxi { // 10 bytes
		i16 w0;
		X16 w2;
		X16 w4;
		X16 w6;
		X16 w8;
	};
	// 
	struct skhero { // 52 bytes
		U16 herodata[26];	// w0[26] : 3 + 7 + 16 = 26
		// SPX: data would be as follow:
		// 00 - 02 : HP, Stamina, Mana
		// 03 - 09 : Attributes (LUCK, STR ... ANTI FIRE)
		// 10 - 26 : Detailed skills experience (4 for FIGHTER, NINJA, PRIEST, WIZARD)

		U16 maxHP() const { return herodata[0]; }	// hp
		U16 maxStamina() const { return herodata[1]; }	// stamina x10
		U16 maxMP() const { return herodata[2]; }	// mp
	};
	// 
	enum {
		EPcls1 = 0,
		EPcls2 = 1,
		EPcls3 = 2,
		EPcls4 = 3,
		EPdata = 4,
		EPcls5 = 5,
		EPcls6 = 6,
	};
	// 
	struct sk01b2 { // 51 bytes
		U8 b0[14];		// @0
		U8 b14[32];		// @14
		U8 b46;			// @46
		U8 b47;			// @47
		U8 b48_[3];
	};
	// 
	struct sk060b { // 80 bytes
		U8 b0[80];
	};
	// 
	struct sk0260 { // 6 bytes
		U32 dw0;	// @0
		U16 w4;		// @4
	};
	// 
	struct skxxxj { // 4 bytes
		U8 b0;	// @0 // index?
		U8 b1;	// @1 // red
		U8 b2;	// @2 // green
		U8 b3;	// @3 // blue
	};
	// 
	enum {
		Scard01 = 1,
		ScardTandy = 2,
		Scard03 = 3,
		Scard04 = 4,
		Scard05 = 5,
		ScardSBlaster = 6,
		Scard07 = 7,
	};
	// 
	struct sk0e80 { // 582 bytes
		U8 b0;			// @0 // ?
		U8 b1;			// @1 // ?
		U8 b2;			// @2 // cx
		U8 b3;			// @3 // cy
		U8 b4;			// @4 // colorkey
		U8 b5_;			// @5 // ?
		U8 b6[24*24];	// @6
	};
	// SPX: Added comments about numbers used in CSBWin
	// (t)imer (ty)pe
	enum {
		// CSBWin			-3 : Monster damaged by cloud or door
		//					-2 : When a monster is damaged
		//					-1 : When a monster is attacked. When we bump into a monster
		tty00 = 0,			// csbwin: empty
		ttyDoorStep			= 0x01, // tty01 step door
		ttyDoorDestroy		= 0x02,		// tty02 CSBWin => Bash door
		tty04 = 0x04, // activation for tile record (such as floor/wall/pitfall/door/trickwall/teleporter)
		// csbwin: 0x0B (11) reenable character attack
		tty0C = 0x0C,		// csbwin 12 ? highlight for champion damaged ?
		ttyResurrect		= 0x0D,	// tty0D CSBWin (13) => VI Altar
		ttyItemBonus		= 0x0E,	// item bonus ?
		// csbwin 0x14 (20) ? queue sound
		ttySound			= 0x15,		// tty15
		// csbwin 0x16 (22) ?
		// tty18			= 0x18, // csbwin (24): remove fluxcage
		ttyCloud			= 0x19,	// csbwin tty19 (25) : cloud processing
		// csbwin 0x01C (28) ??
		tty1D = 0x1D, // (29) step missile 0
		tty1E = 0x1E, // (30) step missile 1
		tty21 = 0x21, // (33) think creature 0
		tty22 = 0x22, // (34) think creature 1
		tty3C = 0x3C,
		tty3D = 0x3D,		// about minion and teleport / opening something ?
		ttyLight			= 0x46,	// tty46 (70) light
		ttyInvisibility		= 0x47,	// tty47 (71) csbwin OH EW SAR
		ttyEnchantment		= 0x48, // tty48 (72) enchantment/aura timer?
#if (XDM1_EXTENDED_SEETHRUWALLS == 1)
		ttySeeThruWalls		= 0x49, // tty49 (73) csbwin : OH EW RA See thru walls
#endif
		//ttyPartyShield		= 0x4A, // tty4A (74) csbwin : YA IR
		ttyPoison			= 0x4B, // tty4B (75) poison timer
		// csbwin 77 Spell shield
		// csbwin 78 Fire shield
		// csbwin 79 YA BRO ROS : Magic Footprints
		ttyWeather			= 0x54,	// tty54 (84)
		ttyOrnateAnimator	= 0x55,	// tty55 (85)
		ttyTickGenerator	= 0x56,	// tty56 (86)
		ttyWallButton		= 0x57,	// tty57 (87) restore wall button to released state
		ttyDoorButton		= 0x58, // tty58 (88) restore door button released
		tty59 = 0x59, // (89)
		tty5A = 0x5A, // (90) queued noise gen
		tty5B = 0x5B, // (91) 
		ttySimpleActuTurnOn = 0x5C,	// tty5C (92) restore ornate (simple actuator) in ON state (used for RESPAWNING flowers, mushrooms, gems ..)
		tty5D = 0x5D, // (93) 
#if	XDMX_FEATURE_AMBIENT_SOUND == 1	// SPX: new timer types
		ttyAmbientSound		= 0xE0,	// CUSTOM timer for ambient sound (i.e. rain)
#endif
#if	XDMX_FEATURE_PLAGUE == 1
		ttyPlague			= 0xE1,	// CUSTOM timer for PLAGUE
#endif
	};
	// ccm (creature command)
	enum {
		ccmNeutral = 0,		// (ccm0) stand .. do nothing
		ccm01 = 0x01, // walk?
		ccm02 = 0x02, // walk?
		ccm03 = 0x03, // turn ?
		ccm04 = 0x04, // turn right and walk ?
		ccmJump = 0x05, // (ccm05) jump
		ccm06 = 0x06,	// turn left -> stand ?
		ccm07 = 0x07,	// turn right -> stand ?
		ccmMeleeAttack = 0x08, // (ccm08) standard attack
		ccm09 = 0x09, // walk?
		ccmSteal = 0x0A, // (ccm0A) steal
		ccm0B = 0x0B,	// another attack ?
		ccm0C = 0x0C, // shopkeeper takes item from pool?
		ccm0D = 0x0D,	// pick item
		ccmShootItem1 = 0x0E, // (ccm0E = 0x0E) throw item
		ccmShootItem2 = 0x0F, // (ccm0F = 0x0F) throw item
		ccm10 = 0x10,	// another attack ?
		ccmSpawn = 0x11,	// (ccm11) spawn
		ccm12 = 0x12,	// unused
		ccmDestroy = 0x13, // (ccm13) death
		ccm14 = 0x14,	// unused
		ccm15 = 0x15,
		ccm16 = 0x16,
		ccmPlaceMerchandise = 0x17, // place money/merchandise at the trade table
		ccmTakeMerchandise = 0x18, // (ccm18) take money/merchandise at the trade table
		ccm19 = 0x19,	// take arrow from quiver ?
		ccm1A = 0x1A,	// pick arrow ?
		ccm1B = 0x1B,	// state 0 / refuse
		ccm1C = 0x1C,	// state 1 / yell / accept
		ccm1D = 0x1D,	// state 2 / blink
		ccm1E = 0x1E,	// state 3 / sorry
		ccm1F = 0x1F,	// sorry (merchant) ?
		ccm20 = 0x20,	// think (merchant) ?
		ccm21 = 0x21,	// unused
		ccm22 = 0x22,	// unused
		ccm23 = 0x23,	// Go to sleep (bat) ?
		ccm24 = 0x24,	// Sleep (bat) ?
		ccm25 = 0x25,	// Awake from sleep (bat) ?
		ccmPushBack = 0x26, // (ccm26 = 0x26) attacking you? knock back ?	
		ccmCastSpell1 = 0x27, // (ccm27 = 0x27) cast spell
		ccmCastSpell2 = 0x28, // (ccm28 = 0x28) cast spell
		ccm29 = 0x29,	// take bone ?
		ccm2A = 0x2A,	// put bone ? (also take?)
		ccm2B = 0x2B,	// pick item and move ?
		ccm2C = 0x2C,	// pick item and move ?
		ccm2D = 0x2D,	// take ?
		ccm2E = 0x2E,	// take ?
		ccm2F = 0x2F,	// switch button ?
		ccm30 = 0x30,	// switch button (giggler) ?
		ccm31 = 0x31,	// switch button (giggler) ?
		ccm32 = 0x32,	// unknown / used at 13E4:0AEC
		ccm33 = 0x33,	// unknown / used at 13E4:0AEC
		ccm34 = 0x34,	// unknown / used at 13E4:0AEC
		ccm35 = 0x35,	// Going up (minion) ?
		ccm36 = 0x36,	// Coming from top (minion) ?
		ccm38 = 0x38,	// Going up (minion) ?
		ccm37 = 0x37,	// Coming from top (minion) ?
		ccm39 = 0x39,	// Going up (minion) ?
		ccm3A = 0x3A,	// Coming from top (minion) ?
		ccmTransform = 0x3B,	// (ccm3B)	creature transforms into another (minion => guard minion)
		ccmTransform2 = 0x3C,
		ccmExplode = 0x3D,	// 3D 1	// (ccm3D) Prepare to explode (fire orb) ?
		ccm3E = 0x3E,	// 3D 2
		ccm3F = 0x3F,	// 3D 3
		ccm40 = 0x40,	// 3D 4

		ccm42 = 0x42,	// Hello 1 (rocky) ?
		ccm52 = 0x52,	// Hello 2 (rocky) ?
		
		ccmCastSpell1or2 = 0x55,	// Cast reflector (Dragoth) ? No, will either transform to ccmCastSpell1 or ccmCastSpell2
		ccmInv = -1,	// Idle
	};
#define MAX_CREATURE_COMMANDS	86
	// prog check ccm is < 86, because using static info ccm table _4976_37a6
	// Cloud(missile) type
	enum {
		missileFireball		= 0,
		missilePoisonBlob	= 1,			// SPX: Only seen in CSB
		missileLightning	= 2,
		missileDispell		= 3,
		missileZoSpell		= 4,
		missileFuse			= 5,				// SPX: Only seen in DM
		missilePoisonBolt	= 6,
		missilePoisonCloud	= 7,

		missileTeleporter	= 14,
		missileDustCloud	= 0x28,		// 40
		missileThunderBolt	= 0x30,		// 48
	};
	// Player ability 7
	enum {
		abLuck = 0,	// SPX added luck
		abStr = 1,
		abDex = 2,
		abWiz = 3,
		abVit = 4,
		abAntiMagic = 5,
		abAntiFire = 6,
	};
	// Entry data type
	enum {
		fmtImage = 1,
		fmtSound = 2,
		fmtHMP = 3, // midi, not avail in DM2 PS/V
		fmtRect = 4,
		fmtText = 5,
		fmt06 = 6, // raw data
		fmt07 = 7, // raw data
		fmt08 = 8, // raw data
		fmtPalIRGB = 9,
		fmtWordVal = 11,
		fmtPicOff = 12,
		fmtPalIndex = 13, // fmt0D
		fmt0E = 14, // raw data
		fmtInvalid = 0xFF,
	};
};

#pragma pack(pop)

//------------------------------------------------------------------------------

#endif // _SK_DM_ENCYCLOPEDIA_
