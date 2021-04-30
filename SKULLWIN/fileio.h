#ifndef FILEIO_H
  #define FILEIO_H

  #include <stdio.h>

  #ifndef TYPES_H
    #include "types.h"
  #endif

  #define MAX_FILEHANDLES 10

  #define FILEMODE_WB "wb"
  #define FILEMODE_RB "rb"
  #define FILEMODE_WT "wt"
  #define FILEMODE_RPB "r+b"

  class c_fileiodata
  {
    public:
      i16 filehandles; // was i32, not needed TODO: a const-define should do it
      i32* devicetableptr;
      i32 statecheck;
      i32 filestatexormask;

      void init(void);
  };

  extern c_fileiodata filedat;

  extern FILE* filehandle[MAX_FILEHANDLES];

  i16 DM2_FILE_OPEN(t_text* eaxtp);
  i16 DM2_FILE_CREATE(t_text* eaxtp);
  bool DM2_FILE_SEEK(i16 eaxw, i32 edxl);
  i32 DM2_GET_FILE_SIZE(i16 eaxw);
  bool DM2_FILE_READ(i16 handle, unk* buffer, i32 amount);
  bool DM2_FILE_WRITE(i16 handle, unk* buffer, i32 amount);
  i16 DM2_FILE_DELETE(t_text* filename);
  void DM2_FILE_RENAME(t_text* oldname, t_text* newname);
  void DM2_FILE_CLOSE(i16 handle);
  void DM2_READ_BINARY(t_text* filename, void* buf, size_t size);

#endif
