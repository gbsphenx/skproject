
#ifndef _SBLAST_H_
#define _SBLAST_H_

#define DSB_MAX 10U
#define ALS_MAX 10U

namespace sblast_sys {
	// 
	struct PlayBuff {
		const BYTE *pbBuff;
		size_t cbBuff;
		int dX;
		int dY;
		int af;
		int bf;
	};
	// 
	class DS {
#ifdef _USE_MFC60
		CComPtr<IDirectSound> pDS;
		CComPtr<IDirectSoundBuffer> pDSB[DSB_MAX];
		CComPtr<IDirectSound3DListener> pDSL;
#else
		CComPtr<IDirectSound8> pDS;
		CComPtr<IDirectSoundBuffer> pDSB[DSB_MAX];
		CComPtr<IDirectSound3DListener8> pDSL;
#endif
	public:
		void Open(HWND hwnd);
		void Close();
		void Play(PlayBuff &pb);
	};
	// 
	class Openal {
		ALCdevice *dev;
		ALCcontext *ctx;
		ALuint sName[ALS_MAX];
		ALuint bufferName[ALS_MAX];
	public:
		Openal();
		void Open();
		void Close();
		void Play(PlayBuff &pb);
	};
};

#endif // _SBLAST_H_
