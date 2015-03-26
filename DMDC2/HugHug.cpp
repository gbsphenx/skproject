
#include "stdafx.h"
#include "exp00069.h"
#include "HugHug.h"
#include "SizeVuff.h"
#include "DO2PathStr.h"
#include "OSP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class CRFC2083CRCSample
{
	/* Table of CRCs of all 8-bit messages. */
	unsigned long crc_table[256];

	/* Flag: has the table been computed? Initially false. */
	int crc_table_computed;

	/* Make the table for a fast CRC. */
	void make_crc_table(void)
	{
		unsigned long c;
		int n, k;
		for (n = 0; n < 256; n++) {
			c = (unsigned long) n;
			for (k = 0; k < 8; k++) {
				if (c & 1)
					c = 0xedb88320L ^ (c >> 1);
				else
					c = c >> 1;
			}
			crc_table[n] = c;
		}
		crc_table_computed = 1;
	}

	/* Update a running CRC with the bytes buf[0..len-1]--the CRC
	should be initialized to all 1's, and the transmitted value
	is the 1's complement of the final running CRC (see the
	crc() routine below)). */

	unsigned long update_crc(
		unsigned long crc, 
		unsigned char *buf,
		int len
	)
	{
		unsigned long c = crc;
		int n;

		if (!crc_table_computed)
			make_crc_table();
		for (n = 0; n < len; n++) {
			c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
		}
		return c;
	}

	/* Return the CRC of the bytes buf[0..len-1]. */
	unsigned long crc(unsigned char *buf, int len)
	{
		return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
	}

public:
	// 
	CRFC2083CRCSample()
	{
		crc_table_computed = 0;
	}
	// 
	DWORD Hash(const void *pv, UINT cb)
	{
		return crc(const_cast<BYTE *>(static_cast<const BYTE *>(pv)), cb);
	}
};

//////////////////////////////////////////////////////////////////////

namespace Conv
{
	// 
	struct CSerializedReader
	{
		// 
		virtual int ReadByte() = NULL;
	};
	// 
	struct CSerializedWriter
	{
		// 
		virtual bool WriteByte(BYTE x) = NULL;
	};
	// 
	struct CMemVuff : CSerializedReader, CSerializedWriter
	{
		// 
		PBYTE pData;
		// 
		int iCur, nMax;

		// 
		bool WriteByte(BYTE x)
		{
			if (nMax < iCur + 1) return false;

			pData[iCur] = x; iCur++;
			return true;
		}
		// 
		bool ProjectBytes(int nBack, int nLen)
		{
			int iReadFrom = iCur - nBack;
			int iReadTo = iReadFrom + nLen;
			if (iReadFrom < 0) return false;
			if (nMax < iReadTo) return false;
			if (nMax < iCur + nLen) return false;

			ASSERT(nBack != 0 && nLen != 0);

			while (iReadFrom < iReadTo) {
				VERIFY(WriteByte(pData[iReadFrom]));
				iReadFrom++;
			}
			return true;
		}
		// 
		void Conv4To8()
		{
			for (int i = 0; i < iCur; i += 2) {
				pData[i / 2] = 0
					|(pData[i+0] * 16)
					|(pData[i+1] & 15)
					;
			}
			iCur /= 2;
		}

		// 
		int ReadByte()
		{
			if (nMax < iCur + 1) return -1;

			BYTE x = pData[iCur]; iCur++;
			return x;
		}
	};
	// 
	struct COleFileIo : CSerializedReader
	{
		// 
		IStreamPtr pOleFile;

		// 
		int ReadByte()
		{
			if (pOleFile == NULL) return -1;

			BYTE x;
			HRESULT hr;
			if (S_OK == (hr = pOleFile->Read(&x, 1, NULL)))
				return x;
			return -1;
		}
	};
	// 
	struct CDMSnesDecoder
	{
		// 
		static bool DecompressType3(CSerializedReader &fRead, CMemVuff &fSave)
		{
			int n1 = fRead.ReadByte();
			int n2 = fRead.ReadByte();
			int n3 = fRead.ReadByte();
			int n4 = fRead.ReadByte();

			int iPos = 0;
			int nLen = 256 * 256 * n2 + 256 * n3 + n4;

			while (iPos < nLen) {
				int v1 = fRead.ReadByte();
				if (v1 < 0)
					return false;
				if (v1 & 0x80) {
					// Project-bytes
					v1 = v1 & 0x7F;
					int v2 = fRead.ReadByte();
					if (v2 < 0)
						return false;
					int nLongBack = (v1 & 0x01) ? 256 : 0;
					int nBack = v2 + nLongBack;
					int nLen = v1 / 2;
					///TRACE3("%5d:PP   :Back=%4d Len=%3d\n", iPos, nBack, nLen); Sleep(1);
					if (!fSave.ProjectBytes(nBack, nLen))
						return false;
					iPos += nLen;
				} else {
					// Write-bytes
					///TRACE2("%5d:WP%3d:\n", iPos, v1); Sleep(1);
					while (v1 != 0) {
						int x = fRead.ReadByte();
						if (x < 0)
							return false;
						if (!fSave.WriteByte(x))
							return false;

						v1--;
						iPos++;
					}
				}
			}
			return (iPos == nLen);
		}
	};
};

using namespace Conv;

//////////////////////////////////////////////////////////////////////

namespace
{
	// 
	DWORD HashFileName(LPCSTR psz)
	{
		return CRFC2083CRCSample().Hash(psz, strlen(psz));
	}
	// 
	bool ExtractStrctdStg(LPCTSTR pszDoc, LPCTSTR pszFileName, CMemVuff &fSave)
	{
		IStoragePtr pOleDoc;
		HRESULT hr;
		hr = StgOpenStorage(
			_bstr_t(pszDoc),
			NULL,
			0 |STGM_DIRECT |STGM_READ |STGM_SHARE_DENY_WRITE,
			NULL,
			0,
			&pOleDoc
			);
		if (!FAILED(hr) && pOleDoc != NULL) {
			TCHAR tcHash[MAX_PATH];
			_stprintf(tcHash, _T("%08lX"), 0UL +HashFileName(pszFileName));
			IStreamPtr pOleFile;
			hr = pOleDoc->OpenStream(
				_bstr_t(tcHash),
				NULL,
				0 |STGM_READ |STGM_SHARE_EXCLUSIVE,
				0,
				&pOleFile
				);
			if (SUCCEEDED(hr) && pOleFile != NULL) {
				COleFileIo ofio;
				ofio.pOleFile = pOleFile;

				if (CDMSnesDecoder::DecompressType3(ofio, fSave)) {
					return true;
				} else {
					ASSERT(FALSE);
				}
			}
		}
		return false;
	}
	// 
	bool ExtractFileTo(CMemVuff &fSave, LPCTSTR pszInto)
	{
		FILE *f = fopen(pszInto, "wb");
		if (f) {
			if (fwrite(fSave.pData, fSave.iCur, 1, f) == 1) {
				fclose(f);
				return true;
			}
			fclose(f);
		}
		return false;
	}
};

using namespace DO2;
using namespace OSP;

bool CHUGHUG::Extract(LPCTSTR pszFileName)
{
	CDO2PathStr strDir(strBaseDir);

	{
		CString strHere = strDir ^ pszFileName;
		DWORD n = GetFileAttributes(strHere);
		if (n != 0xFFFFFFFF && !(n & FILE_ATTRIBUTE_DIRECTORY)) {
			pUnk = NULL;
			strFile = strHere;
			return true;
		}
	}

	SizeBuff sb;
	if (sb.Alloc(1024 * 128)) {
		CMemVuff fSave;
		fSave.pData = sb.GetData();
		fSave.nMax = sb.GetSize();

		if (false
			|| (fSave.iCur = 0, ExtractStrctdStg(strDir ^ "DMDC2A01.bin", pszFileName, fSave))
		) {
			CString strHere = OSP_GetTempFile(NULL, "~HH");
			SetAsTemp(strHere);
			if (ExtractFileTo(fSave, strHere)) {
				return true;
			}
		}
	}
	return false;
}

CString CHUGHUG::Extraction(LPCTSTR pszFileName)
{
	if (!Extract(pszFileName)) {
		pUnk = NULL;
		strFile = _T("");
	}
	return strFile;
}
