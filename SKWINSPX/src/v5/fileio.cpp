#include <startend.h>
#include <util.h> // DM2_COPY_MEMORY
#include <fileio.h>
#include <dm2data.h>

#include <stdlib.h>
#include <errno.h>

#include <dm2debug.h>

#ifdef __LINUX__
#include <unistd.h>	// for unlink
#endif

#ifdef __DJGPP__
#include <unistd.h>
#undef _unlink
#define _unlink unlink
#endif // __DJGPP__



// SPX: changed static of 10 handles to 100 because modern systems like Linux 64 will use handle above 10
/*
FILE* filehandle[MAX_FILEHANDLES] =
{
  NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL
};
*/
FILE** filehandle;

i32 devicetable[0x14] =
{
  0x1, 0x2, 0x2, 0x3, 0x2,
  0x4000, 0x4000, 0x4000,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

c_fileiodata filedat;

void c_fileiodata::init(void)
{
	SPX_DEBUG_PRINT("FILEIODATA:INIT\n");
  filehandles = 0x14; // const
  devicetableptr = devicetable; // const
  statecheck = 0x100; // const
  filestatexormask = 0x0; // const
  
  filehandle = (FILE**) calloc(MAX_FILEHANDLES, sizeof(FILE*));
}

// 0x21/0x2f
i8* SYS_gettransferadress(void)
{
/*
INT 21 - DOS 2+ - GET DISK TRANSFER AREA ADDRESS
  AH = 2Fh
Return: ES:BX -> current DTA
Note:  under the FlashTek X - 32 DOS extender, the pointer is in ES:EBX
SeeAlso: AH=1Ah
*/
  throw(THROW_DMABORT); // TODO
  return NULL;
}

// comments about DOS - calls by Ralf Braun's interrupt list

// ################################################################################
// ################################################################################
// ################################################################################

// 0x21/0x3c
static i16 SYS_createfile(i8 attrib, t_text* filename, bool& boolc)
{
  // DOS - 2+ - CREATE A FILE WITH HANDLE
  // CX = attributes for file - !unused, we just use FILEMODE_WB
  // in fact the values 0 and 1 are used
  // DS:DX -> ASCII filename (may include drive and path)
  // affects the flags
  i16 wh;
  t_text* tp = filename;
  if (tp[1] == ':')
  {
    tp[0] = '.';
    tp[1] = '\\';
  }
  FILE* fhandle = fopen(tp, FILEMODE_WB);
  if (fhandle == NULL)
  {
    switch (errno)
    {
      case ENOENT:
        wh = 2; // file not found
        boolc = true;
      break;
      case EACCES:
      default:
        wh = 255;
        boolc = true;
      // break;
    }
  }
  else
  {
#ifdef __LINUX__
	wh = fileno(fhandle);
#else // __MSVC6__
    wh = fhandle->_file;
#endif
    if (wh < 0 || wh > (MAX_FILEHANDLES-1)) throw(THROW_DMABORT);
    if (filehandle[wh] != NULL) throw(THROW_DMABORT);
    filehandle[wh] = fhandle;
    boolc = false;
  }
  return wh;
}

// 0x21/0x3d
static i16 SYS_openfile(i8 bmode, t_text* filename, bool& flag)
{
  // DOS - 2+ - OPEN DISK FILE WITH HANDLE
  // DS:DX -> ASCII filename
  // AL = access mode
  // 0 - read, 1 - write, 2 - read & write
  // Return: CF clear if successful
  // AX = file handle
  // CF set on error, then:
  //   AX = error code (01h,02h,03h,04h,05h,0Ch,56h) (see #01680 at AH=59h)
  i16 wh;
  if (bmode != 1 && bmode != 0 && bmode != 2)
    throw(THROW_DMABORT);
  t_text* tp = filename;
  if (tp[1] == ':')
  {
    tp[0] = '.';
    tp[1] = '\\';
  }
  FILE* fhandle = fopen(tp, bmode == 0 ? FILEMODE_RB : (bmode == 1 ? FILEMODE_WB : FILEMODE_RPB));
  if (fhandle == NULL)
  {
    switch (errno)
    {
      case ENOENT:
        wh = 2; // file not found
        flag = true;
      break;
      case EACCES:
      default:
        wh = 255;
        flag = true;
      // break;
    }
  }
  else
  {
#ifdef __LINUX__
	wh = fileno(fhandle);
#else // __MSVC6__
    wh = fhandle->_file;
#endif
    if (wh < 0 || wh > (MAX_FILEHANDLES-1)) throw(THROW_DMABORT);
    if (filehandle[wh] != NULL) throw(THROW_DMABORT);
    filehandle[wh] = fhandle;
    flag = false;
  }
  return wh;
}

// 0x21/0x3e
static i16 SYS_closefile(i16 handle)
{
  // DOS - 2+ - CLOSE A FILE WITH HANDLE
  // BX = file handle
  if (handle < 0 || handle > (MAX_FILEHANDLES-1))
    throw(THROW_DMABORT);
  if (filehandle[handle] == NULL)
    throw(THROW_DMABORT);
  i16 wret = fclose(filehandle[handle]);
  filehandle[handle] = NULL;
  return wret;
}

// 0x21/0x3f
static i32 SYS_readfile(i16 handle, i32 size, i8* bbuffer, bool& boolc)
{
  // DOS - 2+ - READ FROM FILE WITH HANDLE
  // BX = file handle, CX = number of bytes to read
  // DS:DX -> buffer
  // assuming DS to be 0
  // Return: CF clear if successful
  //         AX = number of bytes actually read (0 if at EOF before call)
  //         CF set on error
  //         AX = error code (05h,06h) (see #01680 at AH=59h/BX=0000h)
  // sometimes jul is requested after this, which is carry after all
  i32 ret;
  if (handle < 0 || handle > (MAX_FILEHANDLES-1))
    throw(THROW_DMABORT);
  if (filehandle[handle] == NULL)
    throw(THROW_DMABORT);
  size_t readsize = fread(bbuffer, 1, size, filehandle[handle]);
  boolc = mkl(readsize) != size;
  if (boolc)
    ret = 0x80000005;
  else
    ret = mkl(readsize);
  return ret;
}

// 0x21/040
// rotate pulled out of code
static i32 SYS_writefile(i16 handle, i32 size, i8* bbuffer, bool& boolc)
{
  // DOS - 2+ - WRITE TO FILE WITH HANDLE
  // BX = file handle, CX = number of bytes to write
  // Return: CF clear if successful
  //         AX = number of bytes actually written (0 if at EOF before call)
  //         CF set on error
  //         AX = error code (05h,06h) (see #01680 at AH=59h/BX=0000h)
  // sometimes jul is requested after this, which is carry after all
  i32 ret;
  if (handle < 0 || handle > (MAX_FILEHANDLES-1))
    throw(THROW_DMABORT);
  if (filehandle[handle] == NULL)
    throw(THROW_DMABORT);
  size_t writesize = fwrite(bbuffer, 1, size, filehandle[handle]);
  boolc = mkl(writesize) != size;
  if (boolc)
    ret = 0x80000005;
  else
    ret = mkl(writesize);
  return ret;
}

// 0x21/0x41
static i16 SYS_deletefile(t_text* filename)
{
  // DOS - 2+ - DELETE A FILE (UNLINK)
  // DS:DX -> ASCII pathname of file to delete (no wildcards allowed)
  t_text* tp = filename;
  if (tp[1] == ':')
  {
    tp[0] = '.';
    tp[1] = '\\';
  }
#ifdef __LINUX__
  bool boolc = unlink(tp) != 0;
#else
  bool boolc = _unlink(tp) != 0;
#endif // __LINUX__
  return boolc ? 0x5 : 0x0;
}

// 0x21/0x42
static i32 SYS_fileseek(i8 bmode, i16 handle, i32 offset, bool& flag)
{
  //  AH = 42h
  //  AL = origin of move
  //      00h start of file SEEK_SET
  //      01h current file position  SEEK_CUR
  //      02h end of file  SEEK_END
  //  BX = file handle
  // old:
  //  CX:DX = signed offset from origin of new file position
  // Return: CF clear if successful
  //  DX:AX = new file position in bytes from start of file
  //  CF set on error
  //  AX = error code (01h,06h) (see #01680 at AH=59h/BX=0000h)
  // new:
  //  offset now long
  //  return new file position
  if (handle < 0 || handle > (MAX_FILEHANDLES-1))
    throw(THROW_DMABORT);
  if (filehandle[handle] == NULL)
    throw(THROW_DMABORT);
  flag = fseek(filehandle[handle], offset, bmode) != 0;
  if (flag)
    return 0x5;
  return ftell(filehandle[handle]);
}

// 0x21/0x56
static i16 SYS_renamefile(t_text* oldname, t_text* newname)
{
  // DOS - 2+ - RENAME A FILE
  // DS:DX -> ASCII old name (drive and path allowed, no wildcards)
  // ES:DI -> ASCII new name
  if (oldname[1] == ':')
  {
    oldname[0] = '.';
    oldname[1] = '\\';
  }
  if (newname[1] == ':')
  {
    newname[0] = '.';
    newname[1] = '\\';
  }
  return rename(oldname, newname) == 0 ? 0x5 : 0x0;
}

// 0x21/0x44/0x0
static i8 SYS_getdeviceinfo(i16 handle)
{
/*
INT 21 - DOS 2+ - IOCTL - GET DEVICE INFORMATION
  AX = 4400h
  BX = handle
Return: CF clear if successful
      DX = device information word (see #01423)
      AX destroyed
  CF set on error
      AX = error code (01h,05h,06h) (see #01680 at AH=59h/BX=0000h)
Notes:  value in DH corresponds to high byte of device driver's attribute word
    if handle refers to a character device
  Novell NetWare reportedly does not return a drive number in bits 5 - 0
    for a disk file
  this function was not supported by Digital Research's DOS Plus
SeeAlso: AX=4401h,INT 2F/AX=122Bh

Bitfields for device information word:
Bit(s)  Description  (Table 01423)
 character device
  14  device driver can process IOCTL requests (see AX=4402h"DOS 2+")
  13  output until busy supported
  11  driver supports OPEN/CLOSE calls
   8  ??? (set by MS - DOS 6.2x KEYB)
   7  set (indicates device)
   6  EOF on input
   5  raw (binary) mode
   4  device is special (uses INT 29)
   3  clock device
   2  NUL device
   1  standard output
   0  standard input
 disk file
  15  file is remote (DOS 3.0+)
  14  don't set file date/time on closing (DOS 3.0+)
  11  media not removable
   8  (DOS 4 only) generate INT 24 if no disk space on write or read past
    end of file
   7  clear (indicates file)
   6  file has not been written
  5 - 0  drive number (0 = A:)
SeeAlso: INT 29
*/

  // TODO - just a wraparound
  // bool boolc = false;
  // i32 d = 0; // sets RG4Blo=0, that includes the requested information - says: this file exists
  // else, the fileopen - command had cancelled the program already
  return 0; // mkb(rclror32(d, boolc));
}

// id: 0x58BA5
static bool DM2_WATCOM_GETDEVICEINFO(i16 handle)
{
  return (SYS_getdeviceinfo(handle) & bcon(0x80)) != bcon(0x0);
}

// id: 0x58BFE
static i32 DM2_WATCOM_CHECKDEVICE(i16 handle)
{
  if (handle >= filedat.filehandles)
    return 0;
  if (handle <= wcon(0x5))
    if ((filedat.devicetableptr[handle] & lcon(0x4000)) == 0)
    {
      filedat.devicetableptr[handle] |= lcon(0x4000);
      if (DM2_WATCOM_GETDEVICEINFO(handle))
        filedat.devicetableptr[handle] |= lcon(0x2000);
    }
  return filedat.devicetableptr[handle];
}

// id: 0x58C53
// set state with raised 0x4000 for device eaxl
static void DM2_WATCOM_SETDEVICESTATE(i16 handle, i32 state)
{
  filedat.devicetableptr[handle] = state | lcon(0x4000);
}

// id: 0x5275B
// was WATCOM_CREATE_OPEN_FILE_LO
// parameter argb2 removed, set to 0 by default
// then again last parameter removed, is lcon(0x180) always
static i16 DM2_WATCOM_CREATE_OR_OPEN_FILE(t_text* filename, i16 argw1)
{
  bool flag;
  i32 vl_00;
  i16 vw_04;
  i8 vba_08[0x20];
  i32 xhandle; // if long: errorcode, if word: handle

SPX_DEBUG_PRINT("DM2_WATCOM_CREATE_OR_OPEN_FILE %s / %d\n", filename, argw1);

  while (*filename == ' ')
    filename++;
  filename+=2;

  vl_00 = mkl(argw1) & lcon(0x83);
  xhandle = lcon(0xffffffff);
  vw_04 = SYS_openfile(mkb(vl_00), filename, flag);
  if (!flag)
    xhandle = unsignedlong(vw_04);
  if ((argw1 & wcon(0x3)) != 0 && xhandle != lcon(0xffffffff))
    if (!DM2_WATCOM_GETDEVICEINFO(CUTX16(xhandle)))
    {
      if ((argw1 & wcon(0x400)) != 0)
      {
        SYS_closefile(CUTX16(xhandle));
        DM2_WATCOM_MARK_ERROR(lcon(0x7));
        return lcon(0xffffffff);
      }
      if ((argw1 & wcon(0x40)) != 0)
      {
        i32 lamt = SYS_writefile(CUTX16(xhandle), 0, vba_08, flag);
        if (lamt < 0)
        {
          SYS_closefile(CUTX16(xhandle));
          return DM2_WATCOM_ERRHANDLING(CUTX16(lamt));
        }
      }
    }
  if (xhandle == lcon(0xffffffff))
  {
    if ((argw1 & wcon(0x20)) == 0 || vw_04 != wcon(0x2))
      return DM2_WATCOM_ERRHANDLING(vw_04);
    i8 wrmode;
    if (((lcon(0x180) ^ filedat.filestatexormask) & lcon(0x80)) == 0)
      wrmode = 1; // this would be mode 'read only'
    else
      wrmode = 0;
    vw_04 = SYS_createfile(wrmode, filename, flag);
    if (flag)
      return DM2_WATCOM_ERRHANDLING(vw_04);
    vw_04 = SYS_closefile(vw_04);
    if (vw_04 != 0)
      return DM2_WATCOM_ERRHANDLING(vw_04);
    vw_04 = SYS_openfile(mkb(vl_00), filename, flag);
    if (flag)
      return DM2_WATCOM_ERRHANDLING(vw_04);
    xhandle = unsignedlong(vw_04);
  }
  ui32 state2 = DM2_WATCOM_CHECKDEVICE(CUTX16(xhandle)) & lcon(0xffffff3c);
  ui32 state1 = xhandle;
  if (DM2_WATCOM_GETDEVICEINFO(CUTX16(xhandle)))
    state2 |= lcon(0x2000);
  vl_00 &= lcon(0xffffff7f);
  if (vl_00 == 2)
    state2 |= 3;
  if (vl_00 == 0)
    state2 |= 1;
  if (vl_00 == 1)
    state2 |= 2;
  if ((argw1 & wcon(0x10)) != 0)
    state2 |= lcon(0x80);
  state1 = state2 | ulcon(0x40);
  if ((argw1 & wcon(0x300)) == 0)
  {
    if (filedat.statecheck == lcon(0x200)) // happens never...
      state2 = state1;
  }
  else if ((argw1 & wcon(0x200)) != 0)
    state2 = state1;
  DM2_WATCOM_SETDEVICESTATE(CUTX16(xhandle), state2);
  return CUTX16(xhandle);
}

// id: 0x102F
i16 DM2_FILE_OPEN(t_text* filename)
{
  return DM2_WATCOM_CREATE_OR_OPEN_FILE(filename, lcon(0x202));
}

// id: 0x1043
i16 DM2_FILE_CREATE(t_text* filename)
{
  return DM2_WATCOM_CREATE_OR_OPEN_FILE(filename, lcon(0x262));
}

// id: 0x526A8
static i32 DM2_WATCOM_FILESEEK(i16 handle, i8 bmode, i32 offset)
{
  bool flag;

  i32 newpos = SYS_fileseek(bmode, handle, offset, flag);
  if (!flag)
    return newpos;
  DM2_WATCOM_ERRHANDLING(mkw(newpos));
  return lcon(0xffffffff);
}

// id: 0x100e
bool DM2_FILE_SEEK(i16 eaxw, i32 edxl)
{
  return DM2_WATCOM_FILESEEK(eaxw, SEEK_SET, edxl) >= 0;
}

// id: 0x1029
i32 DM2_GET_FILE_SIZE(i16 handle)
{
  i32 pos1 = DM2_WATCOM_FILESEEK(handle, SEEK_CUR, 0);
  if (pos1 == lcon(0xffffffff))
    return lcon(0xffffffff);
  i32 pos2 = DM2_WATCOM_FILESEEK(handle, SEEK_END, 0);
  DM2_WATCOM_FILESEEK(handle, SEEK_SET, pos1);
  return pos2;
}

// id: 0x529D3
static i32 DM2_WATCOM_READFILE(i16 handle, i32 amount, i8* bbuffer)
{
  bool bodummy;

  i32 vl_0c = DM2_WATCOM_CHECKDEVICE(handle);
  if (vl_0c == 0)
  {
    DM2_WATCOM_MARK_ERROR(lcon(0x4));
    return lcon(0xffffffff);
  }
  if ((vl_0c & 1) == 0)
  {
    DM2_WATCOM_MARK_ERROR(lcon(0x6));
    return lcon(0xffffffff);
  }
  if ((vl_0c & lcon(0x40)) != 0)
  {
    i32 fret = SYS_readfile(handle, amount, bbuffer, bodummy);
    if (fret >= 0)
      return fret;
    return signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(fret)));
  }

  i32 vl_04 = amount;
  i32 vl_08 = 0;
  do
  {
    i32 vl_10 = SYS_readfile(handle, vl_04, bbuffer, bodummy);
    if (vl_10 < 0)
      return signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(vl_10)));
    if (vl_10 == 0)
      break;
    i32 offset = vl_08;
    unk* xptr = bbuffer;
    i32 sum = 0;
    i8* pb_14 = bbuffer + vl_10;
    i32 cnt = 0;

    while (DOWNCAST(unk, xptr) < pb_14)
    {
      if (*xptr == bcon(0x1a))
      {
        DM2_WATCOM_FILESEEK(handle, SEEK_CUR, sum - vl_10 + 1);
        return offset;
      }
      else if (*xptr != bcon(0xd))
      {
        offset++;
        bbuffer[cnt++] = *xptr;
      }
      xptr++;
      sum++;
    }
    vl_08 = offset;
    bbuffer += cnt; // BUGFIX phaseE3
    vl_04 -= cnt;
  } while ((vl_0c & lcon(0x2000)) == 0 && vl_04 != 0);
  return vl_08;
}

// id: 0x105F
bool DM2_FILE_READ(i16 handle, unk* buffer, i32 amount)
{
  while (amount > 0)
  {
    i32 amt;
    if (amount <= lcon(0x4000))
      amt = amount;
    else
      amt = lcon(0x4000);
    if (DM2_WATCOM_READFILE(handle, amt, buffer) < 0)
      return false;
    amount -= amt;
    buffer += amt;
  }
  return true;
}

// id: 52AE6
// was R_52AE6
static i32 WATCOM_WRITEFILE(i16 handle, i32 amount, i8* bbuffer)
{
  i32 vl_0c;
  i32 vl_10;
  i8* pb_18;
  ui32 vul_20;
  unk* espP = NULL;
  bool flag;
  i32 retval;

  i32 offset = 0;
  ui32 lsize = 0x200;
 
  vl_0c = DM2_WATCOM_CHECKDEVICE(handle);
  if (vl_0c == 0)
  {
    DM2_WATCOM_MARK_ERROR(lcon(0x4));
    retval = lcon(0xffffffff);
    goto M_ret;
  }
  if ((vl_0c & 2) == 0)
  {
    DM2_WATCOM_MARK_ERROR(lcon(0x6));
    retval = lcon(0xffffffff);
    goto M_ret;
  }
  if ((vl_0c & lcon(0x80)) != 0)
  {
    i32 fpos = SYS_fileseek(2, handle, 0, flag);
    if (flag)
    {
      retval = signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(fpos)));
      goto M_ret;
    }
  }
  if ((vl_0c & lcon(0x40)) != 0)
  {
    i32 amt = SYS_writefile(handle, amount, bbuffer, flag);
    if (flag)
      retval = signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(amt)));
    else if (amt != amount)
    {
      DM2_WATCOM_MARK_ERROR(lcon(0xc));
      retval = amt;
    }
    else
      retval = amount;
    goto M_ret;
  }
#if 0
  RG1L = oldstackpointer - pointer_at(Vx1dfa94);
  if (RG1UL < mkul(lcon(0xb0)))
    WATCOM_58CDD(); // called Debugger
  if (RG1UL < mkul(lcon(0x230)))
    RG6L = lcon(0x80);
  else
    RG6L = lcon(0x200);
  RG1L = (RG6L + lcon(0x3)) & lcon(0xfffffffc);
#endif
  lsize = 0x200;
  espP = new unk[lsize]; // instead of using the local stack, fixed size now

  offset = 0;
  vul_20 = 0;
  vl_10 = 0;
  pb_18 = bbuffer;

  while (vul_20 < mkul(amount))
  {
    if (*pb_18 == lcon(0xa))
    {
      espP[offset] = lcon(0xd);
      offset++;
      if (mkul(offset) == lsize)
      {
        offset = SYS_writefile(handle, lsize, espP, flag);
        if (flag)
        {
          retval = signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(offset)));
          goto M_ret;
        }
        if (mkul(offset) != lsize)
        {
          DM2_WATCOM_MARK_ERROR(lcon(0xc));
          retval = vl_10 + offset;
          goto M_ret;
        }
        offset = 0;
        vl_10 = vul_20;
      }
    }
    espP[offset++] = *pb_18++;
    vul_20++;
    if (mkul(offset) == lsize)
    {
      offset = SYS_writefile(handle, lsize, espP, flag);
      if (flag)
      {
        retval = signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(offset)));
        goto M_ret;
      }
      if (mkul(offset) != lsize)
      {
        DM2_WATCOM_MARK_ERROR(lcon(0xc));
        retval = vl_10 + offset;
        goto M_ret;
      }
      offset = 0;
      vl_10 = vul_20;
    }
  }

  if (offset == 0)
    retval = amount;
  else
  {
    i32 fpos = SYS_writefile(handle, offset, espP, flag);
    if (flag)
      retval = signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(fpos)));
    else if (fpos != offset)
    {
      DM2_WATCOM_MARK_ERROR(lcon(0xc));
      retval = vl_10 + fpos;
    }
    else
      retval = amount;
  }

M_ret:
  if (espP != NULL)
    delete [] espP;
  return retval;
}

// id: 0x10B0
// was R_10B0
bool DM2_FILE_WRITE(i16 handle, unk* buffer, i32 amount)
{
  while (amount > 0)
  {
    i32 amt;
    if (amount <= lcon(0x4000))
      amt = amount;
    else
      amt = lcon(0x4000);
    if (WATCOM_WRITEFILE(handle, amt, buffer) < 0)
      return false;
    amount -= amt;
    buffer += amt; // BUGFIX Phase P1
  }
  return true;
}

// id: 0x5298A
// old name WATCOM_UNLINK_INTERN, later WATCOM_UNLINK
i16 DM2_FILE_DELETE(t_text* filename)
{
  i16 errcode = SYS_deletefile(filename);
  if (errcode == 0)
    return 0;
  return DM2_WATCOM_ERRHANDLING(errcode);
}

// id: 0x529A9
// was WATCOM_RENAME_FILE
void DM2_FILE_RENAME(t_text* oldname, t_text* newname)
{
  i16 errcode = SYS_renamefile(oldname, newname);
  if (errcode != 0)
    DM2_WATCOM_ERRHANDLING(errcode);
}

// id: 0x58C68
// old name WATCOM_CLOSEFILE_INTERN, later WATCOM_CLOSEFILE
void DM2_FILE_CLOSE(i16 handle)
{
  if (SYS_closefile(handle) != 0)
  {
    DM2_WATCOM_MARK_ERROR(lcon(0x4));
    throw(THROW_DMABORT);
  }
  else
    DM2_WATCOM_SETDEVICESTATE(handle, 0);
}

// added
void DM2_READ_BINARY(t_text* filename, void* buf, size_t size)
{
  FILE* f = fopen(filename, FILEMODE_RB);
  if (f == NULL)
    throw(THROW_DMABORT);
  size_t r = fread(buf, size, 1, f);
  fclose(f);
  if (r != 1)
    throw(THROW_DMABORT);
}
