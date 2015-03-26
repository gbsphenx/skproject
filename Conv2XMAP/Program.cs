using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;

namespace Conv2XMAP {
    class Program {
        static void Main(string[] args) {
            if (args.Length < 2) {
                Console.Error.WriteLine("Conv2XMAP in.dat out.dat ");
                Console.Error.WriteLine("Conv2XMAP dungeon.dat dungeon_xmap.dat ");
                Environment.ExitCode = 1;
                return;
            }
            new Program().Run(args[0], args[1]);
        }

        FileStream si, os;
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
        public System.UInt16 NumberOfColumns;

        private void Run(String fpin, String fpout) {
            using (si = File.OpenRead(fpin)) {
                using (os = File.Create(fpout)) {
                    br = new BinaryReader(si);
                    wr = new BinaryWriter(os);

                    List<Int64> posMapOff = new List<long>();

                    RandomGraphicsSeed = DoWord();
                    MapDataSize = DoWord();
                    NumberOfLevels = DoByte();
                    DoByte();
                    TextDataSize = DoWord();
                    StartingPartyPosition = DoWord();
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
                        //posMapOff.Add(os.Position);

                        DoWord();
                        DoWord();
                        DoByte();
                        DoByte();
                        DoByte();
                        DoByte();
                        NumberOfColumns += (System.UInt16)(((DoWord() >> 6) & 0x1F) + 1);
                        DoWord();
                        DoWord();
                        DoWord();
                    }

                    for (i = 0; i < NumberOfColumns; i++) {
                        DoWord();
                    }
                    for (i = 0; i < ObjectListSize; i++) {
                        DoWord();
                    }
                    for (i = 0; i < TextDataSize; i++) {
                        DoWord();
                    }
                    for (i = 0; i < NumberOfDoors; i++) {
                        DoWord();
                        DoWord();
                    }
                    for (i = 0; i < NumberOfTeleporters; i++) {
                        DoWord();
                        DoWord();
                        DoWord();
                    }
                    for (i = 0; i < NumberOfTexts; i++) {
                        DoWord();
                        DoWord();
                    }
                    for (i = 0; i < NumberOfActuators; i++) {
                        DoWord();
                        DoWord();
                        DoWord();
                        DoWord();
                    }
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
                    for (i = 0; i < NumberOfWeapons; i++) {
                        DoWord();
                        DoWord();
                    }
                    for (i = 0; i < NumberOfClothes; i++) {
                        DoWord();
                        DoWord();
                    }
                    for (i = 0; i < NumberOfScrolls; i++) {
                        DoWord();
                        DoWord();
                    }
                    for (i = 0; i < NumberOfPotions; i++) {
                        DoWord();
                        DoWord();
                    }
                    for (i = 0; i < NumberOfChests; i++) {
                        DoWord();
                        DoWord();
                        DoByte();
                        DoByte();
                        DoWord();
                        //
                        MyDWord();
                    }
                    for (i = 0; i < NumberOfMiscItems; i++) {
                        DoWord();
                        DoWord();
                    }
                    for (i = 0; i < NumberOf11; i++) {
                        DoWord();
                        DoWord();
                        DoWord();
                        DoWord();
                    }
                    for (i = 0; i < NumberOf12; i++) {
                        DoWord();
                        DoWord();
                        DoWord();
                        DoWord();
                    }
                    for (i = 0; i < NumberOf13; i++) {
                        DoWord();
                        DoWord();
                        DoWord();
                        DoWord();
                    }
                    for (i = 0; i < NumberOfMissile; i++) {
                        DoWord();
                        DoWord();
                        DoByte();
                        DoByte();
                        DoWord();
                        //
                        MyDWord();
                    }
                    for (i = 0; i < NumberOfCloud; i++) {
                        DoWord();
                        DoWord();
                    }

                    Int64 offMapIn = si.Position;
                    Int64 offMapOut = os.Position;

                    DoBytes(MapDataSize);

                    //DoWord();
                    Debug.WriteLine(si.Position + " " + si.Length + " -- " + os.Position);

                    BinaryReader rr = new BinaryReader(os);

                    foreach (Int64 off in posMapOff) {
                        os.Position = off;
                        ushort v = rr.ReadUInt16();
                        v = Convert.ToUInt16(v + offMapOut - offMapIn);

                        si.Position = off;
                        wr.Write(v);
                    }
                }
            }
        }

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
