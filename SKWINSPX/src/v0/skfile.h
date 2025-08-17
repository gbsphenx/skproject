//------------------------------------------------------------------------------

#ifndef _FILESET_H_
#define _FILESET_H_

//------------------------------------------------------------------------------

#include <sktypes.h>

//------------------------------------------------------------------------------

#define _REALHANDLES_MAX	16

class fileSet {
public:
	// 
	int realHandles[_REALHANDLES_MAX];
	char realFilepath[_REALHANDLES_MAX][256];
	// 
	fileSet();
	// 
	U32 fileGetSize(U16 x);
	// 
	U32 fileSeekTo(U16 x, U32 pos);
	// 
	bool fileDelete(const char *psz);
	// 
	bool fileMove(const char *pszFrom, const char *pszTo);
	// 
	i16 fileCreate(const char *psz);
	// 
	U16 fileWrite(U16 x, U32 buffSize, const void *buff);
	// 
	bool fileClose(U16 x);
	// 
	U16 fileRead(U16 x, U32 buffSize, void *buff);
	//
	i16 fileOpen(const char *psz);
	//
	U32 fileTell(U16 x);
};

//------------------------------------------------------------------------------

#endif // _FILESET_H_

//------------------------------------------------------------------------------

