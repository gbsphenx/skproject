//--- X-SFX-SDL -------------------------------------
// Managing audio through SDL

#ifndef _SK_X_AUDIO_H_
#define _SK_X_AUDIO_H_

//------------------------------------------------------------------------------

int XAUDIO_SDL_INIT();
int XAUDIO_SDL_CLOSE();
int XAUDIO_SDL_PLAY_SOUND_FILE(const char* sFilename, int vol);
int XAUDIO_SDL_PLAY_MIDI_FILE(const char* sFilename, int vol);

//------------------------------------------------------------------------------

#endif // _SK_X_AUDIO_H_

