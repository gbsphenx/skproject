using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;
using DM2Internal;
using Newtonsoft.Json;

namespace convertDungeonDatToJson {
    class Program {
        static void Main(string[] args) {
            if (args.Length < 2) {
                Console.Error.WriteLine("convertDungeonDatToJson in.dat out.json ");
                Console.Error.WriteLine("convertDungeonDatToJson dungeon.dat dungeon.json ");
                Environment.ExitCode = 1;
                return;
            }
            new Program().Run(args[0], args[1]);
        }

        FileStream si;
        MemoryStream os;
        BinaryReader br;
        BinaryWriter wr;

        public System.UInt16 RandomGraphicsSeed;
        public System.UInt16 MapDataSize;
        public System.Byte NumberOfLevels;
        public System.UInt16 TextDataSize;
        public System.UInt16 StartingPartyPosition;
        public System.UInt16 ObjectListSize;
        public System.UInt16 NumberOfDoors;
        public System.UInt16 NumberOfTeleporters;
        public System.UInt16 NumberOfTexts;
        public System.UInt16 NumberOfActuators;
        public System.UInt16 NumberOfCreatures;
        public System.UInt16 NumberOfWeapons;
        public System.UInt16 NumberOfClothes;
        public System.UInt16 NumberOfScrolls;
        public System.UInt16 NumberOfPotions;
        public System.UInt16 NumberOfChests;
        public System.UInt16 NumberOfMiscItems;
        public System.UInt16 NumberOf11;
        public System.UInt16 NumberOf12;
        public System.UInt16 NumberOf13;
        public System.UInt16 NumberOfMissile;
        public System.UInt16 NumberOfCloud;
        // 
        public int NumberOfColumns;

        DungeonJson d = new DungeonJson();
        List<long> offsetsToDatabase = new List<long>();

        private void Run(String fpin, String fpout) {
            using (si = File.OpenRead(fpin)) {
                br = new BinaryReader(si);
                os = new MemoryStream();
                wr = new BinaryWriter(os);

                // http://dmweb.free.fr/?q=node/217

                d.randomGraphicsSeed = RandomGraphicsSeed = DoWord();
                MapDataSize = DoWord();
                NumberOfLevels = DoByte();
                DoByte();
                TextDataSize = DoWord();
                StartingPartyPosition = DoWord();
                d.startX = (StartingPartyPosition >> 0) & 31;
                d.startY = (StartingPartyPosition >> 5) & 31;
                d.startDir = (StartingPartyPosition >> 10) & 3;
                ObjectListSize = DoWord();
                NumberOfDoors = DoWord();
                NumberOfTeleporters = DoWord();
                NumberOfTexts = DoWord();
                NumberOfActuators = DoWord();
                NumberOfCreatures = DoWord();
                NumberOfWeapons = DoWord();
                NumberOfClothes = DoWord();
                NumberOfScrolls = DoWord();
                NumberOfPotions = DoWord();
                NumberOfChests = DoWord();
                NumberOfMiscItems = DoWord();
                NumberOf11 = DoWord();
                NumberOf12 = DoWord();
                NumberOf13 = DoWord();
                NumberOfMissile = DoWord();
                NumberOfCloud = DoWord();

                int i;
                NumberOfColumns = 0;
                for (i = 0; i < NumberOfLevels; i++) {
                    MapDef mapdef = new MapDef();
                    mapdef.mapOffset = DoWord();
                    int w2 = DoWord();
                    mapdef.useTeleporter = 0 != ((w2 >> 5) & 1);
                    mapdef.useDoor0 = 0 != ((w2 >> 7) & 1);
                    mapdef.useDoor1 = 0 != ((w2 >> 8) & 1);
                    mapdef.useUpperPitfall = (w2 & 1) != 0;
                    mapdef.useLowerPitfall = ((w2 >> 1) & 1) != 0;
                    mapdef.useUpStaircase = ((w2 >> 3) & 1) != 0;
                    mapdef.useDownStaircase = ((w2 >> 4) & 1) != 0;

                    DoByte();
                    DoByte();
                    mapdef.shiftX = DoByte();
                    mapdef.shiftY = DoByte();
                    int mapSize = DoWord();
                    mapdef.height = ((mapSize >> 11) & 31) + 1;
                    mapdef.width = ((mapSize >> 6) & 31) + 1;
                    mapdef.level = ((mapSize) & 63);
                    mapdef.currentNumberOfColumns = NumberOfColumns;
                    NumberOfColumns += mapdef.width;

                    int numGraphs = DoWord();
                    mapdef.numWallGraphics = (numGraphs) & 15;
                    mapdef.wallGraphicsRandomDecorations = (numGraphs >> 4) & 15;
                    mapdef.numFloorGraphics = (numGraphs >> 8) & 15;

                    int w12 = DoWord();
                    mapdef.numDoorDecorationGraphics = (w12) & 15;
                    mapdef.numCreaturesTypes = (w12 >> 4) & 15;
                    mapdef.difficulty = (w12 >> 12) & 15;

                    int w14 = DoWord();
                    mapdef.mapGraphicsStyle = (w14 >> 4) & 15;
                    mapdef.doorType0 = (w14 >> 8) & 15;
                    mapdef.doorType1 = (w14 >> 12) & 15;

                    mapdef.tileObjRefs = new int[mapdef.height * mapdef.width];
                    for (int t = 0; t < mapdef.tileObjRefs.Length; t++) {
                        mapdef.tileObjRefs[t] = -1;
                    }

                    d.maps.Add(mapdef);
                }

                List<int> columns = new List<int>();
                List<int> objectList = new List<int>();

                for (i = 0; i < NumberOfColumns; i++) {
                    columns.Add(DoWord());
                }
                for (i = 0; i < ObjectListSize; i++) {
                    objectList.Add(DoWord());
                }
                for (i = 0; i < TextDataSize; i++) {
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOfDoors; i++) {
                    DoWord();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOfTeleporters; i++) {
                    DoWord();
                    DoWord();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOfTexts; i++) {
                    DoWord();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOfActuators; i++) {
                    DoWord();
                    DoWord();
                    DoWord();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOfCreatures; i++) {
                    DoWord();
                    DoWord();
                    DoByte();
                    DoByte();
                    DoWord();
                    DoWord();
                    DoWord();
                    DoWord();
                    DoWord();
                    //
                    MyDWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOfWeapons; i++) {
                    DoWord();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOfClothes; i++) {
                    DoWord();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOfScrolls; i++) {
                    DoWord();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOfPotions; i++) {
                    DoWord();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOfChests; i++) {
                    DoWord();
                    DoWord();
                    DoByte();
                    DoByte();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOfMiscItems; i++) {
                    DoWord();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOf11; i++) {
                    DoWord();
                    DoWord();
                    DoWord();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOf12; i++) {
                    DoWord();
                    DoWord();
                    DoWord();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOf13; i++) {
                    DoWord();
                    DoWord();
                    DoWord();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOfMissile; i++) {
                    DoWord();
                    DoWord();
                    DoByte();
                    DoByte();
                    DoWord();
                }
                offsetsToDatabase.Add(si.Position);
                for (i = 0; i < NumberOfCloud; i++) {
                    DoWord();
                    DoWord();
                }

                Int64 offMapIn = si.Position;

                for (int z = 0; z < d.maps.Count; z++) {
                    var mapdef = d.maps[z];
                    si.Position = offMapIn + mapdef.mapOffset + mapdef.width * mapdef.height;
                    for (int t = 0; t < mapdef.numCreaturesTypes; t++) {
                        mapdef.allowedCreatureTypes.Add(br.ReadByte());
                    }
                    for (int t = 0; t < mapdef.numWallGraphics; t++) {
                        mapdef.wallGraphics.Add(br.ReadByte());
                    }
                    for (int t = 0; t < mapdef.numFloorGraphics; t++) {
                        mapdef.floorGraphics.Add(br.ReadByte());
                    }
                    for (int t = 0; t < mapdef.numDoorDecorationGraphics; t++) {
                        mapdef.doorDecorationGraphics.Add(br.ReadByte());
                    }
                    mapdef.doorDecorationGraphics.Add(-1);

                    si.Position = offMapIn + mapdef.mapOffset;
                    byte[] tileBytes = br.ReadBytes(mapdef.height * mapdef.width);
                    String tiles = "";
                    int tilePos = 0;
                    for (int x = 0; x < mapdef.width; x++) {
                        int currentObjectListIndex = columns[mapdef.currentNumberOfColumns + x];
                        for (int y = 0; y < mapdef.height; y++, tilePos++) {
                            byte tile = tileBytes[tilePos];
                            tiles += tile.ToString("X2");
                            bool hasObj = 0 != (tile & 16);
                            if (hasObj) {
                                int oid = objectList[currentObjectListIndex];
                                mapdef.tileObjRefs[tilePos] = convertObject(mapdef, oid, tile);

                                currentObjectListIndex++;
                            }
                        }
                    }
                    mapdef.tiles = tiles;
                }

                using (StreamWriter writer = new StreamWriter(fpout)) {
                    using (JsonTextWriter jsonWriter = new JsonTextWriter(writer)) {
                        jsonWriter.Formatting = Formatting.Indented;
                        new JsonSerializer().Serialize(jsonWriter, d);
                    }
                }
            }
        }

        int[] dbSizes = new int[] { 4, 6, 4, 8, 16, 4, 4, 4, 4, 8, 4, 0, 0, 0, 8, 4 };

        SortedSet<int> already = new SortedSet<int>();

        private int convertObject(MapDef mapdef, int oid, byte tile) {
            Debug.Assert(already.Add(oid));
            Debug.Assert(oid != oidFree);
            Debug.Assert(oid != oidEnd);
            int dir = (oid >> 14) & 3;
            int cat = (oid >> 10) & 15;
            int num = (oid) & 1023;
            int objRef = d.objs.Count;
            ObjDef newObj = null;
            si.Position = offsetsToDatabase[cat] + dbSizes[cat] * num;
            int nextOid = br.ReadUInt16();
            bool isWall = ((tileTypeIndex)(tile >> 5)) == tileTypeIndex.ttWall;
            switch ((dbIndex)cat) {
                case dbIndex.dbDoor: {
                        int attr = br.ReadUInt16();
                        bool doorType1 = 0 != (attr & 1);
                        int ornateIndex = (attr >> 1) & 15;

                        d.objs.Add(newObj = new DoorDef {
                            doorType = doorType1 ? mapdef.doorType1 : mapdef.doorType0,
                            doorOrnate = mapdef.doorDecorationGraphics[ornateIndex],
                            openVertical = 0 != ((attr >> 5) & 1),
                            button = 0 != ((attr >> 6) & 1),
                            destroyablebyFireball = 0 != ((attr >> 7) & 1),
                            bashablebyChopping = 0 != ((attr >> 8) & 1),
                            //9
                            //10
                            //11
                            buttonState = 0 != ((attr >> 12) & 1),
                            //13
                            //14
                            //15
                        });
                        break;
                    }
                case dbIndex.dbTeleporter: {
                        int attr = br.ReadUInt16();
                        int attr2 = br.ReadUInt16();
                        d.objs.Add(newObj = new TeleporterDef {
                            destMap = (attr2 >> 8) & 255,
                            destX = (attr >> 0) & 31,
                            destY = (attr >> 5) & 31,
                            rotation = (attr >> 10) & 3,
                            absoluteRotation = 0 != ((attr >> 12) & 1),
                            scope = (attr >> 13) & 3,
                            sound = 0 != ((attr >> 15) & 1),
                        });
                        break;
                    }
                case dbIndex.dbText: {
                        int attr = br.ReadUInt16();
                        d.objs.Add(newObj = new TextDef {
                            textVisibility = 0 != ((attr >> 0) & 1),
                            textMode = (attr >> 1) & 3,
                            textIndex = (attr >> 3) & 0x1fff,
                        });
                        break;
                    }
                case dbIndex.dbActuator: {
                        int attr = br.ReadUInt16();
                        int attr2 = br.ReadUInt16();
                        int attr3 = br.ReadUInt16();
                        d.objs.Add(newObj = new ActuatorDef {
                            actuatorType = (attr >> 0) & 127,
                            actuatorData = (attr >> 7) & 511,

                            onceOnlyActuator = 0 != ((attr2 >> 2) & 1),
                            actionType = (attr2 >> 3) & 3,
                            inversion = 0 != ((attr2 >> 5) & 1),
                            sound = 0 != ((attr2 >> 6) & 1),
                            delay = (attr2 >> 7) & 15,
                            floorOrnate = isWall ? -1 : (attr2 >> 12) & 15,
                            wallOrnate = isWall ? (attr2 >> 12) & 15 : -1,

                            newDirection = (attr3 >> 4) & 3,
                            xCoord = (attr3 >> 6) & 31,
                            yCoord = (attr3 >> 11) & 31,
                        });
                        break;
                    }
                case dbIndex.dbCreature: {
                        int childOid = br.ReadUInt16();
                        int creatureType = br.ReadByte();
                        int position = br.ReadByte();
                        int hp1 = br.ReadUInt16();
                        int hp2 = br.ReadUInt16();
                        int hp3 = br.ReadUInt16();
                        int hp4 = br.ReadUInt16();
                        CreatureDef me;
                        d.objs.Add(newObj = me = new CreatureDef {
                            creatureType = creatureType,
                            position = position,
                            hp1 = hp1,
                            hp2 = hp2,
                            hp3 = hp3,
                            hp4 = hp4,
                        });
                        if (childOid != oidEnd) {
                            me.childObjRef = convertObject(mapdef, childOid, tile);
                        }

                        break;
                    }
                case dbIndex.dbWeapon: {
                        int attr = br.ReadUInt16();
                        d.objs.Add(newObj = new WeaponDef {
                            itemType = (attr >> 0) & 127,
                            important = 0 != ((attr >> 7) & 1),
                            charges = (attr >> 10) & 15,
                        });
                        break;
                    }
                case dbIndex.dbCloth: {
                        int attr = br.ReadUInt16();
                        d.objs.Add(newObj = new ClothDef {
                            itemType = (attr >> 0) & 127,
                            important = 0 != ((attr >> 7) & 1),
                            charges = (attr >> 10) & 15,
                        });
                        break;
                    }
                case dbIndex.dbScroll: {
                        int attr = br.ReadUInt16();
                        d.objs.Add(newObj = new ScrollDef {
                            referredText = attr & 0x3ff,
                        });
                        break;
                    }
                case dbIndex.dbPotion: {
                        int attr = br.ReadUInt16();
                        d.objs.Add(newObj = new PotionDef {
                            potionPower = (attr >> 0) & 255,
                            potionType = (attr >> 8) & 127,
                            visiblePower = 0 != ((attr >> 15) & 1),
                        });
                        break;
                    }
                case dbIndex.dbContainer: {
                        int childOid = br.ReadUInt16();
                        int attr = br.ReadByte();
                        int attr2 = br.ReadByte();
                        int attr3 = br.ReadUInt16();
                        ContainerDef me;
                        d.objs.Add(newObj = me = new ContainerDef {
                            isOpened = 0 != ((attr >> 0) & 1),
                            containerType = (attr >> 1) & 3,
                            destX = (attr3 >> 0) & 31,
                            destY = (attr3 >> 5) & 31,
                            destMap = (attr >> 10) & 63,
                        });
                        if (childOid != oidEnd) {
                            me.childObjRef = convertObject(mapdef, childOid, tile);
                        }
                        break;
                    }
                case dbIndex.dbMiscellaneous_item: {
                        int attr = br.ReadUInt16();
                        d.objs.Add(newObj = new MiscItemDef {
                            itemType = (attr >> 0) & 127,
                            important = 0 != ((attr >> 7) & 1),
                            charges = (attr >> 8) & 0x3f,
                        });
                        break;
                    }
                case dbIndex.dbMissile: {
                        int childOid = br.ReadUInt16();
                        MissileDef me;
                        d.objs.Add(newObj = me = new MissileDef {
                        });
                        if (childOid != oidEnd) {
                            me.childObjRef = convertObject(mapdef, childOid, tile);
                        }
                        break;
                    }
                case dbIndex.dbCloud: {
                        d.objs.Add(newObj = new CloudDef {
                        });
                        break;
                    }
                default:
                    throw new NotSupportedException();
            }
            newObj.direction = dir;
            if (nextOid != oidEnd) {
                newObj.nextObjRef = convertObject(mapdef, nextOid, tile);
            }
            return objRef;
        }

        int oidFree = 0xFFFF;
        int oidEnd = 0xFFFE;

        private void MyDWord() {
            wr.Write((uint)0);
        }
        private void MyWord() {
            wr.Write((ushort)0);
        }

        private void DoBytes(int cb) {
            byte[] bin = new byte[cb];
            if (si.Read(bin, 0, cb) != cb) throw new EndOfStreamException();
            os.Write(bin, 0, cb);
        }

        private byte DoByte() {
            byte r;
            wr.Write(r = br.ReadByte());
            return r;
        }

        private ushort DoWord() {
            ushort r;
            wr.Write(r = br.ReadUInt16());
            return r;
        }
    }
}
