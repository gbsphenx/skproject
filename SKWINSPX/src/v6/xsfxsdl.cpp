#include <StdAfx.h>
#include <xsfxsdl.h>
#include <skdebug.h>
#include <skglobal.h>	// mvalid

#if defined(_USE_MFC80) || defined(_USE_MFC60)
	#include <windows.h>
#endif

// Handle unrecognized extended attribute
//#if defined(_USE_MFC80) || defined(_USE_MFC60)
//	#define SDL_DISABLE_DEPRECATED
//#endif


#if defined(__LINUX__) && defined(__SDL__)
	#include <SDL2/SDL.h>
#elif defined (__DJGPP__)
	#include <SDLFAKE.h>
#else
	#include <SDL.h>
#endif // __LINUX__

// Audio data structure
typedef struct {
    SDL_AudioDeviceID device_id;
    Uint8 *audio_buf;
    Uint32 audio_len;
} AudioData;

SDL_AudioDeviceID SDL_AudioDevice = 0;


// 
#if defined (__DJGPP__)

int XAUDIO_SDL_INIT() { return -1; }
int XAUDIO_SDL_CLOSE() { return -1; }
int XAUDIO_SDL_PLAY_SOUND_FILE(const char* sFilename, int vol) { return -1; }
int XAUDIO_SDL_PLAY_MIDI_FILE(const char* sFilename, int vol) { return -1; }

#else

int XAUDIO_SDL_INIT() {
    
	int i = 0;


#if defined(_USE_MFC80) || defined(_USE_MFC60)
    // Initialize COM for the current thread
	HRESULT hr = CoInitialize(NULL); // NULL indicates STA threading
    if (FAILED(hr)) {
        fprintf(stderr, "Failed to initialize COM: 0x%lx\n", hr);
        return 1;
    }
#endif // MFC

#if defined (__LINUX__)
	if (SDL_setenv("SDL_AUDIODRIVER", "pulseaudio", 1) != 0) {
        fprintf(stderr, "Failed to set SDL_AUDIODRIVER: %s\n", SDL_GetError());
    }
#else
	// This to solve the "Failed to open audio: WASAPI can't initialize audio client: CoInitialize has not been called" on Win 7
	if (SDL_setenv("SDL_AUDIODRIVER", "wasapi", 1) != 0) {
        fprintf(stderr, "Failed to set SDL_AUDIODRIVER: %s\n", SDL_GetError());
    }
#endif

	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
	SkD((DLV_DBG_SND, "SDL: Init SDL OK\n"));

    SDL_version iSDLVerCompiled;
    SDL_version iSDLVerRuntime;

    SDL_VERSION(&iSDLVerCompiled);	// Compiled library version
    SDL_GetVersion(&iSDLVerRuntime);	// Runtime version

    SkD((DLV_DBG_SND, "SDL: Compiled SDL version: %d.%d.%d\n", iSDLVerCompiled.major, iSDLVerCompiled.minor, iSDLVerCompiled.patch));
    SkD((DLV_DBG_SND, "SDL:   Linked SDL version: %d.%d.%d\n", iSDLVerRuntime.major, iSDLVerRuntime.minor, iSDLVerRuntime.patch));


	int count = SDL_GetNumAudioDevices(0);
	for (i = 0; i < count; i++) {
		printf("Audio Device #%d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
	}

	printf("Using audio driver: %s\n", SDL_GetCurrentAudioDriver());
	


	// Audio must not be opened in thread, that won't work
    SDL_AudioSpec desired_spec;
	SDL_AudioSpec obtained_spec;
    SDL_memset(&desired_spec, 0, sizeof(desired_spec));
    desired_spec.freq = 44100;
	desired_spec.freq = 44100;
    desired_spec.format = AUDIO_S16LSB;
    desired_spec.channels = 2;
    desired_spec.samples = 4096;
	SDL_AudioDevice = SDL_OpenAudioDevice(NULL, 0, &desired_spec, &obtained_spec, 0);
	printf("Main init Audio device is = %d\n", SDL_AudioDevice);

	/// FORCE AUDIO DEVICE (Win 11 test)
	const char* sAudioDeviceName = NULL;
	sAudioDeviceName = SDL_GetAudioDeviceName(SDL_AudioDevice, 0);
	printf("Selected Audio Output Device: (%s)\n", sAudioDeviceName);


	printf("Desired : FREQ: %d, FMT %d, C: %d, SMP: %04d\n", desired_spec.freq, desired_spec.format, desired_spec.channels, desired_spec.samples);
	printf("Obtained: FREQ: %d, FMT %d, C: %d, SMP: %04d\n", obtained_spec.freq, obtained_spec.format, obtained_spec.channels, obtained_spec.samples);

	return 1;
}

int XAUDIO_SDL_CLOSE() {
	SDL_Quit();
	return 1;
}


// Thread function for audio playback
int threadAudio(void *data) {
    AudioData *audio = (AudioData *)data;

	AudioData sLocalAudioData = {audio->device_id, audio->audio_buf, audio->audio_len};
/*
    SDL_AudioSpec desired_spec;
	SDL_AudioSpec obtained_spec;
    SDL_memset(&desired_spec, 0, sizeof(desired_spec));
    desired_spec.freq = 44100;
    desired_spec.format = AUDIO_S16LSB;
    desired_spec.channels = 2;
    desired_spec.samples = 4096;

	//SDL_AudioDevice = SDL_OpenAudioDevice(NULL, 0, &desired_spec, &obtained_spec, 0);

    // Set the audio callback and user data
    desired_spec.callback = NULL;	// because we'll use Queue
    desired_spec.userdata = NULL;
// Open the audio device
	;
	SDL_AudioDevice = SDL_OpenAudioDevice(NULL, 0, &desired_spec, NULL, 0);
	*/
	/*
    if ( SDL_OpenAudio(&desired_spec, NULL) < 0) {
        fprintf(stderr, "Failed to open audio: %s\n", SDL_GetError());
        //SDL_FreeWAV(wav_buffer);
        //SDL_Quit();
        return 1;
    }*/
	printf("Thread Audio device is = %d\n", SDL_AudioDevice);
	//SDL_AudioDevice = 2;

	audio->device_id = SDL_AudioDevice;
	sLocalAudioData.device_id = SDL_AudioDevice;
	printf("Thread Audio Data => dev = %d, buff = %08X, len = %d\n", sLocalAudioData.device_id, sLocalAudioData.audio_buf, sLocalAudioData.audio_len);

    // Queue the audio buffer
    if (SDL_QueueAudio(SDL_AudioDevice, sLocalAudioData.audio_buf, sLocalAudioData.audio_len) < 0) {
	//if (SDL_QueueAudio(audio->audio_buf, audio->audio_len) < 0) {
        fprintf(stderr, "Failed to queue audio: %s\n", SDL_GetError());
        return 1;
    }

    // Start playback
    SDL_PauseAudioDevice(SDL_AudioDevice, 0);

    // Wait for audio to finish playing
    while (SDL_GetQueuedAudioSize(SDL_AudioDevice) > 0) {
		//printf("Sound still in queue.\n");
        SDL_Delay(100); // Check periodically
    }

    printf("Audio playback finished in thread.\n");
    return 0;
}

int XAUDIO_SDL_PLAY_SOUND_FILE(const char* sFilename, int vol) {
	
	return 1;
	SkD((DLV_DBG_SND, "SDL: Playing SDL soundwav (%s)\n", sFilename));
    
    SDL_AudioSpec xSDLAudioSpec;
    Uint8* xWAVBuffer;
    Uint32 iWAVLength;

    if (SDL_LoadWAV(sFilename, &xSDLAudioSpec, &xWAVBuffer, &iWAVLength) == NULL) {
        fprintf(stderr, "Failed to load WAV file: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
	printf("Wav Audio Data => buff = %08X, len = %d\n", iWAVLength, iWAVLength);

    // Set audio callback and userdata
    //xSDLAudioSpec.callback = audio_callback;
    //xSDLAudioSpec.userdata = wav_buffer;

    // Prepare thread data
    AudioData audio_data = {SDL_AudioDevice, xWAVBuffer, iWAVLength};
	printf("Struct Audio Data => dev = %d, buff = %08X, len = %d\n", audio_data.device_id, audio_data.audio_buf, audio_data.audio_len);

    // Create and start the thread
    SDL_Thread *xSDLThread = SDL_CreateThread(threadAudio, "AudioThread", &audio_data);
    if (!xSDLThread) {
        fprintf(stderr, "Failed to create thread: %s\n", SDL_GetError());
        SDL_CloseAudioDevice(SDL_AudioDevice);
        SDL_FreeWAV(xWAVBuffer);
        SDL_Quit();
        return 1;
    }

	SDL_Delay(100);
	
	return 1;
}

int XAUDIO_SDL_PLAY_MIDI_FILE(const char* sFilename, int vol) {

/*
    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096) < 0) {
        fprintf(stderr, "Failed to initialize SDL_mixer: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    // Enable MIDI support
    if (Mix_Init(MIX_INIT_MID) == 0) {
        fprintf(stderr, "MIDI support not available: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    // Load the MIDI file
    Mix_Music *music = Mix_LoadMUS(midi_file);
    if (!music) {
        fprintf(stderr, "Failed to load MIDI file: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    // Play the MIDI file
    if (Mix_PlayMusic(music, 1) == -1) {
        fprintf(stderr, "Failed to play MIDI file: %s\n", Mix_GetError());
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }*/

	return 1;	
}

#endif // not __DJGPP__