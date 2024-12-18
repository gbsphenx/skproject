#ifndef __GENERAL_DEFINE__
#define __GENERAL_DEFINE__

// SPX BEGIN : new defines replacing hard coded value
// These could also be declared as enums or constants


#define _OPTION_DUNGEON_NO_SPECIFIC_		0	// to be used with original DATA folder
#define _OPTION_DUNGEON_DM1_KID_			1
#define _OPTION_DUNGEON_DM1_DM_				2
#define _OPTION_DUNGEON_DM1_CSB_			3
#define _OPTION_DUNGEON_DM1_TQ_				4
#define _OPTION_DUNGEON_DM2_DM_				5
#define _OPTION_DUNGEON_DM2_CSB_			6
#define _OPTION_DUNGEON_DM2_TQ_				7
#define _OPTION_DUNGEON_DM2_BETA_			8
#define _OPTION_DUNGEON_DM2_DEMO_			9
#define _OPTION_DUNGEON_DM2_SK				10
#define _OPTION_DUNGEON_DMB_BW				11


#define _GDAT_LANG_ENGLISH_					0x10
#define _GDAT_LANG_JAPANESE_				0x20
#define _GDAT_LANG_GERMAN_					0x30
#define _GDAT_LANG_FRENCH_					0x40
#define _GDAT_LANG_SPANISH_					0x50	// SPX: Additional value
#define _GDAT_LANG_ITALIAN_					0x60	// SPX: Additional value

// DataTypes used in GDAT
enum dtIndex 
{
	dtImage		= 1,
	dtSnd		= 2,
	dtHMP		= 3,
	dtRectangle	= 4,
		dt04	= 4,
	dtText		= 5,
	dt06		= 6,
		dt07		= 7,
	dtRaw7		= 7,
	dtRaw8		= 8, // image chain table
		dt08		= 8, // image chain table
	dtPalIRGB	= 9,		// palette for the main 256 colors
		dt09	= 9,
	dtWordValue = 11,
	dtImageOffset = 12,
	dtPalette16	= 13,		// palette for 16 colors, indexed from the 256 colors palette
		dt0d		= 13,
		dt0e		= 14,
	dtWordValueExt	= 15,	// SPX: Normally unused, proceed as Word
};

#if !defined(SPELL_TYPE_POTION)
#define SPELL_TYPE_POTION	1
#define SPELL_TYPE_MISSILE	2
#define SPELL_TYPE_GENERAL	3
#define SPELL_TYPE_SUMMON	4
#endif


#define DIR_NORTH	0
#define DIR_EAST	1
#define DIR_SOUTH	2
#define DIR_WEST	3




// Playback 0x157C = 5500 Hz
#define PLAYBACK_FREQUENCY		5500
#define SKWIN_PLAYBACK_FREQUENCY	6000

// Number of possible champions
#define MAX_CHAMPIONS			4		// That can't be increased with no pain

// Time duration for showing CREDITS screen
#define SCREEN_CREDITS_TIMER	1800

// Maximum freeze value
#define EFFECT_FREEZE_MAX		200

// AI special attributes
#define AI_DEF_ARMOR_MAX		0xFF


// The standard 16 colors palette
#define COLOR_BLACK				0
#define COLOR_DARK_GRAY			1
#define COLOR_GRAY				2
#define COLOR_DARK_BROWN		3
#define COLOR_CYAN				4
#define COLOR_BROWN				5
#define COLOR_GREEN				6
#define COLOR_LIGHT_GREEN		7
#define COLOR_RED				8
#define COLOR_ORANGE			9
#define COLOR_FLESH				10
#define COLOR_YELLOW			11
#define COLOR_DARKEST_GRAY		12
#define COLOR_LIGHTER_GRAY		13
#define COLOR_BLUE				14
#define COLOR_WHITE				15

// Right panels
#define RIGHT_PANEL_UNDEFINED		0xFFFF
#define RIGHT_PANEL_SQUAD_HANDS		0
#define RIGHT_PANEL_HAND_ACTIONS	1
#define RIGHT_PANEL_SPELL			2
#define RIGHT_PANEL_MAGIC_MAP		3
#define RIGHT_PANEL_MONEY_BOX		4
#define RIGHT_PANEL_CONTAINER		5
#define RIGHT_PANEL_ATTACK_RESULT	6

// Attack special values
#define ATTACK_FAILURE_NOT_FRONT	-1
#define ATTACK_REQUIRES_HAND_ITEM	-2	// requires FLASK for potion or MISSILE for SHOOTER item
#define ATTACK_MEANINGLESS_SPELL	-3
#define ATTACK_FAILURE_WIZARD		-4
#define ATTACK_FAILURE_PRIEST		-5
#define ATTACK_FAILURE_X_TELEPORT	-6

// Champions attributes
#define ATTRIBUTE_LUCK			0
	#define ATTRIBUTE_FIRST			ATTRIBUTE_LUCK
#define ATTRIBUTE_STRENGTH		1
#define ATTRIBUTE_DEXTERITY		2
#define ATTRIBUTE_WISDOM		3
#define ATTRIBUTE_VITALITY		4
#define ATTRIBUTE_ANTI_FIRE		5
#define ATTRIBUTE_ANTI_MAGIC	6
	#define ATTRIBUTE_LAST		ATTRIBUTE_ANTI_MAGIC

#define ATTRIBUTE_COUNT			7

#define ATTRIBUTE_CURRENT		0
#define ATTRIBUTE_MAX			1

#define ATTRIBUTE_VALUE_THRESHOLD		170	// 0xAA

//------------------------------------------------------------------------------
//	CHAMPION SKILLS
//------------------------------------------------------------------------------
// Champions skills
#define	SKILL_FIGHTER_GLOBAL			0
#define	SKILL_NINJA_GLOBAL				1
#define	SKILL_PRIEST_GLOBAL				2
#define	SKILL_WIZARD_GLOBAL				3

#define SKILL_MAJOR_COUNT				4

#define SKILL_FIGHTER_SWING				4
#define SKILL_FIGHTER_THRUST			5
#define SKILL_FIGHTER_BLUNT				6
#define SKILL_FIGHTER_PARRY				7

#define SKILL_NINJA_STEAL				8
#define SKILL_NINJA_FIGHT				9
#define SKILL_NINJA_THROW				10
#define SKILL_NINJA_SHOOT				11

#define SKILL_PRIEST_IDENTIFY			12
#define SKILL_PRIEST_HEAL				13
#define SKILL_PRIEST_INFLUENCE			14
#define SKILL_PRIEST_DEFEND				15

#define SKILL_WIZARD_FIRE				16
#define SKILL_WIZARD_AIR				17
#define SKILL_WIZARD_EARTH				18
#define SKILL_WIZARD_WATER				19

#define SKILL_COUNT						20


// Skill levels	(xp base 512)
enum SkillLevel {
	SkLvlNone			=1,				//		   0
	SkLvlNeophyte		=2,				//	     512
	SkLvlNovice			=3,				//     1 024
	SkLvlApprentice		=4,				//     2 048
	SkLvlJourneyman		=5,				//     4 096
	SkLvlCraftsman		=6,				//     8 192
	SkLvlArtisan		=7,				//    16 384
	SkLvlAdept			=8,				//    32 768
	SkLvlExpert			=9,				//    65 536
	SkLvlMaster1		=10,			//   131 072
	SkLvlMasterLo		=10,
	SkLvlMaster2		=11,			//   262 144
	SkLvlMasterUm		=11,
	SkLvlMaster3		=12,			//   524 288
	SkLvlMasterOn		=12,
	SkLvlMaster4		=13,			// 1 048 576
	SkLvlMasterEe		=13,
	SkLvlMaster5		=14,			// 2 097 152
	SkLvlMasterPal		=14,
	SkLvlMaster6		=15,			// 4 194 304
	SkLvlMasterMon		=15,
	SkLvlArchmaster		=16,			// 8 388 608
};

#define SKILL_EXPERIENCE_BASE_LEVEL		512

//------------------------------------------------------------------------------
//	INVENTORY SLOT POSITIONS
//------------------------------------------------------------------------------

// Inventory

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

#define INVENTORY_HAND_RIGHT			0
#define INVENTORY_HAND_LEFT				1
	#define INVENTORY_HAND_LAST				1

#define INVENTORY_HEAD					2
#define INVENTORY_BODY					3	
#define INVENTORY_LEGS					4	
#define INVENTORY_FOOT					5
	#define INVENTORY_BODYPART_LAST			5	

#define INVENTORY_POUCH_2				6
#define INVENTORY_SCABBARD_2			7		
#define INVENTORY_SCABBARD_3			8
#define INVENTORY_SCABBARD_4			9
#define INVENTORY_NECK					10
#define INVENTORY_POUCH_1				11
#define INVENTORY_SCABBARD_1			12
#define INVENTORY_BACKPACK_1			13
#define INVENTORY_BACKPACK_17			29
	#define INVENTORY_BACKPACK_LAST		INVENTORY_BACKPACK_17		

#define INVENTORY_MAX_SLOT				30		// 0x1E

#define INVENTORY_OVER_1			30		// 0x1E
#define INVENTORY_OVER_2			31
#define INVENTORY_OVER_3			32
#define INVENTORY_OVER_4			33
#define INVENTORY_OVER_5			34
#define INVENTORY_OVER_6			35
#define INVENTORY_OVER_7			36
#define INVENTORY_OVER_8			37

	#define INVENTORY_MAX_OVER			38		// 0x26


#define CONTAINER_MAX_SLOT				8

//// This is supposition, not yet verified; could go along bodyFlag of Champion structure
#define WOUND_HEAD			0x0001
#define WOUND_NECK			0x0002
#define WOUND_BODY			0x0004
#define WOUND_LEFT_HAND		0x0008
#define WOUND_RIGHT_HAND	0x0010
#define WOUND_LEGS			0x0020
#define WOUND_FOOT			0x0040
////

// Enchantments types
#define ENCHANTMENT_FIRE_SHIELD			0
#define ENCHANTMENT_SPELL_SHIELD		1
#define ENCHANTMENT_PARTY_SHIELD		2

#define ENCHANTMENT_AURA_STRENGTH		3
#define ENCHANTMENT_AURA_DEXTERITY		4
#define ENCHANTMENT_AURA_WISDOM			5
#define ENCHANTMENT_AURA_VITALITY		6

#define ENCHANTMENT_AURA_FIRST			ENCHANTMENT_AURA_STRENGTH
#define ENCHANTMENT_AURA_LAST			ENCHANTMENT_AURA_VITALITY

// => These numbers are referencing party images in GDAT at 0x01/0x07.
// There is also some distinction of enchantment above 3 which are AURAs.

// Money
//#ifdef DM2_EXTENDED_MODE
//	#define MONEY_ITEM_MAX					0x100	// 256
//#else
	#define MONEY_ITEM_MAX					0x10	// 16
//#endif
#define MONEY_MAX_VALUE					65535


// Rain levels
#define RAIN_THRESHOLD_LEVEL_0			0x00
#define RAIN_THRESHOLD_LEVEL_1			0x10
#define RAIN_THRESHOLD_LEVEL_2			0x40
#define RAIN_THRESHOLD_LEVEL_3			0x80


//------------------------------------------------------------------------------
//	STANDARD SOUNDS CODES
//------------------------------------------------------------------------------

// General sounds
#define SOUND_DEFAULT_STORM				0x00

#define SOUND_CHAMPION_ATTACK			0x00
#define SOUND_CHAMPION_SHOOT			0x01

#define SOUND_ITEM_TAKE					0x60	// SPX: CUSTOM
#define SOUND_ITEM_PUT_DOWN				0x61	// SPX: CUSTOM

#define SOUND_STD_EXPLOSION				0x81	// Message cat GDAT2 V5
#define SOUND_CHAMPION_GETHIT			0x82
#define SOUND_CHAMPION_EAT_DRINK		0x83
#define SOUND_STD_DEFAULT				0x84	// Punch, Fall, Test wall, Gethit
#define SOUND_STD_KNOCK					0x85	// Falling item, punch knock
#define SOUND_STD_THROW					0x86	// Throw / Shoot item
#define SOUND_CHAMPION_SCREAM			0x87

#define SOUND_STD_ACTIVATION			0x88	// GDAT2 V5
#define SOUND_STD_TELEPORT				0x89	// GDAT2 V5

#define SOUND_STD_ACTIVATION_MESSAGE	0x00	// Sound tick in message category, only for PC9821
#define SOUND_STD_SPELL_MESSAGE			0x01	// Sound spell in message category, only for PC9821
#define SOUND_STD_TELEPORT_MESSAGE		0x02	// Sound teleporter in message category, only for PC9821

#define SOUND_CHAMPION_BUMP				0x8A
#define SOUND_MINION_TRANSFORMS			0x8B

#define SOUND_OBJECT_GETHIT				0x8D	// Works for creature and doors
#define SOUND_DOOR_STEP					0x8E
#define SOUND_DOOR_CLOSE				0x8F	// That one is not used by default in PC9821 DM2
#define SOUND_DOOR_OPENED				0x90	// SPX: CUSTOM This is custom and does not exist in retail DM2

#define SOUND_CHAMPION_FOOTSTEP			0x92	// SPX: CUSTOM This is custom and does not exist in retail DM2

#define SOUND_ACTIVATION_LOOP			0xA0	// SPX: CUSTOM, for animated ornate having a default loop sound

#define SOUND_NONE						0xFF

// Creature sounds
#define SOUND_CREATURE_MOVE				0x00
#define SOUND_CREATURE_TURN				0x01	// Minion
#define SOUND_CREATURE_GET_HIT			0x02	// Rocky, Guard-Archer (Shoot)
#define SOUND_CREATURE_REFLECTOR		0x03	// Dragoth
#define SOUND_CREATURE_JUMP				0x04	// Rocky
#define SOUND_CREATURE_SWING_STEP		0x05	// ?? Rocky ?? Put Down ? Land after jump ??
#define SOUND_CREATURE_XXX				0x06	// ?? Rocky, Dragoth (Hard Hit ?), Wolf
#define SOUND_CREATURE_ATTACK_1			0x07
#define SOUND_CREATURE_PICK_STEAL		0x08	// Thief,	Vexirk (Transform?), Magic Merchant (Put Item)
#define SOUND_CREATURE_GET_HIT_2		0x09	// Dragoth (Hit)
#define SOUND_CREATURE_ACTIVATE_TRIGGER	0x0A	// Giggler, Dragoth (Hit?)
#define SOUND_CREATURE_CONSIDER			0x0B	// Merchant
#define SOUND_CREATURE_ACCEPT			0x0C	// Merchant
#define SOUND_CREATURE_REFUSE			0x0D	// Merchant
#define SOUND_CREATURE_YELL				0x0E	// Thorn Demon,		Dragoth (Spawn Minion)
#define SOUND_CREATURE_GROWL			0x0F	// Vegmouth
#define SOUND_CREATURE_SPAWN			0x10	// Rocky, CaveIn, Minion, Mummy
#define SOUND_CREATURE_DEATH			0x11
#define SOUND_CREATURE_ATTACK_2			0x12	// Thorn Demon


//------------------------------------------------------------------------------
//	DATABASE / CATEGORIES
//------------------------------------------------------------------------------

// DB size
#define DB_SIZE_DOOR					0x04
#define DB_SIZE_TELEPORTER				0x06
#define DB_SIZE_TEXT					0x04
	#define DB_SIZE_SIMPLE_ACTUATOR			0x04
#define DB_SIZE_ACTUATOR				0x08
#define DB_SIZE_CREATURE				0x10
#define DB_SIZE_WEAPON					0x04
#define DB_SIZE_CLOTHING				0x04
#define DB_SIZE_SCROLL					0x04
#define DB_SIZE_POTION					0x04
#define DB_SIZE_CONTAINER				0x08
#define DB_SIZE_MISC_ITEM				0x04
#define DB_SIZE_UNUSED					0x00
#define DB_SIZE_MISSILE					0x08
#define DB_SIZE_CLOUD					0x04

#define DB_CATEGORY_DOOR				0x00
#define DB_CATEGORY_TELEPORTER			0x01
#define DB_CATEGORY_TEXT				0x02
#define DB_CATEGORY_SIMPLE_ACTUATOR		0x02
#define DB_CATEGORY_ACTUATOR			0x03
#define DB_CATEGORY_CREATURE			0x04
#define DB_CATEGORY_WEAPON				0x05
#define DB_CATEGORY_CLOTHING			0x06
#define DB_CATEGORY_SCROLL				0x07
#define DB_CATEGORY_POTION				0x08
#define DB_CATEGORY_CONTAINER			0x09
#define DB_CATEGORY_MISC_ITEM			0x0A
#define DB_CATEGORY_MISSILE				0x0E
#define DB_CATEGORY_CLOUD				0x0F


//------------------------------------------------------------------------------
//	RESOURCES CATEGORIES IN GRAPHICS.DAT
//------------------------------------------------------------------------------

#define GDAT_CATEGORY_TECHDATA				0x00
#define GDAT_CATEGORY_INTERFACE_GENERAL		0x01
#define GDAT_CATEGORY_MESSAGES				0x03
#define GDAT_CATEGORY_MUSICS				0x04
#define GDAT_CATEGORY_TITLE					0x05
#define GDAT_CATEGORY_CREDITS				0x06
#define GDAT_CATEGORY_INTERFACE_CHARSHEET	0x07
#define GDAT_CATEGORY_GRAPHICSSET			0x08
#define GDAT_CATEGORY_WALL_GFX				0x09
#define GDAT_CATEGORY_FLOOR_GFX				0x0A
#define GDAT_CATEGORY_DOOR_GFX				0x0B
#define GDAT_CATEGORY_DOOR_BUTTONS			0x0C
#define GDAT_CATEGORY_SPELL_MISSILES		0x0D
#define GDAT_CATEGORY_DOORS					0x0E
#define GDAT_CATEGORY_CREATURES				0x0F
#define GDAT_CATEGORY_WEAPONS				0x10
#define GDAT_CATEGORY_CLOTHES				0x11
#define GDAT_CATEGORY_SCROLLS				0x12
#define GDAT_CATEGORY_POTIONS				0x13
#define GDAT_CATEGORY_CONTAINERS			0x14
	#define GDAT_CATEGORY_MAGICAL_MAPS			0x14
#define GDAT_CATEGORY_MISCELLANEOUS			0x15
#define GDAT_CATEGORY_CHAMPIONS				0x16
#define GDAT_CATEGORY_ENVIRONMENT			0x17
#define GDAT_CATEGORY_TELEPORTERS			0x18
#define GDAT_CATEGORY_DIALOG_BOXES			0x1A
#define GDAT_CATEGORY_JAPANESE_FONT			0x1C

// These are new categories
#define GDAT_CATEGORY_SPELL_DEF				0x02
#define GDAT_CATEGORY_CREATURE_AI			0x19


//------------------------------------------------------------------------------
//	INTERFACE / MESSAGE / CHARACTERS
//------------------------------------------------------------------------------
// Images

#define GDAT_INTERFACE_CLASS_PORTRAIT				0x02
#define GDAT_INTERFACE_CLASS_ARROWS					0x03
#define GDAT_INTERFACE_CLASS_SQUAD					0x04
#define GDAT_INTERFACE_CLASS_SPELLING				0x05
	#define GDAT_IMG_NEED_FLASK						0x0B

#define GDAT_INTERFACE_CLASS_CHAMPION_FACING		0x06
#define GDAT_INTERFACE_CLASS_CHAMPION_AURAS			0x07


#define GDAT_IMG_CREDITS_TOMBSTONE					0x01


#define GDAT_INTERFACE_CHAR_FOOD_WATER_PANEL		0x01
#define GDAT_INTERFACE_FOOD_TEXT					0x06
#define GDAT_INTERFACE_WATER_TEXT					0x07
#define GDAT_INTERFACE_POISON_TEXT					0x08
#define GDAT_INTERFACE_PLAGUED_TEXT					0x09	// CUSTOM
#define GDAT_INTERFACE_BODY_HAND_LEFT				0x34
#define GDAT_INTERFACE_BODY_HAND_RIGHT				0x38


//------------------------------------------------------------------------------
//	GLOBAL IMAGE ATTRIBUTES
//------------------------------------------------------------------------------

#define GDAT_IMG_COLORKEY_1						0x04
#define GDAT_IMG_FLOOR_COLORKEY_2				0x11
#define GDAT_IMG_WALL_COLORKEY_2				0x11
#define GDAT_IMG_DOOR_COLORKEY_2				0x12

#define GDAT_IMG_MAP_CHIP						0xF9

//------------------------------------------------------------------------------
//	WALLSET / GRAPHICSSET ATTRIBUTES or GRAPHICS
//------------------------------------------------------------------------------
// Images
#define GDAT_GFXSET_FLOOR							0x00
#define GDAT_GFXSET_CEIL							0x01

#define GDAT_GFXSET_LETTERS							0x03

#define GDAT_GFXSET_DATA_05							0x05
#define GDAT_GFXSET_DOOR_FRAME_FRONT_D1				0x06

#define GDAT_GFXSET_STAIRS_RAMP_UP_S0_L				0x39
#define GDAT_GFXSET_STAIRS_RAMP_UP_S0_R				0x3A
#define GDAT_GFXSET_STAIRS_RAMP_DOWN_S0_L			0x4D
#define GDAT_GFXSET_STAIRS_RAMP_DOWN_S0_R			0x4E

#define GDAT_GFXSET_SEE_THRU						0xF1	// CUSTOM, for SEE THRU WALLS

#define GDAT_GFXSET_SQUAD_4X						0xF5
#define GDAT_GFXSET_SQUAD_SINGLE					0xF6

#define GDAT_GFXSET_DATA_FD							0xFD

// Attributes
#define GDAT_GFXSET_SCENE_COLORKEY					0x64
#define GDAT_GFXSET_SCENE_FLAGS						0x65	// Some graphicsset flags
	// 0x0001	: Has ceiling and can display ceiling pit (up)
	// 0x0002	: Use some random flip from	map coordinates		 (case 0x20)
	// 0x0004	: Use some random flip from game tick			 (case 0x20)
	// 0x0008	: Use some random flip from	map coordinates		 (case 0x01)
	// 0x0010	: Use some random flip from game tick			 (case 0x01)
	// 0x0020	: Outside +  flip from player's direction		 (case 0x20)
	// 0x0040	: Use some random flip from player's direction	 (case 0x01)
	// 0x0080	:
#define GDAT_GFXSET_SCENE_RAIN						0x66	// An index a rain status. In table static mode, 1 and 4 will make rain
#define GDAT_GFXSET_AMBIANT_LIGHT					0x67	// A value possibly between 0 and 100
#define GDAT_GFXSET_HIGHEST_LIGHT_LEVEL				0x68	// Maximum light level (between 0 and 5) (the lower the brighter)

#define GDAT_GFXSET_VOID_RANDOM_FALL				0x6A	// Used for void, falling in pit will make search a random target marker
													// 0x80.. : use animated floor
													// 0x..YY : floor id to use instead of normal ground
#define GDAT_GFXSET_ANIMATED_FLOOR					0x6B	// Used for void, animated blue floor clouds
#define GDAT_GFXSET_THUNDER_POSITION				0x6C	// Map position (XXYY) for Thunder (will be the same for all maps with that graphicsset)
#define GDAT_GFXSET_AMBIANT_DARKNESS				0x6D

#define GDAT_GFXSET_x72								0x72	// Unknown ?

//------------------------------------------------------------------------------
//	ENVIRONMENT / WEATHER ATTRIBUTES or GRAPHICS
//------------------------------------------------------------------------------

#define GDAT_ENVWTH_THUNDER_1					0x64
#define GDAT_ENVWTH_CLOUDSKY_1					0x67
#define GDAT_ENVWTH_WETGROUND_1					0x6A
#define GDAT_ENVWTH_RAINFALL_OBLIQUE_1			0x6D
#define GDAT_ENVWTH_RAINFALL_STRAIGHT_1			0x71

#define GFX_ENVIRONMENT_IMG_THUNDER			0x64	// 3
#define GFX_ENVIRONMENT_IMG_SKY_CLOUDS		0x67	// 3
#define GFX_ENVIRONMENT_IMG_WET_GROUND		0x6A	// 3
#define GFX_ENVIRONMENT_IMG_RAIN_SLANTED	0x6D	// 4
#define GFX_ENVIRONMENT_IMG_RAIN_STRAIGHT	0x71	// 4

//------------------------------------------------------------------------------
//	WALL ORNATE ATTRIBUTES or GRAPHICS
//------------------------------------------------------------------------------
// Special values
#define GDAT_WALL_ORNATE__CRYOCELL			0x5B

#define GDAT_WALL_ORNATE__POSITION			0x05
//	Here are the 25 possible positions; 13 is the V/H centered default position
// +--+--+--+--+--+
// |01|02|03|04|05|
// +--+--+--+--+--+
// |06|07|08|09|10|
// +--+--+--+--+--+
// |11|12|13|14|15|	-- centered position is either 0 or 13
// +--+--+--+--+--+
// |16|17|18|19|20|
// +--+--+--+--+--+
// |21|22|23|24|25|
// +--+--+--+--+--+

// THEN internally, values are shifted -1 so that they range from 0 to 24 instead.
#define ORNATE_POS__VFARLEFT_HFARTOP		0
#define ORNATE_POS__VLEFT_HFARTOP			1
#define ORNATE_POS__VCENTERED_HFARTOP		2
#define ORNATE_POS__VRIGHT_HFARTOP			3
#define ORNATE_POS__VFARRIGHT_HFARTOP		4

#define ORNATE_POS__VFARLEFT_HTOP			5
#define ORNATE_POS__VLEFT_HTOP				6
#define ORNATE_POS__VCENTERED_HTOP			7
#define ORNATE_POS__VRIGHT_HTOP				8
#define ORNATE_POS__VFARRIGHT_HTOP			9

#define ORNATE_POS__VFARLEFT_HCENTERED		10
#define ORNATE_POS__VLEFT_HCENTERED			11
#define	ORNATE_POS__VCENTERED_HCENTERED		12
#define	ORNATE_POS__VRIGHT_HCENTERED		13	
#define	ORNATE_POS__VFARRIGHT_HCENTERED		14

#define ORNATE_POS__VFARLEFT_HBOTTOM		15
#define ORNATE_POS__VLEFT_HBOTTOM			16
#define ORNATE_POS__VCENTERED_HBOTTOM		17
#define ORNATE_POS__VRIGHT_HBOTTOM			18
#define ORNATE_POS__VFARRIGHT_HBOTTOM		19

#define ORNATE_POS__VFARLEFT_HFARBOTTOM		20
#define ORNATE_POS__VLEFT_HFARBOTTOM		21
#define ORNATE_POS__VCENTERED_HFARBOTTOM	22
#define ORNATE_POS__VRIGHT_HFARBOTTOM		23
#define ORNATE_POS__VFARRIGHT_HFARBOTTOM	24

#define GDAT_WALL_ORNATE__DO_NOT_FLIP		0x07	// 1 = do not flip that graphics
#define GDAT_WALL_ORNATE__0A				0x0A	// 0 (default) = non alcove / 1 = Alcove / 2 = shop glass / 3 = Passive device (cryocell : show champion portrait)

	#define WALL_ORNATE_OBJECT__NONE			0x00
	#define WALL_ORNATE_OBJECT__ALCOVE			0x01
	#define WALL_ORNATE_OBJECT__SHOP_GLASS		0x02
	#define WALL_ORNATE_OBJECT__CRYOCELL		0x03


#define GDAT_WALL_ORNATE__IS_WATER_SPRING	0x0B
#define GDAT_WALL_ORNATE__IS_REBIRTH_ALTAR	0x0C	// Used for DM2 beta

#define GDAT_WALL_FLOOR_ORNATE__ANIMATION_SEQUENCE_DATA		0x0D
#define GDAT_WALL_FLOOR_ORNATE__ANIMATION_LENGTH			0x0D

#define GDAT_WALL_ORNATE__SWITCH_ITEM		0x0E


#define GDAT_WALL_ORNATE__IS_ITEM_TRIGGERED	0x0E
#define GDAT_WALL_ORNATE__OVERLAY			0x0F	// Seen as image, for panel shop overlay

#define GDAT_WALL_ORNATE__IS_LADDER_UP			0x11

#define GDAT_WALL_ORNATE__RESPAWN_COOLDOWN		0x12	// Used for GREEN and BLUE gems veins

#define GDAT_WALL_ORNATE__DATA_13			0x13

#define GDAT_WALL_ORNATE__WINDOW			0x63

#define GDAT_WALL_ORNATE__DATA_6E			0x6E	// used in DRAW_WALL
#define GDAT_WALL_ORNATE__DATA_6F			0x6F	// used in DRAW_WALL

#define GDAT_WALL_ORNATE__DATA_F0			0xF0
#define GDAT_WALL_ORNATE__DATA_F1			0xF1

#define GDAT_WALL_ORNATE__DATA_F2			0xF2
#define GDAT_WALL_ORNATE__DATA_FD			0xFD

#define GDAT_WALL_IMAGE__VIEW_SIDE_LEFT			0x00
#define GDAT_WALL_IMAGE__VIEW_FRONT				0x01
#define GDAT_WALL_IMAGE__VIEW_SIDE_RIGHT		0x02

//#define GDAT_ORNATE_POSITION_TOP_RIGHT			8
//#define GDAT_ORNATE_POSITION_CENTER				13
//#define GDAT_ORNATE_POSITION_CENTER_RIGHT		18

#define GDAT_FLOOR_ORNATE__POSITION			0x05
#define GDAT_FLOOR_ORNATE__DO_NOT_FLIP		0x07	// 1 = do not flip that graphics
#define GDAT_FLOOR_ORNATE__X63				0x63

//------------------------------------------------------------------------------
//	DOOR STATS & ATTRIBUTES
//------------------------------------------------------------------------------
#define GDAT_DOOR_DECORATION_MASK			0x00	// 0 is standard image anyway
#define GDAT_DOOR_MIRRORED					0x20	// CUSTOM, for MIRRORED animation (like Force Field door)
#define GDAT_DOOR_NO_FRAMES					0x40	// When = 0, draw door frames, else, don't!
#define GDAT_DOOR_DESTROYED_MASK			0x41
#define GDAT_DOOR_SEE_THRU_MASK				0x42	// CUSTOM, for SEE THRU WALLS

#define GDAT_DOOR_DESTROYED_MASK_POSITION	0x0A	// 0 = upper, 1 = centered, 2 = low/right -- should always be centered
#define GDAT_DOOR_DEFENSE					0x0E
#define GDAT_DOOR_STRENGTH					0x0F
#define GDAT_DOOR_X10						0x10	// some spell bypass ?

#define GDAT_DOOR_ORNATE__POSITION			0x08	// 0 = upper, 1 = centered, 2 = low/right

#define _DOOR_STATE__OPENED_				0x00
#define _DOOR_STATE__ONE_QUARTER_CLOSED_	0x01
#define _DOOR_STATE__MID_CLOSED_			0x02
#define _DOOR_STATE__THREE_QUARTERS_CLOSED_	0x03
#define _DOOR_STATE__CLOSED_				0x04
#define _DOOR_STATE__DESTROYED_				0x05	// ingame, doorstate = 5 => door is destroyed

//------------------------------------------------------------------------------
//	CREATURES / OBJECTS ID
//------------------------------------------------------------------------------
// Creatures used for specific code
#define CREATURE_GOOD_SCOUT_MINION		0x30
#define CREATURE_GOOD_ATTACK_MINION		0x31
#define CREATURE_GOOD_CARRY_MINION		0x32
#define CREATURE_GOOD_FETCH_MINION		0x33
#define CREATURE_GOOD_GUARD_MINION		0x34
#define CREATURE_GOOD_U_HAUL_MINION		0x35
#define CREATURE_DRAGOTH_ATTACK_MINION	0x4C

#define CREATURE_STAT_00				0x00
#define CREATURE_STAT_01				0x01
#define CREATURE_STAT_AI				0x05
#define CREATURE_STAT_6					0x06	// Seeing this used by SARDES Minion; would it be that creature does not attack ???
#define CREATURE_STAT_DROP_FIRST		0x0A
#define CREATURE_STAT_DROP_LAST			0x14


#define GDAT_CREATURE_ANIM_ATTRIBUTION		0xFB	// ATTR : Animation Attribution
#define GDAT_CREATURE_ANIM_INFO_SEQUENCE	0xFC	// INFO : Info Sequence
#define GDAT_CREATURE_ANIM_FRAME_SEQUENCE	0xFD	// FRAM : Frame Sequence
#define GDAT_CREATURE_ANIM_SCALE_GROUPS		0xFE	// SCAL : Scale Groups


#define	CREATURE_AI_FLAG_0001			0x0001	// 0x0001
#define	CREATURE_AI_FLAG_8000			0x8000	// 0x8000

// Stats slots within GDAT for AI values (custom)
#define GDAT_AI_STAT_ARMOR_CLASS		0x02
#define GDAT_AI_STAT_HIT_POINTS			0x04
#define GDAT_AI_STAT_ATTACK_STRENGTH	0x06
#define GDAT_AI_STAT_ATTACK_POISON		0x07
#define GDAT_AI_STAT_DEFENSE			0x08
#define GDAT_AI_STAT_FLAGS_B9			0x09

	#define AI_ATTACK_FLAGS__MELEE			0x0001
	#define AI_ATTACK_FLAGS__PUSH_BACK		0x0002
	#define AI_ATTACK_FLAGS__STEAL			0x0004
	#define AI_ATTACK_FLAGS__SHOOT			0x0008
	#define AI_ATTACK_FLAGS__FIREBALL		0x0010
	#define AI_ATTACK_FLAGS__DISPELL		0x0020
	#define AI_ATTACK_FLAGS__LIGHTNING		0x0040
	#define AI_ATTACK_FLAGS__POISON_CLOUD	0x0080
	#define AI_ATTACK_FLAGS__POISON_BOLT	0x0100
	#define AI_ATTACK_FLAGS__POISON_BLOB	0x0200
	#define AI_ATTACK_FLAGS__PUSH_SPELL		0x0400
	#define AI_ATTACK_FLAGS__PULL_SPELL		0x0800

//#define GDAT_AI_STAT_W30
	#define	AI_W30_FLAGS__TURN_MISSILE		0x0800

// Note: I allow these values to be found under the Creature category stored as dtWordValue0E to make no conflict with standard Words (0x0B)
#define GDAT_AI_PLAGUE_DAMAGE			0x66	// SPX: Custom

#define CREATURE_GENERATED_DROPS		0	// 0 = do generated drop
#define CREATURE_DROP_POSSESSION_ONLY	1	// 1 = no generated drop, only drop possession
#define	CREATURE_NO_DROP				2	// 2 = no creature drop (not even possession)

//------------------------------------------------------------------------------
//	ITEMS STATS & ATTRIBUTES
//------------------------------------------------------------------------------
// Index of item selection within GDAT2 categories

#define GDAT_ITEM_STATS_GEN_FLAGS			0x00
		#define ITEM_FLAG_DRINKABLE			0x0001
		#define ITEM_FLAG_02				0x0002
		#define ITEM_FLAG_04				0x0004
		#define ITEM_FLAG_08				0x0008
		#define ITEM_FLAG_PRODUCE_LIGHT		0x0010
		#define ITEM_FLAG_20				0x0020
		#define ITEM_FLAG_40				0x0040	// can be absorbed (by creature)
		#define ITEM_FLAG_80				0x0080
		#define ITEM_FLAG_USED				0x1000
		#define ITEM_FLAG_2000				0x2000	// may be : do not decrease charge count ??
		#define ITEM_FLAG_CURRENCY			0x4000


#define GDAT_ITEM_STATS_WEIGHT				0x01
#define GDAT_ITEM_STATS_MONEY_VALUE			0x02
#define GDAT_ITEM_STATS_FOOD_VALUE			0x03
#define GDAT_ITEM_STATS_EQUIP_FLAGS			0x04
	#define ITEM_EQUIP_FLAG_HELM			0x0001
	#define ITEM_EQUIP_FLAG_NECKLACE			0x0002
	#define ITEM_EQUIP_FLAG_BODY			0x0004
	#define ITEM_EQUIP_FLAG_PANTS			0x0008
	#define ITEM_EQUIP_FLAG_BOOTS			0x0010
	#define ITEM_EQUIP_FLAG_POUCH			0x0020
	#define ITEM_EQUIP_FLAG_SCABBARD			0x0040
	#define ITEM_EQUIP_FLAG_SCABBARD_ONE			0x0080
	#define ITEM_EQUIP_FLAG_CONTAINER			0x0100
	#define ITEM_EQUIP_FLAG_0200			0x0200
	#define ITEM_EQUIP_FLAG_0400			0x0400
	#define ITEM_EQUIP_FLAG_0800			0x0800
	#define ITEM_EQUIP_FLAG_CONTAINER_2			0x8000

#define GDAT_ITEM_WEAPON_PROJECTILE_FLAG	0x05
// Projectile item flags (0x05)
	#define WEAPON_FLAGS_SHOOTER			0x8000

#define GDAT_ITEM_STATS_ANIM_FLAGS			0x06
#define GDAT_ITEM_WEAPON_MELEE_STRENGTH		0x08
#define GDAT_ITEM_WEAPON_THROW_STRENGTH		0x09
#define GDAT_ITEM_WEAPON_ACCURACY			0x0A
#define GDAT_ITEM_STATS_ARMOR_CLASS			0x0B
#define GDAT_ITEM_STATS_0x0C				0x0C	// ARROW / SLAYER / ROCK => SHOOT COOLDOWN ?
#define GDAT_ITEM_STATS_POISONOUS			0x0D	// MISSILE STRENGTH or POISON ?

#define GDAT_ITEM_BONUS_0x13				0x13	// MUSHROOM / ILLUMULET
#define GDAT_ITEM_BONUS_MANA				0x14

#define GDAT_ITEM_BONUS_LUCK				0x15	// METEOR METAL / RUNE CHARM / MITHRAL MAIL HUKE
	#define GDAT_ITEM_BONUS_ATTRIBUTE_FIRST		GDAT_ITEM_BONUS_LUCK
#define GDAT_ITEM_BONUS_STRENGTH			0x16
#define GDAT_ITEM_BONUS_DEXTERITY			0x17
#define GDAT_ITEM_BONUS_WISDOM				0x18
#define GDAT_ITEM_BONUS_VITALITY			0x19
#define GDAT_ITEM_BONUS_ANTI_MAGIC			0x1A
#define GDAT_ITEM_BONUS_ANTI_FIRE			0x1B

#define GDAT_ITEM_BONUS_FIGHTER				0x1E	// FIRE PLATE / COMBAT STAFF
	#define GDAT_ITEM_BONUS_SKILL_FIRST				GDAT_ITEM_BONUS_FIGHTER
#define GDAT_ITEM_BONUS_NINJA				0x1F
#define GDAT_ITEM_BONUS_PRIEST				0x20	// SAR SHIELD
#define GDAT_ITEM_BONUS_WIZARD				0x21

#define GDAT_ITEM_BONUS_FGT_SWING			0x22	// BLUE STEELE / VORAX
#define GDAT_ITEM_BONUS_FGT_THRUST			0x23	// EXCSYMYR
#define GDAT_ITEM_BONUS_FGT_BLUNT			0x24	// TECHMACE / WAR CLUB
#define GDAT_ITEM_BONUS_FGT_PARRY			0x25

#define GDAT_ITEM_BONUS_NJA_STEAL			0x26	// BRIGANDINE
#define GDAT_ITEM_BONUS_NJA_FIGHT			0x27	// DAGGER
#define GDAT_ITEM_BONUS_NJA_THROW			0x28
#define GDAT_ITEM_BONUS_NJA_SHOOT			0x29

#define GDAT_ITEM_BONUS_PRS_IDENTIFY		0x2A
#define GDAT_ITEM_BONUS_PRS_HEAL			0x2B	// BANDANA
#define GDAT_ITEM_BONUS_PRS_INFLUENCE		0x2C	// BODICE / GREAT HELM / STAFF OF NETA
#define GDAT_ITEM_BONUS_PRS_DEFEND			0x2D

#define GDAT_ITEM_BONUS_WIZ_FIRE			0x2E	// FINE ROBE (BOTTOM) / NUMENSTAFF
#define GDAT_ITEM_BONUS_WIZ_AIR				0x2F	// FINE ROBE (BOTTOM) / BAINBLOOM
#define GDAT_ITEM_BONUS_WIZ_EARTH			0x30	// FINE ROBE (TOP) / SERPENT STAFF
#define GDAT_ITEM_BONUS_WIZ_WATER			0x31	// FINE ROBE (TOP) / EMERALD ORB

#define GDAT_ITEM_BONUS_LIGHT				0x32
#define GDAT_ITEM_BONUS_WALK_SPEED			0x33	// MITHRAL HOSEN : finally acts like BOOTS OF SPEED would in DM?

#define GDAT_ITEM_STATS_MAX_CHARGES			0x34
#define GDAT_ITEM_BONUS_MONEY_PER_CHARGE	0x35

//--- Above 0x40 are NEW custom defined values
#define GDAT_ITEM_STATS_WATER_VALUE			0x43	// SPX: Custom, for flask or misc (drinkable)
#define GDAT_ITEM_STATS_MISSILE_SPELL		0x4D	// SPX: Custom, for potions (bomb)
#define GDAT_ITEM_MALUS_CURSED				0x55	// SPX: Custom, opposite of BONUS_LUCK


#define ITEM_MISC_LOAD_MAX					0x80
#define ITEM_LOAD_MAX_EXTEND_1				0xFD	// because 0xFE is used for default values

#define GDAT_ITEM_DEFAULT_INDEX				0xFE

//------------------------------------------------------------------------------
//	POTION STATS (specific)
//------------------------------------------------------------------------------

#define  GDAT_POTION_STAT_BEHAVIOUR			0x05	// SPX: Custom


//------------------------------------------------------------------------------
//	CONTAINER STATS (specific)
//------------------------------------------------------------------------------

#define	GDAT_CONTAINER_STAT_10			0x10
#define	GDAT_CONTAINER_STAT_12			0x12

//------------------------------------------------------------------------------
//	TIMER
//------------------------------------------------------------------------------

// Timers -- other defined in DME.h as enum
#define TIMER_NONE						0xFFFF

#if DM2_EXTENDED_MODE == 1
	#define MAX_TIMER_NUMBER					10 //15000
#else
	#define MAX_TIMER_NUMBER					50
#endif

#define TIMER_ACTOR__00			0
#define TIMER_ACTOR__01			1
#define TIMER_ACTOR__02			2
#define TIMER_ACTOR__03			3

#define ACTMSG_OPEN_SET			0
#define ACTMSG_CLOSE_CLEAR		1
#define ACTMSG_TOGGLE			2

#define ACTEFFECT_STEP_ON__OPEN_SET		0
#define ACTEFFECT_STEP_ON__CLOSE_CLEAR	1
#define ACTEFFECT_STEP_ON__TOGGLE		2
#define ACTEFFECT_STEP_CONSTANT__OPEN	3	// STEP ON => OPEN / STEP OUT => CLOSE

#define ACTEFFECT_STEP_CLOSE__OPEN_SET		4
#define ACTEFFECT_STEP_CLOSE__CLOSE_CLEAR	5
#define ACTEFFECT_STEP_CLOSE__TOGGLE		6
#define ACTEFFECT_STEP_CONSTANT__CLOSE		7	// STEP OUT => OPEN / STEP ON => CLOSE


//------------------------------------------------------------------------------
//	TIMER SPECIAL
//------------------------------------------------------------------------------

#define VI_ALTAR_STATUS__EXPLODE			2
#define VI_ALTAR_STATUS__REMOVE_BONE		1
#define VI_ALTAR_STATUS__REVIVE_CHAMPION	0

//------------------------------------------------------------------------------
//	USER INTERFACE EVENT CODES
//------------------------------------------------------------------------------

// UI Event code
#define UI_EVENTCODE_NONE		0x00

#define UI_EVENTCODE_TURN_LEFT	0x01
#define UI_EVENTCODE_TURN_RIGHT	0x02

#define UI_EVENTCODE_MOVE_FORWARD	0x03
#define UI_EVENTCODE_MOVE_RIGHT	0x04
#define UI_EVENTCODE_MOVE_BACK	0x05
#define UI_EVENTCODE_MOVE_LEFT	0x06
	#define UI_EVENTCODE_MOVE_FIRST	UI_EVENTCODE_MOVE_FORWARD
	#define UI_EVENTCODE_MOVE_LAST	UI_EVENTCODE_MOVE_LEFT

#define UI_EVENTCODE_ARROW_DIR_FIRST	UI_EVENTCODE_TURN_LEFT
#define UI_EVENTCODE_ARROW_DIR_LAST		UI_EVENTCODE_MOVE_LEFT


#define UI_EVENTCODE_VIEW_CHAMPION_1	0x07
#define UI_EVENTCODE_VIEW_CHAMPION_2	0x08
#define UI_EVENTCODE_VIEW_CHAMPION_3	0x09
#define UI_EVENTCODE_VIEW_CHAMPION_4	0x0A
#define UI_EVENTCODE_RETURN_VIEWPORT	0x0B

#define UI_EVENTCODE_010	0x10
#define UI_EVENTCODE_011	0x11
#define UI_EVENTCODE_012	0x12
#define UI_EVENTCODE_013	0x13


#define UI_EVENTCODE_ITEM_SLOT_FIRST			0x14

#define UI_EVENTCODE_CHAMPION_1_HAND_RIGHT		0x14
#define UI_EVENTCODE_CHAMPION_1_HAND_LEFT		0x15
#define UI_EVENTCODE_CHAMPION_2_HAND_RIGHT		0x16
#define UI_EVENTCODE_CHAMPION_2_HAND_LEFT		0x17
#define UI_EVENTCODE_CHAMPION_3_HAND_RIGHT		0x18
#define UI_EVENTCODE_CHAMPION_3_HAND_LEFT		0x19
#define UI_EVENTCODE_CHAMPION_4_HAND_RIGHT		0x1A
#define UI_EVENTCODE_CHAMPION_4_HAND_LEFT		0x1B

#define UI_EVENTCODE_INVENTORY_HAND_RIGHT		0x1C
#define UI_EVENTCODE_INVENTORY_HAND_LEFT		0x1D

#define UI_EVENTCODE_INVENTORY_HEAD		0x1E
#define UI_EVENTCODE_INVENTORY_BODY		0x1F
#define UI_EVENTCODE_INVENTORY_LEGS		0x20
#define UI_EVENTCODE_INVENTORY_FOOT		0x21

#define UI_EVENTCODE_INVENTORY_POUCH_2		0x22

#define UI_EVENTCODE_INVENTORY_SCABBARD_2	0x23
#define UI_EVENTCODE_INVENTORY_SCABBARD_3	0x24
#define UI_EVENTCODE_INVENTORY_SCABBARD_4	0x25

#define UI_EVENTCODE_INVENTORY_NECK			0x26
#define UI_EVENTCODE_INVENTORY_POUCH_1		0x27
#define UI_EVENTCODE_INVENTORY_SCABBARD_1	0x28

#define UI_EVENTCODE_BACKPACK_01	0x29
#define UI_EVENTCODE_BACKPACK_FIRST	UI_EVENTCODE_BACKPACK_01
#define UI_EVENTCODE_BACKPACK_02	0x2A
#define UI_EVENTCODE_BACKPACK_03	0x2B
#define UI_EVENTCODE_BACKPACK_04	0x2C
#define UI_EVENTCODE_BACKPACK_05	0x2D
#define UI_EVENTCODE_BACKPACK_06	0x2E
#define UI_EVENTCODE_BACKPACK_07	0x2F
#define UI_EVENTCODE_BACKPACK_08	0x30
#define UI_EVENTCODE_BACKPACK_09	0x31
#define UI_EVENTCODE_BACKPACK_10	0x32
#define UI_EVENTCODE_BACKPACK_11	0x33
#define UI_EVENTCODE_BACKPACK_12	0x34
#define UI_EVENTCODE_BACKPACK_13	0x35
#define UI_EVENTCODE_BACKPACK_14	0x36
#define UI_EVENTCODE_BACKPACK_15	0x37
#define UI_EVENTCODE_BACKPACK_16	0x38
#define UI_EVENTCODE_BACKPACK_17	0x39
#define UI_EVENTCODE_BACKPACK_LAST	UI_EVENTCODE_BACKPACK_17

#define UI_EVENTCODE_CONTAINER_1	0x3A
#define UI_EVENTCODE_CONTAINER_2	0x3B
#define UI_EVENTCODE_CONTAINER_3	0x3C
#define UI_EVENTCODE_CONTAINER_4	0x3D
#define UI_EVENTCODE_CONTAINER_5	0x3E
#define UI_EVENTCODE_CONTAINER_6	0x3F
#define UI_EVENTCODE_CONTAINER_7	0x40
#define UI_EVENTCODE_CONTAINER_8	0x41

#define UI_EVENTCODE_042	0x42

#define UI_EVENTCODE_ITEM_SLOT_LAST	0x42


#define UI_EVENTCODE_MOUTH	0x46
#define UI_EVENTCODE_EYE	0x47

#define UI_EVENTCODE_CLICK_VIEWPORT	0x50

#define UI_EVENTCODE_VIEW_LEADER	0x52

#define UI_EVENTCODE_CHAMPION_TURN_LEFT		0x5D
#define UI_EVENTCODE_CHAMPION_TURN_RIGHT	0x5E

#define UI_EVENTCODE_SPELL_OR_LEADER_1	0x5F	// For choosing leader and invoking spell panel / TOP LEFT
#define UI_EVENTCODE_SPELL_OR_LEADER_2	0x60	// TOP RIGHT
#define UI_EVENTCODE_SPELL_OR_LEADER_3	0x61	// BOTTOM RIGHT
#define UI_EVENTCODE_SPELL_OR_LEADER_4	0x62	// BOTTOM LEFT

#define UI_EVENTCODE_ADD_RUNE_1	0x65
#define UI_EVENTCODE_ADD_RUNE_2	0x66
#define UI_EVENTCODE_ADD_RUNE_3	0x67
#define UI_EVENTCODE_ADD_RUNE_4	0x68
#define UI_EVENTCODE_ADD_RUNE_5	0x69
#define UI_EVENTCODE_ADD_RUNE_6	0x6A

#define UI_EVENTCODE_REMOVE_RUNE	0x6B
#define UI_EVENTCODE_VALIDATE_SPELL	0x6C


#define UI_EVENTCODE_HAND_RUNE_QUIT	0x70	// Cancel action for HAND or Leave rune panel (do not destroy spell)

#define UI_EVENTCODE_HAND_ACTION_1	0x71
#define UI_EVENTCODE_HAND_ACTION_2	0x72
#define UI_EVENTCODE_HAND_ACTION_3	0x73


#define UI_EVENTCODE_CHAMPION1_ACTION_HAND_RIGHT	0x74
#define UI_EVENTCODE_CHAMPION1_ACTION_HAND_LEFT		0x75
#define UI_EVENTCODE_CHAMPION2_ACTION_HAND_RIGHT	0x76
#define UI_EVENTCODE_CHAMPION2_ACTION_HAND_LEFT		0x77
#define UI_EVENTCODE_CHAMPION3_ACTION_HAND_RIGHT	0x78
#define UI_EVENTCODE_CHAMPION3_ACTION_HAND_LEFT		0x79
#define UI_EVENTCODE_CHAMPION4_ACTION_HAND_RIGHT	0x7A
#define UI_EVENTCODE_CHAMPION4_ACTION_HAND_LEFT		0x7B

#define UI_EVENTCODE_TAKE_CHAMPION_TOP_LEFT		0x7D
#define UI_EVENTCODE_TAKE_CHAMPION_TOP_RIGHT	0x7E
#define UI_EVENTCODE_TAKE_CHAMPION_BOTTOM_RIGHT	0x7F
#define UI_EVENTCODE_TAKE_CHAMPION_BOTTOM_LEFT	0x80
#define UI_EVENTCODE_UPDATE_CHAMPION_POSITION	0x81

#define UI_EVENTCODE_DISK_OP	0x8C

#define UI_EVENTCODE_SLEEP			0x91	// Put into sleep
#define UI_EVENTCODE_WAKE			0x92	// Wake from sleep
#define UI_EVENTCODE_PAUSE			0x93	// Invoke game pause
#define UI_EVENTCODE_END_PAUSE		0x94	// Resume from pause

#define UI_EVENTCODE_MAGIC_MAP	0x96

#define UI_EVENTCODE_MAGIC_MAP_RUNE_1	0x97	// Rune 1 : KU for creatures
#define UI_EVENTCODE_MAGIC_MAP_RUNE_2	0x98	// Rune 2 : ROS for illusionary walls
#define UI_EVENTCODE_MAGIC_MAP_RUNE_3	0x99	// Rune 3 : DAIN for spells
#define UI_EVENTCODE_MAGIC_MAP_RUNE_4	0x9A	// Rune 4 : NETA for ... what ?


#define UI_EVENTCODE_REVIVE_CHAMPION	0xA0	// (160)
#define UI_EVENTCODE_EXIT_CRYOCELL		0xA1	// (161) Exit champion-to-be-revived inventory 

#define UI_EVENTCODE_0A5				0xA5
#define UI_EVENTCODE_0C6				0xC6
#define UI_EVENTCODE_0D6				0xD6

#define UI_EVENTCODE_START_NEW_GAME	0xD7

#define UI_EVENTCODE_0D8	0xD8

#define UI_EVENTCODE_RESUME_GAME	0xD9	// Resume from main menu

#define UI_EVENTCODE_SHOW_CREDITS	0xDA

#define UI_EVENTCODE_DIALOG_BUTTON_1	0xDB	// OK, CANCEL Buttons ... depends on DIALOG structure
#define UI_EVENTCODE_DIALOG_BUTTON_2	0xDC	// 
#define UI_EVENTCODE_DIALOG_BUTTON_3	0xDD	//
#define UI_EVENTCODE_DIALOG_BUTTON_4	0xDE	// NOT VERIFIED

#define UI_EVENTCODE_0DF	0xDF

#define UI_EVENTCODE_QUIT_GAME	0xE0

#define UI_EVENTCODE_MOVE_OBJECT	0xE1	// Like move a table around ..

#define UI_EVENTCODE_RELEASE_MOUSE_BUTTON	0xE3

#define UI_EVENTCODE_MONEYBOX_1	0xE4
#define UI_EVENTCODE_MONEYBOX_2	0xE5
#define UI_EVENTCODE_MONEYBOX_3	0xE6
#define UI_EVENTCODE_MONEYBOX_4	0xE7
#define UI_EVENTCODE_MONEYBOX_5	0xE8
#define UI_EVENTCODE_MONEYBOX_6	0xE9


#define UI_EVENTCODE_CLICK_STATS_BAR_1	0xEA	// Clicking on stats bar with item will place it in inventory!
#define UI_EVENTCODE_CLICK_STATS_BAR_2	0xEB
#define UI_EVENTCODE_CLICK_STATS_BAR_3	0xEC
#define UI_EVENTCODE_CLICK_STATS_BAR_4	0xED

#define UI_EVENTCODE_QUIT_CREDITS	0xEF

#define UI_EVENTCODE_LAST_EVENT		0xEF

//------------------------------------------------------------------------------
//	ENGINE GLOBAL VARIABLES
//------------------------------------------------------------------------------

// Global Variables
#define GLOB_BYTE_00				0x40
#define GLOB_BYTE_02				0x42	// Used in attack failure ..
#define GLOB_BYTE_07				0x47	// Used for X teleporter (mark last teleporter source?)
#define GLOB_BYTE_3F				0x7F
#define GLOB_WORD_00				0x80
#define GLOB_WORD_3F				0xBF


//------------------------------------------------------------------------------
// Variables for display functions

// DRAW_DUNGEON_GRAPHICS
#define DRAW_FLAG_FLIP_NONE						0x0000
#define DRAW_FLAG_FLIP_1						0x0001	// One should be vertical and the other horizontal ..
#define DRAW_FLAG_FLIP_2						0x0002

// MAP GRAPHICS FLAGS
#define MAPGFX_FLAG__PIT_UPPER_ROOF				0x0001
#define MAPGFX_FLAG__PIT_LOWER_GROUND			0x0002
#define MAPGFX_FLAG__X3							0x0004
#define MAPGFX_FLAG__STAIRS_GOING_UP			0x0008
#define MAPGFX_FLAG__STAIRS_GOING_DOWN			0x0010
#define MAPGFX_FLAG__TELEPORTER					0x0020
#define MAPGFX_FLAG__X6							0x0040
#define MAPGFX_FLAG__DOOR_0						0x0080
#define MAPGFX_FLAG__DOOR_1						0x0100

//------------------------------------------------------------------------------
// Champions flags

// position changed	=> 1000 ?
// weapon attack
// portrait changed

#define	CHAMPION_FLAG_0010			0x0010	// 0x0010
#define	CHAMPION_FLAG_0400			0x0400	// 0x0400
#define	CHAMPION_FLAG_0800			0x0800	// 0x0800
#define	CHAMPION_FLAG_1000			0x1000	// 0x1000
#define CHAMPION_FLAG_1400			0x1400	// 0x1400
#define	CHAMPION_FLAG_2000			0x2000	// 0x2000
#define	CHAMPION_FLAG_3000			0x3000	// 0x3000
#define	CHAMPION_FLAG_3800			0x3800	// 0x3800
#define	CHAMPION_FLAG_4000			0x4000	// 0x4000
#define	CHAMPION_FLAG_8000			0x8000	// 0x8000
#define CHAMPION_FLAG_FC00			0xfc00	// 0xfc00

//------------------------------------------------------------------------------
//	CONSTANT VALUE OR TYPES FOR FUNCTIONS
//------------------------------------------------------------------------------

// SEARCH_DUNGEON_FOR_SPECIAL_MARKER : Command for function
#define SDFSM_CMD_FIND_MAP_OFFVOID	0x02
#define SDFSM_CMD_COUNT_MARKERS		0x03
#define SDFSM_CMD_X_TELEPORTER		0x04
#define SDFSM_CMD_X_ANCHOR			0x05


//------------------------------------------------------------------------------
//	SYSTEM ERRORS
//------------------------------------------------------------------------------

#define SYSTEM_ERROR__INVALID_ENT1					0x1E	// 30: Invalid header in graphics.dat (LOAD_ENT1)
#define SYSTEM_ERROR__MISSING_GRAPHICS_2			0x1F	// 31: No graphics2.dat
#define SYSTEM_ERROR__CORRUPTED_GRAPHICS_2			0x20	// 32: Corrupted graphics2.dat

#define SYSTEM_ERROR__OUT_OF_EMS_MEM				0x27	// 39: Out of EMS memory
#define SYSTEM_ERROR__OUT_OF_MAIN_MEM				0x28	// 40: Out of main memory

#define SYSTEM_ERROR__MISSING_GRAPHICS				0x29	// 41: No graphics.dat
#define SYSTEM_ERROR__INVALID_GRAPHICS_STRUCTURE	0x2A	// 42: Pb in READ_GRAPHICS_STRUCTURE
#define SYSTEM_ERROR__FREE_CPXHEAP_SIZE				0x2B	// 43: Pb in GUARANTEE_FREE_CPXHEAP_SIZE
#define SYSTEM_ERROR__LOAD_DYN4						0x2C	// 44: Pb in LOAD_DYN4
#define SYSTEM_ERROR__TIMER_MAX_REACHED				0x2D	// 45: Too many timer entries to hold one more (TIMER)
#define SYSTEM_ERROR__CORRUPTED_GRAPHICS			0x2E	// 46: Corrupted graphics.dat

#define SYSTEM_ERROR__CANT_ADD_MEM					0x31	// 49: Pb in ADD_MEM_TO_FREE_POOL

#define SYSTEM_ERROR__MISSING_DUNGEON				0x32	// 50: No dungeon.dat

#define SYSTEM_ERROR__TIMER_BEYOND_MAX				0x46	// 70: Invalid status on game timer (TIMER)
#define SYSTEM_ERROR__CREATURE_ASSOC_FULL			0x47	// 71: No more creature assoc record availability?
#define SYSTEM_ERROR__CANT_APPEND_RECORD			0x48	// 72: Pb in APPEND_RECORD_TO

#define SYSTEM_ERROR__NO_PLAYER_DATA				1001	// 1001: No player data in graphics.dat (kkdf2)

//------------------------------------------------------------------------------
//	ACTUATOR TYPES
//------------------------------------------------------------------------------

#define ACTUATOR_TYPE_X01								0x01	// 01
#define ACTUATOR_TYPE_X02								0x02	// 02

#define ACTUATOR_TYPE_DM1_WALL_SWITCH					0x01	// 01	was unimplemented in DM2, almost similar to 24 (wall switch)

#define ACTUATOR_TYPE_ITEM_WATCHER						0x03	// 03
#define ACTUATOR_TYPE_DM1_ITEM_EATER					0x04	// 04	was unimplemented in DM2, quite similar to item watcher x03

#define ACTUATOR_TYPE_DM1_BITFIELDS_TRIGGER				0x05	// 05	was unimplemented in DM2
#define ACTUATOR_TYPE_DM1_COUNTER						0x06	// 06	was unimplemented in DM2, different from actuator counter x1D

#define ACTUATOR_TYPE_SOME_SHOOTER						0x07	// 07	was unimplemented in DM2 ?
#define ACTUATOR_TYPE_MISSILE_SHOOTER					0x08	// 08
#define ACTUATOR_TYPE_WEAPON_SHOOTER					0x09	// 09	was unimplemented in DM2 ?
#define ACTUATOR_TYPE_MISSILE_SHOOTER_2					0x0A	// 10
//#define ACTUATOR_TYPE_ITEM_SHOOTER						0x0C	// 12
#define ACTUATOR_TYPE_DM1_WALL_TOGGLER						0x0D	// 13	DM1 retrocompatibility
#define ACTUATOR_TYPE_ITEM_SHOOTER						0x0E	// 14
#define ACTUATOR_TYPE_ITEM_SHOOTER_X2					0x0F	// 15	was unimplemented in DM2 ?
#define ACTUATOR_TYPE_THE_END							0x12	// 18


#define ACTUATOR_TYPE_CHARGED_ITEM_WATCHER				0x15	// 21
#define ACTUATOR_TYPE_CROSS_MAP							0x16	// 22
#define ACTUATOR_TYPE_2_STATE_WALL_SWITCH				0x17	// 23
#define ACTUATOR_TYPE_WALL_SWITCH						0x18	// 24
#define ACTUATOR_TYPE_KEY_HOLE							0x1A	// 26

#define ACTUATOR_TYPE_X1B								0x1B	// 27	// ???

#define ACTUATOR_TYPE_SIMPLE_LADDER						0x1C	// 28	Was in BETA
#define ACTUATOR_TYPE_COUNTER							0x1D	// 29
#define ACTUATOR_TYPE_TICK_GENERATOR					0x1E	// 30
#define ACTUATOR_TYPE_RELAY_1							0x20	// 32
#define ACTUATOR_TYPE_ARRIVAL_DEPARTURE					0x21	// 33
#define ACTUATOR_TYPE_FLYING_ITEM_CATCHER				0x22	// 34
#define ACTUATOR_TYPE_FLYING_ITEM_TELEPORTER			0x23	// 35

#define ACTUATOR_TYPE_SWITCH_SIGN_FOR_CREATURE			0x26	// 38

#define ACTUATOR_TYPE_X29								0x29	// 41
#define ACTUATOR_TYPE_ALCOVE_ITEM_TRAP					0x2A	// 42
#define ACTUATOR_TYPE_X2B								0x2B	// 43

#define ACTUATOR_TYPE_ORNATE_ANIMATOR					0x2C	// 44
#define ACTUATOR_TYPE_FINITE_ACTIVATOR_RELAY			0x2D	// 45
#define ACTUATOR_TYPE_CREATURE_GENERATOR				0x2E	// 46

#define ACTUATOR_TYPE_WORK_TIMER						0x31	// 49
#define ACTUATOR_TYPE_ORNATE_ANIMATOR_2					0x32	// 50


#define ACTUATOR_TYPE_X33_V008							0x33	// 51	?????
#define ACTUATOR_TYPE_X34_V016							0x34	// 52	?????
#define ACTUATOR_TYPE_X35_V032							0x35	// 53	?????
#define ACTUATOR_TYPE_X36_V064							0x36	// 54	?????
#define ACTUATOR_TYPE_X37_V128							0x37	// 55	?????



#define ACTUATOR_TYPE_PLACED_ITEM_TELEPORTER			0x3B	// 59
#define ACTUATOR_TYPE_ITEM_GENERATOR					0x3C	// 60
#define ACTUATOR_TYPE_RELAY_2							0x3D	// 61

#define ACTUATOR_TYPE_SHOP_PANEL						0x3F	// 63
#define ACTUATOR_TYPE_ITEM_RECYCLER						0x40	// 64
#define ACTUATOR_TYPE_ORNATE_STEP_ANIMATOR				0x41	// 65

#define ACTUATOR_TYPE_INVERSE_FLAG						0x43	// 67
#define ACTUATOR_TYPE_TEST_FLAG							0x44	// 68
#define ACTUATOR_TYPE_RELAY_3							0x45	// 69
#define ACTUATOR_TYPE_PUSH_BUTTON_WALL_SWITCH			0x46	// 70
#define ACTUATOR_TYPE_ITEM_CAPTURE						0x47	// 71

#define ACTUATOR_TYPE_ITEM_TELEPORT_UNKNOWN				0x48	// 72	DM2 unimplemented ?



#define ACTUATOR_TYPE_RESURECTOR						0x7E	// 126 - DM2 Champion Cell
#define ACTUATOR_TYPE_CHAMPION_MIRROR					0x7F	// 127 - DM1 Champion Mirror


#define ACTUATOR_FLOOR_TYPE__EVERYTHING					0x01	// 01
#define ACTUATOR_FLOOR_TYPE__PARTY						0x03	// 03
#define ACTUATOR_FLOOR_TYPE__ITEM						0x04	// 04
#define ACTUATOR_FLOOR_TYPE__DM1_CREATURE_GENERATOR		0x06	// 06 - DM1 Retrocompatibility
#define ACTUATOR_FLOOR_TYPE__CREATURE					0x07	// 07
#define ACTUATOR_FLOOR_TYPE__ITEM_POSSESSION			0x08	// 08

#define ACTUATOR_FLOOR_TYPE__SHOOTER_X2					0x0A	// 10
#define ACTUATOR_FLOOR_TYPE__CREATURE_KILLER			0x0B	// 11

#define ACTUATOR_FLOOR_TYPE__CROSS_MAP					0x16	// 22

#define ACTUATOR_FLOOR_TYPE__COUNTER					0x1D	// 29
#define ACTUATOR_FLOOR_TYPE__INFINITE_TICK_GENERATOR	0x1E	// 30
#define ACTUATOR_FLOOR_TYPE__RELAY_1					0x20	// 32
#define ACTUATOR_FLOOR_TYPE__ARRIVAL_DEPARTURE			0x21	// 33

#define ACTUATOR_FLOOR_TYPE__MISSILE_EXPLOSION			0x26	// 38
#define ACTUATOR_FLOOR_TYPE__CROSS_SCENE				0x27	// 39
#define ACTUATOR_FLOOR_TYPE__CREATURE_AI_STATE			0x28	// 40
#define ACTUATOR_FLOOR_TYPE__ALCOVE_ITEM				0x2A	// 42
#define ACTUATOR_FLOOR_TYPE__INFINITE_ORNATE_ANIMATOR	0x2C	// 44
#define ACTUATOR_FLOOR_TYPE__FINITE_ACTIVATOR_RELAY		0x2D	// 45
#define ACTUATOR_FLOOR_TYPE__PARTY_TELEPORTER			0x2E	// 46
#define ACTUATOR_FLOOR_TYPE__SHOP						0x30	// 48
#define ACTUATOR_FLOOR_TYPE__ORNATE_ANIMATOR			0x32	// 50

#define ACTUATOR_FLOOR_TYPE__CREATURE_ANIMATOR			0x3A	// 58
#define ACTUATOR_FLOOR_TYPE__ITEM_TELEPORTER			0x3B	// 59
#define ACTUATOR_FLOOR_TYPE__RELAY_2					0x3D	// 61
#define ACTUATOR_FLOOR_TYPE__ITEM_RECYCLER				0x40	// 64
#define ACTUATOR_FLOOR_TYPE__CREATURE_DIRECTION			0x42	// 66
#define ACTUATOR_FLOOR_TYPE__RELAY_3					0x45	// 69
#define ACTUATOR_FLOOR_TYPE__ITEM_CAPTURE_FROM_CREATURE		0x49	// 73



#define ACTUATOR_SIMPLE_USAGE__NONE						0x00	// Still image or alcove	
#define ACTUATOR_SIMPLE_USAGE__CREATURE_TRANSFORMER		0x01	
#define ACTUATOR_SIMPLE_USAGE__ANIMATED					0x02	// Like water basin
#define ACTUATOR_SIMPLE_USAGE__TORCH_HOLDER				0x04
#define ACTUATOR_SIMPLE_USAGE__RECHARGER				0x08	// Like gem vein
#define ACTUATOR_SIMPLE_USAGE__TELEPORTER_WITH_FLAG		0x10	// 

//------------------------------------------------------------------------------
//	SPELLS
//------------------------------------------------------------------------------
//	There are lists of spells in form 0xFFyy and list in form 0xyy
/*
typedef enum
{
	spellMissile_Fireball			= 0x80,
	spellMissile_PoisonBlob			= 0x81,
	spellMissile_LightningBolt		= 0x82,
	spellMissile_Dispell			= 0x83,
	spellMissile_ZoSpell			= 0x84,
	spellMissile_PoisonBolt			= 0x86,
	spellMissile_PoisonCloud		= 0x87,
	spellMissile_Push				= 0x89,
	spellMissile_Pull				= 0x8A,
	spellMissile_Zo2				= 0x8D,
	spellMissile_Reflector			= 0x8E,
	spellMissile_DustCloud			= 0xA8,
	spellMissile_Thunder			= 0xB0,
	spellMissile_ElectricShock		= 0xB1,
	spellMissile_CloudE4			= 0xE4,
} SpellMissile;	//	replacing i16 type
*/



//------------------------------------------------------------------------------
//	SPECIFIC FUNCTIONS
//------------------------------------------------------------------------------

// for PLACE_OR_REMOVE_OBJECT_IN_ROOM
#define FCT_REMOVE_OFF					0
#define FCT_PLACE_ON					1


#endif // __GENERAL_DEFINE__
