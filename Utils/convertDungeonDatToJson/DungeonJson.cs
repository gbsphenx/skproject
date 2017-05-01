using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DM2Internal {
    public class DungeonJson {
        public int randomGraphicsSeed;
        public int startX;
        public int startY;
        public int startDir;
        public int startMap = 0;
        public List<MapDef> maps = new List<MapDef>();
        public List<ObjDef> objs = new List<ObjDef>();
    }

    public class MapDef {
        [NonSerialized]
        public int mapOffset;
        [NonSerialized]
        public int currentNumberOfColumns;
        [NonSerialized]
        public int numWallGraphics;
        [NonSerialized]
        public int numFloorGraphics;
        [NonSerialized]
        public int numFloorRandom;
        [NonSerialized]
        public int numDoorDecorationGraphics;
        [NonSerialized]
        public int numCreaturesTypes;
        [NonSerialized]
        public int doorType0;
        [NonSerialized]
        public int doorType1;
        [NonSerialized]
        public List<int> wallGraphics = new List<int>();
        [NonSerialized]
        public List<int> floorGraphics = new List<int>();
        [NonSerialized]
        public List<int> doorDecorationGraphics = new List<int>();

        /// <summary>
        /// in dm style. hexstr: 00 to FF. top to down, and then left to right.
        /// </summary>
        public string tiles;

        /// <summary>
        /// height*width, top down, then left-to-right.
        /// -1 if no obj.
        /// </summary>
        public int[] tileObjRefs;

        public bool useTeleporter;
        public bool useDoor0;
        public bool useDoor1;
        public bool useUpperPitfall;
        public bool useLowerPitfall;
        public bool useUpStaircase;
        public bool useDownStaircase;
        public byte shiftX;
        public byte shiftY;
        public int height;
        public int width;
        public int level;
        public int difficulty;
        public int mapGraphicsStyle;
        public int wallGraphicsRandomDecorations;
        public List<int> allowedCreatureTypes = new List<int>();
    }

    public class ObjDef {
        public int direction;
        public int nextObjRef = -1;
    }
    public class ContainableDef : ObjDef {
        public int childObjRef = -1;
    }
    public class DoorDef : ObjDef {
        public String type = "door";
        public bool buttonState;
        public bool bashablebyChopping;
        public bool destroyablebyFireball;
        public bool button;
        public bool openVertical;
        /// <summary>
        /// real door type index in gdat.
        /// </summary>
        public int doorType;
        /// <summary>
        /// real door ornate index in gdat. -1 has no one.
        /// </summary>
        public int doorOrnate = -1;

        public bool openingDir;
    }
    public class TeleporterDef : ObjDef {
        public String type = "teleporter";
        public int destX;
        public int destY;
        public int destMap;
        public int rotation;
        public bool absoluteRotation;
        public int scope;
        public bool sound;
    }
    public class TextDef : ObjDef {
        public String type = "text";
        public bool textVisibility;
        public int textMode;
        /// <summary>
        /// either: textIndex, wall ornate or floor ornate.
        /// </summary>
        public int textIndex;
    }
    public class ActuatorDef : ObjDef {
        public String type = "actuator";

        public int actuatorType;
        public int actuatorData;
        public bool activeStatus;
        public bool onceOnlyActuator;
        public int actionType;
        public bool inversion;
        public bool sound;
        /// <summary>
        /// delay or ShopItemPoolNo
        /// </summary>
        public int delay;
        public int floorOrnate = -1;
        public int wallOrnate = -1;
        public int newDirection;
        public int xCoord;
        public int yCoord;
    }
    public class CreatureDef : ContainableDef {
        public String type = "creature";
        public int creatureType;
        public int position;
        public int hp1;
        public int hp2;
        public int hp3;
        public int hp4;
    }
    public class WeaponDef : ObjDef {
        public String type = "weapon";
        public int itemType;
        public bool important;
        public int charges;
    }
    public class ClothDef : ObjDef {
        public String type = "cloth";
        public int itemType;
        public bool important;
        public int charges;
    }
    public class ScrollDef : ObjDef {
        public String type = "scroll";
        public int referredText;
    }
    public class PotionDef : ObjDef {
        public String type = "potion";
        public int potionPower;
        public int potionType;
        public bool visiblePower;
    }
    public class ContainerDef : ContainableDef {
        public String type = "container";
        public bool isOpened;
        public int containerType;
        public int destX;
        public int destY;
        public int destMap;
    }
    public class MiscItemDef : ObjDef {
        public String type = "miscItem";
        public int itemType;
        public bool important;
        public int charges;
    }
    public class MissileDef : ContainableDef {
        public String type = "missile";
    }
    public class CloudDef : ObjDef {
        public String type = "cloud";
    }

    public enum dbIndex {
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
    }

    public enum teleporterScope { // teleporter scope
        items = 0,
        creatures = 1,
        itemsParty = 2,
        everything = 3,
    }

    public enum tileTypeIndex {
        ttWall = 0,		// 0
        ttFloor,		// 1
        ttPit,			// 2
        ttStairs,		// 3
        ttDoor,			// 4
        ttTeleporter,	// 5
        ttTrickWall,	// 6
        ttMapExit,		// 7
    };
}
