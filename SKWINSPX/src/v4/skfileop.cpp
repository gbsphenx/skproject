//------------------------------------------------------------------------------
// SK-FILE-IO
//	Handling file operation
//------------------------------------------------------------------------------

#include <skver.h>

#include <skparam.h>
#include <skdebug.h>
#include <skcore.h>


//^476D:00B1
void SkWinCore::FILE_DELETE(const U8 *curf)
{
	fset.fileDelete(reinterpret_cast<const char *>(curf));
}
//^476D:012C
U16 SkWinCore::FILE_WRITE(i16 handle, U32 buffSize, const void *buff)
{
	return fset.fileWrite(handle, buffSize, buff);
}
//^476D:008F
i16 SkWinCore::FILE_CREATE(const U8 *newf)
{
	return fset.fileCreate(reinterpret_cast<const char *>(newf));
}

//^2066:036E
Bit16u SkWinCore::_2066_036e(void *buff, Bit16u buffSize)
{
	//^2066:036E
	Bit32u bp04 = buffSize;

	//^2066:037D
	if (bp04 != 0) {
		//^2066:0386
		return FILE_WRITE(glbDataFileHandle, bp04, buff);
	}
	return 1;
}

//^2066:03C0
U16 SkWinCore::SKSAVE_WRITE(void *buff, U16 buffSize)
{
	//^2066:03C0
	if (_2066_036e(buff, buffSize) == 0)
		return 0;
	//^2066:03DB
	return 1;
}
//^2066:000F
U8 SkWinCore::SUPPRESS_WRITER(const void *pvData, const void *pvMask, U16 cbData, U32 repeat)
{
	// [bp+06] -> data
	// [bp+0A] -> mask
	// [bp+0E] -> xx
	// [bp+10] -> yy

	const U8 *data = (const U8 *)pvData;
	const U8 *mask = (const U8 *)pvMask;

	s_testSKSave.Write(pvData, pvMask, cbData, repeat);

	//^2066:000F
	Bit32u bp0a = 0;
	U8 bp04 = _4976_5254; // queued nibble
	U8 bp05 = _4976_524e; // current bit position
	Bit32u bp0e = 0;
	//^2066:0034
	while (!(bp0e >= repeat)) {
		//^2066:0037
		Bit16u si = 0;
		//^2066:0039
		while (!(si >= cbData)) {
			//^2066:003C
			U8 data_nibble = *data; // [bp-03]
			data++;
			U8 mask_nibble = *mask; // [bp-02]
			mask++;
			//^2066:0054
			if (mask_nibble != 0) {
				//^2066:005D
				i8 bp01 = 0x07;
				//^2066:0061
				while (bp01 >= 0) {
					//^2066:0063
					if (((1 << bp01) & mask_nibble) != 0) {
						//^2066:0074
						bp04 <<= 1;
						//^2066:0077
						if (((1 << bp01) & data_nibble) != 0) {
							bp04 |= 1;
						}
						//^2066:008C
						bp0a++;
						//^2066:0094
						bp05 = bp05 + 1;
						//^2066:009C
						if (bp05 == 8) {
							//^2066:00A0
							U8 bp0f = bp04;
							if (FILE_WRITE(glbDataFileHandle, 1, &bp0f) == 0)
								return 1;
							//^2066:00C4
							bp05 = bp04 = 0;
						}
					}
					//^2066:00CC
					bp01--;
					//^2066:00CF
				}
			}
			//^2066:00D5
			si++;
			//^2066:00D6
		}
		//^2066:00DE
		mask -= cbData;
		bp0e++;
		//^2066:00EC
	}
	//^2066:0104
	_4976_5254 = bp04;
	_4976_524e = bp05;
	_4976_5258 += bp0a;

	return 0;
}

//^2066:0123
U8 SkWinCore::WRITE_1BIT(U8 nibble)
{
	//^2066:0123
	U8 data = nibble;
	static const U8 mask = 0x01;

	return SUPPRESS_WRITER(&data, &mask, 1, 1);
}

//^2066:06E8
U8 SkWinCore::WRITE_RECORD_CHECKCODE(ObjectID recordLink, U8 writeDir, U8 writeFollowing)
{
	// writeDir=(write-direction-in-record-link)
	// yy=(write-subsequent-records)

	//^2066:06E8

	//^2066:0AE6
	while (recordLink != OBJECT_END_MARKER && recordLink != OBJECT_NULL) {
		//^2066:06F4
		U8 db = recordLink.DBType();
		if (db > dbActuator) {
			//^2066:0704
			if (WRITE_1BIT(1)) // write#more-record=true
				return 1;
			U8 data = db;
			U8 mask = 0x0F;
			if (SUPPRESS_WRITER(&data, &mask, 1, 1)) // write#dbtype
				return 1;
			//^2066:073A
			if (writeDir != 0 && db != dbCreature) {
				U8 data = recordLink.Dir();
				U8 mask = 0x03;
				if (SUPPRESS_WRITER(&data, &mask, 1, 1))
					return 1;
			}
		}
		//^2066:0770
		if (db == dbCloud && _4976_3b55 != 0) {
			Bit16u data = recordLink;
			Bit16u mask = 0x007F;
			if (SUPPRESS_WRITER(&data, &mask, 2, 1))
				return 1;
			break; //->2066:0AF3
		}

		//^2066:07A6
		const void *recordMask = tableMask[db];

		//^2066:07BA
		if (recordMask != NULL) {
			//^2066:07C1
			GenericRecord *record = GET_ADDRESS_OF_RECORD(recordLink);
			//^2066:07CE
			Bit16u _di = 0;
			//^2066:07D0
			switch (db) {
				case dbActuator:	// 3	(or 11-12-13 in extended mode)
					{
						Actuator *bp04 = record->castToActuator();

						//^2066:07E5
						switch (bp04->ActuatorType()) {
						case ACTUATOR_FLOOR_TYPE__CROSS_SCENE: // 0x27: 'Cross scene'
						case ACTUATOR_TYPE_X1B: // 0x1B: '-'
						case ACTUATOR_TYPE_COUNTER: // 0x1D: 'Activator, counter'
						case ACTUATOR_TYPE_ORNATE_STEP_ANIMATOR: // 0x41: 'Ornate step animator'
						case ACTUATOR_FLOOR_TYPE__INFINITE_ORNATE_ANIMATOR: // 0x2C: 'Continuous ornate animator'
						case ACTUATOR_FLOOR_TYPE__ORNATE_ANIMATOR: // 0x32: 'Activator, ornate animator'
						case ACTUATOR_FLOOR_TYPE__SHOP: // 0x30: 'Shop exhibition'
						case ACTUATOR_FLOOR_TYPE__FINITE_ACTIVATOR_RELAY: // 0x2D: 'Activator, finite activator relay'
							{
								//^2066:0825
								Bit16u data = bp04->ActuatorData();
								Bit16u mask = 0x01FF;
								if (SUPPRESS_WRITER(&data, &mask, 2, 1))
									return 1;
								break; //->2066:0926
							}
						default:
							{
								break; //->2066:0926
							}
						}
						break;
					}
				case dbCreature:	// 4
					{
						Creature *bp04 = record->castToCreature();

						//^2066:085B
						U8 data = bp04->CreatureType();
						U8 mask = 0x7F;
						if (SUPPRESS_WRITER(&data, &mask, 1, 1))
							return 1;
						//^2066:0887
						if (QUERY_CREATURE_AI_SPEC_FLAGS(recordLink) & 1) {
							//^2066:0893
							recordMask = _4976_3ac7;
						}
						//^2066:089B
						_4976_5246[recordLink.DBIndex()] = _4976_3952;
						_4976_3952++;
						break;
					}
				case dbWeapon:		// 5
				case dbCloth:		// 6
				case dbScroll:		// 7
				case dbPotion:		// 8
				case dbMiscellaneous_item: // 10
				case db11:
				case db12:
					{
						break; //->2066:0926
					}
				case dbContainer:	// 9
					{
						Container *bp04 = record->castToContainer();

						//^2066:08B4
						U8 data = bp04->ContainerType();
						U8 mask = 0x03;
						if (SUPPRESS_WRITER(&data, &mask, 1, 1))
							return 1;
						//^2066:08E5
						if (IS_CONTAINER_MAP(recordLink) != 0) {
							//^2066:08F0
							recordMask = _4976_3aef;
							_di = 1;
						}
						//^2066:08FB
						_4976_5240[recordLink.DBIndex()] = _4976_3950;
						_4976_3950++;
						break; //->2066:0926
					}
				case dbMissile:		// 14
					{
						//^2066:0914
						if (_4976_3b57 == 0)
							break;
						recordMask = _4976_3b0b;
						_di = 1;
						break;
					}
			}
			//^2066:0926  
			if (SUPPRESS_WRITER(record, recordMask, dbSize[db], 1))
				return 1;
			//^2066:094E
			switch (db) {
				case dbWeapon:		// 5
				case dbCloth:		// 6
				case dbScroll:		// 7
				case dbPotion:		// 8
				case dbMiscellaneous_item: // 10
				case db11:
				case db12:
				case db13:
					{
						break; //->2066:0AD7
					}
				case dbCreature:	// 4
					{
						Creature *bp04 = record->castToCreature();

						//^2066:0963
						_4976_3b57 = 0x0001;

						U8 res = WRITE_RECORD_CHECKCODE(
							bp04->GetPossessionObject(),
							recordMask != _4976_3ab7,
							1
							);
						if (res)
							return 1;

						//^2066:0996
						_4976_3b57 = 0x0000;
						break; //->2066:0AD7
					}
				case dbContainer:	// 9
					{
						Container *bp04 = record->castToContainer();

						//^2066:099F
						if (_di != 0) {
							//^2066:09A3
							ObjectID bp10 = bp04->GetContainedObject();
							//^2066:09AD
							Bit16u bp18 = (bp10 == OBJECT_NULL || bp10 == OBJECT_END_MARKER) ? 0 : 1;
							//^2066:09C3
							if (WRITE_1BIT((U8)bp18))
								return 1;
							//^2066:09D2
							if (!(bp18 != 0))
								break;
							//^2066:09DB
							ADD_MINION_ASSOC(recordLink);
							break;
						}
						//^2066:09E4
						if (IS_CONTAINER_MONEYBOX(recordLink) != 0) {
							//^2066:09EF
							tableMask[dbMiscellaneous_item] = _4976_3afb;
						}
						//^2066:09F9
						if (WRITE_RECORD_CHECKCODE(bp04->GetContainedObject(), 0, 1))
							return 1;
						//^2066:0A12  
						if (IS_CONTAINER_MONEYBOX(recordLink) != 0) {
							//^2066:0A20
							tableMask[dbMiscellaneous_item] = _4976_3af7;
						}
						//^2066:0A2A  
						break; //->2066:0AD7
					}
				case dbMissile:		// 14
					{
						Missile*bp04 = record->castToMissile();

						//^2066:0A2D  
						if (_di != 0) {
							//^2066:0A31  
							ADD_MINION_ASSOC(recordLink);
							//^2066:0A37  
							break; //->2066:0AD7
						}
						//^2066:0A3A  
						_4976_3b55 = 0x0001;
						U8 res = WRITE_RECORD_CHECKCODE(
							bp04->GetMissileObject(),
							0,
							0
							);
						if (res != 0)
							return 1;
						//^2066:0A59
						_4976_3b55 = 0x0000;
						break; //->2066:0AD7
					}
				case dbCloud:		// 15
					{
						//^2066:0A62  
						Timer *bp0c = glbTimersTable;
						Bit16u bp14 = 0;
						//^2066:0AC3
						while (bp14 < glbTimersActiveCount) {
							//^2066:0A76
							if (bp0c->TimerType() == ttyCloud && bp0c->id8() == recordLink) {
								//^2066:0A86
								if (WRITE_1BIT(0x0001))
									return 1;
								//^2066:0A94
                                Bit16u mask = 0x03FF;
								Bit16u data = bp14;
								if (SUPPRESS_WRITER(&data, &mask, 2, 1))
									return 1;
								break; //->2066:0AD7
							}
							//^2066:0ABC
							bp0c++;
							bp14++;
						}
						//^2066:0ACC
						if (WRITE_1BIT(0x0000))
							return 1;
						break; //->2066:0AD7
					}
			}
		}
		//^2066:0AD7
		if (writeFollowing == 0)
			break;
		recordLink = GET_NEXT_RECORD_LINK(recordLink);

	}
	//^2066:0AF3
	if (writeFollowing == 0) {
		//^2066:0AF9
		if (recordLink != OBJECT_NULL)
			return 0;
	}
	//^2066:0AFE
	if (WRITE_1BIT(0))
		return 1;
	//^2066:0B09
	return 0;
}

//^2066:0B44
// _2066_0b44 renamed _2066_0b44_WRITE_TIMERS
i16 SkWinCore::_2066_0b44_WRITE_TIMERS()
{
	//^2066:0B44
	Timer *bp04 = glbTimersTable;
	Bit16u i = 0;
	//^2066:0B58
	while (i < glbTimersCount) {
		switch (bp04->TimerType()) {
			case tty3C:
			case tty3D:
				//^2066:0B6B
				if (WRITE_RECORD_CHECKCODE(bp04->id8(), 0, 0) != 0) {
					//^2066:0B81
					return 1;
				}
				break;
		}
		//^2066:0B86
		bp04++;
		i++;
	}
	//^2066:0B91
	return 0;
}

//^2066:0147
U16 SkWinCore::SUPPRESS_FLUSH()
{
	// TODO: VERIFY ME

	//^2066:0147
	U8 bp01 = _4976_524e;
	//^2066:0151
	if (bp01 == 0)
		return 0;
	U8 bp02 = 8 - bp01;
	U8 bp03 = _4976_5254;
	//^2066:0163
	while (bp02-- != 0) {
		//^2066:0165
		bp03 = (((bp03 & 0x80) == 0) ? 0 : 1) | (bp03 << 1);
		//^2066:017B
	}
	//^2066:0185
	U8 bp04 = bp03;
	//^2066:018B
	if (FILE_WRITE(glbDataFileHandle, 1, &bp04) == 0)
		return 1;
	//^2066:01A9
	_4976_524e = _4976_5254 = 0;
	_4976_5258 = bp01;
	//^2066:01BF
	return 0;
}

//^2066:046D
void SkWinCore::_2066_046d() { // TODO: Unr
	Unr();
}

//^2066:38D9
//SPX : Read the 10 savegame names
void SkWinCore::_2066_38d9()
{
	//^2066:38D9
	ENTER(2);
	//^2066:38DE
	for (i16 bp02 = 0; bp02 < 10; bp02++) {
		//^2066:38E6
		glbSKSaveDigitAlpha = bp02 +0x30;
		//^2066:38EE
		i16 si = OPEN_FILE(FORMAT_SKSTR(ptrSKSave_dat, NULL));
		//^2066:390D
		if (si >= 0) {
			//^2066:3911
			READ_FILE(si, 42, &_4976_5250[bp02]);
			//^2066:3931
			CLOSE_FILE(si);
			//^2066:3938

			// SPX : native DM1 savegames don't have the same format
			if (_4976_5250[bp02].wTimerFlag != 1)
			{
				sprintf((char*)_4976_5250[bp02].sSavegameName, "SKSAVE%d.DAT (NOT DM2 NATIVE)", bp02);
			}
		}
		else {
			//^2066:393A
			_4976_3b5f[0] = glbSKSaveDigitAlpha;
			//^2066:3940
			SK_STRCPY(_4976_5250[bp02].sSavegameName, _4976_3b5f);
			//^2066:3961
			_4976_5250[bp02].w38 = 0xBEEF;
			_4976_5250[bp02].w40 = 0xDEAD;
		}
		//^2066:397B
	}
	//^2066:3987
	return;
}

