
#include "StdAfx.h"
#include "SBlast.h"

// SPX : actually, it may be best to pass playback frequency through parameters instead of fixing it there
#include <defines.h>	// for default frequency
#include <SkCodeParam.h>
// SPX


#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "OpenAL32.lib")

#ifdef _USE_MFC60
#pragma comment(linker, "/delayload:OpenAL32.dll")
#pragma comment(lib, "delayimp.lib")
#endif

namespace sblast_sys {
	static int calcExpDist(int x) {
		if (x < 0)
			return -(1 << (-x));
		if (x > 0)
			return +(1 << (+x));
		return 0;
	}
	void DS::Open(HWND hwnd) {
		HRESULT hr;
		do {
#ifdef _USE_MFC60
			if (FAILED(hr = CoCreateInstance(CLSID_DirectSound, NULL, CLSCTX_ALL, IID_IDirectSound, (void **)&pDS)))
				break;
#else
			if (FAILED(hr = CoCreateInstance(CLSID_DirectSound8, NULL, CLSCTX_ALL, IID_IDirectSound8, (void **)&pDS)))
				break;
#endif
			if (FAILED(hr = pDS->Initialize(NULL)))
				break;
			if (FAILED(hr = pDS->SetCooperativeLevel(hwnd, DSSCL_NORMAL)))
				break;

			DSBUFFERDESC dsbd;
			ZeroMemory(&dsbd, sizeof(dsbd));
			dsbd.dwSize = sizeof(dsbd);
			dsbd.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;

			while (true) {
				CComPtr<IDirectSoundBuffer> pDS1;

				if (FAILED(hr = pDS->CreateSoundBuffer(&dsbd, &pDS1, NULL)))
					break;
#ifdef _USE_MFC60
				if (FAILED(hr = pDS1->QueryInterface(IID_IDirectSound3DListener, (void **)&pDSL)))
					break;
#else
				if (FAILED(hr = pDS1->QueryInterface(IID_IDirectSound3DListener8, (void **)&pDSL)))
					break;
#endif

				D3DVECTOR vo, vt, vp;
				if (FAILED(hr = pDSL->GetOrientation(&vo, &vt)))
					break;
				if (FAILED(hr = pDSL->GetPosition(&vp)))
					break;

				printf("");
				break;
			}
			return;
		} while (false);

		pDS.Release();
	}
	void DS::Close() {
		for (int x=0; x<DSB_MAX; x++)
			pDSB[x].Release();
		pDSL.Release();
		pDS.Release();
	}
	void DS::Play(PlayBuff &pb) {
		if (pDS == NULL)
			return;
		HRESULT hr;
		for (int x=0; x<DSB_MAX; x++) {
			if (pDSB[x] != NULL) {
				DWORD st = 0;
				if (SUCCEEDED(hr = pDSB[x]->GetStatus(&st)))
					if ((st & DSBSTATUS_PLAYING) != 0)
						continue;
			}
			WAVEFORMATEX wfex;

			// SPX
			int iBasePlaybackFrequency = SKWIN_PLAYBACK_FREQUENCY;	// 6000
			int iPlaybackFrequency = iBasePlaybackFrequency;	
			int iRandomValue = rand()%750 - 500;
			if (SkCodeParam::bUseVaryingPlaybackFrequency)
				iPlaybackFrequency += iRandomValue;
			// SPX

			ZeroMemory(&wfex, sizeof(wfex));
			wfex.wFormatTag = WAVE_FORMAT_PCM;
			wfex.nChannels = 1;
			wfex.nSamplesPerSec = iPlaybackFrequency;
			wfex.nAvgBytesPerSec = iPlaybackFrequency;
			wfex.nBlockAlign = 1;
			wfex.wBitsPerSample = 8;

#ifdef _USE_MFC80
			const GUID *pid = &DS3DALG_DEFAULT;
			if (pb.af == 1) pid = &DS3DALG_HRTF_FULL;
			if (pb.af == 2) pid = &DS3DALG_HRTF_LIGHT;
#endif

			DSBUFFERDESC dsbd;
			ZeroMemory(&dsbd, sizeof(dsbd));
			dsbd.dwSize = sizeof(dsbd);
			dsbd.dwFlags = DSBCAPS_CTRL3D|DSBCAPS_STICKYFOCUS
				|((pb.bf == 1) ? DSBCAPS_LOCHARDWARE : 0)
				|((pb.bf == 2) ? DSBCAPS_LOCSOFTWARE : 0)
				;
			dsbd.dwBufferBytes = static_cast<DWORD>(pb.cbBuff);
			dsbd.lpwfxFormat = &wfex;
#ifdef _USE_MFC80
			dsbd.guid3DAlgorithm = *pid;
#endif

			pDSB[x].Release();

			if (FAILED(hr = pDS->CreateSoundBuffer(&dsbd, &pDSB[x], NULL)))
				break;

			void *pv = NULL;
			void *pv2 = NULL;
			DWORD cb = 0;
			DWORD cb2 = 0;
			if (FAILED(hr = pDSB[x]->Lock(0, 0, &pv, &cb, &pv2, &cb2, DSBLOCK_ENTIREBUFFER)))
				break;

			for (DWORD t=0; t<pb.cbBuff; t++)
				((PBYTE)pv)[t] = 0x80 +pb.pbBuff[t];

			VERIFY(SUCCEEDED(hr = pDSB[x]->Unlock(pv, cb, pv2, cb2)));

			CComQIPtr<IDirectSound3DBuffer, &IID_IDirectSound3DBuffer> pDS3DB = pDSB[x];
			if (pDS3DB != NULL) {
				//TRACE3("DS3DB: #%d,(%2d,%2d)\n", x, pb.dX, pb.dY);
				VERIFY(SUCCEEDED(hr = pDS3DB->SetPosition(D3DVALUE(calcExpDist(pb.dX)), 0, D3DVALUE(calcExpDist(-pb.dY)), DS3D_IMMEDIATE)));
			}

			VERIFY(SUCCEEDED(hr = pDSB[x]->Play(0, 0, 0)));
			break;
		}
	}

//
// Exception information
//
#define FACILITY_VISUALCPP  ((LONG)0x6d)
#define VcppException(sev,err)  ((sev) | (FACILITY_VISUALCPP<<16) | err)

	int filterDelayLoadError(int c) {
		if (c == VcppException(ERROR_SEVERITY_ERROR, ERROR_MOD_NOT_FOUND))
			return EXCEPTION_EXECUTE_HANDLER;
		if (c == VcppException(ERROR_SEVERITY_ERROR, ERROR_PROC_NOT_FOUND))
			return EXCEPTION_EXECUTE_HANDLER;

		return EXCEPTION_CONTINUE_SEARCH;
	}

	Openal::Openal() {
		dev = NULL;
	}
	void Openal::Open() {
		__try {
			dev = alcOpenDevice(NULL);
		}
		__except(filterDelayLoadError(GetExceptionCode())) {
			MessageBox(NULL, "OpenAL 1.1 may be missed on your PC!\n\n" "OpenAL sound output is currently disabled.", "", MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
			return;
		}
		if (dev != NULL) {
			ctx = alcCreateContext(dev, NULL);
			if (ctx != NULL) {
				if (alcMakeContextCurrent(ctx)) {
					alGenSources(ALS_MAX, sName);
					alGenBuffers(ALS_MAX, bufferName);
				}
			}
		}
	}
	void Openal::Close() {
		if (ctx != NULL)
			alcDestroyContext(ctx), ctx = NULL;
		if (dev != NULL)
			alcCloseDevice(dev), dev = NULL;
	}
	void Openal::Play(PlayBuff &pb) {
		if (dev == NULL || ctx == NULL)
			return;
		ALenum er;
		for (int x=0; x<ALS_MAX; x++) {
			ALint st = 0;
			alGetSourcei(sName[x], AL_SOURCE_STATE, &st);
			if (st == AL_PLAYING)
				continue;

			alSourceStop(sName[x]);
			er = alGetError();

			alSourcei(sName[x], AL_BUFFER, 0);
			er = alGetError();

			BYTE tempBuff[30000];
			for (DWORD t=0; t<pb.cbBuff; t++) {
				tempBuff[t] = pb.pbBuff[t] +0x80;
			}

			alBufferData(bufferName[x], AL_FORMAT_MONO8, tempBuff, static_cast<ALsizei>(min(sizeof(tempBuff), pb.cbBuff)), 6000);
			er = alGetError();

			alSourcei(sName[x], AL_BUFFER, bufferName[x]);
			er = alGetError();

			alSource3f(sName[x], AL_POSITION, ALfloat(calcExpDist(pb.dX)), 0, ALfloat(calcExpDist(-pb.dY)));
			er = alGetError();

			alSourcePlay(sName[x]);
			er = alGetError();
			break;
		}
	}
};