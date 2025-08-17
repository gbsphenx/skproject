#ifndef ARCHIBIT_CREATURE_H
  #define ARCHIBIT_CREATURE_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  class c_creature // size 0x22
  {
    public:
		  i16 w_00;	// @0 // index of creature db
		  i16 w_02;	// @2 // timerid
		  i8 b_04;	// @4
		  i8 b_05;	// @5
		  i8 b_06;	// @6
		  ui8 b_07;	// @7	// spx: can this be related to target champion ?? (high speculation)
		  i8 b_08;	// @8 // const base index. _4976_5fde[w8 + w10] // sk1c9a02c3::w0
		  i8 b_09;	// @9
		  i8 b_0a;	// @a // variant index. // sk1c9a02c3::w2
		  i8 b_0b;	// @b
		  SPX_Ax3 w_0c;	// @c TODO type is t_record
		  i16 w_0e;	// @e TODO type is t_record
		  i16 w_10;	// @10 TODO type is t_record
		  i8 x;			// @12 // b18 _4976_3672[xx][]  , b18 is xx
		  i8 y;			// @13 // b19 _4976_3672[xx][yy], b19 is yy
		  i16 w_14;	// @14 // recent damage for creature
		  i8 b_16;	// @16
		  ui8 b_17;	// @17 // Command2 U8 b23 // pre creature action pattern? ccmXX
		  SPX_Ax3 w_18;	// @18 // creature's target, a point to the target.
		  i8 b_1a;	// @1a // Command i8 b26 // creature action pattern? ccmXX
		  i8 b_1b;	// @1b // i8 b27 // direction ?
		  ui8 b_1c;	// @1c
		  i8 b_1d;	// @1d	// next direction ??
		  i8 b_1e;	// @1e	// ItemToThrow b30	ItemToThrow or CreatureToSummon
		  i8 b_1f;	// @1f
		  i8 b_20;	// @20
		  ui8 b_21;	// @21	// result of proceed_ccm ?
  };

  extern c_creature* creatures;

	class SPX_Creature // size 0x10
  {
    public:
		  ObjectID w_00; // Next object ID.
		  ObjectID possession; // w2 Next possession object ID. Although not recommended, it is possible to
		  ui8 b_04; // SPX_Creature type. Her
		  ui8 b_05; // Position of each c // where &sk4ebe[b5]
		  ui16 w_06; // Hit points of creature 1
		  ui16 w_08; // Hit points of creature 2 // sk1c9a02c3::w0
		  ui16 w_0a; // Hit points of creature 3 // sk1c9a02c3::w2
		  ui16 w_0c; // Hit points of creature 4
		  ui16 w_0e; // TODO tell SPX it's a word here
	};

  void init_creatures(void);

  i16 DM2_APPLY_CREATURE_POISON_RESISTANCE(i32 eaxl, i32 edxl);
  void DM2_ROTATE_CREATURE(i32 eaxl, i32 edxl, i32 ebxl);
  i16 DM2_CREATURE_CAN_HANDLE_ITEM_IN(i16 eaxw, i32 edxl, i32 ebxl);
  i32 DM2_WOUND_CREATURE(i32 eaxl);
  void DM2_ATTACK_CREATURE(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1);
  i32 DM2_CREATURE_ATTACKS_CREATURE(i32 eaxl, i32 edxl);
  i32 DM2_CREATURE_ATTACKS_PLAYER(unk* xeaxp, i32 edxl);
  i32 DM2_CREATURE_ATTACKS_PARTY(void);
  i32 DM2_CONFUSE_CREATURE(i32 eaxl, i32 edxl, i32 ebxl);
  i32 DM2_CREATURE_CAN_HANDLE_IT(i32 eaxl, i32 edxl);
  i32 DM2_CREATURE_WALK_NOW(void);
  i32 DM2_PROCEED_CCM(void);
  i32 DM2_GET_CREATURE_ANIMATION_FRAME(i32 eaxl, i32 edxl, i16* ebxpw, i16* ecxpw, unk** argpp0, i32 argl1);
  i32 DM2_4FCC(i32 eaxl, i32 edxl, i16* ebxpw, unk** xecxpp);

#endif // C_CREATURE_H
