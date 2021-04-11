#ifndef FILEIO_H
  #define FILEIO_H

  #include <stdio.h>

  #ifndef TYPES_H
    #include "../types.h"
  #endif

  #define MAX_FILEHANDLES 10

  extern FILE* filehandle[MAX_FILEHANDLES];

  x16 DM2_FILE_OPEN(t_text* eaxt);
  x16 DM2_FILE_CREATE(t_text* eaxt);
  bool DM2_FILE_SEEK(x16 eaxw, x32 edxl);
  x32 DM2_GET_FILE_SIZE(x16 eaxw);
  bool DM2_FILE_READ(x16 handle, xxx* buffer, x32 amount);
  bool SKW_FILE_WRITE(x16 handle, xxx* buffer, x32 amount);
  x16 SKW_FILE_DELETE(t_text* filename);
  void SKW_FILE_RENAME(t_text* oldname, t_text* newname);
  x16 DM2_FILE_CLOSE(x16 handle);
  x16 R_1278(x16 val); // calls user interrupt
  void R_1296(x8* ptr); // calls user interrupt

  i8  SYS_findfirst(i8*);
  i8* SYS_gettransferadress(void);

#endif
