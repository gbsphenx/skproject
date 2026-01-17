
// +--------------------------------------------------
// |
// | MemVuff.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#ifndef _MEMVUFF_H_

#pragma once

#include <string.h>

struct CMemVuff {
	// 
	uint8_t *pData;
	// 
	size_t iData, nData;

	// 
	bool fLE;

	// 
	int ReadByte()
	{
		int x = -1;
		if (iData < nData) {
			x = pData[iData]; iData++;
		}
		return x;
	}
	// 
	bool ReadByte(uint8_t &x)
	{
		if (iData < nData) {
			x = pData[iData]; iData++;
			return true;
		}
		return false;
	}
	// 
	int ReadWord()
	{
		int x0 = ReadByte();
		int x1 = ReadByte();
		if (x0 < 0 || x1 < 0) return -1;
		if (fLE)
			return x0 + 256 * x1;
		return 256 * x0 + x1;
	}
	// 
	bool ReadWord(uint16_t &x)
	{
		int x0 = ReadByte();
		int x1 = ReadByte();
		if (x0 < 0 || x1 < 0) return false;
		if (fLE) {
			x = x0 + 256 * x1;
		} else {
			x = 256 * x0 + x1;
		}
		return true;
	}
	// 
	int ReadDWord()
	{
		int x0 = ReadWord();
		int x1 = ReadWord();
		if (x0 < 0 || x1 < 0) return -1;
		if (fLE)
			return x0 + 65536 * x1;
		return 65536 * x0 + x1;
	}
	// 
	bool ReadDWord(uint32_t &x)
	{
		int x0 = ReadWord();
		int x1 = ReadWord();
		if (x0 < 0 || x1 < 0) return false;
		if (fLE) {
			x = x0 + 65536 * x1;
		} else {
			x = 65536 * x0 + x1;
		}
		return true;
	}
	// 
	bool SurelyReadBytes(void *pv, size_t cb)
	{
		if (iData + cb <= nData) {
			memcpy(pv, pData +iData, cb);
			iData += cb;
			return true;
		}
		return false;
	}
	// 
	bool SurelySkipBytes(size_t cb)
	{
		if (iData + cb <= nData) {
			iData += cb;
			return true;
		}
		return false;
	}

	// 
	bool WriteByte(uint8_t x)
	{
		if (iData < nData) {
			pData[iData] = x; iData++;
			return true;
		}
		return false;
	}
	// 
	bool WriteWord(uint16_t x)
	{
		union {
			uint16_t x;
			struct { uint8_t a, b; } y;
		}	z;

		if (iData + 1 < nData) {
			z.x = x;
			if (fLE)
				pData[iData] = z.y.a, iData++, 
				pData[iData] = z.y.b, iData++;
			else
				pData[iData] = z.y.b, iData++, 
				pData[iData] = z.y.a, iData++;
			return true;
		}
		return false;
	}
	// 
	bool WriteDWord(uint32_t x)
	{
		union {
			uint32_t x;
			struct { uint8_t a, b, c, d; } y;
		}	z;

		if (iData + 3 < nData) {
			z.x = x;
			if (fLE)
				pData[iData] = z.y.a, iData++, 
				pData[iData] = z.y.b, iData++,
				pData[iData] = z.y.c, iData++,
				pData[iData] = z.y.d, iData++;
			else
				pData[iData] = z.y.d, iData++, 
				pData[iData] = z.y.c, iData++,
				pData[iData] = z.y.b, iData++,
				pData[iData] = z.y.a, iData++;
			return true;
		}
		return false;
	}
	// 
	bool WriteBytes(const void *pv, size_t cb)
	{
		if (iData + cb <= nData) {
			memcpy(pData +iData, pv, cb);
			iData += cb;
			return true;
		}
		return false;
	}
};

#endif // _MEMVUFF_H_
