// FM.h: FM クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FM_H__329036D2_5231_4775_BF2F_2EA9206B96B1__INCLUDED_)
#define AFX_FM_H__329036D2_5231_4775_BF2F_2EA9206B96B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class fm
{
public:
	//
	fm(): hFile(0), hFileMap(0), lpMapped(0) { }
	// 
	~fm() { Close(); }
	//
	BOOL Open(LPCSTR lpszFileName)
	{
		if (hFile != NULL || hFileMap != NULL)
			return FALSE;
		hFile = ::CreateFileA(
			lpszFileName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN,
			NULL
			);
		if (hFile == INVALID_HANDLE_VALUE) {
			Close();
			hFile = NULL;
			return FALSE;
		}
		hFileMap = ::CreateFileMappingA(
			hFile,
			NULL,
			PAGE_READONLY|SEC_COMMIT,
			0,
			0,
			NULL
			);
		if (hFileMap == NULL) {
			Close();
			return FALSE;
		}
		return TRUE;
	}
	LPVOID MapView()
	{
		if (lpMapped)
			return lpMapped;
		lpMapped = ::MapViewOfFile(
			hFileMap,
			FILE_MAP_READ,
			0,
			0,
			0
			);
		if (lpMapped == NULL)
			return NULL;
		return lpMapped;
	}
	BOOL UnmapView()
	{
		if (!UnmapViewOfFile(lpMapped))
			return FALSE;
		lpMapped = NULL;
		return TRUE;
	}
	BOOL Close()
	{
		if (hFileMap) {
			UnmapView();
			::CloseHandle(hFileMap);
			hFileMap = NULL;
		}
		if (hFile) {
			::CloseHandle(hFile);
			hFile = NULL;
		}
		return TRUE;
	}
	DWORD GetFileSize() const
	{
		DWORD dwHi = 0, dwLo = ::GetFileSize(hFile, &dwHi);
		DWORD dwErr = 0;
		if (dwLo == 0xFFFFFFFF && (dwErr = ::GetLastError()) != 0)
			return 0;
		if (dwHi)
			dwLo = 0xFFFFFFFF;
		return dwLo;
	};

private:
	//
	HANDLE hFile;
	//
	HANDLE hFileMap;
	//
	LPVOID lpMapped;

};

#endif // !defined(AFX_FM_H__329036D2_5231_4775_BF2F_2EA9206B96B1__INCLUDED_)
