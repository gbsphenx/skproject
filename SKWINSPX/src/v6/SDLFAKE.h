// FAKE SDL include, just to make it compile even if there is nothing behind.

#ifndef SDL_h_
#define SDL_h_

#define SDL_MAJOR_VERSION   0
#define SDL_MINOR_VERSION   0
#define SDL_PATCHLEVEL      0

#define SDLCALL	// __cdecl
#define STDCALL	// __stdcall


typedef char Uint8;
typedef short int Uint16;
typedef long int Uint32;

typedef int SDL_AudioDeviceID;
//typedef Uint32* SDL_Thread;
typedef Uint16 SDL_AudioFormat;

struct SDL_Thread;
typedef struct SDL_Thread SDL_Thread;

typedef int (SDLCALL * SDL_ThreadFunction) (void *data);
typedef unsigned int(SDLCALL * pfnSDL_CurrentBeginThread)
                   (void *, unsigned, unsigned (STDCALL *func)(void *),
                    void * /*arg*/, unsigned, unsigned * /* threadID */);
typedef void (SDLCALL * pfnSDL_CurrentEndThread) (unsigned code);


typedef void (SDLCALL * SDL_AudioCallback) (void *userdata, Uint8 * stream, int len);

typedef struct
{
	int major;
	int minor;
	int patch;
} SDL_version;

typedef struct SDL_AudioSpec
{
    int freq;
    SDL_AudioFormat format;
    Uint8 channels;
    Uint8 silence;
    Uint16 samples;
    Uint16 padding;
    Uint32 size;
    SDL_AudioCallback callback;
    void *userdata;
} SDL_AudioSpec;

#define SDL_INIT_TIMER          0x00000001u
#define SDL_INIT_AUDIO          0x00000010u
#define SDL_INIT_VIDEO          0x00000020u  /**< SDL_INIT_VIDEO implies SDL_INIT_EVENTS */
#define SDL_INIT_JOYSTICK       0x00000200u  /**< SDL_INIT_JOYSTICK implies SDL_INIT_EVENTS */
#define SDL_INIT_HAPTIC         0x00001000u
#define SDL_INIT_GAMECONTROLLER 0x00002000u  /**< SDL_INIT_GAMECONTROLLER implies SDL_INIT_JOYSTICK */
#define SDL_INIT_EVENTS         0x00004000u
#define SDL_INIT_NOPARACHUTE    0x00100000u  /**< compatibility; this flag is ignored. */
#define SDL_INIT_EVERYTHING ( \
                SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | \
                SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER \

#define AUDIO_S16LSB    0x8010  /**< Signed 16-bit samples */

#define SDL_VERSION(x)                          \
{                                   \
    (x)->major = SDL_MAJOR_VERSION;                 \
    (x)->minor = SDL_MINOR_VERSION;                 \
    (x)->patch = SDL_PATCHLEVEL;                    \
}

const char* SDL_GetError() { return NULL ;}
int SDL_setenv(const char* sVariable, const char* sValue, int iValue) { return -1; };
int SDL_Init(int iFlags) { return -1; };
int SDL_GetVersion(SDL_version * ver) { return -1; };
int SDL_Quit() { return -1; };
int SDL_GetNumAudioDevices(int iscapture) { return -1; };
int SDL_CloseAudioDevice(SDL_AudioDeviceID iAudioID) { return -1; };
int SDL_FreeWAV(Uint8* xWAVBuffer) { return -1; };

int SDL_memset(void* pData, Uint8 iFillValue, int iSize) { return -1; };

const char* SDL_GetAudioDeviceName(int index, int iscapture) { return NULL; };
int SDL_QueueAudio(SDL_AudioDeviceID dev, const void *data, Uint32 len) { return -1; };

SDL_AudioDeviceID SDL_OpenAudioDevice(const char *device, int iscapture, const SDL_AudioSpec * desired, SDL_AudioSpec * obtained, int allowed_changes) { return -1; }

const char * SDL_GetCurrentAudioDriver(void) { return NULL; };

Uint32 SDL_GetQueuedAudioSize(SDL_AudioDeviceID dev) { return -1; }

void SDL_PauseAudioDevice(SDL_AudioDeviceID dev, int pause_on) { return ;}

int SDL_Delay(int iMillis) { return -1; };

SDL_AudioSpec* SDL_LoadWAV(const char* file, SDL_AudioSpec * spec, Uint8 ** audio_buf, Uint32 * audio_len) { return NULL; };

SDL_Thread *SDLCALL SDL_CreateThread(SDL_ThreadFunction fn, const char *name, void *data) { return NULL; }; 


#endif /* SDL_h_ */

