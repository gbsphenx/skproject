
#ifndef _DDATX_H_
#define _DDATX_H_

class CDDATx
{
public:
	// 
	struct CSerializedReader {
		// 
		virtual BYTE ReadByte() = NULL;
		// 
		virtual WORD ReadWord() = NULL;
		// 
		virtual DWORD ReadDWord() = NULL;
		// 
		virtual bool HasError() = NULL;
	};
	// 
	struct CSurelySerializedReader {
		// 
		virtual BYTE SurelyReadByte() = NULL;
		// 
		virtual WORD SurelyReadWord() = NULL;
	};
	// 
	struct CSerializedWriter {
		// 
		virtual bool WriteByte(BYTE x) = NULL;
		// 
		virtual bool WriteWord(WORD x) = NULL;
		// 
		virtual bool WriteDWord(DWORD x) = NULL;
	};
	// 
	struct CReadERR { };
	// 
	struct CSerializedReaderWithChkSum : CSerializedReader
	{
		// 
		CSerializedReader &f;
		// 
		DWORD nValue;

		// 
		CSerializedReaderWithChkSum(CSerializedReader &f)
			: f(f)
			, nValue(0)
		{

		}
		// 
		BYTE ReadByte()
		{
			BYTE x = f.ReadByte();
			nValue += LOBYTE(x);
			return x;
		}
		// 
		WORD ReadWord()
		{
			WORD x = f.ReadWord();
			nValue += LOBYTE(x);
			nValue += HIBYTE(x);
			return x;
		}
		// 
		DWORD ReadDWord()
		{
			int x = f.ReadDWord();
			nValue += (BYTE)(x);
			nValue += (BYTE)(x >> 8);
			nValue += (BYTE)(x >> 16);
			nValue += (BYTE)(x >> 24);
			return x;
		}
		// 
		bool HasError() {
			return f.HasError();
		}
	};
	// 
	struct CSerializedWriterWithChkSum : CSerializedWriter
	{
		// 
		CSerializedWriter &f;
		// 
		DWORD nValue;

		// 
		CSerializedWriterWithChkSum(CSerializedWriter &f)
			: f(f)
			, nValue(0)
		{

		}
		// 
		bool WriteByte(BYTE x)
		{
			if (!f.WriteByte(x)) return false;
			nValue += LOBYTE(x);
			return true;
		}
		// 
		bool WriteWord(WORD x)
		{
			if (!f.WriteWord(x)) return false;
			nValue += LOBYTE(x);
			nValue += HIBYTE(x);
			return true;
		}
		// 
		bool WriteDWord(DWORD x)
		{
			if (!f.WriteDWord(x)) return false;
			nValue += (BYTE)(x >> 0);
			nValue += (BYTE)(x >> 8);
			nValue += (BYTE)(x >> 16);
			nValue += (BYTE)(x >> 24);
			return true;
		}
	};
	// 
	struct CMemVuff : CSerializedWriter {
		// 
		PBYTE pData;
		// 
		int iCur, nMax;
		// 
		bool fLE;

		// 
		int ReadByte()
		{
			if (nMax < iCur + 1) return -1;

			BYTE x = pData[iCur]; iCur++;
			return x;
		}
		// 
		bool WriteByte(BYTE x)
		{
			if (nMax < iCur + 1) return false;

			pData[iCur] = x; iCur++;
			return true;
		}
		// 
		bool WriteWord(WORD x)
		{
			if (nMax < iCur + 2) return false;

			if (fLE) {
				pData[iCur] = LOBYTE(x); iCur++;
				pData[iCur] = HIBYTE(x); iCur++;
			}
			else {
				pData[iCur] = HIBYTE(x); iCur++;
				pData[iCur] = LOBYTE(x); iCur++;
			}
			return true;
		}
		// 
		bool WriteDWord(DWORD x)
		{
			if (nMax < iCur + 4) return false;

			if (fLE) {
				pData[iCur] = (BYTE)(x >> 0); iCur++;
				pData[iCur] = (BYTE)(x >> 8); iCur++;
				pData[iCur] = (BYTE)(x >> 16); iCur++;
				pData[iCur] = (BYTE)(x >> 24); iCur++;
			}
			else {
				pData[iCur] = (BYTE)(x >> 24); iCur++;
				pData[iCur] = (BYTE)(x >> 16); iCur++;
				pData[iCur] = (BYTE)(x >> 8); iCur++;
				pData[iCur] = (BYTE)(x >> 0); iCur++;
			}
			return true;
		}
	};
	// 
	struct CEnclosedWisoFileIo : CSerializedReader, CSurelySerializedReader, CSerializedWriter {
		// 
		CFile &f;
		// 
		bool fLE;
		// 
		bool fErr;

		// 
		CEnclosedWisoFileIo(CFile &f, bool fLE): f(f), fLE(fLE) { }
		// 
		~CEnclosedWisoFileIo()
		{
			Close();
		}
		// 
		void Close(bool fLE = true)
		{
			this->fLE = fLE;
			this->fErr = false;
		}
		// 
		virtual BYTE ReadByte()
		{
			BYTE x;
			if (f.Read(&x, 1) == 1) {
				return x;
			}
			fErr = true;
			return -1;
		}
		// 
		virtual WORD ReadWord()
		{
			BYTE x[2];
			if (f.Read(&x, 2) == 2) {
				return fLE
					? MAKEWORD(x[0], x[1])
					: MAKEWORD(x[1], x[0])
				;
			}
			fErr = true;
			return -1;
		}
		// 
		virtual DWORD ReadDWord()
		{
			BYTE x[4];
			if (f.Read(&x, 4) == 4) {
				DWORD v = 0;
				if (fLE) {
					v <<= 8; v |= x[3];
					v <<= 8; v |= x[2];
					v <<= 8; v |= x[1];
					v <<= 8; v |= x[0];
				}
				else {
					v <<= 8; v |= x[0];
					v <<= 8; v |= x[1];
					v <<= 8; v |= x[2];
					v <<= 8; v |= x[3];
				}
				return v;
			}
			fErr = true;
			return -1;
		}
		// 
		virtual bool HasError() {
			return fErr;
		}
		// 
		virtual BYTE SurelyReadByte()
		{
			BYTE x;
			if (f.Read(&x, 1) == 1) {
				return x;
			}
			throw CReadERR();
		}
		// 
		virtual WORD SurelyReadWord()
		{
			BYTE x[2];
			if (f.Read(&x, 2) == 2) {
				return fLE
					? MAKEWORD(x[0], x[1])
					: MAKEWORD(x[1], x[0])
				;
			}
			throw CReadERR();
		}
		// 
		virtual bool WriteByte(BYTE x)
		{
			TRY {
				f.Write(&x, 1);
				return true;
			} CATCH_ALL(e) {
				return false;
			} END_CATCH_ALL
		}
		// 
		virtual bool WriteWord(WORD x)
		{
			TRY {
				if (fLE) {
					BYTE v[2] = {LOBYTE(x), HIBYTE(x)};
					f.Write(v, 2);
				}
				else {
					BYTE v[2] = {HIBYTE(x), LOBYTE(x)};
					f.Write(v, 2);
				}
				return true;
			} CATCH_ALL(e) {
				return false;
			} END_CATCH_ALL
		}
		// 
		virtual bool WriteDWord(DWORD x)
		{
			TRY {
				if (fLE) {
					BYTE v[4] = {
						(BYTE)(x >> 0),
						(BYTE)(x >> 8),
						(BYTE)(x >> 16),
						(BYTE)(x >> 24),
					};
					f.Write(v, 4);
				}
				else {
					BYTE v[4] = {
						(BYTE)(x >> 24),
						(BYTE)(x >> 16),
						(BYTE)(x >> 8),
						(BYTE)(x >> 0),
					};
					f.Write(v, 4);
				}
				return true;
			} CATCH_ALL(e) {
				return false;
			} END_CATCH_ALL
		}
	};
};

#endif // _DDATX_H_
