#include "startend.h"
#include "util.h" // SKW_COPY_MEMORY
#include "fileio.h"
#include "dm2data.h"
#include "../driver.h" // driver_userint

#include <stdlib.h>
#include <errno.h>

#include "dm2debug.h"

FILE* filehandle[MAX_FILEHANDLES] =
{
  NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL
};

// >> SPX: add hard copies of data file in order to process memory copies instead of sysfile read where I had issues with
typedef struct
{
	unsigned long int	iFileSize;
	unsigned char*		xDataBuffer;
	unsigned long int	iCurrentCursor;
	bool				bFileOpen;
} t_hard_file_copy;

t_hard_file_copy xHardFileCopies[MAX_FILEHANDLES];
// SPX <<

// 0x21/0x2f
i8* SYS_gettransferadress(void)
{
/*
INT 21 - DOS 2+ - GET DISK TRANSFER AREA ADDRESS
	AH = 2Fh
Return: ES:BX -> current DTA
Note:	under the FlashTek X - 32 DOS extender, the pointer is in ES:EBX
SeeAlso: AH=1Ah
*/
  throw(THROW_DMABORT); // TODO
  return NULL;
}

// 0x21/0x11
i8 SYS_findfirst(i8* ptr)
{
/*
INT 21 - DOS 1+ - FIND FIRST MATCHING FILE USING FCB
	AH = 11h
	DS:DX -> unopened FCB (see #01345), may contain '?' wildcards
Return: AL = status
	    00h successful
		[DTA] unopened FCB for first matching file
	    FFh no matching filename, or bad FCB
*/
  throw(THROW_DMABORT); // TODO
  return 0;
}

// comments about DOS - calls by Ralf Braun's interrupt list

// ################################################################################
// ################################################################################
// ################################################################################

// 0x21/0x3c
static i16 SYS_createfile(i8 attrib, t_text* filename, bool& c)
{
  // DOS - 2+ - CREATE A FILE WITH HANDLE
  // CX = attributes for file - !unused, we just use "wb"
  // in fact the values 0 and 1 are used
  // DS:DX -> ASCII filename (may include drive and path)
  // affects the flags
  i16 h;
  char* fp = STRINGCAST(filename);
  if (fp[1] == ':')
  {
    fp[0] = '.';
    fp[1] = '\\';
  }
  FILE* handle = fopen(fp, "wb");
  if (handle==NULL)
  {
    switch (errno)
    {
      case ENOENT:
        h = 2; // file not found
        c = true;
      break;
      case EACCES:
      default:
        h = 255;
        c = true;
      break;
    }
  }
  else
  {
    h = handle->_file;
    if (h<0 || h>9)
      throw(THROW_DMABORT);
    if (filehandle[h] != NULL)
      throw(THROW_DMABORT);
    filehandle[h] = handle;
    c = false;
  }
  return h;
}

// 0x21/0x3d
static i16 SYS_openfile(i8 mode, t_text* filename, bool& flag)
{
  // DOS - 2+ - OPEN DISK FILE WITH HANDLE
  // DS:DX -> ASCII filename
  // AL = access mode
  // 0 - read, 1 - write, 2 - read & write
  // Return: CF clear if successful
  // AX = file handle
  // CF set on error, then:
  //   AX = error code (01h,02h,03h,04h,05h,0Ch,56h) (see #01680 at AH=59h)
  i16 h;
  if (mode != 1 && mode != 0 && mode !=2)
    throw(THROW_DMABORT);
  char* fp = STRINGCAST(filename);
  if (fp[1] == ':')
  {
    fp[0] = '.';
    fp[1] = '\\';
  }
  FILE* handle = fopen(fp, (mode==0)?"rb":(((mode==1))?"wb":"r + b"));
  // SPX: Here I insert a hard copy of file, if in read mode
  if ((mode==0 || mode==2) && handle != NULL)
  {
	int iLocalHandleNum = handle->_file;
	xHardFileCopies[iLocalHandleNum].iFileSize = 0;
	xHardFileCopies[iLocalHandleNum].xDataBuffer = NULL;
	xHardFileCopies[iLocalHandleNum].iCurrentCursor = 0;
	xHardFileCopies[iLocalHandleNum].bFileOpen = false;
	fseek(handle, 0, SEEK_END);
	xHardFileCopies[iLocalHandleNum].iFileSize = ftell(handle);
	fseek(handle, 0, SEEK_SET);
	xHardFileCopies[iLocalHandleNum].xDataBuffer = (unsigned char*) calloc(xHardFileCopies[iLocalHandleNum].iFileSize, sizeof(unsigned char));
	if (xHardFileCopies[iLocalHandleNum].xDataBuffer != NULL)
	{
		fread(xHardFileCopies[iLocalHandleNum].xDataBuffer, 1, xHardFileCopies[iLocalHandleNum].iFileSize, handle);
		xHardFileCopies[iLocalHandleNum].bFileOpen = true;
	}
	else
		throw(THROW_DMABORT);
  }
  // end of SPX
  if (handle == NULL)
  {
    switch (errno)
    {
      case ENOENT:
        h = 2; // file not found
        flag = true;
      break;
      case EACCES:
      default:
        h = 255;
        flag = true;
      break;
    }
  }
  else
  {
    h = handle->_file;
    if (h<0 || h>9)
      throw(THROW_DMABORT);
    if (filehandle[h] != NULL)
      throw(THROW_DMABORT);
    filehandle[h] = handle;
    flag = false;
  }
  return h;
}

// 0x21/0x3e
static i16 SYS_closefile(i16 handle)
{
  // DOS - 2+ - CLOSE A FILE WITH HANDLE
  // BX = file handle
  if (handle<0 || handle>9)
    throw(THROW_DMABORT);
  if (filehandle[handle]==NULL)
    throw(THROW_DMABORT);
  i16 ret = fclose(filehandle[handle]);
  filehandle[handle] = NULL;
  return ret;
}

static i32 SPX_fread(void* buffer, size_t size, size_t count, i16 handle)
{
//LOGX(("SPX_fread handle %d size*count = %d * %d = %d to buffer %08x\n", handle, size, count, size * count, buffer ));
	i32 iBytesRead = 0;
	if (xHardFileCopies[handle].xDataBuffer != NULL && xHardFileCopies[handle].bFileOpen)
	{
		unsigned char* xSourceBuffer = xHardFileCopies[handle].xDataBuffer;
		unsigned int iBytesToCopy = 0;
		unsigned int iFixedCursor = xHardFileCopies[handle].iCurrentCursor;
		if (xHardFileCopies[handle].iCurrentCursor >= xHardFileCopies[handle].iFileSize)
			iFixedCursor = xHardFileCopies[handle].iFileSize;
		xSourceBuffer += iFixedCursor;
		iBytesToCopy = size * count;
		if ( iBytesToCopy > (xHardFileCopies[handle].iFileSize - iFixedCursor ) )
			iBytesToCopy = xHardFileCopies[handle].iFileSize - iFixedCursor;
		if (iBytesToCopy > 0)
			memcpy((unsigned char*)buffer, (unsigned char*) xSourceBuffer, (iBytesToCopy));
		xHardFileCopies[handle].iCurrentCursor = (iFixedCursor + iBytesToCopy);
		iBytesRead = (iBytesToCopy);
	}
	return iBytesRead;
}

static i32 SPX_fseek(i16 handle, long offset, int mode)
{
// SEEK_SET, SEEK_CUR, ou SEEK_END
//LOGX(("SPX_fseek handle %d offset %08d (%08X) mode %d\n", handle, offset, offset, mode ));
	if (xHardFileCopies[handle].bFileOpen)
	{
		switch (mode)
		{
			case SEEK_SET:
				xHardFileCopies[handle].iCurrentCursor = offset;
				break;
			case SEEK_CUR:
				xHardFileCopies[handle].iCurrentCursor += offset;
				break;
			case SEEK_END:
				xHardFileCopies[handle].iCurrentCursor = xHardFileCopies[handle].iFileSize + offset;
				break;
		}
/*		if (xHardFileCopies[handle].iCurrentCursor >= xHardFileCopies[handle].iFileSize)
		{
			xHardFileCopies[handle].iCurrentCursor = xHardFileCopies[handle].iFileSize;
			return 1;
		}
		*/
	}
	return 0;
}

static unsigned long int SPX_ftell(i16 handle)
{
	if (xHardFileCopies[handle].bFileOpen)
	{
//LOGX(("SPX_ftell handle %d returns %08d (%08X)\n", handle, xHardFileCopies[handle].iCurrentCursor, xHardFileCopies[handle].iCurrentCursor ));
		return xHardFileCopies[handle].iCurrentCursor;
	}
	return 0;
}


// 0x21/0x3f
static i32 SYS_readfile(i16 handle, i32 size, i8* buffer, bool& c)
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
  if (handle<0 || handle>9)
    throw(THROW_DMABORT);
  if (filehandle[handle]==NULL)
    throw(THROW_DMABORT);
//  size_t readsize = fread(buffer, 1, size, filehandle[handle]);
  size_t readsize = SPX_fread(buffer, 1, size, handle);	// SPX: replace with file hard copy in memory
//SPX_DEBUG_MESSAGE_INFO("SYS_readfile on fh %d >> Asked %d and read %d\n", handle, size, readsize);
  c = readsize != size;
  if (c)
    ret = 0x80000005;
  else
    ret = I32CAST(readsize);
  return ret;
}

// 0x21/040
// rotate pulled out of code
static i32 SYS_writefile(i16 handle, i32 size, i8* buffer, bool& c)
{
  // DOS - 2+ - WRITE TO FILE WITH HANDLE
  // BX = file handle, CX = number of bytes to write
  // Return: CF clear if successful
  //         AX = number of bytes actually written (0 if at EOF before call)
  //         CF set on error
  //         AX = error code (05h,06h) (see #01680 at AH=59h/BX=0000h)
  // sometimes jul is requested after this, which is carry after all
  i32 ret;
  if (handle<0 || handle>9)
    throw(THROW_DMABORT);
  if (filehandle[handle] == NULL)
    throw(THROW_DMABORT);
  size_t writesize = fwrite(buffer, 1, size, filehandle[handle]);
  c = writesize != size;
  if (c)
    ret = 0x80000005;
  else
    ret = I32CAST(writesize);
  return ret;
}

// 0x21/0x41
static i16 SYS_deletefile(t_text* filename)
{
  // DOS - 2+ - DELETE A FILE (UNLINK)
  // DS:DX -> ASCII pathname of file to delete (no wildcards allowed)
  char* fp = STRINGCAST(filename);
  if (fp[1] == ':')
  {
    fp[0] = '.';
    fp[1] = '\\';
  }
  bool c = _unlink(fp) != con(0x0);
  return c ? 0x5 : 0x0;
}

// 0x21/0x42
static i32 SYS_fileseek(i8 mode, i16 handle, i32 offset, bool& flag)
{
  //	AH = 42h
  //	AL = origin of move
  //	    00h start of file SEEK_SET
  //	    01h current file position  SEEK_CUR
  //	    02h end of file  SEEK_END
  //	BX = file handle
  // old:
  //	CX:DX = signed offset from origin of new file position
  // Return: CF clear if successful
  //	DX:AX = new file position in bytes from start of file
  //	CF set on error
  //	AX = error code (01h,06h) (see #01680 at AH=59h/BX=0000h)
  // new:
  //  offset now long
  //  return new file position
  if (handle<0 || handle>9)
    throw(THROW_DMABORT);
  if (filehandle[handle]==NULL)
    throw(THROW_DMABORT);
  flag = fseek(filehandle[handle], offset, mode) != con(0x0);
  flag = SPX_fseek(handle, offset, mode) != con(0x0);	// SPX: replace with file hard copy in memory
  if (flag) 
	  return 0x5;
  //SPX_ftell(handle);
  //SPX_fseek(handle, offset, mode);
  int iCurrentCursor = 0;
  iCurrentCursor = ftell(filehandle[handle]);
  //return ftell(filehandle[handle]);
//  if (iCurrentCursor != SPX_ftell(handle))
//	  LOGX(("break\n"));
  return iCurrentCursor;
}

// 0x21/0x56
static i16 SYS_renamefile(t_text* oldname, t_text* newname)
{
  // DOS - 2+ - RENAME A FILE
  // DS:DX -> ASCII old name (drive and path allowed, no wildcards)
  // ES:DI -> ASCII new name
  char* o = STRINGCAST(oldname);
  if (o[1]==':')
  {
    o[0] = '.';
    o[1] = '\\';
  }
  char* n = STRINGCAST(newname);
  if (n[1]==':')
  {
    n[0] = '.';
    n[1] = '\\';
  }
  bool c = rename(o, n) == con(0x0);
  return c ? 0x5 : 0x0;
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
Notes:	value in DH corresponds to high byte of device driver's attribute word
	  if handle refers to a character device
	Novell NetWare reportedly does not return a drive number in bits 5 - 0
	  for a disk file
	this function was not supported by Digital Research's DOS Plus
SeeAlso: AX=4401h,INT 2F/AX=122Bh

Bitfields for device information word:
Bit(s)	Description	(Table 01423)
 character device
  14	device driver can process IOCTL requests (see AX=4402h"DOS 2+")
  13	output until busy supported
  11	driver supports OPEN/CLOSE calls
   8	??? (set by MS - DOS 6.2x KEYB)
   7	set (indicates device)
   6	EOF on input
   5	raw (binary) mode
   4	device is special (uses INT 29)
   3	clock device
   2	NUL device
   1	standard output
   0	standard input
 disk file
  15	file is remote (DOS 3.0+)
  14	don't set file date/time on closing (DOS 3.0+)
  11	media not removable
   8	(DOS 4 only) generate INT 24 if no disk space on write or read past
	  end of file
   7	clear (indicates file)
   6	file has not been written
  5 - 0	drive number (0 = A:)
SeeAlso: INT 29
*/

  // TODO - just a wraparound
  // bool c = false;
  // i32 d = 0; // sets RG4Blo=0, that includes the requested information - says: this file exists
  // else, the fileopen - command had cancelled the program already
  return con(0x0); // I8CAST(rclror32(d, c));
}

// id: 0x58BA5
static bool DM2_WATCOM_GETDEVICEINFO(i16 handle)
{
  return (SYS_getdeviceinfo(handle) & con(0xffffff80)) != con(0x0);
}

// id: 0x58BFE
static x32 DM2_WATCOM_CHECKDEVICE(x16 handle)
{
  if (handle >= ddata.filehandles)
    return con(0x0);
  if (handle <= con(0x5))
  {
    if ((ddata.devicetableptr[handle] & con(0x4000)) == con(0x0))
    {
      ddata.devicetableptr[handle] |= con(0x4000);
      if (DM2_WATCOM_GETDEVICEINFO(handle))
        ddata.devicetableptr[handle] |= con(0x2000);
    }
  }
  return ddata.devicetableptr[handle];
}

// id: 0x58C53
// set state with raised 0x4000 for device eaxl
static void DM2_WATCOM_SETDEVICESTATE(x16 handle, x32 state)
{
  ddata.devicetableptr[handle] = state | con(0x4000);
}

// id: 0x5275B
// was WATCOM_CREATE_OPEN_FILE_LO
// parameter argb2 removed, set to con(0x0) by default
// then again last parameter removed, is con(0x180) always
static x16 DM2_WATCOM_CREATE_OR_OPEN_FILE(t_text* filename, x16 argw1)
{
  bool flag;
  x32 vl_00;
  x16 vw_04;
  x8 vba_08[0x20];
  x32 xhandle; // if long: errorcode, if word: handle

  while (*filename == ' ')
    filename++;

  vl_00 = argw1 & con(0x83);
  xhandle = con(0xffffffff);
  vw_04 = SYS_openfile(CUTX8(vl_00), filename, flag);
  if (!flag)
    xhandle = unsignedlong(vw_04);
  if ((argw1 & con(0x3)) != 0 && xhandle != con(0xffffffff))
  {
    if (!DM2_WATCOM_GETDEVICEINFO(CUTX16(xhandle)))
    {
      if ((argw1 & con(0x400)) != 0)
      {
        SYS_closefile(CUTX16(xhandle));
        DM2_WATCOM_MARK_ERROR(con(0x7));
        return con(0xffffffff);
      }
      if ((argw1 & con(0x40)) != 0)
      {
        x32 wamt = SYS_writefile(CUTX16(xhandle), con(0x0), vba_08, flag);
        if (wamt < con(0x0))
        {
          SYS_closefile(CUTX16(xhandle));
          return DM2_WATCOM_ERRHANDLING(CUTX16(wamt));
        }
      }
    }
  }
  if (xhandle == con(0xffffffff))
  {
    if ((argw1 & con(0x20)) == 0 || vw_04 != con(0x2))
      return DM2_WATCOM_ERRHANDLING(vw_04);
    x8 wrmode;
    if (((con(0x180) ^ ddata.longfilestatexormask) & con(0x80)) == 0)
      wrmode = con(0x1); // this would be mode 'read only'
    else
      wrmode = con(0x0);
    vw_04 = SYS_createfile(wrmode, filename, flag);
    if (flag)
      return DM2_WATCOM_ERRHANDLING(vw_04);
    vw_04 = SYS_closefile(vw_04);
    if (vw_04 != con(0x0))
      return DM2_WATCOM_ERRHANDLING(vw_04);
    vw_04 = SYS_openfile(CUTX8(vl_00), filename, flag);
    if (flag)
      return DM2_WATCOM_ERRHANDLING(vw_04);
    xhandle = unsignedlong(vw_04);
  }
  ui32 state2 = DM2_WATCOM_CHECKDEVICE(CUTX16(xhandle)) & con(0xffffff3c);
  ui32 state1 = xhandle;
  if (DM2_WATCOM_GETDEVICEINFO(CUTX16(xhandle)))
    state2 |= con(0x2000);
  vl_00 &= con(0xffffff7f);
  if (vl_00 == con(0x2))
    state2 |= con(0x3);
  if (vl_00 == con(0x0))
    state2 |= con(0x1);
  if (vl_00 == con(0x1))
    state2 |= con(0x2);
  if ((argw1 & con(0x10)) != 0)
    state2 |= con(0x80);
  state1 = state2 | con(0x40);
  if ((argw1 & con(0x300)) == 0)
  {
    if (ddata.longstatecheck == con(0x200))
      state2 = state1;
  }
  else if ((argw1 & con(0x200)) != 0)
    state2 = state1;
  DM2_WATCOM_SETDEVICESTATE(CUTX16(xhandle), state2);
  return CUTX16(xhandle);
}

// id: 0x102F
x16 DM2_FILE_OPEN(t_text* filename)
{
  return DM2_WATCOM_CREATE_OR_OPEN_FILE(filename, con(0x202));
}

// id: 0x1043
x16 DM2_FILE_CREATE(t_text* filename)
{
  return DM2_WATCOM_CREATE_OR_OPEN_FILE(filename, con(0x262));
}

// id: 0x526A8
static x32 DM2_WATCOM_FILESEEK(x16 handle, x8 mode, x32 offset)
{
  bool flag;

  x32 newpos = SYS_fileseek(mode, handle, offset, flag);
  if (!flag)
    return newpos;
  DM2_WATCOM_ERRHANDLING(I16CAST(newpos));
  return con(0xffffffff);
}

// id: 0x100e
bool DM2_FILE_SEEK(x16 eaxw, x32 edxl)
{
  return DM2_WATCOM_FILESEEK(eaxw, SEEK_SET, edxl) >= con(0x0);
}

// id: 0x1029
x32 DM2_GET_FILE_SIZE(x16 handle)
{
  x32 pos1 = DM2_WATCOM_FILESEEK(handle, SEEK_CUR, con(0x0));
  if (pos1 == con(0xffffffff))
    return con(0xffffffff);
  x32 pos2 = DM2_WATCOM_FILESEEK(handle, SEEK_END, con(0x0));
  DM2_WATCOM_FILESEEK(handle, SEEK_SET, pos1);
  return pos2;
}

// id: 0x529D3
static x32 DM2_WATCOM_READFILE(x16 handle, x32 amount, x8* buffer)
{
  bool dummy;

  x32 vl_0c = DM2_WATCOM_CHECKDEVICE(handle);
  if (vl_0c == con(0x0))
  {
    DM2_WATCOM_MARK_ERROR(con(0x4));
    return con(0xffffffff);
  }
  if ((vl_0c & con(0x1)) == 0)
  {
    DM2_WATCOM_MARK_ERROR(con(0x6));
    return con(0xffffffff);
  }
  if ((vl_0c & con(0x40)) != 0)
  {
    x32 fret = SYS_readfile(handle, amount, buffer, dummy);
    if (fret >= con(0x0))
      return fret;
    return signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(fret)));
  }

  x32 vl_04 = amount;
  x32 vl_08 = 0;
  do
  {
    x32 vl_10 = SYS_readfile(handle, vl_04, buffer, dummy);
    if (vl_10 < con(0x0))
      return signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(vl_10)));
    if (vl_10 == con(0x0))
      break;
    x32 offset = vl_08;
    xxx* ptr = buffer;
    x32 sum = con(0x0);
    x8* pb_14 = buffer + vl_10;
    x32 cnt = con(0x0);

    while (ptr < pb_14)
    {
      if (*ptr == con(0x1a))
      {
        DM2_WATCOM_FILESEEK(handle, SEEK_CUR, sum - vl_10 + 1);
        return offset;
      }
      else if (*ptr != con(0xd))
      {
        offset++;
        buffer[cnt++] = *ptr;
      }
      ptr++;
      sum++;
    }
    vl_08 = offset;
    buffer += cnt; // BUGFIX phaseE3
    vl_04 -= cnt;
  } while ((vl_0c & con(0x2000)) == 0 && vl_04 != con(0x0));
  return vl_08;
}

// id: 0x105F
bool DM2_FILE_READ(x16 handle, xxx* buffer, x32 amount)
{
//SPX_DEBUG_PUSH_FUNCTION_INFO("DM2_FILE_READ");
//SPX_DEBUG_MESSAGE_INFO("Handle: %d / Amount = %d\n", handle, amount);
  while (amount > con(0x0))
  {
    x32 amt;
    if (amount <= con(0x4000))
      amt = amount;
    else
      amt = con(0x4000);
    if (DM2_WATCOM_READFILE(handle, amt, buffer) < con(0x0))
	{
//SPX_DEBUG_POP;
      return false;
	}
    amount -= amt;
    buffer += amt;
  }
//SPX_DEBUG_POP;
  return true;
}

// id: 52AE6
// was R_52AE6
static x32 WATCOM_WRITEFILE(x16 handle, x32 amount, x8* buffer)
{
  x32 v0c;
  x32 v10;
  x8* p18;
  ui32 v20;
  xxx* espP = NULL;
  bool flag;
  x32 retval;

  // SPX, moved up these 2 variables ro avoid "error C2362: initialization of 'offset' is skipped by 'goto ret'"
  ui32 lsize = 0x200;
  x32 offset = con(0x0);

  v0c = DM2_WATCOM_CHECKDEVICE(handle);
  if (v0c == con(0x0))
  {
    DM2_WATCOM_MARK_ERROR(con(0x4));
    retval = con(0xffffffff);
    goto ret;
  }
  if ((v0c & con(0x2)) == con(0x0))
  {
    DM2_WATCOM_MARK_ERROR(con(0x6));
    retval = con(0xffffffff);
    goto ret;
  }
  if ((v0c & con(0x80)) != 0)
  {
    x32 fpos = SYS_fileseek(con(0x2), handle, con(0x0), flag);
    if (flag)
    {
      retval = signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(fpos)));
      goto ret;
    }
  }
  if ((v0c & con(0x40)) != 0)
  {
    x32 amt = SYS_writefile(handle, amount, buffer, flag);
    if (flag)
      retval = signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(amt)));
    else if (amt != amount)
    {
      DM2_WATCOM_MARK_ERROR(con(0xc));
      retval = amt;
    }
    else
      retval = amount;
    goto ret;
  }
#if 0
  RG1L = oldstackpointer - pointer_at(Vx1dfa94);
  if (RG1UL < UI32CAST(con(0xb0)))
    WATCOM_58CDD(); // called Debugger
  if (RG1UL < UI32CAST(con(0x230)))
    RG6L = con(0x80);
  else
    RG6L = con(0x200);
  RG1L = (RG6L + con(0x3)) & con(0xfffffffc);
#endif

  
  espP = new xxx[lsize]; // instead of using the local stack, fixed size now

  v20 = con(0x0);
  v10 = con(0x0);
  p18 = buffer;
  

  while (v20 < UI16CAST(amount))
  {
    if (*p18 == con(0xa))
    {
      espP[offset] = con(0xd);
      offset++;
      if (offset == lsize)
      {
        offset = SYS_writefile(handle, lsize, espP, flag);
        if (flag)
        {
          retval = signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(offset)));
          goto ret;
        }
        if (offset != lsize)
        {
          DM2_WATCOM_MARK_ERROR(con(0xc));
          retval = v10 + offset;
          goto ret;
        }
        offset = con(0x0);
        v10 = v20;
      }
    }
    espP[offset++] = *p18++;
    v20++;
    if (offset == lsize)
    {
      offset = SYS_writefile(handle, lsize, espP, flag);
      if (flag)
      {
        retval = signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(offset)));
        goto ret;
      }
      if (offset != lsize)
      {
        DM2_WATCOM_MARK_ERROR(con(0xc));
        retval = v10 + offset;
        goto ret;
      }
      offset = con(0x0);
      v10 = v20;
    }
  } // loop

  if (offset == con(0x0))
    retval = amount;
  else
  {
    x32 fpos = SYS_writefile(handle, offset, espP, flag);
    if (flag)
      retval = signedlong(DM2_WATCOM_ERRHANDLING(CUTX16(fpos)));
    else if (fpos != offset)
    {
      DM2_WATCOM_MARK_ERROR(con(0xc));
      retval = v10 + fpos;
    }
    else
      retval = amount;
  }

ret:
  if (espP != NULL)
    delete [] espP;
  return retval;
}

// id: 0x10B0
// was R_10B0
bool SKW_FILE_WRITE(x16 handle, xxx* buffer, x32 amount)
{
  while (amount > con(0x0))
  {
    x32 amt;
    if (amount <= con(0x4000))
      amt = amount;
    else
      amt = con(0x4000);
    if (WATCOM_WRITEFILE(handle, amt, buffer) < con(0x0))
      return false;
    amount -= amt;
    buffer += amt; // BUGFIX Phase P1
  }
  return true;
}

// id: 0x5298A
// old name WATCOM_UNLINK_INTERN, later WATCOM_UNLINK
x16 SKW_FILE_DELETE(t_text* filename)
{
  i16 errcode = SYS_deletefile(filename);
  if (errcode == con(0x0))
    return con(0x0);
  return DM2_WATCOM_ERRHANDLING(errcode);
}

// id: 0x529A9
// was WATCOM_RENAME_FILE
void SKW_FILE_RENAME(t_text* oldname, t_text* newname)
{
  i16 errcode = SYS_renamefile(oldname, newname);
  if (errcode != con(0x0))
    DM2_WATCOM_ERRHANDLING(errcode);
}

// id: 0x58C68
// old name WATCOM_CLOSEFILE_INTERN, later WATCOM_CLOSEFILE
// return never requested
x16 DM2_FILE_CLOSE(x16 handle)
{
  if (SYS_closefile(handle) != con(0x0))
  {
    DM2_WATCOM_MARK_ERROR(con(0x4));
    return con(0xffffffff);
  }
  DM2_WATCOM_SETDEVICESTATE(handle, con(0x0));
  return con(0x0);
}

// id: 0x1278
// TODO: driver's 'readsectors' - removed in the driver
x16 R_1278(x16 val)
{
  userinterrupt(con(0x13), val);
  return *UPCAST(x16, ddata.drvinterfaceptr);
}

// id: 0x1296
// TODO: driver's 'findfirst' - removed in the driver
void R_1296(x8* ptr)
{
  userinterrupt(con(0x14), con(0x0));
  SKW_COPY_MEMORY(ddata.drvinterfaceptr, con(0x10), ptr);
}
