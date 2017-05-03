using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Linq;
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
                    mapdef.v2 = DoWord();
                    mapdef.v4 = DoWord();
                    mapdef.shiftX = DoByte();
                    mapdef.shiftY = DoByte();
                    mapdef.v8 = DoWord();
                    mapdef.v10 = DoWord();
                    mapdef.v12 = DoWord();
                    mapdef.v14 = DoWord();

                    mapdef.width = (byte)(((mapdef.v8 >> 6) & 31) + 1);
                    mapdef.height = (byte)(((mapdef.v8 >> 11) & 31) + 1);

                    NumberOfColumns += mapdef.width;

                    d.maps.Add(mapdef);
                }

                List<int> perColumn = new List<int>();
                List<int> groundStacks = new List<int>();
                List<int> textData = new List<int>();

                d.mapTilesObjectIndexPerColumn = perColumn;
                d.groundStacks = groundStacks;
                d.textData = textData;

                for (i = 0; i < NumberOfColumns; i++) {
                    perColumn.Add(DoWord());
                }
                for (i = 0; i < ObjectListSize; i++) {
                    groundStacks.Add(DoWord());
                }
                for (i = 0; i < TextDataSize; i++) {
                    textData.Add(DoWord());
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
                byte[] mapDataBytes = br.ReadBytes(MapDataSize);
                String mapData = BitConverter.ToString(mapDataBytes).Replace("-", "");

                d.mapData = mapDataBytes.Select(v => (int)v).ToList();

                int currentColumn = 0;

                for (int z = 0; z < d.maps.Count; z++) {
                    var mapdef = d.maps[z];
                    int tilePos = mapdef.mapOffset;

                    for (int x = 0; x < mapdef.width; x++, currentColumn++) {
                        int currentObjectListIndex = perColumn[currentColumn];

                        for (int y = 0; y < mapdef.height; y++, tilePos++) {
                            byte tile = mapDataBytes[tilePos];
                            bool hasObj = 0 != (tile & 16);
                            if (hasObj) {
                                int oldOid = groundStacks[currentObjectListIndex];
                                groundStacks[currentObjectListIndex] = convertObject(mapdef, oldOid, tile);

                                currentObjectListIndex++;
                            }
                        }
                    }
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
            int objRef = d.dataTypeAndIndexMap.Count;
            ObjDef newObj = null;
            si.Position = offsetsToDatabase[cat] + dbSizes[cat] * num;
            int nextOid = br.ReadUInt16();
            bool isWall = ((tileTypeIndex)(tile >> 5)) == tileTypeIndex.ttWall;
            switch ((dbIndex)cat) {
                case dbIndex.dbDoor: {
                        int attr1 = br.ReadUInt16();

                        var target = d.doors;
                        d.dataTypeAndIndexMap.Add(0x100000 * dir + 0x10000 * cat + target.Count);
                        target.Add(new DoorDef {
                            attr1 = attr1,
                        });
                        newObj = target.Last();
                        break;
                    }
                case dbIndex.dbTeleporter: {
                        int attr1 = br.ReadUInt16();
                        int attr2 = br.ReadUInt16();
                        var target = d.teleporters;
                        d.dataTypeAndIndexMap.Add(0x100000 * dir + 0x10000 * cat + target.Count);
                        target.Add(new TeleporterDef {
                            attr1 = attr1,
                            attr2 = attr2,
                        });
                        newObj = target.Last();
                        break;
                    }
                case dbIndex.dbText: {
                        int attr1 = br.ReadUInt16();
                        var target = d.texts;
                        d.dataTypeAndIndexMap.Add(0x100000 * dir + 0x10000 * cat + target.Count);
                        target.Add(new TextDef {
                            attr1 = attr1,
                        });
                        newObj = target.Last();
                        break;
                    }
                case dbIndex.dbActuator: {
                        int attr1 = br.ReadUInt16();
                        int attr2 = br.ReadUInt16();
                        int attr3 = br.ReadUInt16();
                        var target = d.actuators;
                        d.dataTypeAndIndexMap.Add(0x100000 * dir + 0x10000 * cat + target.Count);
                        target.Add(new ActuatorDef {
                            attr1 = attr1,
                            attr2 = attr2,
                            attr3 = attr3,
                        });
                        newObj = target.Last();
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
                        int attr1 = br.ReadUInt16();
                        CreatureDef me;
                        var target = d.creatures;
                        d.dataTypeAndIndexMap.Add(0x100000 * dir + 0x10000 * cat + target.Count);
                        target.Add(me = new CreatureDef {
                            creatureType = creatureType,
                            position = position,
                            hp1 = hp1,
                            hp2 = hp2,
                            hp3 = hp3,
                            hp4 = hp4,
                            attr1 = attr1,
                        });
                        newObj = target.Last();
                        if (childOid != oidEnd) {
                            me.childObjRef = convertObject(mapdef, childOid, tile);
                        }

                        break;
                    }
                case dbIndex.dbWeapon: {
                        int attr1 = br.ReadUInt16();
                        var target = d.weapons;
                        d.dataTypeAndIndexMap.Add(0x100000 * dir + 0x10000 * cat + target.Count);
                        target.Add(new WeaponDef {
                            attr1 = attr1,
                        });
                        newObj = target.Last();
                        break;
                    }
                case dbIndex.dbCloth: {
                        int attr1 = br.ReadUInt16();
                        var target = d.clothings;
                        d.dataTypeAndIndexMap.Add(0x100000 * dir + 0x10000 * cat + target.Count);
                        target.Add(new ClothDef {
                            attr1 = attr1,
                        });
                        newObj = target.Last();
                        break;
                    }
                case dbIndex.dbScroll: {
                        int attr1 = br.ReadUInt16();
                        var target = d.scrolls;
                        d.dataTypeAndIndexMap.Add(0x100000 * dir + 0x10000 * cat + target.Count);
                        target.Add(new ScrollDef {
                            attr1 = attr1,
                        });
                        newObj = target.Last();
                        break;
                    }
                case dbIndex.dbPotion: {
                        int attr1 = br.ReadUInt16();
                        var target = d.potions;
                        d.dataTypeAndIndexMap.Add(0x100000 * dir + 0x10000 * cat + target.Count);
                        target.Add(new PotionDef {
                            attr1 = attr1,
                        });
                        newObj = target.Last();
                        break;
                    }
                case dbIndex.dbContainer: {
                        int childOid = br.ReadUInt16();
                        int attr1 = br.ReadUInt16();
                        int attr2 = br.ReadUInt16();
                        ContainerDef me;
                        var target = d.containers;
                        d.dataTypeAndIndexMap.Add(0x100000 * dir + 0x10000 * cat + target.Count);
                        target.Add(me = new ContainerDef {
                            attr1 = attr1,
                            attr2 = attr2,
                        });
                        newObj = target.Last();
                        if (childOid != oidEnd) {
                            me.childObjRef = convertObject(mapdef, childOid, tile);
                        }
                        break;
                    }
                case dbIndex.dbMiscellaneous_item: {
                        int attr1 = br.ReadUInt16();
                        var target = d.miscs;
                        d.dataTypeAndIndexMap.Add(0x100000 * dir + 0x10000 * cat + target.Count);
                        target.Add(new MiscItemDef {
                            attr1 = attr1,
                        });
                        newObj = target.Last();
                        break;
                    }
                case dbIndex.dbMissile: {
                        int childOid = br.ReadUInt16();
                        int attr1 = br.ReadUInt16();
                        int attr2 = br.ReadUInt16();
                        MissileDef me;
                        var target = d.missiles;
                        d.dataTypeAndIndexMap.Add(0x100000 * dir + 0x10000 * cat + target.Count);
                        target.Add(me = new MissileDef {
                            attr1 = attr1,
                            attr2 = attr2,
                        });
                        newObj = target.Last();
                        if (childOid != oidEnd) {
                            me.childObjRef = convertObject(mapdef, childOid, tile);
                        }
                        break;
                    }
                case dbIndex.dbCloud: {
                        int attr1 = br.ReadUInt16();
                        var target = d.clouds;
                        d.dataTypeAndIndexMap.Add(0x100000 * dir + 0x10000 * cat + target.Count);
                        target.Add(new CloudDef {
                            attr1 = attr1,
                        });
                        newObj = target.Last();
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
