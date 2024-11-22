// SPX: Routines specifically created for emulating MIDI (HMP) music into SKWin
//	These MIDI routines are based on David J. Rager code (djrager@fourthwoods.com)
//

#include <stdafx.h>
#include <SkMIDI.h>

#if !defined(__DJGPP__) && !defined(__LINUX__)
#include <windows.h>
#endif

#include <stdio.h>
#include <SkParam.h>
#include <XDEFINES.H>

int iDungeonGame = 0;


#if !defined(__DJGPP__) && !defined(__MINGW__) && !defined(__LINUX__)


//#define MAX_BUFFER_SIZE (512 * 12)
#define MAX_BUFFER_SIZE (32 * 12)	// short buffer, allowing music change more quickly
HANDLE event;
#pragma pack(push, 1)

struct _mid_header {
	unsigned int	id;		// identifier "MThd"
	unsigned int	size;	// always 6 in big-endian format
	unsigned short	format;	// big-endian format
	unsigned short  tracks;	// number of tracks, big-endian
	unsigned short	ticks;	// number of ticks per quarter note, big-endian
};

struct _mid_track {
	unsigned int	id;		// identifier "MTrk"
	unsigned int	length;	// track length, big-endian
};

#pragma pack(pop)

struct trk {
	struct _mid_track* track;
	unsigned char* buf;
	unsigned char last_event;
	unsigned int absolute_time;
};

struct evt {
	unsigned int absolute_time;
	unsigned char* data;
	unsigned char event;
};

static unsigned char* load_file(const unsigned char* filename, unsigned int* len)
{
	unsigned char* buf;
	unsigned int ret;
	FILE* f = fopen((char*)filename, "rb");
	if(f == NULL)
		return 0;

	fseek(f, 0, SEEK_END);
	*len = ftell(f);
	fseek(f, 0, SEEK_SET);

	buf = (unsigned char*)malloc(*len);
	if(buf == 0)
	{
		fclose(f);
		return 0;
	}

	ret = fread(buf, 1, *len, f);
	fclose(f);

	if(ret != *len)
	{
		free(buf);
		return 0;
	}

	return buf;
}

static unsigned long read_var_long(unsigned char* buf, unsigned int* bytesread)
{
	unsigned long var = 0;
	unsigned char c;

	*bytesread = 0;

	do
	{
		c = buf[(*bytesread)++];
		var = (var << 7) + (c & 0x7f);
	}
	while(c & 0x80);

	return var;
}

static unsigned short swap_bytes_short(unsigned short in)
{
	return ((in << 8) | (in >> 8));
}

static unsigned long swap_bytes_long(unsigned long in)
{
	unsigned short *p;
	p = (unsigned short*)&in;

	return (  (((unsigned long)swap_bytes_short(p[0])) << 16) |
				(unsigned long)swap_bytes_short(p[1]));
}

static struct evt get_next_event(const struct trk* track)
{
	unsigned char* buf;
	struct evt e;
	unsigned int bytesread;
	unsigned int time;

	buf = track->buf;

	time = read_var_long(buf, &bytesread);
	buf += bytesread;

	e.absolute_time = track->absolute_time + time;
	e.data = buf;
	e.event = *e.data;

	return e;
}

static int is_track_end(const struct evt* e)
{
	if(e->event == 0xff) // meta-event?
		if(*(e->data + 1) == 0x2f) // track end?
			return 1;

	return 0;
}

static void CALLBACK example9_callback(HMIDIOUT out, UINT msg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    switch (msg)
    {
	case MOM_DONE:
 		SetEvent(event);
		break;
	case MOM_POSITIONCB:
	case MOM_OPEN:
	case MOM_CLOSE:
		break;
    }
}

static unsigned int get_buffer(struct trk* tracks, unsigned int ntracks, unsigned int* out, unsigned int* outlen)
{
	MIDIEVENT e, *p;
	unsigned int streamlen = 0;
	unsigned int i;
	static unsigned int current_time = 0;

	if(tracks == NULL || out == NULL || outlen == NULL)
		return 0;

	*outlen = 0;

	while(TRUE)
	{
		unsigned int time = (unsigned int)-1;
		unsigned int idx = -1;
		struct evt evt;
		unsigned char c;

		if(((streamlen + 3) * sizeof(unsigned int)) >= MAX_BUFFER_SIZE)
			break;

		// get the next event
		for(i = 0; i < ntracks; i++)
		{
			evt = get_next_event(&tracks[i]);
			if(!(is_track_end(&evt)) && (evt.absolute_time < time))
			{
				time = evt.absolute_time;
				idx = i;
			}
		}

		// if idx == -1 then all the tracks have been read up to the end of track mark
		if(idx == -1)
			break; // we're done

		e.dwStreamID = 0; // always 0

		evt = get_next_event(&tracks[idx]);

		tracks[idx].absolute_time = evt.absolute_time;
		e.dwDeltaTime = tracks[idx].absolute_time - current_time;
		current_time = tracks[idx].absolute_time;

		if(!(evt.event & 0x80)) // running mode
		{
			unsigned char last = tracks[idx].last_event;
			c = *evt.data++; // get the first data byte
			e.dwEvent = ((unsigned long)MEVT_SHORTMSG << 24) |
						((unsigned long)last) |
						((unsigned long)c << 8);
			if(!((last & 0xf0) == 0xc0 || (last & 0xf0) == 0xd0))
			{
				c = *evt.data++; // get the second data byte
				e.dwEvent |= ((unsigned long)c << 16);
			}

			p = (MIDIEVENT*)&out[streamlen];
			*p = e;

			streamlen += 3;

			tracks[idx].buf = evt.data;
		}
		else if(evt.event == 0xff) // meta-event
		{
			evt.data++; // skip the event byte
			unsigned char meta = *evt.data++; // read the meta-event byte
			unsigned int len;

			switch(meta)
			{
			case 0x51: // only care about tempo events
				{
					unsigned char a, b, c;
					len = *evt.data++; // get the length byte, should be 3
					a = *evt.data++;
					b = *evt.data++;
					c = *evt.data++;

					e.dwEvent = ((unsigned long)MEVT_TEMPO << 24) |
								((unsigned long)a << 16) |
								((unsigned long)b << 8) |
								((unsigned long)c << 0);

					p = (MIDIEVENT*)&out[streamlen];
					*p = e;

					streamlen += 3;
				}
				break;
			default: // skip all other meta events
				len = *evt.data++; // get the length byte
				evt.data += len;
				break;
			}

			tracks[idx].buf = evt.data;
		}
		else if((evt.event & 0xf0) != 0xf0) // normal command
		{
			tracks[idx].last_event = evt.event;
			evt.data++; // skip the event byte
			c = *evt.data++; // get the first data byte
			e.dwEvent = ((unsigned long)MEVT_SHORTMSG << 24) |
						((unsigned long)evt.event << 0) |
						((unsigned long)c << 8);
			if(!((evt.event & 0xf0) == 0xc0 || (evt.event & 0xf0) == 0xd0))
			{
				c = *evt.data++; // get the second data byte
				e.dwEvent |= ((unsigned long)c << 16);
			}

			p = (MIDIEVENT*)&out[streamlen];
			*p = e;

			streamlen += 3;

			tracks[idx].buf = evt.data;
		}

	}

	*outlen = streamlen * sizeof(unsigned int);

	return 1;
}

//unsigned int example_midi()
unsigned long
SkWinMIDI_MIDI_LOOP(void* parameters)
{
	unsigned char* midibuf = NULL;
	unsigned int midilen = 0;
	char midifilename[32];

	struct _mid_header* hdr = NULL;

	unsigned int i;

	unsigned short ntracks = 0;
	struct trk* tracks = NULL;

	unsigned int streambufsize = MAX_BUFFER_SIZE;
	unsigned int* streambuf = NULL;
	unsigned int streamlen = 0;

	static int iThreadCounter = 0;
	iThreadCounter++;

	HMIDISTRM out;
	MIDIPROPTIMEDIV prop;
	MIDIHDR mhdr;
	unsigned int device = 0;


	// Build the filename for MID to read
	SkWinMIDI* xWinMIDI = (SkWinMIDI*) parameters;
	int iHexNum;
	xWinMIDI->bCurrentlyPlaying = true;
	xWinMIDI->iCurrentRequestedMusic = xWinMIDI->iNextRequestedMusic;
	iHexNum = xWinMIDI->iCurrentRequestedMusic;

	char sDataRootFolder[64];
	sprintf(sDataRootFolder, "DATA");	// default

	// SPX: If a specific data folder name is given (within SkWin directory), it will be used instead
	if (xWinMIDI->sRootFolder != NULL && xWinMIDI->sRootFolder[0] != 0)
	{
		sprintf(sDataRootFolder, xWinMIDI->sRootFolder);
	}

	if (SkCodeParam::bNoAudio == true || SkCodeParam::bNoMusic == true)
		SkCodeParam::bMIDIMusicEnabled = false;

	//printf("%d> Requesting music %02x (%02d)\n", iThreadCounter, iHexNum, iHexNum);
	sprintf(midifilename, "./%s/music/%02x.hmp.mid", sDataRootFolder, iHexNum);

	midibuf = load_file((unsigned char*)midifilename, &midilen);
	if(midibuf == NULL)
	{
		printf("%d> could not open %s\n", iThreadCounter, midifilename);
		return 0;
	}

	hdr = (struct _mid_header*)midibuf;
	midibuf += sizeof(struct _mid_header);
	ntracks = swap_bytes_short(hdr->tracks);

	tracks = (struct trk*)malloc(ntracks * sizeof(struct trk));
	if(tracks == NULL)
		goto error1;

	for(i = 0; i < ntracks; i++)
	{
		tracks[i].track = (struct _mid_track*)midibuf;
		tracks[i].buf = midibuf + sizeof(struct _mid_track);
		tracks[i].absolute_time = 0;
		tracks[i].last_event = 0;

		midibuf += sizeof(struct _mid_track) + swap_bytes_long(tracks[i].track->length);
	}

	streambuf = (unsigned int*)malloc(sizeof(unsigned int) * streambufsize);
	if(streambuf == NULL)
		goto error2;

	memset(streambuf, 0, sizeof(unsigned int) * streambufsize);

    if ((event = CreateEvent(0, FALSE, FALSE, 0)) == NULL)
    	goto error3;

	if (midiStreamOpen(&out, &device, 1, (DWORD)example9_callback, 0, CALLBACK_FUNCTION) != MMSYSERR_NOERROR)
		goto error4;


	prop.cbStruct = sizeof(MIDIPROPTIMEDIV);
	prop.dwTimeDiv = swap_bytes_short(hdr->ticks);
	if(midiStreamProperty(out, (LPBYTE)&prop, MIDIPROP_SET|MIDIPROP_TIMEDIV) != MMSYSERR_NOERROR)
		goto error5;

	mhdr.lpData = (char*)streambuf;
	mhdr.dwBufferLength = mhdr.dwBytesRecorded = streambufsize;
	mhdr.dwFlags = 0;

	if(midiOutPrepareHeader((HMIDIOUT)out, &mhdr, sizeof(MIDIHDR)) != MMSYSERR_NOERROR)
		goto error5;

	if(midiStreamRestart(out) != MMSYSERR_NOERROR)
		goto error6;

	//printf("%d> buffering... (first)\n", iThreadCounter);
	get_buffer(tracks, ntracks, streambuf, &streamlen);
	
	while(streamlen > 0)
	{
		mhdr.dwBytesRecorded = streamlen;

		if(midiStreamOut(out, &mhdr, sizeof(MIDIHDR)) != MMSYSERR_NOERROR)
			goto error7;

		midiOutSetVolume((HMIDIOUT)out, 0x0000);

		WaitForSingleObject(event, INFINITE);

		//printf("%d> buffering... (loop)\n", iThreadCounter);
		get_buffer(tracks, ntracks, streambuf, &streamlen);

		// SPX: if music has to change, then stop the current to let the new one start.
		if (xWinMIDI->iCurrentRequestedMusic != xWinMIDI->iNextRequestedMusic)
			break;
		if (!SkCodeParam::bMIDIMusicEnabled)
			break;
	}
	xWinMIDI->bCurrentlyPlaying = false;
	//printf("%d> done.\n", iThreadCounter);

error7:
	midiOutReset((HMIDIOUT)out);

error6:
	midiOutUnprepareHeader((HMIDIOUT)out, &mhdr, sizeof(MIDIHDR));

error5:
	midiStreamClose(out);

error4:
	CloseHandle(event);

error3:
	free(streambuf);

error2:
	free(tracks);

error1:
	free(hdr);

	return(0);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// static table of musics mapped to maps
int tMusicMaps[64] =
{
	0x02, 0x11, 0x0e, 0x1b,		0x04, 0x0c, 0x0c, 0x12,
	0x0f, 0x0d, 0x0c, 0x0c,		0x10, 0x06, 0x15, 0x0e,
	
	0x11, 0x11, 0x11, 0x11,		0x03, 0x08, 0x11, 0x0e,
	0x02, 0x17, 0x16, 0x14,		0x11, 0x00, 0x02, 0x02,
	
	0x02, 0x09, 0x02, 0x03,		0x0e, 0x10, 0x1c, 0x16,
	0x13, 0x09, 0x16, 0x03,		0x11, 0x02, 0xFF, 0xFF,

	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,		0xFF, 0xFF, 0xFF, 0xFF
};



//------------------------------------------------------------------------------

SkWinMIDI::SkWinMIDI(int dung, const char* folder, const char* defaultfolder)
{
	iDungeonGame = dung;
	iCurrentRequestedMusic = -1; // no music
	iNextRequestedMusic = -1;
	bCurrentlyPlaying = false;
	//CreateThread(NULL,0, (LPTHREAD_START_ROUTINE) example_midi,NULL,0,NULL);
	//example_midi();
	memset(sRootFolder, 0, 256);
	if (folder != NULL)
		sprintf(sRootFolder, folder);
	else if (defaultfolder != NULL)
		sprintf(sRootFolder, defaultfolder);
}


void
SkWinMIDI::REQUEST_PLAY_MUSIC_FROM_MAP(int iMapNumber)
{
	if (!SkCodeParam::bMIDIMusicEnabled)
		return;
	//printf("Asking music for map %02d\n", iMapNumber);
	PlaySound(NULL, NULL, SND_PURGE);
	iNextRequestedMusic = tMusicMaps[iMapNumber];
	if (bCurrentlyPlaying == false)
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) SkWinMIDI_MIDI_LOOP, this, 0, NULL);
}

//------------------------------------------------------------------------------

#else // __DJGPP__ , __MINGW__ , __LINUX__


SkWinMIDI::SkWinMIDI(int dung, const char* folder, const char* defaultfolder)
{
	iDungeonGame = dung;
	iCurrentRequestedMusic = -1; // no music
	iNextRequestedMusic = -1;
	bCurrentlyPlaying = false;
	memset(sRootFolder, 0, 256);
	if (folder != NULL)
		strcpy(sRootFolder, folder);
	else if (defaultfolder != NULL)
		strcpy(sRootFolder, defaultfolder);
}


void
SkWinMIDI::REQUEST_PLAY_MUSIC_FROM_MAP(int iMapNumber)
{
	;
}


#endif // __DJGPP__