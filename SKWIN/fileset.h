//------------------------------------------------------------------------------

#ifndef _FILESET_H_
#define _FILESET_H_

//------------------------------------------------------------------------------

#include <DME.h>

//------------------------------------------------------------------------------

//! DM2 filesystem support
class fileSet {
public:
	//! io handles
	int realHandles[16];
	//! ctor
	fileSet();
	//! get file length
	U32 fileGetSize(U16 x);
	//! seek to absolute position
	//! @return new file position
	U32 fileSeekTo(U16 x, U32 pos);
	//! delete a file
	bool fileDelete(const char *psz);
	//! rename a file
	bool fileMove(const char *pszFrom, const char *pszTo);
	//! create a file
	//! @return new file handle
	i16 fileCreate(const char *psz);
	//! write to the file
	U16 fileWrite(U16 x, U32 buffSize, const void *buff);
	//! close file handle
	bool fileClose(U16 x);
	//! read from file
	U16 fileRead(U16 x, U32 buffSize, void *buff);
	//! open existing file
	i16 fileOpen(const char *psz);
	//! tell file position
	//! @return file position
	U32 fileTell(U16 x);
};

//------------------------------------------------------------------------------

#endif // _FILESET_H_

//------------------------------------------------------------------------------

