#include <StdAfx.h>
#include <fileset.h>

#include <skwindebug.h>

fileSet::fileSet() {
	for (int x=0; x<16; x++)
		realHandles[x] = -1;
}
// 
U32 fileSet::fileGetSize(U16 x) {
	if (x < 16 && !(realHandles[x] < 0)) {
		U32 pos = _tell(realHandles[x]);
		U32 res = _lseek(realHandles[x], 0, SEEK_END);
		_lseek(realHandles[x], pos, SEEK_SET);
		return res;
	}
	return 0;
}
// 
U32 fileSet::fileSeekTo(U16 x, U32 pos) {
	if (x < 16 && !(realHandles[x] < 0)) {
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
	for (int x=0; x<16; x++) {
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
	if (x < 16 && !(realHandles[x] < 0)) {
		return _write(realHandles[x], buff, buffSize);
	}
	return 0;
}
// 
bool fileSet::fileClose(U16 x) {
	if (x < 16 && !(realHandles[x] < 0)) {
		ATLASSERT(realHandles[x] != -1);
		_close(realHandles[x]);
		realHandles[x] = -1;
		return true;
	}
	return false;
}
// 
U16 fileSet::fileRead(U16 x, U32 buffSize, void *buff) {
	if (x < 16 && !(realHandles[x] < 0)) {
		return _read(realHandles[x], buff, buffSize);
	}
	return 0;
}
//
i16 fileSet::fileOpen(const char *psz) {
	SkD((DLV_FIL,"FIL: fileOpen(\"%s\")\n", psz));
	for (int x=0; x<16; x++) {
		if (realHandles[x] < 0) {
			realHandles[x] = _open(psz, _O_BINARY|_O_RDONLY);
			if (realHandles[x] < 0)
				return -1;
			return x;
		}
	}
	return -1;
}
//
U32 fileSet::fileTell(U16 x) {
	if (x < 16 && !(realHandles[x] < 0)) {
		return _lseek(realHandles[x], 0, SEEK_CUR);
	}
	return 0;
}

