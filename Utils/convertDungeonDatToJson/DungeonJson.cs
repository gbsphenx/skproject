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

        public List<DoorDef> doors = new List<DoorDef>();
        public List<TeleporterDef> teleporters = new List<TeleporterDef>();
        public List<TextDef> texts = new List<TextDef>();
        public List<ActuatorDef> actuators = new List<ActuatorDef>();
        public List<CreatureDef> creatures = new List<CreatureDef>();
        public List<WeaponDef> weapons = new List<WeaponDef>();
        public List<ClothDef> clothings = new List<ClothDef>();
        public List<ScrollDef> scrolls = new List<ScrollDef>();
        public List<PotionDef> potions = new List<PotionDef>();
        public List<ContainerDef> containers = new List<ContainerDef>();
        public List<MiscItemDef> miscs = new List<MiscItemDef>();
        public List<MissileDef> missiles = new List<MissileDef>();
        public List<CloudDef> clouds = new List<CloudDef>();

        public List<int> mapTilesObjectIndexPerColumn;
        public List<int> groundStacks;
        public List<int> textData;
        public List<int> mapData;

        public List<int> dataTypeAndIndexMap = new List<int>();
    }

    public class MapDef {
        public int mapOffset;
        public int v2;
        public int v4;
        public int shiftX;
        public int shiftY;
        public int v8;
        public int v10;
        public int v12;
        public int v14;

        public int width;
        public int height;

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
        public int attr1;
    }
    public class TeleporterDef : ObjDef {
        public String type = "teleporter";
        public int attr1;
        public int attr2;
    }
    public class TextDef : ObjDef {
        public String type = "text";
        public int attr1;
    }
    public class ActuatorDef : ObjDef {
        public String type = "actuator";
        public int attr1;
        public int attr2;
        public int attr3;
    }
    public class CreatureDef : ContainableDef {
        public String type = "creature";
        public int creatureType;
        public int position;
        public int hp1;
        public int hp2;
        public int hp3;
        public int hp4;
        public int attr1;
    }
    public class WeaponDef : ObjDef {
        public String type = "weapon";
        public int attr1;
    }
    public class ClothDef : ObjDef {
        public String type = "cloth";
        public int attr1;
    }
    public class ScrollDef : ObjDef {
        public String type = "scroll";
        public int attr1;
    }
    public class PotionDef : ObjDef {
        public String type = "potion";
        public int attr1;
    }
    public class ContainerDef : ContainableDef {
        public String type = "container";
        public int attr1;
        public int attr2;
    }
    public class MiscItemDef : ObjDef {
        public String type = "miscItem";
        public int attr1;
    }
    public class MissileDef : ContainableDef {
        public String type = "missile";
        public int attr1;
        public int attr2;
    }
    public class CloudDef : ObjDef {
        public String type = "cloud";
        public int attr1;
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
