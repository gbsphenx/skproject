#ifndef C_MAP_H
  #define C_MAP_H

  #ifndef TYPES_H
    #include <types.h>
  #endif

  #ifndef C_RECORD_H
    #include <skrecord.h>
  #endif

  typedef ui8 t_tile; // must be unsigned!

  class c_mapdata
  {
    public:
      i16 map_width; // was savegamewa
      i16 map_height; // was savegamewb
      t_tile** map; // was savegamepp1
      t_tile** tmpmap; // was v1e03cc TODO: adjust type, still always casted

      void init(void);
  };

  extern c_mapdata mapdat;

  i16 DM2_GET_OBJECT_INDEX_FROM_TILE(i16 x, i16 y);
  t_record DM2_GET_TILE_RECORD_LINK(i16 x, i16 y);
  t_record* DM2_GET_ADDRESS_OF_TILE_RECORD(i16 x, i16 y);

  t_tile DM2_GET_TILE_VALUE(i32 eaxl, i32 edxl);
// TODO:
  #define dm2_GET_TILE_VALUE(x, y) tile_to_ulong(DM2_GET_TILE_VALUE(x, y))
  ui32 tile_to_ulong(t_tile);
  ui8 tile_to_ubyte(t_tile);

  void DM2_CHANGE_CURRENT_MAP_TO(i32 eaxl);
  i32 DM_LOCATE_OTHER_LEVEL(i32 eaxl, i32 edxl, i16* ebxpw, i16* ecxpw, unk** argpp0);
  i32 DM2_SET_DESTINATION_OF_MINION_MAP(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);
  i32 DM2_map_0cee_17e7(i32 eaxl, i32 edxl, i32 ebxl);
  i32 DM2_map_0cee_1815(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0);
  void DM2_map_0cee_185a(unk* xeaxp, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0, i32 argl1, i16 argw2, i16 argw3);
  void DM2_ARRANGE_DUNGEON(void);
  i32 DM2_map_0cee_04e5(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl, i16 argw0);
  void DM2_map_3B001(i32 eaxl, i32 edxl);
  void DM2_map_3BF83(i32 eaxl, i32 edxl, i32 ebxl, i32 ecxl);

#endif
