#ifndef __LINUX__
#include <stdafx.h>
#endif // __LINUX__
#include <skfile.h>
#include <skdebug.h>

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#if defined(__LINUX__)
	#include <unistd.h>
#else
	#include <io.h>
#endif // __LINUX__
#include <fcntl.h> // _O_BINARY etc...

// SPX: that part for non Microsoft compiler
#if defined(__DJGPP__) || defined (__MINGW__) || defined (__LINUX__)
	#include <sys/stat.h>
	#include <unistd.h>
	#ifndef _S_IWRITE
	#define _S_IWRITE S_IWRITE
	#endif // _S_IWRITE
	#ifndef _S_IREAD
	#define _S_IREAD S_IREAD
	#endif // _S_IREAD

	#ifndef _lseek
	#define _lseek lseek
	#endif

	off_t tell(int fd) {
		return lseek(fd, 0, SEEK_CUR);
	}

	#ifndef _open
	#define _open open
	#endif

	#ifndef _tell
	#define _tell tell
	#endif

	#ifndef _close
	#define _close close
	#endif

	#ifdef __LINUX__
		#ifndef _write
		#define _write write
		#endif
		#ifndef _read
		#define _read read
		#endif
		#define _O_BINARY	0
		#define _O_CREAT	O_CREAT
		#define _O_RDWR		O_RDWR
		#define _O_RDONLY	O_RDONLY
	#endif // __LINUX__

#endif // __DJGPP__


fileSet::fileSet() {
	for (int x = 0; x < _REALHANDLES_MAX; x++)
		realHandles[x] = -1;
}
// 
U32 fileSet::fileGetSize(U16 x) {
	if (x < _REALHANDLES_MAX && !(realHandles[x] < 0)) {
		U32 pos = _tell(realHandles[x]);
		U32 res = _lseek(realHandles[x], 0, SEEK_END);
		_lseek(realHandles[x], pos, SEEK_SET);
		return res;
	}
	return 0;
}
// 
U32 fileSet::fileSeekTo(U16 x, U32 pos) {
	if (x < _REALHANDLES_MAX && !(realHandles[x] < 0)) {
		return _lseek(realHandles[x], pos, SEEK_SET);
	}
	return U32(-1);
}
// 
bool fileSet::fileDelete(const char *psz) {
	SkD((DLV_FIL,"FIL: fileDelete(\"%s\")\n", psz));
	if (remove(psz) == 0)
		return true;
	return false;
}
// 
bool fileSet::fileMove(const char *pszFrom, const char *pszTo) {
	SkD((DLV_FIL,"FIL: fileMove(\"%s\", \"%s\")\n", pszFrom, pszTo));
	if (rename(pszFrom, pszTo) == 0)
		return true;
	return false;
}
// 
i16 fileSet::fileCreate(const char *psz) {
	SkD((DLV_FIL,"FIL: fileCreate(\"%s\")\n", psz));
	for (int x = 0; x < _REALHANDLES_MAX; x++) {
		if (realHandles[x] < 0) {
			realHandles[x] = _open(psz, _O_BINARY|_O_CREAT|_O_RDWR, _S_IREAD|_S_IWRITE);
			if (realHandles[x] < 0)
				return -1;
			return x;
		}
	}
	return -1;
}
// 
U16 fileSet::fileWrite(U16 x, U32 buffSize, const void *buff) {
	if (x < _REALHANDLES_MAX && !(realHandles[x] < 0)) {
		return _write(realHandles[x], buff, buffSize);
	}
	return 0;
}
// 
bool fileSet::fileClose(U16 x) {
	
	if (x < _REALHANDLES_MAX && !(realHandles[x] < 0)) {
		ATLASSERT(realHandles[x] != -1);
		_close(realHandles[x]);
		realHandles[x] = -1;
		return true;
	}
	return false;
}
// 
U16 fileSet::fileRead(U16 x, U32 buffSize, void *buff) {
	if (x < _REALHANDLES_MAX && !(realHandles[x] < 0)) {
		return _read(realHandles[x], buff, buffSize);
	}
	return 0;
}
//
i16 fileSet::fileOpen(const char *psz) {
	SkD((DLV_FIL,"FIL: fileOpen(\"%s\")\n", psz));
	for (int x = 0; x < _REALHANDLES_MAX; x++) {
		//printf("handle %d = %d (%64s)\n", x, realHandles[x], realFilepath[x]);
		if (!strcmp(psz, realFilepath[x])) // if handle was already open, then resuse it!)
			return x;
		if (realHandles[x] < 0) {
			realHandles[x] = _open(psz, _O_BINARY|_O_RDONLY);
			strcpy(realFilepath[x], psz);
			if (realHandles[x] < 0)
				return -1;
			return x;
		}
	}
	return -1;
}
//
U32 fileSet::fileTell(U16 x) {
	if (x < _REALHANDLES_MAX && !(realHandles[x] < 0)) {
		return _lseek(realHandles[x], 0, SEEK_CUR);
	}
	return 0;
}

