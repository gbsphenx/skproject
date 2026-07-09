//------------------------------------------------------------------------------
// SK-SPELL
//	Spells
//------------------------------------------------------------------------------

#include <skcore.h>
#include <skparam.h>
#include <skdebug.h>

//^2759:213D
SpellDefinition* SkWinCore::FIND_SPELL_BY_RUNES(U8* runes)
{
	// CSBwinSimilarity: TAG01c88e,Incantation2Spell
	ENTER(10);
	if (runes[1] == 0)
		return NULL;
	i16 iBitShifter = 24; // bp06
	i16 iSpellIndex = 0; // bp06 (was used for 2 different things)
	U32 iSpellRunesMask = 0;	// bp0a

	do {
		iSpellRunesMask |= U32(*(runes++)) << iBitShifter;
	} while (runes[0] != 0 && (iBitShifter -= 8) >= 0);
	
	SpellDefinition *xSpell = const_cast<SpellDefinition *>(dSpellsTable);	// bp04
	U32 iLocalMaxSpell = MAXSPELL_ORIGINAL;	// SPX added this to prevent overflow when switching tables

	// SPX : Use extended spells table read from GDAT if we are in custom mode
	if (SkCodeParam::bUseCustomSpells)
	{
		xSpell = const_cast<SpellDefinition *>(dSpellsTableCustom);
		iLocalMaxSpell = MAXSPELL_CUSTOM;
	} // end SPX

	for (iSpellIndex = iLocalMaxSpell; iSpellIndex-- != 0; xSpell++) {	// (bp06 = MAXSPELL; bp06-- != 0; bp04++)
		if ((xSpell->symbols & 0xFF000000) != 0) {
			if (xSpell->symbols != iSpellRunesMask)
				continue;
			return xSpell;
		}
	    if ((iSpellRunesMask & 0x00FFFFFF) == xSpell->symbols)
			return xSpell;
	}
	return NULL;
}


//^32CB:0649
// TODO: image related / spell cloud inside
U8 *SkWinCore::_32cb_0649(U8 cls1, U8 cls2, U8 cls4, i16 colorkey)
{
	ENTER(6);
	U8 *bp04 = QUERY_GDAT_IMAGE_LOCALPAL(cls1, cls2, cls4);
	i16 bp06;
	if (bp04 == NULL) {
		if (_4976_5a88 == 63)
			return NULL;
		for (bp06 = 0; bp06 < 256; bp06++) {
			_4976_582a[bp06] = U8(bp06);
		}
		bp06 = 256;
	}
	else {
		COPY_MEMORY(bp04, _4976_582a, 16);
		bp06 = 16;
	}
	return _0b36_037e(_4976_582a, U8(_4976_5a88), colorkey, -1, bp06);
}


//^075F:1791
X16 SkWinCore::CALC_CLOUD_DAMAGE(ObjectID rlCloud, ObjectID rlTarget)
{
	//^075F:1791
	ENTER(12);
	//^075F:1797
	Cloud *bp04 = GET_ADDRESS_OF_RECORDF(rlCloud);
	i16 si = 0;
	U16 bp0a = bp04->CloudType();
	//^075F:17B5
	if (bp0a >= 8)
		return si;
	//^075F:17BD
	U8 bp0b = _4976_00b4[RCJ(8, bp0a)];
	if (bp0b == 0)
		return si;
	//^075F:17D0
	GenericRecord *bp08;
	U16 di = 0; // defaulting to 0
	if (rlTarget == OBJECT_NULL) {
		if ((bp0b & 4) == 0)
			return si;
	}
	else {
		//^075F:17E4
		di = rlTarget.DBType();
		if (di == dbDoor && (bp0b & 2) != 0) {
			//^075F:17F9
			bp08 = GET_ADDRESS_OF_RECORD(rlTarget);
		}
		else {
			//^075F:180A
			if (di != dbCreature)
				return si;
			//^075F:1812
			if ((bp0b & 8) == 0)
				return si;
		}
	}
	//^075F:1820
	if (di == dbCreature && (QUERY_CREATURE_AI_SPEC_FROM_RECORD(rlTarget)->w24 & 0x1000) != 0 && bp0a != 0)
		return si;
	//^075F:1843
	si = bp04->b3_0_f();
	if ((bp0b & 1) != 0) {
		//^075F:1855
		si = (bp04->b3_0_f() >> 1) +1;
		si = RAND16(si) +si +1;
	}
	//^075F:186D
	switch (bp0a) {
		case missileLightning: // 2
			//^075F:187C
			si >>= 1;
			goto _187e;
		case missileFireball: // 0
			//^075F:187E
_187e:
			if (di == dbDoor && bp08->castToDoor()->DestroyablebyFireball() == 0)
				si = 0;
			break;
		case missileDispell: // 3
			//^075F:1897
			if ((QUERY_CREATURE_AI_SPEC_FLAGS(rlTarget) & 0x20) == 0)
				si = 0;
			break;
		case missilePoisonCloud: // 7
			//^075F:18A9
			si = max_value(1, min_value(bp04->b3_5_f(), 4) + RAND01());
			if (di == 4) {
				si = APPLY_CREATURE_POISON_RESISTANCE(rlTarget, si);
			}
		case missilePoisonBlob: // 1=
		case missileZoSpell:	// 4: // 4=Zo Spell
		case missileFuse:		// 5=
		case missilePoisonBolt: // 6: // 6=Poison bolt
			break;
	}
	//^075F:18E7
	return si;
}


//^2759:222E
// SPX: _2759_222e replaced by CAST_SPELL_PLAYER
// CSBWin: partially TAG01ca0c
U16 SkWinCore::CAST_SPELL_PLAYER(U16 player, SpellDefinition *ref, U16 power)
{
	// CSBwinSimilarity: TAG01ca0c,CastSpell

	// 0 if succeeds
	// not 0 if fails

	ENTER(30);
	Champion *champion = &glbChampionSquad[player];	//*bp04
	U16 bp0e = (ref->w6_a_f() * (power +18)) / 24;
	U16 bp08 = ref->difficulty +power; // global difficulty of the spell
	U16 bp0c = 0
		+((RAND() & 7) + (bp08 << 4))
		+((ref->difficulty * (power -1)) << 3)
		+(bp08 * bp08)
		;
	U16 bp06 = QUERY_PLAYER_SKILL_LV(player, ref->requiredSkill, 1);
	U16 di;
	i16 bp0a;
	for (bp0a = bp08 -bp06; (bp0a--) > 0; ) {
		if ((RAND() & 127) > min_value(GET_PLAYER_ABILITY(champion, abWiz, 0) +15, 115)) {
			ADJUST_SKILLS(player, ref->requiredSkill, bp0c << (bp08 -bp06));
			di = 16;
			goto _26fe; // go to end, spell failed ?
		}
	}
	U16 bp1a;
	//SPX: SPELL TYPE => 16 possible spell types!
	switch (ref->w6 & 15) {
		case C1_SPELL_TYPE_POTION: // 1: transform flask to potion
			{
				bp0a = FIND_HAND_WITH_EMPTY_FLASK(champion);
				if (bp0a < 0) {
					di = 48;		// SPX: for displaying graphics : needs flask in hand
					goto _26fe;
				}
				ObjectID si = REMOVE_POSSESSION(player, bp0a);
				SET_ITEMTYPE(si, ref->SpellCastIndex());
				GET_ADDRESS_OF_RECORD8(si)->PotionPower((RAND() & 15) + (power * 40));
				EQUIP_ITEM_TO_INVENTORY(player, si, bp0a);
				break;
			}
		case C2_SPELL_TYPE_MISSILE: // 2: shoot spell
			{
				if (ref->SpellCastIndex() == 4) // zo spell ?
					bp06 <<= 1; // multiply by 2
				CAST_CHAMPION_MISSILE_SPELL(
					player,
					ObjectID::MissileRecord(ref->SpellCastIndex()),
					BETWEEN_VALUE(21, ((bp06 << 1) +4) * (power +2), 255),
					0
					);
				break;
			}
		case C4_SPELL_TYPE_SUMMON: // 4: summon creature
			{
				// SPX: beware, hard test for U-Haul minion (0x35)
				if (ref->SpellCastIndex() == CREATURE_GOOD_U_HAUL_MINION) {
					Creature *bp1e = reinterpret_cast<Creature *>(glbDBObjectData[dbCreature]);
					for (bp08 = dunHeader->nRecords[dbCreature]; (bp08--) != 0; bp1e++) {
						if (true
							&& bp1e->w0 != OBJECT_NULL 
							&& bp1e->CreatureType() == CREATURE_GOOD_U_HAUL_MINION
							&& (bp0a = bp1e->iID) != 0x00FF
							&& glbTabCreaturesInfo[bp0a].Command != ccm13_Destroy	// 0x13
						) {
							glbTabCreaturesInfo[bp0a].Command2 = ccm13_Destroy;	// 0x13
							goto _26cf;
						}
					}
				}
				ObjectID si = CREATE_MINION(
					ref->SpellCastIndex(),
					((RAND02() + (bp06 << 1)) * power) / 6,
					(cd.pi.glbPlayerDir +2) &3,
					cd.pi.glbPlayerPosX,
					cd.pi.glbPlayerPosY,
					cd.pi.glbPlayerMap,
					-1,
					cd.pi.glbPlayerDir
					);
				if (si == OBJECT_NULL)	// SPX: Summon failed? create a cloud
					CREATE_CLOUD(OBJECT_EFFECT_CLOUD, 0x6E, cd.pi.glbPlayerPosX, cd.pi.glbPlayerPosY, 255);	// oFFA8
				break;
			}
		case C3_SPELL_TYPE_GENERAL: // 3: enchantments + general
			{
				Timer bp18;
				bp18.actor = TIMER_ACTOR__00;
				bp08 = (power +1) << 2; // (power+1)*4
				ObjectID si;
				switch (ref->SpellCastIndex()) {
				case C00_SPELL_ENCHANTMENT_LONG_LIGHT:		// Long Light (OH IR RA)
					bp0a = CmLongLight;
					goto _2540;

				case C01_SPELL_ENCHANTMENT_DARKNESS:		// Darkness (DES IR SAR)
					bp0a = CmDarkness;
					goto _2540;

				case C05_SPELL_ENCHANTMENT_LIGHT:		// Light (FUL)
					bp0a = CmLight;
_2540:	// Do light
					PROCEED_LIGHT(bp0a, bp08 * 9);
					break;

				case C03_SPELL_ENCHANTMENT_INVISIBILITY:		// Invisibility (OH EW SAR)
					bp18.TimerType(ttyInvisibility);
					if (glbGlobalSpellEffects.Invisibility++ == 0 && glbChampionInventory != 0) {
						glbChampionTable[glbChampionInventory].heroFlag |= CHAMPION_FLAG_4000;	// 0x4000
					}
					bp08 <<= 3;
					bp18.SetMap(cd.pi.glbPlayerMap);
					bp18.SetTick(glbGameTick +bp08);
					QUEUE_TIMER(&bp18);
					break;

				// SPX: (DM1 : OH EW RA See thru walls, used TT_71)
				case C02_SPELL_ENCHANTMENT_SPELLSHIELD:	// Spell shield
					bp0a = ENCHANTMENT_SPELL_SHIELD;	// type shield = 1
_25b4:
					bp08 = bp08 * bp08 + 100;
					goto _25e8;

				case C08_SPELL_ENCHANTMENT_FIRESHIELD:	// Fire shield
					bp0a = ENCHANTMENT_FIRE_SHIELD;		// type shield = 0
					goto _25b4;

				case C04_SPELL_ENCHANTMENT_PARTY_SHIELD:	// Party shield (YA IR)
					bp0a = ENCHANTMENT_PARTY_SHIELD;	// type shield = 2
					bp08 = bp08 * bp08;
					goto _25e8;

					// SPX: DM1: Was YA-BRO-ROS Foot prints
				case C06_SPELL_ENCHANTMENT_AURA_WISDOM:	// Aura of Wisdom
					bp0a = ENCHANTMENT_AURA_WISDOM;		// type = 5
_25db:
					bp08 += 3;
					bp08 = bp08 * bp08;
_25e8:
					CALL_ENCHANTMENT_SELF(champion, bp0a, bp08, 0);
					break;
				// SPX DM1 : Was ZO KATH RA
				case C07_SPELL_ENCHANTMENT_AURA_DEXTERITY:	// Aura of Dexterity
					bp0a = ENCHANTMENT_AURA_DEXTERITY;	// type = 4
					goto _25db;

				case C09_SPELL_ENCHANTMENT_AURA_VITALITY:	// Aura of Vitality
					bp0a = ENCHANTMENT_AURA_VITALITY;	// type = 6
					goto _25db;

				case C10_SPELL_ENCHANTMENT_AURA_STRENGTH:	// Aura of Strength
					bp0a = ENCHANTMENT_AURA_STRENGTH;	// type = 3
					goto _25db;

				case C11_SPELL_ENCHANTMENT_AURA_SPEED:	// Aura of Speed
					glbGlobalSpellEffects.AuraOfSpeed = U8(min_value((bp08 << 3) + glbGlobalSpellEffects.AuraOfSpeed, 255));
					break;

				case C14_SPELL_ENCHANTMENT_SPELL_REFLECTOR:	// Spell reflector : oFF8E
					CREATE_CLOUD(
						OBJECT_EFFECT_REFLECTOR, 
						BETWEEN_VALUE(21, ((bp06 << 1) +4) * (power +2), 255),
						cd.pi.glbPlayerPosX,
						cd.pi.glbPlayerPosY,
						255
						);
					break;

				case C15_SPELL_ENCHANTMENT_ITEM_CREATOR:	// Magical Marker => Item creator!
					bp1a = QUERY_GDAT_ENTRY_DATA_INDEX(GDAT_CATEGORY_x0D_SPELL_MISSILES, 0x0F, dtWordValue, 0x42);
					//SPX original is querying missile (0x0D) number 0x0F, read bin0B (dtWordValue) entry 0x42.
					//--> In the GDAT, it holds the value 013E (from item activation list) which targets the misc 0x3E = YaEw marker.
					//--> To change the generated value, the GDAT entry must be changed!
					

					si = ALLOC_NEW_DBITEM(bp1a);
					if (cd.pi.glbLeaderHandPossession.object == OBJECT_NULL) {
						TAKE_OBJECT(si, 1);
						break;
					}
					else {
						bp08 = (RAND01() + cd.pi.glbPlayerDir) & 3;
						MOVE_RECORD_TO(
							ObjectID(si, bp08),
							-1,
							0,
							cd.pi.glbPlayerPosX,
							cd.pi.glbPlayerPosY
							);

						break;
					}
					// SPX: Two unused cases, would be nice to have Window spell back. Can we do this here?
					// In a more general way, the effects should not be hard coded ...
				/*case 12:	// NEW, used for restoring all party health
					if (SkCodeParam::bUseDM2ExtendedMode)
					{
						Champion*	ch;
						for (int championIndex = 0; championIndex < glbChampionsCount; championIndex++) {
							ch = &glbChampionSquad[championIndex];
							if (ch->curHP() == 0)
								continue;
							ch->curHP(ch->maxHP());
							ch->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800
						}
						// This only does not refresh the HP bar.
					}
					break;
				case 13:	// NEW, used for restoring current champion health
					if (SkCodeParam::bUseDM2ExtendedMode)
					{
						champion->curHP(champion->maxHP());
						champion->heroFlag |= CHAMPION_FLAG_0800;	// 0x0800
						// This only does not refresh the HP bar.
					}
					break;*/
#if XDM1_EXTENDED_SEETHRUWALLS == 1
				case 12: // SPX: Reimplementation of OH EW RA See thru walls, use TT_71)
					bp18.TimerType(ttySeeThruWalls);
					glbGlobalSpellEffects.SeeThruWalls++;
					bp08 /= 2;
					// csbwin tag01cda4
					bp08 = bp08 * bp08;
					// csbwin tag01cdaa (not yet correctly done, see below)
					bp18.SetMap(cd.pi.glbPlayerMap);
					bp18.SetTick(glbGameTick + bp08);
					QUEUE_TIMER(&bp18);
					break;
					/*
tag01cdaa:
          D0L = d.Time;
          D6L &= 0xffff;
          D0L += D6L;
          D1L = d.partyLevel << 24;
          timer_20.timerTime = D1L | D0L;
          gameTimers.SetTimer(&timer_20);
		  */
#endif
				}

				break;
			}
	}
_26cf:
	di = 0;
	if (bp0e != 0) {
		ADJUST_SKILLS(player, ref->requiredSkill, bp0c);
		if (!SkCodeParam::bDebugInfiniteSpells)
			ADJUST_HAND_COOLDOWN(player, bp0e, 2);
	}
_26fe:
	if (di != 0) {
		bp08 = ref->requiredSkill;
		// SPX: if it is beyond the 4 global skills
		if (bp08 > 3) {
			bp08 = (bp08 -4) >> 2;
			// SPX: Set to 1st detailed skill?
		}
		di |= bp08;
	}
	return di;
}


