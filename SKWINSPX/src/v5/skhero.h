#ifndef C_HERO_H
  #define C_HERO_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  #define HERO_SIZE_1STNAME  8
  #define HERO_SIZE_2NDNAME 20

  #define NUM_ABILITIES 7

  enum e_ability
  {
    E_LUCK,
    E_STRENGTH,
    E_DEXTERITY,
    E_WIZARDRY,
    E_VITALITY,
    E_ANTIMAGIC,
    E_ANTIFIRE
  };

  enum e_curmax
  {
    E_CUR,
    E_MAX
  };

  enum e_profession
  {
    E_FIGHTER,
    E_NINJA,
    E_WIZARD,
    E_PRIEST
  };

  #define NUM_ITEMS  (30)

  class c_hero // size 0x107 <=> 263 NOTE: DON'T CHANGE SIZES! (savegame)
  {
    public:
      t_text name1[HERO_SIZE_1STNAME]; // @00
      t_text name2[HERO_SIZE_2NDNAME]; // @08
      i8 absdir; // @1c absolute direction 0=N 1=E 2=S 3=W TODO: tell SPX that here are two E
      i8 partypos; // @1d player position in party 0=TL, 1=TR, 2=BR, 3=BL
      i8 nrunes; // @1e runes count tell, SPX - he has a word here, but are two bytes - no word access for me
      i8 poisoned; // @1f

      i8 handcmd[2]; // @20
      t_text rune[5]; // @22 0-3 spelled rune-string (rune[4] should stay '\0')
      i8 b_27; // @27 unk (unused!)
      i8 b_28; // @28 unk (some direction) set, but never read!
      i8 b_29; // @29 unk

      i8 handcooldown[4]; // @2a, the 3rd byte is used in a loop together with the others!
                          // the fourth looks unused

      i16 timeridx; // @2e
      i16 damagesuffered; // @30
      i16 heroflag; // @32 bitfield TODO
      i16 bodyflag; // @34
      i16 curHP; // @36
      i16 maxHP; // @38
      i16 curStamina; // @3a
      i16 maxStamina; // @3c
      i16 curMP; // @3e
      i16 maxMP; // @40
      i8 handdefenseclass[2]; // @42
      i16 food; // @44
      i16 water; // @46

      i16 poison; // @48 extra (to SPX version) word !!! TODO tell SPX

      i8 ability[NUM_ABILITIES][2]; // @4a cur/max Luck,Str,Dex,Wiz,Vit,AntiM,AntiF
                                    // 1st index: e_ability 2nd index: e_curmax
      i8 eability[NUM_ABILITIES]; // then enhanced, e_ability

      i32 skill[5][4]; // @5f
    // skill[0][n] -> sum of skill[n+1][0-3]
		// 0 to 3 ... fighter, ninja, wiz, priest
		// 4 to 19 ... fighter x4, ninja x4, wiz x4, priest x4

      i8 sbonus[5][4]; // @af skill bonus

      i16 item[NUM_ITEMS]; // @c3

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

      i16 weight; // @ff weight
      i8 herotype; // @101
      i8 ench_aura; // @102 enchantmentAura
      i16 ench_power; // @103 enchantmentPower
      i8 walkspeed; // @105
      i8 b_106; // @106 unk (unused)

    public:
      void init(void);
      c_hero(void) { init(); }

      i16 get_adj_ability1(e_ability abi, e_curmax curmax);
      i32 get_adj_ability2(e_ability abi, i16 in);
      i16 get_stamina_adj(i16 in);
      i16 get_max_load(void);
      bool use_luck(i16 in);
  };

  #define MAX_HEROS 4

  union u_fourteen // size 0x14
  {
    i16 warr2d_00[5][2]; // TODO: what is 1st-index 0 good for? 1..4 are for the heros
    // base adress: 0x1d672
    // 1st extra adress: 0x1d676 -> [+1][0]
    // 2nd extra adress: 0x1d678 -> [+1][1]
    i8 barr_00[20]; // why linear?
  };

  class c_party
  {
    public:
      c_hero hero[MAX_HEROS]; // index: e_hero
      i16 heros_in_party; // was savegamew2
      i16 absdir; // was v1e0276
      e_hero curactevhero; // was v 1d67d7 latest hero set by eventqueue.event_heroidx
      i16 hand_container[CONTAINERSIZE]; // was v1e095c, this is the one to pick from in a hand
      i16 curactmode; // was v1e0b76 latest activation-type: 0 = left-hand 1 = right-hand 2 = spell
      // Note: 0 and 1 are pretty related to the item-places 0 and 1 which represent the hands!!!
      i16 curacthero; // was v1e0b74 latest activated hero (related to activation-type), value is e_hero + 1, so 0 means NOHERO
      u_fourteen handitems; // was v1d67c2

    public:
      void init(void);
      void rotate(i16 eaxw);
      void set_hero_flags(void);
      i16 get_player_weight(e_hero heroidx);
      void calc_player_weight(e_hero heroidx);
  };

  extern c_party party;

  i16 DM2_CALC_PLAYER_ATTACK_DAMAGE(c_hero* hero, i16 heroidx, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1, i16 argw2, i16 argw3, i16 argw4);
  void DM2_hero_39796(c_hero* hero);
  void DM2_hero_2c1d_0300(c_hero* hero, i32 edxl, i32 ebxl);
  void DM2_SHOOT_CHAMPION_MISSILE(c_hero* hero, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0);
  bool DM2_hero_2c1d_0186(c_hero* hero, i32 edxl, i32 ebxl, i32 ecxl);
  void DM2_BRING_CHAMPION_TO_LIFE(i16 eaxw);
  void DM2_SELECT_CHAMPION(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  i32 DM2_ADJUST_SKILLS(i16 heroidx, i32 edxl, i32 ebxl);
  i16 DM2_WOUND_PLAYER(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  i32 DM2_ADJUST_STAMINA(i16 heroidx, i16 edxw);
  void DM2_UPDATE_CHAMPIONS_STATS(void);
  i16 DM2_USE_DEXTERITY_ATTRIBUTE(i16 heroidx);
  bool DM2_WIELD_WEAPON(i16 eaxw, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i16 argw1);
  void DM2_EQUIP_ITEM_TO_HAND(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_ADD_ITEM_TO_PLAYER(e_hero heroidx, i16 edxw);
  void DM2_BURN_PLAYER_LIGHTING_ITEMS(void);
  i32 DM2_REMOVE_OBJECT_FROM_HAND(void);
  void DM2_SELECT_CHAMPION_LEADER(i32 eaxl);
  i32 DM2_GET_PARTY_SPECIAL_FORCE(void);
  void DM2_ADJUST_HAND_COOLDOWN(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_PLAYER_DEFEATED(i32 eaxl);
  i32 DM2_REMOVE_POSSESSION(i32 eaxl, i32 edxl);
  void DM2_PROCESS_PLAYERS_DAMAGE(void);
  void DM2_PUT_ITEM_TO_PLAYER(i16 eaxw);
  void DM2_PERFORM_TURN_SQUAD(i16 eaxw);
  void DM2_RESET_SQUAD_DIR(void);
  i32 DM2_RESUME_FROM_WAKE(void);
  void DM2_SET_SPELLING_CHAMPION(i16 eaxw);
  void DM2_PLAYER_CONSUME_OBJECT(i32 eaxl, i32 edxl, i32 ebxl);
  i32 DM2_ATTACK_PARTY(i32 eaxl, i32 edxl, i32 ebxl);
  void DM2_PROCESS_POISON(i32 eaxl, i32 edxl);
  i32 DM2_TAKE_COIN_FROM_WALLET(i32 eaxl, i32 edxl);
  i32 DM2_ADD_COIN_TO_WALLET(i32 eaxl, i32 edxl);
  void DM2_LOAD_PROJECTILE_TO_HAND(i32 eaxl, i32 edxl);
  i32 DM2_CAST_CHAMPION_MISSILE_SPELL(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  i32 DM2_hero_2c1d_1de2(e_hero heroidx, i32 edxl, i32 ebxl);
  void DM2_CHANGE_PLAYER_POS(i16 eaxw);
  void DM2_CHAMPION_SQUAD_RECOMPUTE_POSITION(void);

#endif

