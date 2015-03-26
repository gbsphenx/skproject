
#ifndef _ENDIAN_UTIL_H_
#define _ENDIAN_UTIL_H_

inline WORD readew(const void *pw, bool fLE) {
	WORD w = *reinterpret_cast<const WORD *>(pw);
	if (fLE) return w;
	return (w << 8) | (w >> 8);
}

inline void writeew(void *pv, WORD w, bool fLE) {
	WORD *pw = reinterpret_cast<WORD *>(pv);
	if (fLE) {
		*pw = w;
	}
	else {
		*pw = (w << 8) | (w >> 8);
	}
}

#endif // _ENDIAN_UTIL_H_