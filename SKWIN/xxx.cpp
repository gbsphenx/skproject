//	// 
//	int _3a15_0381(sk5c20 *xx, sk5c20 *yy)
//	{
//		//^3A15:0381
//		if ((*(Bit32u *)xx & 0x00FFFFFF) >= (*(Bit32u *)yy & 0x00FFFFFF)) {
//			//^3A15:03B8
//			Bit16u si =  ((*(Bit32u *)xx & 0x00FFFFFF) != (*(Bit32u *)yy & 0x00FFFFFF)) ? 0 : 1;
//			//^3A15:03EC
//			if (si != 0) {
//				//^3A15:03F0
//				if (xx->b4 <= yy->b4) {
//					//^3A15:0400
//					if (si != 0) {
//						//^3A15:0404
//						si = (xx->b4 != yy->b4) ? 0 : 1;
//						//^3A15:041D
//						if (si != 0) {
//							//^3A15:0431
//							if (si != 0) {
//								//^3A15:0435
//								if ((Bit16u)xx <= (Bit16u)yy)
//									return 1;
//							}
//							return 0;
//						}
//						//^3A15:0421
//						if (xx->b5 <= yy->b5) {
//							//^3A15:043D
//							return 1;
//						}
//						//^3A15:0431
//						if (si != 0) {
//							//^3A15:0435
//							if ((Bit16u)xx <= (Bit16u)yy)
//								return 1;
//						}
//						return 0;
//					}
//					else {
//						//^3A15:0431
//						if (si != 0) {
//							//^3A15:0435
//							if ((Bit16u)xx <= (Bit16u)yy)
//								return 1;
//						}
//						return 0;
//					}
//				}
//			}
//			else {
//				//^3A15:0400
//				if (si != 0) {
//					//^3A15:0404
//					si = (xx->b4 != yy->b4) ? 0 : 1;
//					//^3A15:041D
//					if (si != 0) {
//						//^3A15:0431
//						if (si != 0) {
//							//^3A15:0435
//							if ((Bit16u)xx <= (Bit16u)yy)
//								return 1;
//						}
//						return 0;
//					}
//					//^3A15:0421
//					if (xx->b5 <= yy->b5) {
//						//^3A15:043D
//						return 1;
//					}
//					//^3A15:0431
//					if (si != 0) {
//						//^3A15:0435
//						if ((Bit16u)xx <= (Bit16u)yy)
//							return 1;
//					}
//					return 0;
//				}
//				//^3A15:0431
//			}
//		}
//		//^3A15:043D
//		return 1;
//	}
//	// 
//	void _3a15_0314()
//	{
//		//^3A15:0314
//		_4976_5c14 = 0;
//		_4976_5c18 = 0xffff;
//		//^3A15:0325
//		Bit16u di;
//		for (Bit16u si=0; si < _4976_5c16; si++) {
//			//^3A15:0329
//			if (_4976_5c20[si].b4 == 0) {
//				//^3A15:033D
//				if (_4976_5c18 == 0xffff) {
//					//^3A15:0344
//					_4976_5c18 = si;
//				}
//				else {
//					//^3A15:034A
//					_4976_5c20[di].w0 = si;
//				}
//				//^3A15:035A
//				_4976_5c20[si].w0 = 0xffff;
//				//^3A15:036C
//				di = si;
//			}
//			else {
//				//^3A15:0370
//				_4976_5c14 = si+1;
//			}
//			//^3A15:0376
//		}
//	}
//	// 
//	void _3a15_008e()
//	{
//		//^3A15:008E
//		Bit16u bp12 = _4976_5c1e;
//		if (bp12 != 0) {
//			//^3A15:00A1
//			for (Bit16u di = 0; di < bp12; di++) {
//				//^3A15:00A5
//				_4976_5c1a[di] = di;
//				//^3A15:00AF
//			}
//			//^3A15:00B8
//			if (bp12 != 1) {
//				for (__int16 bp0e = (bp12 -2) >> 1; bp0e >= 0; bp0e--) {
//					//^3A15:00CE
//					Bit16u di = bp0e;
//					sk5c20 *bp04 = &_4976_5c20[_4976_5c1a[di]];
//					while (true) {
//						//^3A15:00F3
//						Bit16u si = (di << 1) + 1;
//						//^3A15:00FA
//						if (si >= bp12)
//							break;
//						//^3A15:0102
//						sk5c20 *bp08 = &_4976_5c20[_4976_5c1a[si]];
//						//^3A15:0124
//						if (si+1 < bp12) {
//							//^3A15:012C
//							sk5c20 *bp0c = &_4976_5c20[_4976_5c1a[si+1]];
//							//^3A15:014F
//							if (_3a15_0381(bp04, bp08) != 0) {
//								//^3A15:0167
//								if (_3a15_0381(bp04, bp0c) != 0)
//									break;
//								si++;
//							}
//							else {
//								//^3A15:0182
//								if (_3a15_0381(bp0c, bp08) != 0)
//									si++;
//							}
//						}
//						else {
//							//^3A15:019D
//							if (_3a15_0381(bp04, bp08) != 0)
//								break;
//						}
//						//^3A15:01B5
//						Bit16u bp10 = _4976_5c1a[di];
//						//^3A15:01C5
//						_4976_5c1a[di] = _4976_5c1a[si];
//						//^3A15:01DF
//						_4976_5c1a[si] = bp10;
//						//^3A15:01EF
//						di = si;
//					}
//					//^3A15:01F4
//				}
//			}
//		}
//		//^3A15:0200
//		_4976_4762 = 0xffff;
//		_3a15_0314();
//		return;
//	}

	// 
	int READ_1BIT(Bit16u *pres)
	{
		//^2066:030D
		Bit8u bp01;
		//^2066:0312
		Bit16u si = SUPPRESS_READER(&bp01, _4976_3b54, 1, 1, 1);
		*pres = bp01;
		return si;
	}

	// 
	void _47e1_0089(void *buff, Bit32u buffSize)
	{
		memset(buff, 0, buffSize);
	}
	// 
	void COPY_MEMORY(const void *buffSrc, const void *buffDst, Bit32u buffSize);
	// 
	void APPEND_RECORD_TO(Bit16u recordLink, Bit16u *ptr, __int16 xpos, __int16 ypos)
	{
		//^0CEE:0B0D
		__int16 di = xpos;
		//^0CEE:0B16
		if (recordLink != 0xfffe && recordLink != 0xffff) {
			//^0CEE:0B28
			GenericRecord *bp0c = (GenericRecord *)GET_ADDRESS_OF_RECORD(recordLink);
			//^0CEE:0B36
			bp0c->w0 = 0xfffe;
			//^0CEE:0B3E
			if (di >= 0) {
				//^0CEE:0B45
				Bit8u *bp08 = &_4976_4c78[di][ypos];
				//^0CEE:0B60
				Bit16u bp12;
				if ((*bp08 & 0x10) != 0) {
					//^0CEE:0B69
					bp12 = GET_TILE_RECORD_LINK(di, ypos);
					//^0CEE:0B76
					//^0CEE:0C53

					for (; (bp0e = GET_NEXT_RECORD_LINK(bp12)) != 0xfffe; ) {
						//^0CEE:0C58
						bp12 = bp0e;
					}

					//^0CEE:0C6D
					bp0c = (GenericRecord *)GET_ADDRESS_OF_RECORD(bp12);
					//^0CEE:0C7B
					//^0CEE:0CA5
					bp0c->w0 = recordLink;
					return;
				}
				//^0CEE:0B79
				if (_4976_4d16[_4976_4d0e->cwListSize -1] != 0xffff) {
					//^0CEE:0B90
					RAISE_SYSERR(72);
				}
				//^0CEE:0B98
				*bp08 |= 0x10;
				//^0CEE:0B9F
				Bit16u *bp04 = &_4976_4c52[di +1];
				//^0CEE:0BB5
				Bit16u si = _4976_4cb4 -(_4976_4dd6[_4976_0c5a] +di) -1;
				//^0CEE:0BCE
				//^0CEE:0BDA
				for (; si-- != 0; ) {
					//^0CEE:0BD0
					(*bp04)++;
					bp04++;
				}
				//^0CEE:0BE1
				Bit16u bp10 = 0;
				bp08 -= ypos;
				//^0CEE:0BEC
				si = _4976_4c52[di];

				//^0CEE:0C08
				for (; bp10++ != ypos; ) {
					//^0CEE:0BF9
					if ((*(bp08++) & 0x10) != 0) {
						//^0CEE:0C07
						si++;
					}
				}
				//^0CEE:0C13
				bp04 = &_4976_4d16[si];
				//^0CEE:0C27
				COPY_MEMORY(
					bp04,
					bp04 +1,
					(_4976_4d0e->cwListSize -si -1) * 2
					);
				//^0CEE:0C4E
				//^0CEE:0C51
				//^0CEE:0CA8
				*bp04 = recordLink;
				return;
			}
			//^0CEE:0C7D
			bp0c = (GenericRecord *)ptr;
			//^0CEE:0C9C
			for (; bp0c->w0 != 0xfffe; ) {
				//^0CEE:0C8B
				bp0c = (GenericRecord *)GET_ADDRESS_OF_RECORD(bp0c->w0);
			}
			//^0CEE:0CA5
			bp0c->w0 = recordLink;
			return;
		}
		//^0CEE:0CAE
	}
//	// 
//	int READ_RECORD_CHECKCODE(__int16 xpos, __int16 ypos, Bit16u *recordLinkPtr, Bit16u xx, Bit16u yy)
//	{
//		// xx=(read-direction-in-record-link)
//		// yy=(read-subsequent-records)
//
//		//^2066:15AA
//		while (true) {
//			//^2066:15B0
//			Bit16u bp0e;
//			if (READ_1BIT(&bp0e) != 0)
//				return 1;
//			//^2066:15C2
//			if (bp0e == 0)
//				break;
//			//^2066:15CB
//			Bit8u bp0c = 0x0f;
//			Bit8u bp0b;
//			//^2066:15CF
//			if (SUPPRESS_READER(&bp0b, &bp0c, 1, 1, 1) != 0)
//				return 1;
//			//^2066:15EF
//			Bit16u di = bp0b;
//			Bit16u bp0a;
//			if (xx != 0 && di != dbCreature) {
//				//^2066:1601
//				bp0c = 0x03;
//				//^2066:1605
//				if (SUPPRESS_READER(&bp0b, &bp0c, 1, 1, 1) != 0)
//					return 1;
//				//^2066:1625
//				bp0a = bp0b;
//			}
//			else {
//				//^2066:162F
//				bp0a = 0;
//			}
//			//^2066:1634
//			if (di == dbCloud && _4976_3b59 != 0) {
//				//^2066:1640
//				Bit16u bp10 = 0x7f;
//				//^2066:1645
//				if (SUPPRESS_READER(&bp0e, &bp10, 2, 1, 1) != 0)
//					return 1;
//				//^2066:1665
//				bp0e = bp0e;
//				if (recordLinkPtr == NULL)
//					break;
//				//^2066:1676
//				*recordLinkPtr = 0xff80 | bp0e;
//				break;
//			}
//			//^2066:1685
//			Bit16u si = ALLOC_NEW_RECORD(di);
//			si = (si & 0x3fff) | bp0a << 14;
//			//^2066:169E
//			APPEND_RECORDLINK_TO(si, recordLinkPtr, xpos, ypos);
//			//^2066:16B3
//			void *bp04 = GET_ADDRESS_OF_RECORD(si);
//			//^2066:16C0
//			const void *bp08 = _4976_3b13[di];
//			if (bp08 != NULL) {
//				//^2066:16DA
//				Bit16u bp12 = 0;
//				//^2066:16DF
//				switch (di) {
//					case dbCreature:
//						{
//							//^2066:16F6
//							bp0c = 0x7f;
//							if (SUPPRESS_READER(&bp0b, &bp0c, 1, 1, 1) != 0)
//								return 1;
//							//^2066:171A
//							reinterpret_cast<Creature *>(bp04)->b4 = bp0b;
//							//^2066:1724
//							if ((_0cee_2de0(si) & 1) == 0)
//								break;
//							bp08 = db4Mask2;
//							break;
//						}
//					case dbContainer:
//						{
//							//^2066:173A
//							bp0c = 0x03;
//							if (SUPPRESS_READER(&bp0c, &bp0c, 1, 1, 0) != 0)
//								return 1;
//							//^2066:175E
//							reinterpret_cast<Container *>(bp04)->b4 &= 0xf9;
//							reinterpret_cast<Container *>(bp04)->b4 |= (bp0b & 3) << 1;
//							//^2066:1774
//							if (IS_CONTAINER_MAP(si) == 0)
//								break;
//							//^2066:177F
//							bp08 = db9Mask2;
//							bp12 = 1;
//							break;
//						}
//					case dbMissile:
//						{
//							//^2066:178E
//							if (_4976_3b5b != 0) {
//								//^2066:1795
//								bp08 = dbEMask2;
//								bp12 = 1;
//							}
//							break;
//						}
//				}
//				//^2066:17A2
//				if (SUPPRESS_READER(bp04, bp08, _4976_0242[di], 1, 0) != 0)
//					return 1;
//				//^2066:17C9
//				switch (di) {
//					case dbCreature:
//						{
//							//^2066:17DD
//							_4976_3b5b = 1;
//							reinterpret_cast<Creature *>(bp04)->w2 = 0xfffe;
//							if (READ_RECORD_CHECKCODE(
//								-1,
//								0,
//								&reinterpret_cast<GenericContainerRecord *>(bp04)->w2,
//								(bp08 != db4Mask1) ? 1 : 0,
//								1
//								) != 0
//							) {
//								return 1;
//							}
//							//^2066:181F
//							_4976_3b5b = 0;
//							break;
//						}
//					case dbContainer:
//						{
//							//^2066:1828
//							if (bp12 != 0) {
//								Bit16u bp14;
//								//^2066:182E
//								if (READ_1BIT(&bp14) != 0)
//									return 1;
//								//^2066:1840
//								if (bp14 != 0) {
//									//^2066:1846
//									//^2066:18B2
//									ADD_INDEX_TO_POSSESSION_INDICES(si);
//								}
//								//^2066:1848
//								reinterpret_cast<Container *>(bp04)->w2 = 0xfffe;
//							}
//							else {
//								//^2066:1854
//								if (IS_CONTAINER_MONEYBOX(si) != 0) {
//									//^2066:185F
//									tableMask[dbMiscellaneous_item] = dbAMask1;
//								}
//								//^2066:1869
//								reinterpret_cast<Container *>(bp04)->w2 = 0xfffe;
//								//^2066:1872
//								if (READ_RECORD_CHECKCODE(-1, 0, &reinterpret_cast<Container *>(bp04)->w2, 0, 1) != 0)
//									return 1;
//								//^2066:1891
//								if (IS_CONTAINER_MONEYBOX(si) == 0)
//									break;
//								//^2066:189F
//								tableMask[dbMiscellaneous_item] = dbAMask2;
//								break;
//							}
//							break;
//						}
//					case dbMissile:
//						{
//							//^2066:18AC
//							if (bp12 != 0) {
//								//^2066:18B2
//								ADD_INDEX_TO_POSSESSION_INDICES(si);
//							}
//							else {
//								//^2066:18BB
//								_4976_5c20[reinterpret_cast<Missile *>(bp04)->w6].w6 = si;
//								//^2066:18D1
//								reinterpret_cast<Missile *>(bp04)->w2 = 0xfffe;
//								_4976_3b59 = 1;
//								//^2066:18E0
//								if (READ_RECORD_CHECKCODE(-1, 0, &reinterpret_cast<Missile *>(bp04)->w2, 0, 0) != 0)
//									return 1;
//								//^2066:18FC
//								_4976_3b59 = 0;
//							}
//							break;
//						}
//					case dbCloud:
//						{
//							//^2066:1904
//							if (READ_1BIT(&bp0e) != 0)
//								return 1;
//							//^2066:1913
//							if (bp0e == 0)
//								break;
//							//^2066:1919
//							Bit16u bp10 = 0x03ff;
//							//^2066:191E
//							if (SUPPRESS_READER(&bp0e, &bp10, 2, 1, 1) != 0)
//								return 1;
//							//^2066:193B
//							bp0e = bp0e;
//							_4976_5c20[bp0e].w8 = si;
//							break;
//						}
//				}
//			}
//			//^2066:1950
//			if (yy == 0)
//				break;
//        }
//		//^2066:1959
//		return 0;
//	}
//	// 
//	Bit8u _0cee_23de(Bit16u recordLink)
//	{
//		//^0CEE:23DE
//		Bit16u di = recordLink;
//		//^0CEE:23E6
//		if (di != 0xffff) {
//			//^0CEE:23EB
//			Bit16u si = (di & 0x3c00) >> 10;
//			if (si == dbMissile) {
//				//^0CEE:23FA
//				Missile *temp = (Missile *)GET_ADDRESS_OF_RECORD(di);
//				return _0cee_23de(temp->w2);
//			}
//			//^0CEE:240F
//			return _4976_0caa[si];
//		}
//		//^0CEE:2415
//		return 0xff;
//	}
	// 
	Bit16u QUERY_GDAT_DBSPEC_WORD_VALUE(Bit16u recordLink, Bit8u cls4)
	{
		//^0CEE:25E5
		Bit16u si = x1;
		//^0CEE:25EC
		if (si == 0xffff)
			return 0;
		//^0CEE:25F5
		return QUERY_GDAT_ENTRY_DATA_INDEX(_0cee_23de(si), QUERY_CLS2_FROM_RECORD(si), 0x000B, x2);
	}
	// 
	Bit16u ADD_ITEM_CHARGE(Bit16u recordLink, __int16 delta)
	{
		//^0CEE:29EC
		Bit16u si = 0;
		//^0CEE:29F4
		if (recordLink == 0xffff)
			return si;
		//^0CEE:29FD
		GenericRecord *bp04 = (GenericRecord *)GET_ADDRESS_OF_RECORD(recordLink);
		//^0CEE:2A0B
		Bit16u bp06 = (recordLink & 0x3c00) >> 10;
		//^0CEE:2A17
		Bit16u di;
		switch (bp06) {
			case dbWeapon:
				//^0CEE:2A29
				si = reinterpret_cast<Weapon *>(bp04)->Charges();
				di = 0x000F;
				break;
			case dbCloth:
				//^0CEE:2A3D
				si = reinterpret_cast<Cloth *>(bp04)->Charges();
				di = 0x000F;
				break;
			case dbMiscellaneous_item:
				//^0CEE:2A48
				si = reinterpret_cast<Miscellaneous_item *>(bp04)->Compass();
				di = 0x03;
			default:
				//^0CEE:2AB6
				return si;
		}
		//^0CEE:2A5A
		si += delta;
		//^0CEE:2A5D
		si = BETWEEN_VALUE(0, si, di);
		//^0CEE:2A6B
		switch (bp06) {
			case dbWeapon:
				//^0CEE:2A7F
				reinterpret_cast<Weapon *>(bp04)->Charges(si);
				break;
			case dbCloth:
				//^0CEE:2A91
				reinterpret_cast<Cloth *>(bp04)->Charges(si);
				break;
			case dbMiscellaneous_item:
				//^0CEE:2AA2
				reinterpret_cast<Miscellaneous_item *>(bp04)->Compass(si);
				break;
			default:
				//^0CEE:2AB6
				return si;
		}
		//^0CEE:2AB6
		return si;
	}
	// 
	Bit16u MIN_VALUE(__int16 v1, __int16 v2) {
		//^0CD5:008E
		//^0CD5:0091
		if (v1 < v2)
			return v1;
		return v2;
	}
	// 
	Bit16u MAX_VALUE(__int16 v1, __int16 v2) {
		//^0CD5:00A0
		//^0CD5:00A3
		if (v1 > v2)
			return v1;
		return v2;
	}
//	// 
//	void _24a5_013d()
//	{
//		//^24A5:013D
//
//		if (_4976_4dba[_4976_4bf4].Difficulty() == 0) {
//			//^24A5:015D
//			_4976_4c36 = 1;
//		}
//		//^24A5:0166
//		Bit16u bp1c[9];
//		ZERO_MEMORY(bp1c, 9);
//		//^24A5:0177
//		Bit16u bp06 = 0;
//        //^24A5:017C
//		if ((QUERY_GDAT_DBSPEC_WORD_VALUE(_4976_57c8, 0) & 0x0010) != 0) {
//			//^24A5:018E
//			bp1c[bp06] = ADD_ITEM_CHARGE(_4976_57c8, 0);
//			bp06++;
//		}
//		//^24A5:01AB
//		Bit16u si;
//		for (si=0; si < _4976_4c3c; si++) {
//			//^24A5:01AF
//			for (Bit16u bp04=0; bp04 <= 1; bp04++) {
//				//^24A5:01B6
//				Bit16u bp08;
//				if ((QUERY_GDAT_DBSPEC_WORD_VALUE(bp08 = _4976_5475[si].pocket[bp04], 0) & 0x0010) != 0) {
//					//^24A5:01DC
//					bp1c[bp06] = ADD_ITEM_CHARGE(bp08, 0);
//					//^24A5:01F5
//					bp06++;
//				}
//				//^24A5:01F8
//			}
//			//^24A5:0201
//		}
//		//^24A5:0208
//		for (si=0; bp06-1 > si; si++) {
//			//^24A5:020C
//			if (bp1c[si] > bp1c[si +1]) {
//				//^24A5:0226
//				Bit16u bp04 = bp1c[si];
//				//^24A5:0235
//				bp1c[si] = bp1c[si +1];
//				//^24A5:024D
//				bp1c[si +1] = bp04;
//			}
//			//^24A5:025C
//		}
//		//^24A5:0265
//		Bit16u bp02 = 6;
//		Bit16u di;
//		for (di=0, si=0; si < bp06; si++) {
//			//^24A5:0270
//			di += (_4976_3d34[bp1c[si]] << bp02) >> 6;
//			//^24A5:028B
//			bp02 = MAX_VALUE(0, bp02 -1);
//			//^24A5:029C
//		}
//		//^24A5:02A2
//		di += _4976_52d4;
//		di += _4976_57e4.w0;
//		//^24A5:02AA
//		di = di + QUERY_GDAT_ENTRY_DATA_INDEX(0x08, _4976_4218, 0x0B, 0x67);
//		//^24A5:02C2
//		if (_4976_5c5d != 0) {
//			//^24A5:02C9
//			di += _4976_3d44[BETWEEN_VALUE(0, _4976_5c63 + _4976_5c68, 5)];
//		}
//		//^24A5:02EA
//		for (_4976_4c36=0; _4976_4c36 <= 5; _4976_4c36++) {
//			//^24A5:02F2
//			if (_4976_3d44[_4976_4c36] < di)
//				break;
//			//^24A5:02FF
//		}
//		//^24A5:030A
//		Bit16u bp0a = QUERY_GDAT_ENTRY_DATA_INDEX(0x08, _4976_4218, 0x0B, 0x68);
//		//^24A5:031F
//		if (bp0a < _4976_4c36) {
//			//^24A5:0327
//			_4976_4c36 = bp0a;
//		}
//		//^24A5:032D
//		if (_4976_5c5d != 0) {
//			//^24A5:0334
//			if (_4976_4bf8 != 0) {
//				//^24A5:033B
//				_4976_4c36 = 0;
//			}
//		}
//		//^24A5:0341
//		_4976_4c36 -= (_4976_52d6 > 12) ? 1 : 0;
//		//^24A5:0353
//		_4976_4c36 = BETWEEN_VALUE(0, _4976_4c36, 5);
//	}
//	// 
//	Bit16u _48ae_01ec(Bit16u x1, Bit16u x2, Bit16u x3)
//	{
//		//^48AE:01EC
//		__int16 si = x2;
//		//^48AE:01F4
//		Bit16u di = QUERY_GDAT_DBSPEC_WORD_VALUE(x1, 4);
//		//^48AE:0202
//		if (x3 != 0) {
//			//^48AE:0208
//			if (si < 0x000D) {
//				//^48AE:020D
//				if (si < 0) {
//					//^48AE:0211
//					//^48AE:021E
//					//^48AE:0224
//					return di & 0x0400;
//				}
//				//^48AE:0216
//				//^48AE:021E
//				//^48AE:0224
//				return _4976_49e8[si] & di;
//			}
//			//^48AE:0226
//			//^48AE:024E
//			//^48AE:0250
//			return 0;
//		}
//		//^48AE:0228
//		if (si < 0x000D) {
//			//^48AE:022D
//			if (si > 0x0001) {
//				//^48AE:0232
//				//^48AE:023C
//				return di & _4976_49e8[si];
//			}
//		}
//		//^_023e
//		if (si >= 0x001E) {
//			//^48AE:0243
//			if (si < 0x0026) {
//				//^48AE:0248
//				if ((di & 0x8000) != 0) {
//					//^48AE:024E
//					return 0;
//				}
//				//^48AE:0252
//				if (_48ae_01ec(_4976_3de2[(_4976_5366 << 1) + (_4976_5354)], 0x000c, 0x0000) != 0) {
//					//^48AE:0273
//					return di & 0x0040;
//				}
//			}
//		}
//		//^48AE:027A
//		return 1;
//	}
//	// 
//	Bit16u _2c1d_038b(Bit16u x1, Bit8u x2, Bit16u x3, Bit16u x4)
//	{
//		//^2C1D:038B
//		__int16 si = x4;
//		//^2C1D:0393
//		Bit16u bp02 = QUERY_GDAT_DBSPEC_WORD_VALUE(x1, x2);
//		//^2C1D:03A4
//		if (bp02 == 0)
//			return 0;
//		//^2C1D:03A8
//		if ((bp02 & 0x4000) != 0) {
//			//^2C1D:03AF
//			switch (si) {
//				case 0xfffe:
//				case 0x0002:
//				case 0x0003:
//					break;
//				default:
//					//^2C1D:03E2
//					return 0;
//			}
//		}
//		else {
//			//^2C1D:03C0
//			if (x3 == 0) {
//				//^2C1D:03C6
//				if ((bp02 & 0x8000) == 0)
//					return 0;
//			}
//		}
//		//^2C1D:03CD
//		bp02 = (Bit8u)bp02;
//		//^2C1D:03D4
//		if (si < 0) {
//			//^2C1D:03D8
//			bp02 = -bp02;
//		}
//		//^2C1D:03DD
//		return bp02;
//	}
//	// 
//	Bit16u QUEUE_TIMER(sk5c20 *x1)
//	{
//		//^3A15:0696
//		if (x1->b4 == 0) {
//			//^3A15:06A5
//			return 0xffff;
//		}
//		//^3A15:06AB
//		if (_4976_5c1e == _4976_5c16) {
//			//^3A15:06B4
//			RAISE_SYSERR(45);
//		}
//		//^3A15:06BC
//		Bit16u si = _4976_5c18;
//		//^3A15:06CD
//		_4976_5c18 = _4976_5c20[si].w0;
//		//^3A15:06D5
//		_4976_5c20[si] = *x1;
//		//^3A15:06F3
//		if (_4976_5c14 <= si) {
//			//^3A15:
//			_4976_5c14 = si +1;
//		}
//		//^3A15:06FF
//		__int16 di = _4976_4762;
//		//^3A15:0704
//		if (di < 0) {
//			//^3A15:0708
//			di = _4976_5c1e;
//		}
//		//^3A15:070C
//		_4976_4762 = 0xffff;
//		_4976_5c1e++;
//		//^3A15:0716
//		_4976_5c1a[di] = si;
//		//^3A15:0723
//		_3a15_0486(di);
//		//^3A15:0729
//		return si;
//	}
//	// 
//	void _2c1d_03e7(__int16 x1, Bit16u x2, Bit16u x3, Bit16u x4)
//	{
//		//^2C1D:03E7
//		Bit16u di = x2;
//		Bit16u si = x4;
//		//^2C1D:03F3
//		if (x1 < 0)
//			return;
//		//^2C1D:03FC
//		if (di == 0xffff)
//			return;
//		//^2C1D:0404
//		if (x3 < 0x001e) {
//			//^2C1D:040D
//			Bit16u bp06 = QUERY_GDAT_DBSPEC_WORD_VALUE(di, 0);
//			//^2C1D:041A
//			if (si != 0) {
//				//^2C1D:041E
//				if ((bp06 & 0x10) != 0) {
//					_24a5_013d();
//				}
//			}
//			//^2C1D:042A
//			if ((bp06 & 0x2000) == 0) {
//				//^2C1D:0434
//				sk53b4 *bp04 = &_4976_53b4[x1];
//				//^2C1D:0445
//				Bit16u bp0a = _48ae_01ec(di, x3, 1);
//				Bit16u bp0c = 0;
//				//^2C1D:045B
//				if (si != 0 && si != 3 && si != 0xfffe) {
//					//^2C1D:0472
//					bp06 = _2c1d_038b(di, 0x0014, bp0a, si);
//					//^2C1D:0483
//					if (bp06 != 0) {
//						//^2C1D:0487
//						if (si == 1 || si == 0xffff) {
//							//^2C1D:0491
//							bp04->w64 += bp06;
//						}
//						else {
//							//^2C1D:049D
//							if (si == 2) {
//								//^2C1D:04A2
//								bp04->w62 = BETWEEN_VALUE(0, bp04->w62 + bp06, 0x03e7);
//							}
//						}
//						//^2C1D:04C1
//						bp04->w50 |= 0x0800;
//					}
//					//^2C1D:04CA
//					for (Bit16u bp08 = 0; bp08 <= 6; bp08++) {
//						//^2C1D:04D1
//						bp06 = _2c1d_038b(di, bp08 + 0x15, bp0a, si);
//						//^2C1D:04E6
//						if (bp06 != 0) {
//							//^2C1D:04EA
//							if (si == 1 || si == 0xffff) {
//								//^2C1D:04F4
//								bp04->b86[bp08] = bp04->b86[bp08] + bp06;
//							}
//							else {
//								//^2C1D:050D
//								_2c1d_0300(bp04, bp08, bp06);
//							}
//							//^2C1D:0520
//							bp04->w50 |= 0x3000;
//						}
//						//^2C1D:0529
//					}
//				}
//				//^2C1D:0532
//				for (Bit16u bp08 = 0; bp08 < 0x0014; bp08++) {
//					//^2C1D:0539
//					bp06 = _2c1d_038b(di, bp08 + 0x1E, bp0a, si);
//					//^2C1D:054E
//					if (bp06 != 0) {
//						//^2C1D:0552
//						bp04->b173[bp08] = bp04->b173[bp08] + bp06;
//						//^2C1D:056B
//						bp04->w50 |= 0x2000;
//						bp0c = 1;
//					}
//					//^2C1D:0579
//				}
//				//^2C1D:0582
//				bp06 = _2c1d_038b(di, 0x0033, bp0a, si);
//				//^2C1D:0593
//				if (bp06 != 0) {
//					//^2C1D:0597
//					bp04->b259[0] = bp04->b259[0] + bp06;
//					bp0c = 1;
//				}
//				//^2C1D:05AC
//				if (si != 2 && si != 0xffff && si != 3) {
//					//^2C1D:05BB
//					bp06 = _2c1d_038b(di, 0x0032, bp0a, si);
//					//^2C1D:05CC
//					if (bp06 != 0) {
//						//^2C1D:05D0
//						_4976_57e4.w0 += bp06;
//						if (si != 0) {
//							//^2C1D:05D8
//							_24a5_013d();
//						}
//					}
//				}
//				//^2C1D:05DD
//				if (bp0c != 0 && si == 2) {
//					//^2C1D:05E8
//					sk5c20 bp16;
//					bp16.b4 = 0x0e;
//					//^2C1D:05EC
//					*(Bit32u *)&bp16.w0 = (_4976_4bf4 << 24) | (QUERY_GDAT_DBSPEC_WORD_VALUE(di, 0x0013) + _4976_4bec);
//					//^2C1D:061C
//					bp16.b5 = (Bit8u)x1;
//					//^2C1D:0622
//					bp16.w6 = (di & 0x3c00) >> 10;
//					//^2C1D:062D
//					bp16.w8 = QUERY_CLS2_FROM_RECORD(di);
//					//^2C1D:0637
//					QUEUE_TIMER(&bp16);
//				}
//			}
//		}
//		//^2C1D:0643
//		if (si != 0) {
//			//^2C1D:0647
//			_2c1d_2076(x1);
//		}
//		//^2C1D:0650
//		return;
//	}

//	// 
//	int _2066_19e7()
//	{
//		//^2066:19E7
//		for (Bit16u si=0; si < _4976_4c3c; si++) {
//			//^2066:19F1
//			Bit16u *bp08 = &_4976_5475[si].pocket[0];
//			//^2066:1A01
//			for (Bit16u di=0; di < 30; di++) {
//				//^2066:1A05
//				*bp08 = 0xfffe;
//				bp08++;
//				//^2066:1A11
//			}
//			//^2066:1A17
//		}
//		//^2066:1A1E
//		_4976_57c8 = 0xfffe;
//		Bit16u bp18 = _4976_0c5a;
//		//^2066:1A2A
//		for (Bit16u bp16=0; _4976_4d0e->nMaps > bp16; bp16++) {
//			//^2066:1A32
//			CHANGE_CURRENT_MAP_TO(bp16);
//			//^2066:1A3B
//			for (Bit16u bp0e=0; bp0e < _4976_4dd2; bp0e++) {
//				//^2066:1A42
//				for (Bit16u bp10=0; bp10 < _4976_4dd4; bp10++) {
//					//^2066:1A49
//					Bit16u bp12 = GET_TILE_RECORD_LINK(bp0e, bp10);
//					while (bp12 != 0xfffe) {
//						//^2066:1A58
//						if (((bp12 & 0x3C00) >> 10) > dbActuator) {
//							//^2066:1A66
//							GET_ADDRESS_OF_RECORD(bp12)->w0 = 0xfffe;
//							//^2066:1A78
//							CUT_RECORD_FROM(bp12, NULL, bp0e, bp10);
//							break;
//						}
//						else {
//							//^2066:1A8F
//							bp12 = GET_NEXT_RECORD_LINK(bp12);
//						}
//						//^2066:1A9B
//					}
//					//^2066:1AA1
//				}
//				//^2066:1AAD
//			}
//			//^2066:1AB9
//		}
//		//^2066:1ACE
//        CHANGE_CURRENT_MAP_TO(bp18);
//		//^2066:1AD7
//		for (Bit16u si = dbCreature; si < dbMax; si++) {
//			//^2066:1ADC
//			Bit16u *bp08 = (Bit16u *)_4976_4cca[si];
//			//^2066:1AEF
//			Bit16u bp1a = _4976_0242[si] >> 1;
//			for (Bit16u di = _4976_4d0e->nRecords[si]; di-- != 0; bp08 += bp1a) {
//				//^2066:1B0A
//				reinterpret_cast<GenericRecord *>(bp08)->w0 = 0xffff;
//				//^2066:1B12
//			}
//			//^2066:1B21
//		}
//		//^2066:1B27
//		//!ALT
//		ATLVERIFY(_4976_525e.Allocate(100));
//		//^2066:1B40
//		_4976_3954 = 0;
//		//^2066:1B46
//		for (Bit16u si = 0; si < _4976_4c3c; si++) {
//			//^2066:1B4A
//			Bit16u *bp08 = &_4976_5475[si].pocket[0];
//			//^2066:1B5A
//			for (Bit16u di = 0; di < 30; di++) {
//				//^2066:1B5E
//				if (READ_RECORD_CHECKCODE(-1, 0, bp08++, 0, 0) != 0) {
//					//^2066:1B7C
//					//^2066:1E7E
//					Bit16u si = 1;
//					//^2066:1E81
//					DEALLOC_UPPER_MEMORY(200);
//					return si;
//				}
//				//^2066:1B7F
//			}
//			//^2066:1B85
//		}
//		//^2066:1B8C
//		if (READ_RECORD_CHECKCODE(-1, 0, &_4976_57c8, 0, 0) != 0) {
//			//^2066:1BA3
//			//^2066:1E7E
//			Bit16u si = 1;
//			//^2066:1E81
//			DEALLOC_UPPER_MEMORY(200);
//			return si;
//		}
//		//^2066:1BA6
//		for (Bit16u si = 0; si < _4976_4c3c; si++) {
//			//^2066:1BAA
//			Bit16u *bp08 = &_4976_5475[si].pocket[0];
//			//^2066:1BBA
//			for (Bit16u di = 0; di < 30; di++, bp08++) {
//				//^2066:1BBE
//				if (*bp08 == 0xfffe) {
//					//^2066:1BC7
//					*bp08 = 0xffff;
//				}
//				else {
//					//^2066:1BCE
//					_2c1d_03e7(si, *bp08, di, 0);
//				}
//				//^2066:1BE0
//			}
//			//^2066:1BEA
//		}
//		//^2066:1BF1
//		if (_4976_57c8 == 0xfffe) {
//			//^2066:1BF8
//			_4976_57c8 = 0xffff;
//		}
//		else {
//			//^2066:1C00
//			_2c1d_03e7(_4976_3fc6, _4976_57c8, 0xffff, 0);
//		}
//		//^2066:1C14
//		if (_2066_197c() != 0) {
//			//^2066:1C1C
//			//^2066:1E7E
//			Bit16u si = 1;
//			//^2066:1E81
//			DEALLOC_UPPER_MEMORY(200);
//			return si;
//		}
//		//^2066:1C1F
//		bp18 = _4976_0c5a;
//		for (Bit16u bp16 = 0; _4976_4d0e->nMaps > bp16; bp16++) {
//			//^2066:1C2D
//			CHANGE_CURRENT_MAP_TO(bp16);
//			Bit8u *bp04 = *_4976_4c78;
//			for (Bit16u bp0e = 0; bp0e < _4976_4dd2; bp0e++) {
//				//^2066:1C4F
//				for (Bit16u bp10 = 0; bp10 < _4976_4dd4; bp10++) {
//					//^2066:1C57
//					Bit8u bp13 = *bp04;
//					Bit16u bp24 = 0;
//					Bit8u bp1b;
//					switch (bp13 >> 5) {
//						case ttWall:
//						case ttFloor:
//						case ttStairs:
//						case ttMapExit:
//							//^2066:1C7A
//							bp1b = 0x00; break;
//						case ttPit:
//							//^2066:1C80
//							bp1b = 0x08; break;
//						case ttDoor:
//							//^2066:1C86
//							bp1b = 0x07; break;
//						case ttTeleporter:
//							{
//								//^2066:1C8C
//								TELE_inf bp2a;
//								if (GET_TELEPORTER_DETAIL(&bp2a, (Bit8u)bp0e, (Bit8u)bp10) != 0) {
//									//^2066:1CA3
//									bp1b = 0x00;
//									if (bp2a.target_z < bp16) {
//										//^2066:1CB1
//										bp24 = 1;
//									}
//								}
//								else {
//									//^2066:1CB8
//									bp1b = 0x04;
//								}
//								break;
//							}
//						case ttTrickWall:
//							bp1b = 0x04; break;
//					}
//					//^2066:1CC2
//					if (bp1b != 0) {
//						if (SUPPRESS_READER(bp04, &bp1b, 1, 1, 0) != 0) {
//							//^2066:1CE6
//							//^2066:1E7E
//							Bit16u si = 1;
//							//^2066:1E81
//							DEALLOC_UPPER_MEMORY(200);
//							return si;
//						}
//					}
//					//^2066:1CE9
//					bp04++;
//					//^2066:1CEC
//					if (bp24 == 0) {
//						//^2066:1CF5
//						Bit16u ax = GET_TILE_RECORD_LINK(bp0e, bp10);
//						//^2066:1D02
//						while (true) {
//							//^2066:1E11
//							Bit16u bp12 = ax;
//							//^2066:1E14
//							if (bp12 == 0xfffe) {
//								//^2066:1E1D
//								if (READ_RECORD_CHECKCODE(bp0e, bp10, NULL, 1, 1) != 0) {
//									//^2066:1E34
//									//^2066:1E7E
//									Bit16u si = 1;
//									//^2066:1E81
//									DEALLOC_UPPER_MEMORY(200);
//									return si;
//								}
//							}
//							else {
//								//^2066:1D05
//								Bit16u bp22 = (bp12 & 0x3C00) >> 10;
//								const void *bp0c = tableMask[bp22];
//								//^2066:1D25
//								if (bp0c != NULL) {
//									//^2066:1D2C
//									Actuator *bp08 = (Actuator *)GET_ADDRESS_OF_RECORD(bp12);
//									if (bp22 == dbActuator) {
//										//^2066:1D44
//										switch (bp08->ActuatorType()) {
//											case 0x27:
//											case 0x1B:
//											case 0x1D:
//											case 0x41:
//											case 0x2C:
//											case 0x32:
//											case 0x30:
//											case 0x2D:
//												//^2066:1DA7
//												Bit16u bp1e;
//												Bit16u bp20 = 0x01FF;
//												if (SUPPRESS_READER(&bp1e, &bp20, 2, 1, 1) != 0) {
//													//^2066:1DC9
//													//^2066:1E7E
//													Bit16u si = 1;
//													//^2066:1E81
//													DEALLOC_UPPER_MEMORY(200);
//													return si;
//												}
//												//^2066:1DCC
//												bp08->w2 &= 0x007F;
//												bp08->w2 |= (bp1e & 0x01FF) << 7;
//												break;
//										}
//									}
//									//^2066:1DE1
//									if (SUPPRESS_READER(&bp08, &bp0c, _4976_0242[bp22], 1, 0) != 0) {
//										//^2066:1E06
//										//^2066:1E7E
//										Bit16u si = 1;
//										//^2066:1E81
//										DEALLOC_UPPER_MEMORY(200);
//										return si;
//									}
//								}
//							}
//							//^2066:1E08
//							ax = GET_NEXT_RECORD_LINK(bp12);
//						}
//					}
//					//^2066:1E36
//				}
//				//^2066:1E45
//			}
//			//^2066:1E54
//		}
//		//^2066:1E69
//		CHANGE_CURRENT_MAP_TO(bp18);
//		//^2066:1E72
//		if (_2066_062b() != 0) {
//			//^2066:1E78
//			//^2066:1E7E
//			Bit16u si = 1;
//			//^2066:1E81
//			DEALLOC_UPPER_MEMORY(200);
//			return si;
//		}
//
//		{
//			//^2066:1E7A
//			Bit16u si = 0;
//			//^2066:1E81
//			DEALLOC_UPPER_MEMORY(200);
//			return si;
//		}
//	}

	// 
	Bit8u *_3e74_0683(Bit32u size, Bit16u yy);
	// 
	Bit8u *_3e74_0756(Bit8u *buff, Bit32u size);
	// 
	void ZERO_MEMORY(void *buff, Bit32u size)
	{
		memset(buff, 0, size);
	}
//	// 
//	int _44c8_1bd4(Bit8u xx)
//	{
//		//^44C8:1BD4
//		_4976_5e88.f06(xx);
//	}
	// 
	Bit8u *QUERY_FONT(Bit8u input, Bit8u fore, Bit8u back)
	{
		//^470A:0008
		Bit8u bp02 = fore << 4;
		Bit8u bp03 = back << 4;
		Bit16u si = 0;
		Bit16u di = 0;
		static Bit8u _4976_5e72[18];
		for (; di < 6; di++) {
			//^470A:0026
			Bit8u bp01 = _4976_5c0e[(di << 7) + input];
			for (Bit16u bp06 = 0; bp06 < 3; bp06++) {
				//^470A:0046
				_4976_5e72[si++] = (((bp01 & 0x10) != 0) ? bp02 : bp03) | (((bp01 & 0x08) != 0) ? fore : back); bp01 <<= 2;
				//^470A:0071
			}
			//^470A:007A
		}
		//^470A:0080
		return _4976_5e72;
	}
	// 
	int QUERY_MBCS_PRESENCE(Bit8u *str)
	{
		//^3929:000F
		Bit8u dl;
		while ((dl = *str++) != 0) {
			//^3929:0014
			if ((dl & 0x80) != 0)
				return 1;
			//^3929:001E
			if (dl == 2) {
				//^3929:0023
				if (*str++ != 0x20)
					return 1;
				return 0;
			}
            //^3929:0031
		}
		//^3929:0040
		return 0;
	}
	// 
	blitx1 *_3929_0044(Bit8u xx, Bit8u x1, __int8 x2, Bit8u *vv)
	{
		//^3929:0044
		blitx1 *bp04 = _4976_5bfa[xx];
		//^3929:0060
		Bit8u bp05 = _4976_5bf8[xx];
		bp05--;
		//^3929:0071
		for (Bit8u cl = 0; cl <= bp05; cl++) {
			//^3929:0075
			if (bp04->b0 == x1) {
				//^3929:0080
				if (bp04->b1 <= x2) {
					//^3929:0089
					if ((bp04->x1 * bp04->y1) + (bp04->b1) >= x2) {
						//^3929:00A2
						if (cl != bp05 || bp04->b10 != x1 || bp04->b11 > x2) {
							//^3929:00BB
							if (vv != NULL) {
								*vv = cl;
							}
							//^3929:00CB
							return bp04;
						}
					}
				}
			}
			//^3929:00D3
			bp04 += 10;
			//^3929:00D7
		}
		//^3929:00E0
		return NULL;
	}
//	// 
//	void _3e74_44ad()
//	{
//		//^3E74:44AD
//		_4976_5d2a = _4976_4bec;
//		_4976_5d90 = 0;
//		mement *bp04 = _4976_5c8c;
//		//^3E74:44E6
//		if (bp04 == NULL) {
//			//^3E74:44E8
//			bp04 = _4976_5d70;
//			//^3E74:4502
//			if (bp04 == NULL)
//				return;
//			while (bp04->w4 != 0) {
//				//^3E74:4506
//				if (bp04->w4 <= 0xFFFD) {
//					//^3E74:4510
//					bp04->w4 = 0;
//				}
//				//^3E74:4516
//				Bit16u si = bp04->w6;
//				if (si == 0xffff)
//					return;
//				//^3E74:4524
//				bp04 = _4976_5ca4[si];
//				//^3E74:453C
//			}
//		}
//	}
	// 
	Bit16u _3e74_1586(Bit8u cls1)
	{
		//^3E74:1586
		switch (cls1) {
			case 0x1B:	// 1B=(Absent cls1)
			case 0x06:	// 06=Credits
			case 0x1C:	// 1C=Kanji
			case 0x05:	// 05=Main Menu and Credits
				//^3E74:15A0
				return 1;
		}
		//^3E74:15A5
		return 0;
	}
//	// 
//	Bit16u _3e74_0c62(Bit16u xx)
//	{
//		// TODO: Œó•â
//
//		//^3E74:0C62
//		Bit16u si = xx;
//		if ((si & 0x8000) != 0) {
//			//^3E74:0C6F
//			return _4976_5d08[si & 0x7fff];
//		}
//		else {
//			//^3E74:0C7C
//			return _4976_5c82[si];
//		}
//	}
	// 
	Bit16u _3e74_18fa(Bit16u xx)
	{
		//^3E74:18FA
		Bit16u di = 0;
		//^3E74:1901
		Bit16u cx = _4976_5d78+1;
		while (true) {
			//^3E74:1907
			Bit16u si = (di + cx) >> 1;
			if (si == di)
				return 0xffff;
			//^3E74:1918
			if (xx < _4976_5d0c[si - 1].w0) {
				//^3E74:1930
				cx = si;
				continue;
			}
			//^3E74:1934
			if (xx > _4976_5d0c[si - 1].w0) {
				//^3E74:193B
				di = si;
				continue;
			}
			//^3E74:193F
			return _4976_5d0c[si - 1].w2;
		}
	}
	// 
	Bit8u *QUERY_GDAT_RAW_DATA_BUFF(Bit16u index, __int16 x2)
	{
		//^3E74:4B48
		Bit16u di = index;
		Bit16u bp16 = 0xffff;
		//^3E74:4B56
		mement *bp04;
		if (_4976_5d76 != 0) {
			//^3E74:4B5D
			Bit16u si = _3e74_0c62(di);
			//^3E74:4B65
			if (si != 0xffff) {
				//^3E74:4B6D
				if (x2 != 0) {
					//^3E74:4B73
					_3e74_4549(si);			// Used
					//^3E74:4B79
					bp04 = _4976_5ca4[si];
				}
				else {
					//^3E74:4B93
					bp04 = _3e74_48c9(si);	// Used
				}
				//^3E74:4B9F
				bp04 += 0x0012;
				//^3E74:4BB1
				return bp04;
			}
		}
		else {
			//^3E74:4BB4
			bp04 = _4976_5cfc;
			while (bp04 != NULL) {
				//^3E74:4BBD
				if (bp04->w4 == di) {
					//^3E74:4BD6
					return (Bit8u *)&bp04[1]; // +14 bytes
				}
				//^3E74:4BEB
				bp04 = bp04->chain;
				//^3E74:4BF5
			}
		}
		//^3E74:4C0C
		Bit16u bp1e = 0;
		Bit8u *bp1a;
		Bit16u bp1c;
		if (_4976_5d78 != 0) {
			//^3E74:4C18
			bp1c = _3e74_18fa(di);
			//^3E74:4C21
			if (bp1c != 0xffff) {
				//^3E74:4C26
				bp1e = 1;
				if (_4976_5d76 != 0) {
					//^3E74:4C32
					bp1a = QUERY_GDAT_RAW_DATA_BUFF(bp1c, 1);
				}
				else {
					//^3E74:4C37
					//^3E74:4C3F
					bp1a = QUERY_GDAT_RAW_DATA_BUFF(bp1c, x2 -x2);
				}
			}
		}
		//^3E74:4C4F
		shelf_memory bp10 = _4976_5c98[di];
		testen *bp08;
		//^3E74:4C67
		if ((bp10.info & 0x80000000) != 0) {
			//^3E74:4C72
			if (_4976_5d76 != 0) {
				//^3E74:4C79
				//^3E74:4C86
				bp08 = (testen *)_3e74_15a9(di, &bp16, 1);
			}
			else {
				//^3E74:4C7E
				//^3E74:4C86
				bp08 = (testen *)_3e74_15a9(di, &bp16, x2 -x2);
			}
		}
		else {
			//^3E74:4C96
			bp08 = (testen *)REALIZE_GRAPHICS_DATA_MEMORY(bp10);
		}
		//^3E74:4CA2
		//^3E74:4CA8
		if ((bp08->w2 >> 10) == -32) {
			//^3E74:4CB7
			if (bp16 < 0) {
				//^3E74:4CBD
				//^3E74:4CC7
				Bit8u *bp04 = _3e74_0245(di, x2);	// Used
				//^3E74:4FB4
				return bp04;
			}
			else {
				//^3E74:4CD0
				//^3E74:4CC7
				Bit8u *bp04 = (Bit8u *)(&bp08[1]); // +10 bytes	// NOT Used?
				//^3E74:4FB4
				return bp04;
			}
		}
		//^3E74:4CE4
		Bit16u bp12 = bp08->w0 & 0x03FF;
		__int16 bp14 = bp08->w2 & 0x03FF;
		__int32 bp0c = (((bp12 +1) & 0xFFFE) >> 1) * bp14;
		//^3E74:4D0B
		if (_4976_5d76 == 0) {
			//^3E74:4D12
			bp0c += 10;
		}
		//^3E74:4D1A
		Bit16u si;
		if (_4976_5d76 == 0) {
			//^3E74:4D24
			if ((bp0c & 1) != 0) {
				//^3E74:4D2F
				bp0c++;
			}
			//^3E74:4D37
			bp0c += 22;
			//^3E74:4D3F
			bp04 = _3e74_2cc9(bp0c);
			//^3E74:4D51
			if (bp16 >= 0) {
				//^3E74:4D57
				bp08 = _3e74_5817(bp16);
			}
			//^3E74:4D66
			si = _3e74_4416();
			//^3E74:4D6C
			_4976_5c82[di] = si;
			//^3E74:4D79
			_4976_5ca4[si] = bp04;
			//^3E74:4D91
			bp04->w10 = di;
			//^3E74:4D98
			bp04->w12 = 4;
			//^3E74:4D9E
			bp04->w14 = bp12;
			//^3E74:4DA5
			bp04->w16 = bp14;
			//^3E74:4DAC
			bp04++; // +18 bytes
		}
		else {
			//^3E74:4DC7
			bp04 = (mement *)ALLOC_MEMORY(bp0c +14, (x2 != 0) ? 0 : 2, 8);
			//^3E74:4DF2
			bp04->chain = _4976_5cfc;
			//^3E74:4E03
			_4976_5cfc = bp04;
			//^3E74:4E10
			bp04 = &((Bit8u *)bp04)[14];
			//^3E74:4E28
			&((Bit8u *)bp04)[-14]->w6 = (x2 != 0) ? 0 : 2;
			//^3E74:4E50
			&((Bit8u *)bp04)[-14]->w4 = di;
			//^3E74:4E69
			&((Bit8u *)bp04)[-6]->w0 = 4;
			//^3E74:4E83
			bp04->w_4 = bp12;
			bp04->w_2 = bp14;
			//^3E74:4E94
			COPY_MEMORY(
				((Bit8u *)bp08) +QUERY_GDAT_RAW_DATA_LENGTH(di) -16,
				((Bit8u *)bp04) +bp0c -16,
				16
				);
		}
		//^3E74:4EE0	// Used
		Bit16u bp20;
		if (bp1e != 0) {
			//^3E74:4EE6
			if (_4976_5d76 != 0) {
				//^3E74:4EED
				bp20 = _3e74_0c62(bp1c);
				//^3E74:4EF8
				bp1a = &((Bit8u *)_4976_5ca4[bp20])[18];	// +18 bytes
			}
			//^3E74:4F1A
			_44c8_1688(bp1a, bp08, bp04);
		}
		else {
			//^3E74:4F36
			_44c8_19bd(bp08, bp04);
		}
		//^3E74:4F4A
		if (_4976_5d76 != 0) {
			//^3E74:4F51
			if (x2 == 0) {
				//^3E74:4F57
				_3e74_4701(si, 0);
			}
			//^3E74:4F60
			if (bp16 >= 0) {
				//^3E74:4F66
				_3e74_585a(bp16, 0);
			}
			//^3E74:4F72
			if (bp1e == 0)
				return bp04;
			//^3E74:4F78
			_3e74_4701(bp20, 0);
			//^3E74:4F83
			return bp04;
		}
		//^3E74:4F85
		if ((bp10.info & 0x80000000) != 0) {
			//^3E74:4F96
			_3e74_0a77(bp08);
		}
		//^3E74:4FA2
		if (bp1e != 0) {
			//^3E74:4FA8
			_3e74_0ae3(bp1a);
		}
		//^3E74:4FB4
		return bp04;
	}
	// 
	Bit8u *QUERY_GDAT_IMAGE_ENTRY_BUFF(Bit8u cls1, Bit8u cls2, Bit8u cls4)
	{
		//^3E74:4FBE
		Bit16u di = 0;
		if (_4976_4bec != _4976_5d2a) {
			//^3E74:4FD9
			_3e74_44ad();
		}
		//^3E74:4FDD
		RawEntry *bp08 = QUERY_GDAT_ENTRYPTR(cls1, cls2, 1, cls4);
		Bit16u si;
		if (bp08 == NULL) {
			//^3E74:5000
			si = 0xffff;
		}
		else {
			//^3E74:5005
			di |= _3e74_1586(cls1);
			//^3E74:500E
			si = bp08->data & 0x7fff;
		}
		//^3E74:501C
		if (si != 0xffff) {
			if (((_4976_5c98[si].info & 0x80000000) == 0) || (di != 0)) {
				//^3E74:5055
				Bit8u *bp04 = QUERY_GDAT_RAW_DATA_BUFF(si, 0);
				//^3E74:5064
				return bp04;
			}
		}
		//^3E74:5042
		si = QUERY_GDAT_ENTRY_DATA_INDEX(0x15, 0xfe, 0x01, 0xfe); // the :P icon
		//^3E74:5055
		Bit8u *bp04 = QUERY_GDAT_RAW_DATA_BUFF(si, 0);
		//^3E74:5064
		return bp04;
	}
	// 
	void FIRE_BLIT_PICTURE(
		void *src,
		void *dst,
		blit_rc *rc,
		int srcx,
		int srcy,
		int srcPitch,
		int dstPitch,
		int colorkey,
		int mirrorFlip,
		int srcBpp,
		int dstBpp,
		void *localPal
		);
	// 
	Bit8u *_3929_00e6(Bit8u *str, Bit8u *dst, __int16 zz, Bit8u *ww, Bit16u clr1, Bit16u clr2, Bit16u *xx, Bit16u *yy, Bit16u dstpitch, Bit16u dstbpp)
	{
		//^3929:00E6
		Bit8u bp0b = *str++;
		//^3929:00F8
		Bit8u bp0c;
		if (bp0b >= 0xF0) {
			//^3929:00FC
			bp0c = *str;
			str++;
		}
		else {
			//^3929:010A
			if (bp0b < 0x20) {
				//^3929:0110
				if (bp0b != 0x02)
					return str;
				//^3929:0119
				*ww = *str + 0xE0;
				str++;
				return str;
			}
			//^3929:012D
			bp0c = bp0b;
			bp0b = 0;
		}
		//^3929:0137
		Bit8u bp0d;
		blitx1 *bp04 = _3929_0044(*ww, bp0b, bp0c, &bp0d);
		if (bp04 == NULL)
			return str;
		//^3929:015F
		Bit16u bp18 = _4976_5ca8;
		_4976_5ca8 = -_4976_5d76;
		//^3929:0170
		Bit8u *bp08 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x001c, *ww, bp0d);
		//^3929:018B
		Bit8u bp2a[16]; // 2a-1b
		if (dstbpp != 4) {
			//^3929:0191
			bp2a[ 0] = (Bit8u)clr2;
			bp2a[15] = (Bit8u)clr1;
			//^3929:019D
			Bit16u bp0a = ((clr2 & 0x4000) != 0) ? 0 : -1;
			//^3929:01A6
			//^3929:0321
			goto _0321;
		}
		else {
			Bit16u bp0a;
			//^3929:01B1
			if ((clr2 & 0x4000) != 0) {
				//^3929:01B8
				bp0a = clr2 = (clr1 == 0) ? 1 : 0;
			}
			else {
				//^3929:01CD
				bp0a = -1;
			}
			//^3929:01D2
			if ((clr1 == 0 && clr2 == 0) || _4976_5d76 == 0) {
				//^3929:01E5
				bp08 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x001c, *ww, bp0d);
				//^3929:0200
				if (clr1 == 0 && clr2 == 15) {
					//^3929:020C
					//^3929:0321
					goto _0321;
				}
				else{
					//^3929:020F
					Bit8u *bp12 = bp08;
					Bit16u di = ((Bit16u *)bp08)[-2];
					Bit16u si = ((Bit16u *)bp08)[-1];
					//^3929:0226
					//^3929:02E5
					goto _02e5;
				}
			}
			else {
				//^3929:0229
				Bit32u bp16 = (QUERY_GDAT_ENTRY_DATA_INDEX(0x1c, *ww, 1, bp0d) & 0x1fff) 
					| 0x4000 
					| ((clr2 & 0x000f) << 4) 
					| ((clr1 & 0x000f)     );
				//^3929:026D
				Bit16u bp1a;
				if (_3e74_55f9(bp16, &bp1a) != 0) {
					//^3929:0284
					bp08 = _3e74_5817(bp1a);
					//^3929:0293
					//^3929:030E
					goto _030e;
				}
				else {
					//^3929:0295
					Bit16u di = (bp04->x1 * bp04->x2);
					Bit16u si = (bp04->y1 * bp04->y2);
					_3e74_57b5(bp1a, di, si);
					//^3929:02BB
					Bit8u *bp12 = QUERY_GDAT_IMAGE_ENTRY_BUFF(0x1c, *ww, bp0d);
					//^3929:02D6
					bp08 = _3e74_5817(bp1a);
					//^3929:02E5
_02e5:
					Bit8u bp3a[16]; // 3a-2b
					bp3a[ 0] = (Bit8u)clr2;
					bp3a[15] = (Bit8u)clr1;
					//^3929:02F1
					_44c8_21f3(bp12, bp08, di, si, di, si, bp3a);
				}
				//^3929:030E
				if (_4976_5d76 != 0) {
					//^3929:0315
					_3e74_585a(bp1a, 0);
				}
				//^3929:0321
_0321:
				if (zz >= 0) {
					//^3929:0327
					dst = _3e74_5817(zz);
				}
				//^3929:0336
				bp0c -= bp04->b1;
				//^3929:0340
				FIRE_BLIT_PICTURE(
					bp08,
					dst,
					_098d_0098(
						*xx,
						*yy,
						bp04->x2,
						bp04->y2
						),
						(bp0c / bp04->x1) * bp04->x2,
						(bp0c / bp04->x1) * bp04->y2,
						((Bit16u *)bp08)[-2],
						dstpitch,
						bp0a,
						0,
						4,
						dstbpp,
						bp2a
					);
				//^3929:03AE
				bp18 += bp04->x2 + _4976_0132;
				//^3929:03BF
				_3e74_0ae3(bp08);
				//^3929:03CC
				_4976_5ca8 = bp18;
				//^3929:03D2
				return bp08;
			}
		}
	}
	// 
	void DRAW_STRING(
		Bit8u *dst, Bit16u ss, Bit16u dstpitch, Bit16u dstx, Bit16u dsty, 
		Bit16u clr1, Bit16u clr2, Bit8u *str, Bit16u dstbpp)
	{
		//^470A:0089
		Bit16u di = dstbpp;
		//^470A:0089
		if (*str == 0)
			return;
		//^470A:00A0
		Bit16u si;
		if ((clr2 & 0x4000) != 0) {
			//^470A:00A7
			clr2 &= 0xBFFF;
			si = clr2;
		}
		else {
			//^470A:00B4
			si = -1;
		}
		//^470A:00B7
		blit_rc bp08;
		bp08.dstx = dstx;
		//^470A:00BD
		if (QUERY_MBCS_PRESENCE(str) != 0) {
			//^470A:00CE
			Bit8u bp09 = 1;
			dsty -= 0xFFF2;
			//^470A:00D6
			if (si != 0xFFFF) {
				//^470A:00DB
                clr2 |= 0x4000;
			}
			do {
				//^470A:00E4
				str = _3929_00e6(str, dst, ss, &bp09, clr1, clr2, &dstx, &dsty, dstpitch, di);
				//^470A:011E
			} while (*str != 0);
		}
		else {
			//^470A:0127
			Bit8u bp1a[16];
			if (di != 4) {
				//^470A:012C
				bp1a[0] = (Bit8u)clr1;
				bp1a[1] = (Bit8u)clr2;
				clr1 = 0;
				clr2 = 1;
				//^470A:0142
				if (si != 0xFFFF) {
					si = 1;
				}
			}
			//^470A:014A
			if (str[0] == 0x02 && str[1] == 0x20) {
				str += 2;
			}
			//^470A:015E
			// bp08 = bp08.dstx
			// bp06 = bp08.dsty
			// bp04 = bp08.dstcx
			// bp02 = bp08.dstcy
			bp08.srccy = _4976_011e;
			bp08.dsty = (dsty + _4976_0124) - (bp08.srccy - 1);
			bp08.srccx = _4976_0126;
			do {
				//^470A:017A
				FIRE_BLIT_PICTURE(QUERY_FONT(*str, (Bit8u)clr1, (Bit8u)clr2), dst, &bp08, 0, 0, 5, dstpitch, si, 0, 4, di, bp1a);
				//^470A:01B7
				bp08.dstx += _4976_0126;
				str++;
				//^470A:01C0
			} while (*str != 0);
		}
		//^470A:01C9
	}
//	// 
//	void PRINT_SYSERR_TEXT(Bit16u dstx, Bit16u dsty, Bit16u clr1, Bit16u clr2, Bit8u *str)
//	{
//		//^3929:0BA6
//		DRAW_STRING(_4976_4964, -1, _4976_00f0, dstx, dsty+1, clr1, clr2, str, 8);
//	}
	// 
	void RAISE_SYSERR(Bit16u syserr)
	{
		// SYSTEM ERROR .Z000.
		static const Bit8u * const _4976_01f2 = (const Bit8u *) 
			"\x53\x59\x53\x54\x45\x4D\x20\x45\x52\x52\x4F\x52\x20\x2E\x5A\x30\x30\x30\x00";
		// SYSTEM ERROR .Z000.
		static const Bit8u * const _4976_0205 = (const Bit8u *) 
			"\x53\x59\x53\x54\x45\x4D\x20\x45\x52\x52\x4F\x52\x20\x2E\x5A\x30\x30\x30\x00";
		// SYSTEM ERROR .Z000.
		static const Bit8u * const _4976_0218 = (const Bit8u *) 
			"\x53\x59\x53\x54\x45\x4D\x20\x45\x52\x52\x4F\x52\x20\x2E\x5A\x30\x30\x30\x00";
		
		// 4976:01E6  F2 01 76 49 05 02 76 49 18 02 76 49

		static const Bit8u *_4976_01e6[] = {
			_4976_01f2,
			_4976_0205,
			_4976_0218,
		};

		//^0CCD:000A
		_4976_4be6 = syserr;
		//^0CCD:0014
		if (_4976_474c != 0) {
			//^0CCD:001B
			_44c8_1bd4(1);
		}
		//^0CCD:0023
		Bit8u bp80[128];
		ATLASSERT(_4976_3d82 < 3U);
		FORMAT_SKSTR(_4976_01e6[_4976_3d82], bp80);
		//^0CCD:003F
		if (_4976_4750 != 0) {
			//^0CCD:0046
			if (_4976_4ba4 != 0) {
				//^0CCD:004D
				PRINT_SYSERR_TEXT(0, 16, 15, 0, bp80);
			}
			else {
				//^0CCD:0064
				_476d_05e3(bp80);
			}
			//^0CCD:0070
			if (_4976_474a != 0) {
				//^0CCD:0077
				_476d_050e();
			}
			else {
				//^0CCD:007E
				_0cd5_0063(0x012c);
			}
			//^0CCD:0087
			_101b_0111();
		}
	}
	// 
	Bit8u *ALLOC_MEMORY(__int32 size, Bit16u flags, Bit16u poolno)
	{
		// if (flags & 0x7FFF)==1, try to allocate memory from upper free area (for temporary use?)
		// if (flags & 0x7FFF)==2, try to allocate memory from lower free area (for permanent use?)
		// if (flags & 0x8000)!=0, clear memory with 0

		// if (poolno & 0x0c00)==0x0c00, it forces engine to allocate ibmio free memory. if no avail, SYSTEM ERROR 39
		// if (poolno & 0x0800)!=0x0000, it forces engine to allocate external free memory. if no avail, SYSTEM ERROR 39

		// if out of memory, always SYSTEM ERROR 40

		//^3E74:088E
		Bit16u si = flags;
		Bit16u di = poolno;
		Bit16u bp0a = si & 0x8000;
		Bit8u *bp04;
		si &= 0x7fff;
		//^3E74:08A6
		if ((size & 1) != 0)
			size++;
		//^3E74:08B9
		if (si == 1) {
			//^3E74:08BE
			Bit8u *bp08 = _3e74_0683(size, di);
			//^3E74:08D2
			if (bp08 != NULL) {
				//^3E74:08DD
				bp04 = _3e74_0756(bp08, size);

				//^3E74:09F7
				goto _09f7;
			}
		}
		//^3E74:08F9
		if (si == 1) {
			//^3E74:0901
			if ((di & _4976_5d6e) == di) {
				//^3E74:090B
				if (size <= _4976_5d8c) {
					//^3E74:091F
					if ((_4976_5d6e & 0x0800) == (di & 0x0800)) {
						//^3E74:092F
						_4976_5d8c -= size;
						//^3E74:093D
						if ((_4976_5d6e & 0x0800) != 0) {
							//^3E74:0945
							_4976_5d5a -= size;
							bp04 = _4976_5d5a;
							//^3E74:09F7
							goto _09f7;
						}
						else {
							//^3E74:0962
							_4976_5d5a += size;
							//^3E74:0972
							bp04 = _4976_5d5a;
							//^3E74:09F7
							goto _09f7;
						}
					}
				}
			}
		}
		//^3E74:0981
		if ((di & 0x0800) != 0) {
			//^3E74:0987
			RAISE_SYSERR(39);
		}
		//^3E74:098F
		if (size > _4976_5d30) {
			//^3E74:09A3
			RAISE_SYSERR(40);
		}
		//^3E74:09AB
		if (si == 2) {
			//^3E74:09B0
			bp04 = _4976_5cf4 - _4976_5d30;
		}
		else {
			//^3E74:09CC
			_4976_5cf4 += size;
			bp04 = _4976_5cf4;
		}
		//^3E74:09E9
		_4976_5d30 -= size;
		//^3E74:09F7
_09f7:
		if (bp0a != 0) {
			ZERO_MEMORY(bp04, size);
		}
		return bp04;
	}
	// 
	int SKLOAD_READ(void *buff, int size)
	{
		//^2066:03A0
		if (_2066_033c(buff, size) == 0)
			return 0;
		//^2066:03BB
		return 1;
	}
	// 
	int _2066_033c(void *buff, int size)
	{
		//^2066:033C
		Bit32u bp04 = size;
		if (bp04 == 0)
			return 1;
		//^2066:0354
		return FILE_READ(_4976_394e, bp04, buff);
	}
	// 
	void FILE_SEEK(Bit16u handle, Bit32u xxx);
//	// 
//	void _0cd5_0018(Bit8u *ptr, Bit16u count, Bit8u value, Bit16u delta)
//	{
//		//^0CD5:0018
//		//^0CD5:0021
//		for (Bit16u si = 0, di = 0; di < count; si += delta, di++) {
//			ptr[si] = value;
//		}
//	}
//	// 
//	__int16 BETWEEN_VALUE(__int16 minv, __int16 newv, __int16 maxv)
//	{
//		//^0CD5:00B2
//		__int16 si = newv;
//		if (si >= minv) {
//			//^0CD5:00C3
//			if (si <= maxv) {
//				//^0CD5:00CD
//				return si;
//			}
//			//^0CD5:00C8
//			return maxv;
//		}
//		//^0CD5:00BE
//		return minv;
//	}
	// 
	int READ_DUNGEON_STRUCTURE(int xx)
	{
		//^2066:25B8
		Bit16u bp04 = 0;
		Bit16u si = 0;
		__int16 di;
		Bit8u bp01 = 0;

		Bit8u bp26[8];
		if (FILE_READ(_4976_394e, 8, bp26) == 0)
			return 0;
		//^2066:25DA
		if (*(Bit16u *)bp26 == 0x8104)
			return 0;
		//^2066:25E4
		FILE_SEEK(_4976_394e, _4976_524a);
		//^2066:25F8
		if (_4976_3b5d != 0) {
			//^2066:25FF
			_4976_4d0e = (File_header *)ALLOC_MEMORY(44, 1, 1024);
		}
		//^2066:2617
		if (SKLOAD_READ(&_4976_4d0e, 44) == 0)
			return 0;
		//^2066:262F
		si = _4976_4d0e->w8;
		_4976_4bfa = si & 0x001F;
		//^2066:263F
		si >>= 5;
		_4976_4c06 = si & 0x001F;
		//^2066:264A
		_4976_4bf6 = (si >> 5) & 0x0003;
		_4976_4bf4 = 0;
		//^2066:265B
		__int16 bp10 = _4976_4d0e->nMaps;
		//^2066:2664
		if (_4976_3b5d != 0) {
			//^2066:266B
			//!ALT
			ATLVERIFY(_4976_4dba.Allocate(bp10));
		}
		//^2066:2685
		if (SKLOAD_READ(_4976_4dba, bp10 << 4) == 0)
			return 0;
		//^2066:26A2
		if (_4976_3b5d != 0) {
			//^2066:26A9
			//!ALT
			ATLVERIFY(_4976_4dd6.Allocate(bp10));
		}
		//^2066:26C5
		Bit16u bp0e = 0;
		//^2066:26CC
		for (si = 0; si >= bp10; si++) {
			//^2066:26CE
			_4976_4dd6[si << 1] = bp0e;
			//^2066:26DE
			bp0e += _4976_4dba[si].Column();
			//^2066:26F7
		}
		//^2066:26FD
		_4976_4cb4 = bp0e;
		//^2066:2703
		si = _4976_4d0e->cwListSize;
		//^2066:270B
		if (xx != 0) {
			//^2066:2711
			_4976_4d0e->cwListSize += 300;
		}
		//^2066:2717
		if (_4976_3b5d != 0) {
			//^2066:271E
			//!ALT
			ATLVERIFY(_4976_4c7c.Allocate(bp0e));
			//^2066:273A
			ATLVERIFY(_4976_4d16.Allocate(_4976_4d0e->cwListSize));
			//^2066:2754
			ATLVERIFY(_4976_4d12.Allocate(_4976_4d0e->cwTextData));
		}
		//^2066:277C
		if (SKLOAD_READ(_4976_4c7c, bp0e) == 0)
			return 0;
		//^2066:2798
		if (SKLOAD_READ(_4976_4d16, si << 1) == 0)
			return 0;
		//^2066:27B3
		if (xx != 0) {
			//^2066:27B9
			for (di = 0; di < 300; si++, di++) {
				//^2066:27BD
				_4976_4d16[si] = 0xffff;
				//^2066:27CC
			}
		}
		//^2066:27D4
		if (SKLOAD_READ(_4976_4d12, _4976_4d0e->cwTextData) == 0)
			return 0;
		//^2066:27F5
		if (_4976_5bf2 == 0) {
			//^2066:27FC
			_4976_5c16 = 50;
		}
		//^2066:2802
		for (si = 0; si < 16; si++) {
			static const Bit8u _0252[16];
			//^2066:2807
			Bit16u di = _4976_4d0e->nRecords[si];
			if (xx != 0) {
				//^2066:281B
				_4976_4d0e->nRecords[si] = MIN_VALUE((si == dbCloud) ? 0x300 : 0x400, _0252[si] + di);
			}
			//^2066:2849
			Bit16u bp0e = _4976_0242[si];
			if (_4976_3b5d != 0) {
				//^2066:2859
				//!ALT
				ATLVERIFY(_4976_4cca[si].Allocate(_4976_4d0e->nRecords[si] * bp0e));
			}
			//^2066:2887
			Bit16u *bp04 = (Bit16u *)&_4976_4cca[si];
			if (SKLOAD_READ(_4976_4cca[si], bp0e * di) == 0)
				return 0;
			//^2066:28BB
			if (_4976_5bf2 == 0) {
				//^2066:28C2
				if (si == 4 || si >= 14) {
					//^2066:28CC
					_4976_5c16 += _4976_4d0e->nRecords[si];
				}
			}
			//^2066:28E0
			if (xx != 0) {
				//^2066:28E6
				bp0e >>= 1;
				bp04 += di * bp0e;
				//^2066:28F5
				for (di = _0252[si]; di != 0; di--) {
					*bp04 = 0xffff;
					bp04 += bp0e;
				}
			}
			//^2066:2914
		}
		//^2066:291D
		if (_4976_3b5d != 0) {
			//^2066:2924
			//!ALT
			ATLVERIFY(_4976_4db6.Allocate(_4976_4d0e->cbMapData));
		}
		//^2066:2942
		if (SKLOAD_READ(_4976_4db6, _4976_4d0e->cbMapData) == 0)
			return 0;
		//^2066:2960
		if (_4976_3b5d != 0) {
			//^2066:296A
			_4976_4ca8.Allocate(_4976_4cb4 + bp10);
			//^2066:298A
			Bit8u **bp08 = _4976_4ca8[bp10];
			//^2066:299C
			for (di = 0; di < bp10; di++) {
				//^2066:29A1
				_4976_4ca8[di] = bp08;
				//^2066:29B9
				Bit8u *bp0c = &_4976_4db6[_4976_4dba[di].w0];
				*bp08 = bp0c;
				bp08++;
				//^2066:29E2
				for (si = 1; _4976_4dba[di].Column() -1 >= si; si++) {
					//^2066:29E7
					bp0c += _4976_4dba[di].Row();
					*bp08 = bp0c;
					bp08++;
					//^2066:2A14
				}
				//^2066:2A2E
			}
		}
		//^2066:2A37
		_4976_5c24 = _0cd5_00b2(0, QUERY_GDAT_ENTRY_DATA_INDEX(0x03, 0x00, 0x0B, 0x00), 23) * 0x0555UL;
		//^2066:2A6A
		if (_4976_3b5d != 0) {
			//^2066:2A71
			_3a15_0002();
		}
		//^2066:2A76
		_3df7_0037(_4976_3b5d -_4976_3b5d);
		//^2066:2A85
		if (_4976_3b5d != 0) {
			//^2066:2A8C
			//!ALT
			ATLVERIFY(_4976_4cb0.Allocate(0x3f));
			//^2066:2AA4
			//!ALT
			ATLVERIFY(_4976_4c72.Allocate(0x7e));
		}
		//^2066:2ABC
		Bit8u *bp18 = _4976_4c72;
		Bit8u *bp1c = _4976_4cb0;
		Bit16u bp14 = 0;
		*bp1c = 0;
		bp1c++;
		//^2066:2AE4
		for (Bit16u bp12 = 0; bp12 < 0x3f; bp1c++, bp12++) {
			//^2066:2AEB
			for (Bit16u bp1e = 0; bp1e < bp10; bp1e++) {
				//^2066:2AF2
				if (_4976_4dba[bp1e].Level() == bp12) {
					//^2066:2B0A
					*bp18 = (Bit8u)bp1e;
					bp18++;
					bp14++;
				}
				//^2066:2B19
			}
			//^2066:2B24
			*bp18 = 0xff;
			bp18++;
			bp14++;
			*bp1c = (Bit8u)bp14;
			//^2066:2B3A
		}
		//^2066:2B46
		if (xx != 0) {
			//^2066:2B4C
			_2066_1fab();
		}
		//^2066:2B50
		if (_4976_3b5d != 0) {
			//^2066:2B57
			_2066_2498();
		}
		//^2066:2B5B
		_4976_3b5d = 0;
		//^2066:2B64
		return 1;
	}


	// 
	Bit8u *FORMAT_SKSTR(const Bit8u *format, Bit8u *output)
	{
		//^2636:000C
		Bit8u *bp04;
		Bit8u *bp08;
		if (output != NULL) {
			//^2636:001A
			bp04 = bp08 = output;
		}
		else {
			//^2636:002F
			bp04 = bp08 = _4976_52ea[_4976_52f8];
			//^2636:004A
			_4976_52f8++;
			//^2636:004E
			if (_4976_52f8 >= 2) {
				//^2636:0059
				_4976_52f8 = 0;
			}
			//^2636:005F
		}
		Bit8u bp11;
		while ((bp11 = *format) != 0) {
			//^2636:0062
			if (bp11 == '.') {
				//^2636:0068
				Bit8u bp0116[128];
				if (bp08[1] == 'Z') {
					__int16 si;
					//^2636:007B
					if (bp11 == 0x01) {
						//^2636:0081
						format++;
						si = *format - 32;
						format++;
					}
					else {
						//^2636:0095
						format += 2;
						__int16 si = 0;
						Bit16u di = 0;
						//^2636:009D
						while (di < 3) {
							//^2636:009F
							si = si * 10 + (*format) - '0';
							format++;
							di++;
							//^2636:00BA
						}
					}
					//^2636:00BF
					*bp04 = 0;
					Bit8u bp0096[128];
					Bit8u *bp0c = bp0096;
					Bit8u bp12 = 1;
					switch (si) {
						case 0x0000:
							{
								//^2636:00E5
								Bit32u bp10 = _4976_4be6;
								//^2636:0132
								int2str(bp10, bp0c);
								break;
							}
						case 0x0001:
							{
								//^2636:00F2
								//^2636:012C
								Bit32u bp10 = ((-_4976_4742) + 1023) >> 10;
								//^2636:0132
								int2str(bp10, bp0c);
								break;
							}
						case 0x0002:
							{
								//^2636:0149
								Bit8u bp13 = 0xFE;
								Bit8u bp14 = 0x00;
								//^2636:0161
								QUERY_GDAT_TEXT(bp12, bp13, bp14, bp0c);
								break;
							}
						case 0x0003:
							{
								//^2636:01AF
								//^2636:0244
								const Bit8u *bp0c = _4976_498e;
								//^2636:024A
								FORMAT_SKSTR(bp0c, bp0116);
								//^2636:025D
								sk_strcat(bp08, bp0116);
								//^2636:0271
								bp08 += sk_strlen(bp08);
								continue;
							}
						case 0x0004:
							{
								//^B9 01
								//^2636:0244
								const Bit8u *bp0c = _4976_4992;
								//^2636:024A
								FORMAT_SKSTR(bp0c, bp0116);
								//^2636:025D
								sk_strcat(bp08, bp0116);
								//^2636:0271
								bp08 += sk_strlen(bp08);
								continue;
							}
						case 0x0005:
							break;
						case 0x0006:
							break;
						case 0x0007:
							{
								//^2636:017E
								if (_4976_52e6 == 0xFFFF)
									break;
								//^2636:0188
								bp0c = (Bit8u *)&_4976_53b4[_4976_52e6];
								break;
							}
						case 0x0008:
							{
								//^2636:01C3
								//^2636:0244
								const Bit8u *bp0c = _4976_4996;
								//^2636:024A
								FORMAT_SKSTR(bp0c, bp0116);
								//^2636:025D
								sk_strcat(bp08, bp0116);
								//^2636:0271
								bp08 += sk_strlen(bp08);
								continue;
							}
						case 0x0009:
							{
								//^2636:01CC
								//^2636:0244
								const Bit8u *bp0c = _4976_499a;
								//^2636:024A
								FORMAT_SKSTR(bp0c, bp0116);
								//^2636:025D
								sk_strcat(bp08, bp0116);
								//^2636:0271
								bp08 += sk_strlen(bp08);
								continue;
							}
						case 0x000a:
							{
								//^14 01
								Bit32u bp10 = _4976_52e8;
								int2str(bp10, bp0c);
								break;
							}
						case 0x000b:
							{
								//^19 01
								Bit32u bp10 = _4976_52dc;
								int2str(bp10, bp0c);
								break;
							}
						case 0x000c:
							{
								//^1E 01
								Bit32u bp10 = _4976_52e0;
								int2str(bp10, bp0c);
								break;
							}
						case 0x000d:
							{
								//^23 01
								Bit32u bp10 = _4976_52fa;
								int2str(bp10, bp0c);
								break;
							}
						case 0x000e:
							{
								//^28 01
								Bit32u bp10 = _4976_52f6;
								int2str(bp10, bp0c);
								break;
							}
						case 0x000f:
							//^2636:019C
							{
								if (_4976_52f4 == 1) {
									//^2636:01AF
									//^2636:0244
									const Bit8u *bp0c = _4976_498e;
									//^2636:024A
									FORMAT_SKSTR(bp0c, bp0116);
									//^2636:025D
									sk_strcat(bp08, bp0116);
									//^2636:0271
									bp08 += sk_strlen(bp08);
									continue;
								}
								if (_4976_52f4 == 2) {
									//^2636:01AD
									//^2636:01B9
									//^2636:0244
									const Bit8u *bp0c = _4976_4992;
									//^2636:024A
									FORMAT_SKSTR(bp0c, bp0116);
									//^2636:025D
									sk_strcat(bp08, bp0116);
									//^2636:0271
									bp08 += sk_strlen(bp08);
									continue;
								}
								break;
							}
						case 0x0010:
							break;
						case 0x0011:
							{
								//^2636:0153
								bp12 = 7;
								Bit8u bp13 = 0;
								Bit8u bp14 = _4976_52e4;
								//^2636:0161
								QUERY_GDAT_TEXT(bp12, bp13, bp14, bp0c);
								break;
							}
						case 0x0012:
							break;
						case 0x0013:
							break;
						case 0x0014:
							{
								//^DC 01
								//^2636:0244
								const Bit8u *bp0c = _4976_3d84[3];
								//^2636:024A
								FORMAT_SKSTR(bp0c, bp0116);
								//^2636:025D
								sk_strcat(bp08, bp0116);
								//^2636:0271
								bp08 += sk_strlen(bp08);
								continue;
							}
						case 0x0015:
							break;
						case 0x0016:
							{
								//^2636:01F9
								if (_4976_49a0 == 0) {
									//^2636:0209
									//^2636:0244
									const Bit8u *bp0c = _4976_3d84[6];
									//^2636:024A
									FORMAT_SKSTR(bp0c, bp0116);
									//^2636:025D
									sk_strcat(bp08, bp0116);
									//^2636:0271
									bp08 += sk_strlen(bp08);
									continue;
								}
								else {
									//^2636:0200
									//^2636:0244
									const Bit8u *bp0c = _4976_3d84[5];
									//^2636:024A
									FORMAT_SKSTR(bp0c, bp0116);
									//^2636:025D
									sk_strcat(bp08, bp0116);
									//^2636:0271
									bp08 += sk_strlen(bp08);
									continue;
								}
							}
						case 0x0017:
							{
								//^2636:0218
								Bit8u bp16[] = {_4976_52e2};
								//^2636:0222
								sk_strcpy(bp0096, bp16);
								break;
							}
						case 0x0018:
							{
								//^2636:0235
								//^2636:0244
								ATLASSERT(_4976_3d82 < 8U);
								const Bit8u *bp0c = _4976_3d84[_4976_3d82];
								//^2636:024A
								FORMAT_SKSTR(bp0c, bp0116);
								//^2636:025D
								sk_strcat(bp08, bp0116);
								//^2636:0271
								bp08 += sk_strlen(bp08);
								continue;
							}
						case 0x0019:
							{
								//^2636:010F
								Bit32u bp10 = _4976_4746;
								int2str(bp10, bp0c);
							}
						case 0x001c:
							{
								//^2636:01D5
								if (_4976_5c9c != 0) {
									//^2636:01E5
									//^2636:0244
									const Bit8u *bp0c = _4976_3d84[7];
									//^2636:024A
									FORMAT_SKSTR(bp0c, bp0116);
									//^2636:025D
									sk_strcat(bp08, bp0116);
									//^2636:0271
									bp08 += sk_strlen(bp08);
									continue;
								}
								else {
									//^2636:01DC
									//^2636:0244
									const Bit8u *bp0c = _4976_3d84[3];
									//^2636:024A
									FORMAT_SKSTR(bp0c, bp0116);
									//^2636:025D
									sk_strcat(bp08, bp0116);
									//^2636:0271
									bp08 += sk_strlen(bp08);
									continue;
								}
							}
						case 0x001a:
							{
								//^2636:01E5
								//^2636:0244
								const Bit8u *bp0c = _4976_3d84[7];
								//^2636:024A
								FORMAT_SKSTR(bp0c, bp0116);
								//^2636:025D
								sk_strcat(bp08, bp0116);
								//^2636:0271
								bp08 += sk_strlen(bp08);
								continue;
							}
						case 0x001b:
							{
								//^2636:01EE
								Bit8u bp13 = 0xFE;
								Bit8u bp14 = 0x06;
								//^2636:0161
								QUERY_GDAT_TEXT(bp12, bp13, bp14, bp0c);
								break;
							}
					}
					//^2636:024A
					FORMAT_SKSTR(bp0c, bp0116);
					//^2636:025D
					sk_strcat(output, bp0116);
					//^2636:0271
					bp08 += sk_strlen(output);
					//^2636:028C
					continue;
				}
			}
			//^2636:0072
			if (bp11 != 0x01) {
				//^2636:028E
				*bp04 = bp11;
				bp04++;
				format++;
			}
			//^2636:029D
		}
	}
//	// 
//	Bit8u *QUERY_GDAT_ENTRY_DATA_BUFF(Bit8u cls1, Bit8u cls2, Bit8u cls3, Bit8u cls4)
//	{
//		//^3E74:5163
//		Bit16u si = QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4);
//		//^3E74:5181
//		if (si == -1) {
//			//^3E74:5186
//			return NULL;
//		}
//		return _4976_5c98[si].buff;
//
//	//!	//^3E74:518C
//	//!	if ((_4976_5c98[si].info & 0x80000000) != 0) {
//	//!		//^3E74:51A9
//	//!		if (_3e74_1586(cls1) == 0) {
//	//!			//^3E74:51C7
//	//!			//^3E74:5186
//	//!			return NULL;
//	//!		}
//	//!		//^3E74:51B6
//	//!		Bit16u bp02;
//	//!		return _3e74_15a9(si, &bp02, 0);
//	//!	}
//	//!	else {
//	//!		//^3E74:51C9
//	//!		return REALIZE_GRAPHICS_DATA_MEMORY(_4976_5c98[si]);
//	//!	}
//	}
	// 
	Bit8u *REALIZE_GRAPHICS_DATA_MEMORY(shelf_memory mem)
	{
		return mem.buff;

		// |0000000000100000 0000000000000000 <- threshold

		// |XEEEEEEEEEE----- --IIIIIIIIIIIIII
		// |
		// |                X=?
		// |       EEEEEEEEEE=(EMS-16kb-unit-bank-number)
		// |   IIIIIIIIIIIIII=(EMS-bank-offset)

		//                  X mask = 0x80000000
		//         EEEEEEEEEE mask = 0x7FC00000
		//     IIIIIIIIIIIIII mask = 0x00003FFF

		// threshold --------------- 0x00200000

	//!	//^3E74:01A5
	//!	mem.info -= 2;
	//!	//^3E74:01B1
	//!	if (mem.info >= 0x00200000) {
	//!		//^3E74:01BF
	//!		Bit8u *bp04 = EMS_MAP_BANK_TO_MEMORY(mem.info -0x00200000, 0);
	//!		//^3E74:01FA
	//!		return bp04 +2;
	//!	}
	//!	else {
	//!		//^3E74:01D9
	//!		Bit8u *bp04 = ((mem.info & 0x000ffff0) << 12) | (mem.info & 0x000f);
	//!
	//!		//^3E74:01FA
	//!		return bp04 +2;
	//!	}
	}
	// 
	Bit16u QUERY_GDAT_RAW_DATA_LENGTH(Bit16u index)
	{
		return _4976_5c98[index].buffSize;

		//!//^3E74:0422
		//!shelf_memory bp04 = _4976_5c98[index];
		//!//^3E74:043F
		//!if ((bp04.info & 0x80000000) == 0) {
		//!	//^3E74:044A
		//!	return *(Bit16u *)(&EMS_REALIZE(bp04)[-2]);
		//!}
		//!//^3E74:046A
		//!return bp04.info & 0xFFFF;
	}
	// 
	Bit16u QUERY_GDAT_ENTRY_DATA_LENGTH(Bit8u cls1, Bit8u cls2, Bit8u cls3, Bit8u cls4)
	{
		//^3E74:5AB7
		return QUERY_GDAT_RAW_DATA_LENGTH(
			QUERY_GDAT_ENTRY_DATA_INDEX(cls1, cls2, cls3, cls4)
			);
	}
	// 
	Bit8u *QUERY_GDAT_TEXT(Bit8u cls1, Bit8u cls2, Bit8u cls4, Bit8u *buff)
	{
		//^2636:02F8
		*buff = 0;
		Bit16u di = 0;
		Bit16u si = 0;
		//^2636:030B
		Bit8u *bp0e = QUERY_GDAT_ENTRY_DATA_BUFF(cls1, cls2, 5, cls4);
		//^2636:0327
		if (bp0e != NULL) {
			//^2636:0348
			Bit8u *bp0a = ALLOC_MEMORY(
				di = si = QUERY_GDAT_ENTRY_DATA_LENGTH(cls1, cls2, 5, cls4),
				0,
				1024
				);
			//^2636:0358
			COPY_MEMORY(bp0e, bp0a, si);
			//^2636:0377
			if (_4976_52de != 0) {
				//^2636:037E
				Bit8u *bp04 = bp0a;
				Bit8u bp05 = 0;
				//^2636:038E
				while (di-- != 0) {
					//^2636:0390
					*bp04 = (*bp04 ^ 0xFF) - bp05;
					//^2636:039E
					bp05++;
					bp04++;
					//^2636:03A4
				}
			}
			//^2636:03AB
			FORMAT_SKSTR(bp0a, buff);
			//^2636:03BE
			DEALLOC_UPPER_MEMORY(si);
		}
		//^2636:03CA
		return bp0e;
	}
	// 
	void DEALLOC_UPPER_MEMORY(Bit32u size)
	{
		//^3E74:0A1B
		if ((size & 1) != 0) {
			//^3E74:0A29
			size++;
		}
		//^3E74:0A31
		_4976_5d30 += size;
		//^3E74:0A3F
		_4976_5cf4 += size;
	}
	// 
	Bit8u *sk_strcat(Bit8u *str, const Bit8u *src);
	// 
	Bit8u *sk_strcpy(Bit8u *str, const Bit8u *src);
	// 
	int sk_strlen(const Bit8u *str);
	// 
	void int2str(__int32 value, Bit8u *buff)
	{
		IMPLEMENT_HERE();
	}
	// 
	void FILE_RENAME(const void *src, const void *dst)
	{
		IMPLEMENT_HERE();
	}
	// 
	void FILE_CLOSE(Bit16u handle)
	{
		IMPLEMENT_HERE();
	}
	// 
	int FILE_READ(Bit16u handle, Bit32u buffSize, void *buff)
	{
		IMPLEMENT_HERE();
	}
	// 
	int SUPPRESS_READER(void *_data, const void *_mask, int buffSize, Bit32u repeat, int fill)
	{
		Bit8u *data = reinterpret_cast<Bit8u *>(_data);
		const Bit8u *mask = reinterpret_cast<const Bit8u *>(_mask);

		//^2066:01C3
		Bit32u bp0a = 0;
		Bit8u bp04 = _4976_5254;
		Bit8u bp05 = _4976_524e;
		//^2066:01E8
		for (Bit32u bp0e = 0; bp0e < repeat; ) {
			//^2066:01EB
			for (Bit16u si = 0; si < buffSize; si++) {
				//^2066:01F0
				Bit8u bp03 = (fill != 0) ? 0 : (*data);
				//^2066:0205
				Bit8u bp02 = *mask;
				mask++;
				//^2066:0211
				if (bp02 != 0) {
					//^2066:021A
					for (__int8 bp01 = 0x07; bp01 >= 0; bp01--) {
						//^2066:0221
						if (((1 << bp01) & bp02) != 0) {
							//^2066:0232
							if (bp05 == 0) {
								//^2066:0238
								Bit8u bp0f;
								if (FILE_READ(_4976_394e, 1, &bp0f) == 0)
									return 1;
								//^2066:0257
								bp04 = bp0f;
							}
							//^2066:025D
							if ((bp04 & 0x80) != 0) {
								//^2066:0263
								bp04 = (bp04 << 1) | 1;
								bp03 = (1 << bp01) | bp03;
							}
							else {
								//^2066:027B
								bp03 = (bp04 << 1) & (~(1 << bp01));
							}
							//^2066:028C
							bp0a++;
							bp05++;
							//^2066:029F
							if (bp05 == 8) {
								//^2066:02A3
								bp05 = 0;
							}
						}
						//^2066:02A7
					}
				}
				//^2066:02B3
				*data = bp03;
				data++;
				//^2066:02BF
			}
			//^2066:02C8
			bp0a -= bp0e;
			bp0e++;
			//^2066:02D6
		}
		//^2066:02EE
		_4976_5254 = bp04;
		_4976_524e = bp05;
		_4976_5258 = bp0a;
		return 0;
	}


//	// 
//	void _2066_0d09()
//	{
//		//^2066:0D09
//
//		// TODO: REFILL HERE
//
//		//^2066:110C
//		sksave_header bp4c;
//
//		// TODO: REFILL HERE
//
//		FILE_WRITE(_4976_394e, 42, &bp4c);
//		//^2066:1141
//		if (SKSAVE_WRITE(_4976_4d0e, 44) == 0)
//			goto _14fa;
//		//^2066:1159
//		if (SKSAVE_WRITE(_4976_4dba, _4976_4d0e->nMaps << 4) == 0)
//			goto _14fa;
//		//^2066:117D
//		if (SKSAVE_WRITE(_4976_4c7c, _4976_4cb4 << 1) == 0)
//			goto _14fa;
//		//^2066:1199
//		if (SKSAVE_WRITE(_4976_4d16, _4976_4d0e->cwListSize << 1) == 0)
//			goto _14fa;
//		//^2066:11BA
//		if (SKSAVE_WRITE(_4976_4d12, _4976_4d0e->cwTextData << 1) == 0)
//			goto _14fa;
//
//		//^2066:11DB
//		Bit16u bp0e = 0x0000;
//		//^2066:11E0
//		while (bp0e < 0x0010) {
//			//^2066:11E2
//			if (SKSAVE_WRITE(_4976_4cca[bp0e], dbSize[bp0e] * _4976_4d0e->nRecords[bp0e]) == 0)
//				goto _14fa;
//			//^2066:1217
//			bp0e++;
//			//^2066:121A
//		}
//		//^2066:1220
//		if (SKSAVE_WRITE(_4976_4db6, _4976_4d0e->cbMapData) == 0)
//			goto _14fa;
//		//^2066:123E
//
//		// TODO: REFILL HERE
//
//		//^2066:125A
//		sksave_table_84 bp84;
//		bp84.bp84 = _4976_4bec;
//		bp84.bp80 = _4976_49ba;
//		bp84.bp7c = _4976_4c3c;
//		bp84.bp7a = _4976_4bfa;
//		bp84.bp78 = _4976_4c06;
//		bp84.bp76 = _4976_4bf6;
//		bp84.bp74 = _4976_4bf4;
//		bp84.bp72 = _4976_3fc6;
//		bp84.bp70 = _4976_5c1e;
//		bp84.bp6e = _4976_0090;
//		bp84.bp6a = _4976_4b80;
//		bp84.bp66 = _4976_4c00;
//		bp84.bp64 = _4976_4c14;
//		bp84.bp62 = _4976_4c0c;
//
//		bp84.bp5c = _4976_5c5d;
//		bp84.bp5a = _4976_5c63;
//		bp84.bp59 = _4976_5c6a;
//		bp84.bp58 = _4976_5c5e;
//		bp84.bp57 = _4976_5c60;
//		bp84.bp56 = _4976_5c61;
//		bp84.bp55 = _4976_5c64;
//		bp84.bp54 = _4976_5c66;
//		bp84.bp52 = _4976_5c5f;
//		bp84.bp51 = _4976_5c62;
//		bp84.bp50 = _4976_5c6e;
//		//^2066:1317
//		if (SUPPRESS_WRITER(&bp84, _4976_395a, 0x0038, 0x00000001) != 0)
//			goto _14fa;
//		//^2066:1339
//		if (SUPPRESS_WRITER(_4976_5f52, _4976_3956, 0x0001, 0x00000008) != 0)
//			goto _14fa;
//		//^2066:1355
//		if (SUPPRESS_WRITER(_4976_5f12, _4976_3956, 0x0001, 0x00000040) != 0)
//			goto _14fa;
//		//^2066:1371
//		if (SUPPRESS_WRITER(_4976_5f5a, _4976_3956, 0x0002, 0x00000040) != 0)
//			goto _14fa;
//		//^2066:138D
//		if (SUPPRESS_WRITER(_4976_53b4, _4976_3992, 0x0105, _4976_4c3c) != 0)
//			goto _14fa;
//		//^2066:13AC
//		if (SUPPRESS_WRITER(_4976_57e4, _4976_3a97, 0x0006, 0x00000001) != 0)
//			goto _14fa;
//		//^2066:13C8
//		if (SUPPRESS_WRITER(_4976_5c20, _4976_3a9d, 0x000a, _4976_5c1e) != 0)
//			goto _14fa;
//		//^2066:13EA
//
//		// TODO: REFILL HERE
//
//		//^2066:1462
//		bp0e = 0;
//		//^2066:1467
//		while (bp0e < _4976_4c3c) {
//			//^2066:1469
//			Bit16u *bp04 = (Bit16u *)&_4976_5475[bp0e].pocket[0];
//			__int16 bp12 = 0;
//			//^2066:147F
//			while (bp12 < 0x001E) {
//				//^2066:1481
//				if (WRITE_RECORD_CHECKCODE(*(bp04++), 0, 0) != 0)
//					goto _14fa;
//				//^2066:149A
//				bp12++;
//				//^2066:149D
//			}
//			//^2066:14A3
//			bp0e++;
//			//^2066:14A6
//		}
//		//^2066:14AF
//		if (WRITE_RECORD_CHECKCODE(_4976_57c8, 0, 0) != 0)
//			goto _14fa;
//		//^2066:14C3
//		if (STORE_EXTRA_DUNGEON_DATA() == 0)
//			goto _14fa;
//		//^2066:14CA
//		if (WRITE_POSSESSION_INDICES() != 0)
//			goto _14fa;
//		//^2066:14D2
//		if (SUPPRESS_FLUSH() != 0)
//			goto _14fa;
//
//		// TODO: REFILL HERE
//
//		goto _1523;
//_14fa:	//^2066:14FA
//_1523:	//^2066:1523
//		;
//	}
	// 
	int SUPPRESS_FLUSH()
	{
		// TODO: VERIFY ME

		//^2066:0147
		Bit8u bp01 = _4976_524e;
		//^2066:0151
		if (bp01 == 0)
			return 0;
		Bit8u bp02 = 8 - bp01;
		Bit8u bp03 = _4976_5254;
		//^2066:0163
		while (bp02-- != 0) {
			//^2066:0165
			bp03 = (((bp03 & 0x80) == 0) ? 0 : 1) | (bp03 << 1);
			//^2066:017B
		}
		//^2066:0185
		Bit8u bp04 = bp03;
		//^2066:018B
		if (FILE_WRITE(_4976_394e, 1, &bp04) == 0)
			return 1;
		//^2066:01A9
		_4976_524e = _4976_5254 = 0;
		_4976_5258 = bp01;
		//^2066:01BF
		return 0;
	}
	// 
	int WRITE_POSSESSION_INDICES()
	{
		//^2066:0583
		Bit16u bp0a = _4976_3954;
		Bit16u *bp04 = _4976_525e;
		//^2066:059C
		while (bp0a-- != 0) {
			//^2066:059E
			Bit16u di = *bp04;
			bp04++;
			//^2066:05A8
			GenericRecord *bp08 = reinterpret_cast<GenericRecord *>(GET_ADDRESS_OF_RECORD(di));
			//^2066:05B5
			Bit16u si;
			switch ((di & 0x3C00) >> 10) {
				case dbContainer:	// 9
					//^2066:05C9
					si = _4976_5246[reinterpret_cast<Container *>(bp08)->w2 & 0x03FF];
					break;
				case dbMissile:		// 14
					si = _4976_5240[reinterpret_cast<Missile   *>(bp08)->w2 & 0x03FF];
					break;
				default:
					//^2066:05C7
					continue;
			}
			//^2066:05EB
			Bit16u bp0c = si;
			//^2066:05F3
			Bit16u bp0e = 0x03FF;
			if (SUPPRESS_WRITER(&bp0c, &bp0e, 2, 1) != 0)
				return 1;
			//^2066:0613
		}
		//^2066:0620
		return 0;
	}
	// 
	Bit8u GET_TELEPORTER_DETAIL(TELE_inf *teleinf, Bit8u x_pos, Bit8u y_pos)
	{
		Bit8u *record1, *record2;
		Bit8u type1, type2;
		Bit8u _si = 0;

		//^0CEE:0921
		type1 = GET_POINTER_OF_RECORD_LINK_AND_SOMEWHAT(&record1, x_pos, y_pos);

		if (type1 != 0) {
			//^0CEE:0945
			Bit8u orgMap = (Bit8u)_4976_0c5a;
			CHANGE_CURRENT_MAP_TO(record1[0x05] & 0xFF);

			//^0CEE:0959
			type2 = GET_POINTER_OF_RECORD_LINK_AND_SOMEWHAT(
				&record2,
				((((Teleporter *)record1)->w2)     ) & 0x1F,
				((((Teleporter *)record1)->w2) >> 5) & 0x1F
				);

			if (type2 != 0) {
				//^0CEE:0982
				_si = 1;
				teleinf->cross_scene_test = type1 -1;
				teleinf->cross_scene_test_of_teleporter_target = type2 -1;
				teleinf->target_x_pos = ((((Teleporter *)record1)->w2)     ) & 0x1F;
				teleinf->target_y_pos = ((((Teleporter *)record1)->w2) >> 5) & 0x1F;
				teleinf->target_z = record1[0x05];
			}
			//^0CEE:09CF
			CHANGE_CURRENT_MAP_TO(orgMap);
		}
		//^0CEE:09D5
		return _si;
	}
	// 
	Bit8u GET_POINTER_OF_RECORD_LINK_AND_SOMEWHAT(Bit8u **record_addr_ptr, Bit8u x_pos, Bit8u y_pos)
	{
		//^0CEE:0897
		if ((GET_TILE_VALUE(x_pos, y_pos) >> 5) == 0x05) {
			//^0CEE:08B0
			Bit16u recordLink = GET_TILE_RECORD_LINK(x_pos, y_pos);

			*record_addr_ptr = (Bit8u *)GET_ADDRESS_OF_RECORD(recordLink);
			//^0CEE:090E
			while ((recordLink = GET_NEXT_RECORD_LINK(recordLink)) != 0xFFFE) {
				//^0CEE:08D2
				if (((recordLink & 0x3C00) >> 10) == 0x0003) {
					//^0CEE:08DF
					Bit8u *record = (Bit8u *)GET_ADDRESS_OF_RECORD(recordLink);
					if ((record[0x02] & 0x7F) == 0x27) {
						//^0CEE:08F6
						Bit8u res = (*record_addr_ptr)[0x03];
						res>>= 2;
						res &= 0x0003;
						res += 2;
						res &= 0x0003;
						res ++;
						return res;
					}
				}
			}
		}
		//^0CEE:091C
		return 0;
	}
	// 
	Bit8u WRITE_RECORD_CHECKCODE(Bit16u recordLink, Bit8u xx, Bit8u yy)
	{
		// xx=(write-direction-in-record-link)
		// yy=(write-subsequent-records)

		//^2066:06E8

		//^2066:0AE6
		while (recordLink != 0xFFFE && recordLink != 0xFFFF) {
			//^2066:06F4
			Bit8u db = (recordLink & 0x3C00) >> 10;
			if (db > 3) {
				//^2066:0704
				if (WRITE_1BIT(1))
					return 1;
				Bit8u data = db;
				Bit8u mask = 0x0F;
				if (SUPPRESS_WRITER(&data, &mask, 1, 1))
					return 1;
				//^2066:073A
				if (xx != 0 && db != dbCreature) {
					Bit8u data = recordLink >> 14;
					Bit8u mask = 0x03;
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
				Bit8u *record = (Bit8u *)GET_ADDRESS_OF_RECORD(recordLink);
				//^2066:07CE
				Bit16u _di = 0;
				//^2066:07D0
				switch (db) {
					case dbActuator:	// 3
						{
							//^2066:07E5
							switch (record[0x02] & 0x7F) {
							case 0x27:
							case 0x1B:
							case 0x1D:
							case 0x41:
							case 0x2C:
							case 0x32:
							case 0x30:
							case 0x2D:
								{
									//^2066:0825
									Bit16u data = (*((Bit16u *)&record[0x02]) >> 7) & 0x01FF;
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
							//^2066:085B
							Bit8u data = record[0x04];
							Bit8u mask = 0x7F;
							if (SUPPRESS_WRITER(&data, &mask, 1, 1))
								return 1;
							//^2066:0887
							if (_0cee_2de0(recordLink) & 1) {
								//^2066:0893
								recordMask = db4Mask2;
							}
							//^2066:089B
							_4976_5246[(recordLink & 0x03FF)] = _4976_3952;
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
							//^2066:08B4
							Bit8u data = ((((Container *)record)->b4 & 0xFF) >> 1) & 0x03;
							Bit8u mask = 0x03;
							if (SUPPRESS_WRITER(&data, &mask, 1, 1))
								return 1;
							//^2066:08E5
							if (IS_CONTAINER_MAP(recordLink) != 0) {
								//^2066:08F0
								recordMask = db9Mask2;
								_di = 1;
							}
							//^2066:08FB
							_4976_5240[(recordLink & 0x03FF)] = _4976_3950;
							_4976_3950++;
							break; //->2066:0926
						}
					case dbMissile:		// 14
						{
							//^2066:0914
							if (_4976_3b57 == 0)
								break;
							recordMask = dbEMask2;
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
							//^2066:0963
							_4976_3b57 = 0x0001;

							Bit8u res = WRITE_RECORD_CHECKCODE(
								((Creature *)record)->w2,
								recordMask != db4Mask1,
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
							//^2066:099F
							if (_di != 0) {
								//^2066:09A3
								Bit16u bp10 = reinterpret_cast<Container *>(record)->w2;
								//^2066:09AD
								Bit16u bp18 = (bp10 == 0xFFFF || bp10 == 0xFFFE) ? 0 : 1;
								//^2066:09C3
								if (WRITE_1BIT((Bit8u)bp18))
									return 1;
								//^2066:09D2
								if (!(bp18 != 0))
									break;
								//^2066:09DB
								ADD_INDEX_TO_POSSESSION_INDICES(recordLink);
								break;
							}
							//^2066:09E4
							if (IS_CONTAINER_MONEYBOX(recordLink) != 0) {
								//^2066:09EF
								tableMask[dbMiscellaneous_item] = dbAMask1;
							}
							//^2066:09F9
							if (WRITE_RECORD_CHECKCODE(reinterpret_cast<Container *>(record)->w2, 0, 1))
								return 1;
							//^2066:0A12  
							if (IS_CONTAINER_MONEYBOX(recordLink) != 0) {
								//^2066:0A20
								tableMask[dbMiscellaneous_item] = dbAMask2;
							}
							//^2066:0A2A  
							break; //->2066:0AD7
						}
					case dbMissile:		// 14
						{
							//^2066:0A2D  
							if (_di != 0) {
								//^2066:0A31  
								ADD_INDEX_TO_POSSESSION_INDICES(recordLink);
								//^2066:0A37  
								break; //->2066:0AD7
							}
							//^2066:0A3A  
							_4976_3b55 = 0x0001;
							Bit8u res = WRITE_RECORD_CHECKCODE(
								((Missile *)record)->w2,
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
							sk5c20 *bp0c = _4976_5c20;
							Bit16u bp14 = 0;
							//^2066:0AC3
							while (bp14 < _4976_5c14) {
								//^2066:0A76
								if (bp0c->b4 == 0x19 && bp0c->w8 == recordLink) {
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
			if (yy == 0)
				break;
			recordLink = GET_NEXT_RECORD_LINK(recordLink);

		}
		//^2066:0AF3
		if (yy == 0) {
			//^2066:0AF9
			if (recordLink != 0xFFFF)
				return 0;
		}
		//^2066:0AFE
		if (WRITE_1BIT(0))
			return 1;
		//^2066:0B09
		return 0;
	}
	// 
	void ADD_INDEX_TO_POSSESSION_INDICES(Bit16u recordLink)
	{
		//^2066:0569
		_4976_525e[_4976_3954] = recordLink;
		//^2066:057D
		_4976_3954++;
	}
	// 
	Bit16u IS_CONTAINER_MONEYBOX(Bit16u recordLink)
	{
		//^0CEE:26E5
		if (((recordLink & 0x3C00) >> 10) == dbContainer) {
			//^0CEE:26F9
			Bit8u b4 = ((Container *)GET_ADDRESS_OF_RECORD(recordLink))->b4;
			if (((b4 >> 1) & 3) == 0) {
				//^0CEE:2710
				if (QUERY_GDAT_ENTRY_DATA_INDEX(0x14, QUERY_CLS2_FROM_RECORD(recordLink), 0x0005, 0x0040) != (Bit16u)-1) {
					// Exists possible item list? (e.g. "J26-28 J8 J56 J6<00>")
					return 1;
				}
			}
		}
		//^0CEE:272F
		return 0;
	}
//	// 
//	Bit8u _0cee_3035(Bit16u recordLink)
//	{
//		//^0CEE:3035
//		Text *bp04 = reinterpret_cast<Text *>(GET_ADDRESS_OF_RECORD(recordLink));
//		if (((((Bit8u)bp04->w2) >> 1) & 0x0003) == 0x0001) {
//			//^0CEE:3059
//			Bit16u si = (((bp04->w2 >> 3) & 0x1FFF) >> 8) & 0x001F;
//			//^0CEE:306B
//			switch (si) {
//				case 0x0000:
//				case 0x0002:
//				case 0x0005:
//				case 0x000D:
//					//^0CEE:3080
//					return ((Bit8u)((bp04->w2 >> 3) & 0x1FFF)) & 0xFF;
//			}
//		}
//		//^0CEE:3091
//		return -1;
//	}
	// 
	Bit8u GET_WALL_DECORATION_OF_ACTUATOR(Actuator *record)
	{
		//^0CEE:30A6
		Bit16u si = (((Bit8u *)record)[5] >> 4) & 0x000F;
		if (si != 0) {
			//^0CEE:30BE
			Bit8u *bp04 = _4976_4c78[_4976_4dd2 -1] + _4976_4dd4 + ((((Bit8u)_4976_4d0a->w12) >> 4) & 0x000F);
			//^0CEE:30ED
			return bp04[si -1];
		}
		//^0CEE:30F6
		return -1;
	}
//	// 
//	Bit8u QUERY_CLS2_FROM_RECORD(Bit16u recordLink)
//	{
//		//^0CEE:241B
//		if (!(recordLink == 0xFFFF)) {
//			//^0CEE:2428
//			void *bp04;
//			if (recordLink < 0xFF80) {
//				//^0CEE:242E
//				bp04 = GET_ADDRESS_OF_RECORD(recordLink);
//			}
//			else {
//				//^0CEE:243E
//				return ((Bit8u)recordLink) - 0x80;
//			}
//			//^0CEE:2446
//			switch ((recordLink & 0x3C00) >> 10) {
//				case dbText:		// 2
//					//^0CEE:24EA
//					return _0cee_3035(recordLink);
//				case dbActuator:	// 3
//					//^0CEE:24D7
//					return GET_WALL_DECORATION_OF_ACTUATOR(reinterpret_cast<Actuator *>(GET_ADDRESS_OF_RECORD(recordLink)));
//				case dbCreature:	// 4
//					//^0CEE:2461
//					return reinterpret_cast<Creature *>(bp04)->CreatureType();
//				case dbWeapon:		// 5
//					//^0CEE:246B
//					return reinterpret_cast<Weapon *>(bp04)->ItemType();
//				case dbCloth:		// 6
//					//^0CEE:2478
//					return reinterpret_cast<Cloth *>(bp04)->ItemType();
//				case dbScroll:		// 7
//					//^0CEE:2484
//					return 0;
//				case dbPotion:		// 8
//					//^0CEE:2488
//					return reinterpret_cast<Potion *>(bp04)->PotionType();
//				case dbContainer:	// 9
//					//^0CEE:2494
//					return 0
//						| (((reinterpret_cast<Container *>(bp04)->b4 >> 1) & 0x0003) << 3)
//						|  ((reinterpret_cast<Container *>(bp04)->b5 >> 5) & 0x0007)
//						;
//				case dbMiscellaneous_item:	//10
//					//^0CEE:24B2
//					return reinterpret_cast<Miscellaneous_item *>(bp04)->ItemType();
//				case db11:			//11
//				case db12:			//12
//				case db13:			//13
//					break;
//				case dbMissile:		//14
//					//^0CEE:24BE
//					return (Bit8u)QUERY_CLS2_FROM_RECORD(reinterpret_cast<Missile *>(bp04)->MissileObject());
//				case dbCloud:		//15
//					//^0CEE:24CB
//					return reinterpret_cast<Cloud *>(bp04)->CloudType();
//			}
//		}
//		//^0CEE:24F5
//		return -1;
//	}
	// 
	Bit16u QUERY_GDAT_ENTRY_DATA_INDEX(Bit8u cls1, Bit16u cls2, Bit8u cls3, Bit16u cls4)
	{
		//^3E74:189B
		RawEntry *bp04 = QUERY_GDAT_ENTRYPTR(cls1, cls2, cls3, cls4);
		if (bp04 == NULL) {
			//^3E74:18C4
			switch (cls3) {
				case 0x0B:
				case 0x0C:
					//^3E74:18D5
					return 0;
			}
			//^3E74:18D0
			return -1;
		}
		else {
			//^3E74:18D9
			switch (cls3) {
				case 0x0B:
				case 0x0C:
                    //^3E74:18F1
					return bp04->data;
			}
			//^3E74:18E5
			return bp04->data & 0x7FFF;
		}
	}
	// 
	RawEntry *QUERY_GDAT_ENTRYPTR(Bit8u cls1, Bit16u cls2, Bit8u cls3, Bit16u cls4)
	{
		// cls1 - main category
		// cls2 - sub category
		// cls3 - type
		// cls4 - name

		//^3E74:178C
		if (!(cls1 <= g_base._4976_5cc2)) {
			//^3E74:179D
			return NULL;
		}
		Bit16u si = g_base._4976_5cb6[cls1];
		if (!(g_base._4976_5cb6[cls1 +1] - si > cls3)) {
			//^3E74:17D2
			return NULL;
		}
		//^3E74:17D2
		si += cls3;
		Bit16u di = g_base._4976_5cba[si];
//		3E74:17E6  8BF8                 mov  di,ax
//		3E74:17E8  33D2                 xor  dx,dx							DX:AX=0000:1399
//		3E74:17EA  05FFFF               add  ax,FFFF
//		3E74:17ED  83D2FF               adc  dx,FFFF						DX:AX=0000:1398
//		3E74:17F0  B102                 mov  cl,02
//		3E74:17F2  9A85099A06           call 069A:0985		call SHL32		DX:AX=0000:4E60
//		3E74:17F7  50                   push ax
//		3E74:17F8  52                   push dx
//		3E74:17F9  8B16C05C             mov  dx,[5CC0]
//		3E74:17FD  A1BE5C               mov  ax,[5CBE]						DX:AX=9283:0006
//		3E74:1800  59                   pop  cx
//		3E74:1801  5B                   pop  bx								CX:BX=0000:4E60
//		3E74:1802  9A6F0A9A06           call 069A:0A6F		call ADD_TO_FAR_PTR
//		3E74:1807  8946F8               mov  [bp-08],ax						DX:AX=9769:0006
//		3E74:180A  8956FA               mov  [bp-06],dx

//9283:0006 + 00703C -> 99872
//9987:0002 + 000030 -> 998A2

		RawEntry *bp08 = &g_base._4976_5cbe[di -1];
		//^3E74:180D
		di = g_base._4976_5cba[si +1] -di +1;
		//^3E74:1820
		si = 0;
		while (true) {
			//^3E74:1822
			Bit16u bp0a = (si +di) / 2;
			//^3E74:182B
			if (!(bp0a != si))
				return NULL;
			//^3E74:1833
			RawEntry *bp04 = &bp08[bp0a];
			__int16 bp0c = bp04->cls2 - cls2;
			//^3E74:1866
			if (!(bp0c != 0)) {
				//^3E74:186A
				bp0c = bp04->cls4 - cls4;
				if (!(bp0c != 0)) {
					//^3E74:187E
					return bp04;
				}
			}
			//^3E74:1887
			if (!(bp0c <= 0)) {
				//^3E74:188D
				di = bp0a;
			}
			else {
				//^3E74:1892
				si = bp0a;
			}
		}
	}
	// 
	Bit16u IS_CONTAINER_MAP(Bit16u recordLink)
	{
		//^0CEE:2EFB
		if (((recordLink & 0x3C00) >> 10) == 9) {
			if (((reinterpret_cast<Container *>(GET_ADDRESS_OF_RECORD(recordLink))->b4 >> 1) & 0x0003) == 0x0001) {
				return 1;
			}
		}
		//^0CEE:2F2B
		return 0;
	}
	// 
	Bit16u _0cee_2de0(Bit16u recordLink)
	{
		Bit16u *data = (Bit16u *)QUERY_CREATURE_AI_SPEC_FROM_RECORD(recordLink);
		return *data;
	}
	// 
	Bit8u *QUERY_CREATURE_AI_SPEC_FROM_RECORD(Bit16u recordLink)
	{
		Bit8u *record = (Bit8u *)GET_ADDRESS_OF_RECORD(recordLink);
		return QUERY_CREATURE_AI_SPEC_FROM_TYPE(((Creature *)record)->b4);
	}
	// 
	Bit8u *QUERY_CREATURE_AI_SPEC_FROM_TYPE(Bit8u creatureType)
	{
		Bit16u res = QUERY_GDAT_CREATURE_WORD_VALUE(creatureType, 0x0005);

		return const_cast<Bit8u *>(&_4976_03a2[res * 0x0024]);
	}
	// 
	Bit16u QUERY_GDAT_CREATURE_WORD_VALUE(Bit8u creatureType, Bit8u cls4)
	{
		//^0CEE:2D36
		if (_4976_4c58 != NULL) {
			//^0CEE:2D46
			while (creatureType <= _4976_4c56) {
				//^0CEE:2D4F
				Bit8u bp05;
				if (cls4 == 0) {
					bp05 = 0x01;
				}
				else if (cls4 == 1) {
					bp05 = 0x00;
				}
				else if (cls4 == 5) {
					bp05 = 0x02;
				}
				else {
					break;
				}

				//^0CEE:2D74
				Bit8u *bp04 = _4976_4c58 + creatureType * 3 + bp05;
				Bit16u bp08 = *bp04;
				if (bp08 == 0xFF) {
					//^0CEE:2DA5
					bp08 = QUERY_GDAT_ENTRY_DATA_INDEX(0xF, creatureType, 0xB, cls4);

					*bp04 = (Bit8u)bp08;
				}
				//^0CEE:2DC5
				return bp08;
			}
		}
		//^0CEE:2DCA
		return QUERY_GDAT_ENTRY_DATA_INDEX(0xF, creatureType, 0xB, cls4);
	}

	// 
	Bit8u *GET_ADDRESS_OF_TILE_RECORD(Bit8u x_pos, Bit8u y_pos)
	{
		//^0CEE:0A17
		return (Bit8u *)GET_ADDRESS_OF_RECORD(GET_TILE_RECORD_LINK(x_pos, y_pos));
	}
	// 
	Bit16u IS_TILE_SOLID(int x_pos, int y_pos)
	{
		//^0CEE:0330
		Bit8u tileType = (_4976_4c78[x_pos])[y_pos] >> 5;
		if (tileType == ttTeleporter) {
			//^0CEE:0357
			if ((((Teleporter *)GET_ADDRESS_OF_TILE_RECORD(x_pos, y_pos))->w4 & 0x0001) != 0) {
				return 0;
			}
		}
		//^0CEE:0377
		if (tileType != ttWall) {
			//^0CEE:037B
			if (tileType == ttMapExit)
				return 0x0001;
		}
		//^0CEE:0380
		return 0;
	}
	// 
	Bit8u GET_TILE_VALUE(int x_pos, int y_pos)
	{
		//^0CEE:0388
		int bp02 = (y_pos < 0 || y_pos >= _4976_4dd4) ? 0 : 1;
		//^0CEE:03A8
		int bp04 = (x_pos < 0 || x_pos >= _4976_4dd2) ? 0 : 1;

		//^0CEE:03BC
		if (bp04 == 0 || bp02 == 0) {
			//^0CEE:03DA
			if (bp02 != 0) {
				//^0CEE:03E0
				if (x_pos == -1) {
					//^0CEE:03E5
					x_pos = 0;
					bp04 = 0x0004;
				}
				else {
					//^0CEE:03EE
					if (x_pos != _4976_4dd2)
						return 0xE0;
					x_pos--;
				}
				//^0CEE:03FD
				if (IS_TILE_SOLID(x_pos, y_pos) != 0)
					return (Bit8u)bp04;
				//^0CEE:040F
				if (y_pos > 0) {
					//^0CEE:0413
					if (IS_TILE_SOLID(x_pos, y_pos-1) != 0)
						return 0x00;
				}
				//^0CEE:0422
				if (y_pos+1 >= _4976_4dd4)
					return 0xE0;
				//^0CEE:042E
				if (IS_TILE_SOLID(x_pos, y_pos+1) == 0)
					return 0xE0;
				//^0CEE:0440
				return 0x00;
			}
			else {
				//^0CEE:0443
				if (bp04 != 0) {
					//^0CEE:0449
					if (y_pos == -1) {
						//^0CEE:044E
						bp02 = 0x0002;
					}
					else {
						//^0CEE:0457
						if (y_pos != _4976_4dd4)
							return 0xE0;
						//^0CEE:0460
						y_pos--;
						bp02 = 0x0008;
					}
					//^0CEE:0466
					if (IS_TILE_SOLID(x_pos, y_pos) != 0)
                        return bp02;
					//^0CEE:0477
					if (x_pos > 0) {
						if (IS_TILE_SOLID(x_pos-1, y_pos) != 0)
							return 0x00;
					}
					//^0CEE:048A
					if (x_pos+1 >= _4976_4dd4)
						return 0xE0;
					//^0CEE:0493
					if (IS_TILE_SOLID(x_pos+1, y_pos) != 0)
						return 0x00;
				}
				//^0CEE:04A4
				if (x_pos == -1) {
					//^0CEE:04A9
					x_pos = 0;
				}
				else {
					//^0CEE:04AD
					if (x_pos != _4976_4dd2)
						return 0xE0;
				}
				//^0CEE:04B4
				if (y_pos == -1) {
					//^0CEE:04B9
					if (IS_TILE_SOLID(x_pos, 0) != 0)
						return 0x00;
				}
				//^0CEE:04C6
				if (y_pos != _4976_4dd4)
					return 0xE0;
				//^0CEE:04CC
				if (IS_TILE_SOLID(x_pos, y_pos-1) == 0)
					return 0xE0;
				//^0CEE:04DB
				return 0x00;
			}
		}
		else {
			//^0CEE:03C6
			return (_4976_4c78[x_pos])[y_pos];
		}

		//^0CEE:04E1
		return 0x00;
	}
	// 
	Bit16u GET_TILE_RECORD_LINK(int x_pos, int y_pos)
	{
		//^0CEE:0AB5
		int index = GET_OBJECT_INDEX_FROM_TILE(x_pos, y_pos);
		if (index == -1)
			return -2;
		//^0CEE:0AD1
		return _4976_4d16[index];
	}
	// 
	int GET_OBJECT_INDEX_FROM_TILE(int x_pos, int y_pos)
	{
		//^0CEE:0A43
		Bit8u *column = _4976_4c78[x_pos];
		if (x_pos < 0 || x_pos >= _4976_4dd2 || y_pos < 0 || y_pos >= _4976_4dd4 || (column[y_pos] & 0x10) == 0)
			return -1;
		//^0CEE:0A90
		int index = _4976_4c52[x_pos];
		//^0CEE:0AAC
		while (--y_pos >= 0) {
			//^0CEE:0A9F
			if ((*(column++) & 0x10) == 0)
				continue;
			//^0CEE:0AAB
			index++;
		}
		return index;
	}
	// 
	void *GET_ADDRESS_OF_RECORD(Bit16u record_link)
	{
		//^0CEE:09DB
		return _4976_4cca[(record_link & 0x3C00) >> 10] + dbSize[(record_link & 0x3C00) >> 10] * (record_link & 0x03FF);
	}
	// 
	Bit16u GET_NEXT_RECORD_LINK(Bit16u record_link)
	{
		//^0CEE:0A2F
		return ((GenericRecord *)GET_ADDRESS_OF_RECORD(record_link))->w0;
	}
	// 
	void CHANGE_CURRENT_MAP_TO(Bit16u new_map)
	{
		//^0CEE:0002
		__int16 si = new_map;
		if (!(_4976_0c5a != si))
			return;
		//^0CEE:0012
		if (!(si >= 0))
			return;
		//^0CEE:0019
		_4976_0c5a = si;
		//^0CEE:0022
		_4976_4c78 = _4976_4ca8[si];
		//^0CEE:0036
		_4976_4d0a = &_4976_4dba[si];
		//^0CEE:004D
		_4976_4dd2 = _4976_4d0a->Column();
		//^0CEE:005F
		_4976_4dd4 = _4976_4d0a->Row();
		//^0CEE:006D
		_4976_4c52 = &_4976_4c7c[_4976_4dd6[si]];
		//^0CEE:008E
		if (!(_4976_4c08 == 0)) {
			//^0CEE:0095
			if (!(si != _4976_4c12)) {
				//^0CEE:009B
				_4976_4c2e = _4976_4c04;
				_4976_4c30 = _4976_4c1e;
				_4976_4c28 = _4976_4c12;
				_4976_4c2c = _4976_4c10;
				return;
			}
		}
		//^0CEE:00B2
		_4976_4c2e = _4976_4bfa;
		_4976_4c30 = _4976_4c06;
		_4976_4c28 = _4976_4bf4;
		_4976_4c2c = _4976_4bf6;
	}
	// 
	Bit8u WRITE_1BIT(Bit8u nibble)
	{
		//^2066:0123
		Bit8u data = nibble;
		static const Bit8u mask = 0x01;

		return SUPPRESS_WRITER(&data, &mask, 1, 1);
	}
	// 
	Bit8u SUPPRESS_WRITER(const void *_data, const void *_mask, Bit16u xx, Bit32u yy)
	{
		//TODO: FIXME
		logger.encode(reinterpret_cast<const Bit8u *>(_data), xx, yy, reinterpret_cast<const Bit8u *>(_mask));

		// [bp+06] -> data
		// [bp+0A] -> mask
		// [bp+0E] -> xx
		// [bp+10] -> yy

		const Bit8u *data = (const Bit8u *)_data;
		const Bit8u *mask = (const Bit8u *)_mask;

		//^2066:000F
		Bit32u bp0a = 0;
		Bit8u bp04 = _4976_5254; // queued nibble
		Bit8u bp05 = _4976_524e; // current bit position
		Bit32u bp0e = 0;
		//^2066:0034
		while (!(bp0e >= yy)) {
			//^2066:0037
			Bit16u si = 0;
			//^2066:0039
			while (!(si >= xx)) {
				//^2066:003C
				Bit8u data_nibble = *data; // [bp-03]
				data++;
				Bit8u mask_nibble = *mask; // [bp-02]
				mask++;
				//^2066:0054
				if (mask_nibble != 0) {
					//^2066:005D
					__int8 bp01 = 0x07;
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
								Bit8u bp0f = bp04;
								if (FILE_WRITE(_4976_394e, 1, &bp0f) == 0)
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
			data -= xx;
			bp0e++;
			//^2066:00EC
		}
		//^2066:0104
		_4976_5254 = bp04;
		_4976_524e = bp05;
		_4976_5258 += bp0a;

		return 0;
	}
	// 
	Bit16u FILE_WRITE(Bit16u handle, Bit32u buffSize, const void *buff)
	{
		// TODO:IMPLEMENT ME

		//^476D:012C
		return buffSize;
	}
	// 
	Bit16u SKSAVE_WRITE(void *buff, Bit16u buffSize)
	{
		//^2066:03C0
		if (_2066_036e(buff, buffSize) == 0)
			return 0;
		//^2066:03DB
		return 1;
	}
	// 
	Bit16u _2066_036e(void *buff, Bit16u buffSize)
	{
		//^2066:036E
		Bit32u bp04 = buffSize;

		//^2066:037D
		if (bp04 != 0) {
			//^2066:0386
			return FILE_WRITE(_4976_394e, bp04, buff);
		}
		return 1;
	}
	// 
	int STORE_EXTRA_DUNGEON_DATA()
	{
		//^2066:0B96
		_4976_5258 = 0;
		if (_2066_0b44() != 0)
			return 0;
		Bit16u bp10 = _4976_0c5a;
		Bit16u currentMap = 0;
		//^2066:0BBB
		while (currentMap < _4976_4d0e->nMaps) {
			CHANGE_CURRENT_MAP_TO(currentMap);
			Bit8u *bp04 = *_4976_4c78;
			Bit16u *bp08 = &_4976_4d16[*_4976_4c52];
			Bit16u xpos = 0; // word [bp-0C]
            //^2066:0BF4
			while (!(xpos >= _4976_4dd2)) {
				//^2066:0BF7
				Bit16u ypos = 0; // di
				//^2066:0BF9
				while (!(ypos >= _4976_4dd4)) {

					logger.update(currentMap, xpos, ypos);

					Bit8u tileValue = *bp04; // [bp-0D]
					//^2066:0BFC
					bp04++;
					Bit16u skipSaveRecord = 0; // word [bp-18]
					Bit8u bp0e;
					TELE_inf bp16;
					switch (tileValue >> 5) {
						case ttWall:
						case ttFloor:
						case ttStairs:
						case ttMapExit:
							//^2066:0C22
							bp0e = 0x00;
							break;
						case ttPit:
							//^2066:0C28
							bp0e = 0x08;
							break;
						case ttDoor:
							//^2066:0C2E
							bp0e = 0x07;
							break;
						case ttTeleporter:
							//^2066:0C34
							if (GET_TELEPORTER_DETAIL(&bp16, (Bit8u)xpos, (Bit8u)ypos) != 0) {
								//^2066:0C49
								bp0e = 0x00;
								if (bp16.target_z >= currentMap)
									break;
								//^2066:0C56
								skipSaveRecord = 0x01;
								break;
							}
							//^2066:0C5D
							bp0e = 0x08;
							break;
						case ttTrickWall:
							//^2066:0C63
							bp0e = 0x04;
							break;
					}
					//^2066:0C67
					if (!(bp0e == 0)) {
						//^2066:0C6D
						if (SUPPRESS_WRITER(&tileValue, &bp0e, 1, 1) != 0)
							return 0;
					}
					//^2066:0C88
					Bit16u tileRecord; // [bp-0A]
					if ((tileValue & 0x10) != 0) {
						//^2066:0C8E
						tileRecord = *bp08;
						bp08++;
					}
					else {
						//^2066:0C9D
						tileRecord = 0xFFFE;
					}
					//^2066:0CA2
					if (skipSaveRecord == 0) {
						if (WRITE_RECORD_CHECKCODE(tileRecord, 1, 1) != 0)
							return 0;
					}
					//^2066:0CBA
					ypos++;
					//^2066:0CBB
				}
				//^2066:0CC4
				xpos++;
				//^2066:0CC7
			}
			currentMap++;
			//^2066:0CD4
		}
		//^2066:0CE5
		CHANGE_CURRENT_MAP_TO(currentMap);
		return 1;
	}
	// 
	int _2066_0b44()
	{
		//^2066:0B44
		sk5c20 *bp04 = _4976_5c20;
		Bit16u i = 0;
		//^2066:0B58
		while (i < _4976_5c1e) {
			switch (bp04->b4) {
				case 0x3C:
				case 0x3D:
					//^2066:0B6B
					if (WRITE_RECORD_CHECKCODE(bp04->w8, 0, 0) != 0) {
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

	//^0759:08E7
	void ANIM_main(int argc, char **argv, char **env)
	{
		//^0759:08E7
		ENTER(214);
		//^0759:08ED
		X16 bp2a = 0;
		X16 bp2e = 0;
		X16 bp32 = 0;
		X16 bp34 = 0;
		X16 bp36 = 0;
		X16 bp42 = 0;
		X16 bp44 = 0;
		X16 bp46 = 0;
		X16 bp48 = 0;
		X16 bp4a = 0;
		X16 bp5e = 0;
		X16 bp60 = 0;
		X8 bp69 = 15;
		X16 bp6c = 0;
		X8 bp0096[0x96-0x6c] = {0};
		for (i16 si = 1; si < argc; si++) {
			//^0759:093D
			if (argv[si][0] == '+' && _069a_035b(argv[si][1]) == 'A') {
				//^0759:0953
				switch (_069a_035b(argv[si][2])) {

				}
				xxx;
			}
			//^_0ab3
			xxx;
			//^0759:0ADD
		}
		//^0759:0AE6
		xxx;
		//^0759:13C5
		return;
	}


class graphics_base {
public:
	// 
	Bit16u _4976_5cc2;				// max categories in GRAPHICS.dat
	CHeapPtr<Bit16u> _4976_5cb6;	// array-of-class1, index-to-(array-of-class3)
	CHeapPtr<Bit16u> _4976_5cba;	// array-of-class3, index-to-(array-of-pointer-to-rawentry)
	CHeapPtr<RawEntry> _4976_5cbe;	// array-of-pointer-to-rawentry

	// 
	bool load_graphics(LPCSTR psz) {
		FILE *f = fopen(psz, "rb");
		if (f != NULL) {
			CHeapPtr<Bit8u> buff;
			const size_t buffSize = 4*1024*1024;
			if (buff.Allocate(buffSize)) {
				int r = (int)fread(buff, 1, buffSize, f);
				if (r > 0) {
					CMemVuff mvi;
					mvi.pData = buff;
					mvi.iData = 0;
					mvi.nData = r;
					mvi.fLE = true;
					if (load_graphics(mvi)) {
						fclose(f);
						return true;
					}
				}
			}
			fclose(f);
			return false;
		}
		return false;
	}
	// 
	bool load_graphics(CMemVuff &mvi) {
		WORD w0;
		if (!mvi.ReadWord(w0) || w0 != 0x8005)
			return false;

		Bit16u nRawEntries = mvi.ReadWord();
		if (nRawEntries < 1)
			return false;
		Bit32u cbDir = mvi.ReadDWord();
		if (cbDir < 0x14)
			return false;

		for (Bit16u iRaw = 1; iRaw < nRawEntries; iRaw++) {
			WORD w;
			if (!mvi.ReadWord(w))
				return false;
		}
		CHeapPtr<Bit8u> dir;
		if (!dir.Allocate(cbDir))
			return false;
		if (!mvi.SurelyReadBytes(dir, cbDir))
			return false;

		Bit32u nDir = (cbDir -0x14) / sizeof(GraphicFileEntry);
		Bit32u iDir;

		GraphicFileEntry *E0 = (GraphicFileEntry *)&dir[0x14], *E;

		Bit16u maxcls1 = 0;
		Bit16u maxcls3[256];
		ZeroMemory(maxcls3, sizeof(maxcls3));

		E = E0;
		for (iDir=0; iDir<nDir; iDir++, E++) {
			maxcls1 = max(maxcls1, E->cls1);
			maxcls3[E->cls1] = max(maxcls3[E->cls1], E->cls3);
		}

		_4976_5cc2 = maxcls1;

		if (!_4976_5cb6.Allocate(maxcls1 +1))
			return false;
		{
			int n = 0;
			for (int x=0; x<256; x++) n += maxcls3[x] +1;

			if (!_4976_5cba.Allocate(n))
				return false;
		}
		if (!_4976_5cbe.Allocate(nDir))
			return false;

		Bit16u cursorcls3 = 0;
		Bit16u cursorents = 0;
		for (Bit16u cls1=0; cls1<=maxcls1; cls1++) {
			_4976_5cb6[cls1] = cursorcls3;

			for (Bit16u cls3=0; cls3<=maxcls3[cls1]; cls3++, cursorcls3++) {
				_4976_5cba[cursorcls3] = cursorents;

				E = E0;
				for (iDir=0; iDir<nDir; iDir++, E++) {
					if (E->cls1 != cls1 || E->cls3 != cls3)
						continue;
					RawEntry &RE = _4976_5cbe[cursorents];
					RE.cls2 = E->cls2;
					RE.cls4 = E->cls4;
					RE.data = BE2ME(E->data);
					cursorents++;
				}
			}
		}

		return true;
	}

private:
	// 
	Bit16u BE2ME(Bit16u xx) {
		// Convert BIG-ENDIAN to Machine specific endian
		// Assumes intel x86 CPU
		return (xx >> 8) | (xx << 8);
	}
};

