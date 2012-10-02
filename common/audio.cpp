#include "audio.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#if __APPLE__
    #include <SDL/SDL.h>
    #include <SDL_mixer/SDL_mixer.h>
#else
    #include <SDL.h>
    #include <SDL_mixer.h>
#endif



/* Mix_Music actually holds the music information.  */
Mix_Music *music = NULL;

//void handleKey(SDL_KeyboardEvent key);
void musicDone();

    struct MusicInfo {
       int currentPosition;
       Mix_Music *music;
    };

    void musicLengthCallback(void *udata, Uint8 *stream, int len)
    {
        MusicInfo *music = static_cast<MusicInfo *>(udata);
        music->currentPosition += len;
        //std::cout << music->currentPosition / 22050.0 / 4.0 << std::endl;
    }



int StartAudio(void) {

  //SDL_Surface *screen;
  //SDL_Event event;
  //int done = 0;

  /* We're going to be requesting certain things from our audio
     device, so we set them up beforehand */
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
  int audio_channels = 2;
  int audio_buffers = 1024; //4096;

  SDL_Init(SDL_INIT_AUDIO);
  //SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  /* This is where we open up our audio device.  Mix_OpenAudio takes
     as its parameters the audio format we'd /like/ to have. */
  if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
    printf("Unable to open audio!\n");
    exit(1);
  }

  /* If we actually care about what we got, we can ask here.  In this
     program we don't, but I'm showing the function call here anyway
     in case we'd want to know later. */
  Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
  std::cout 
    << "Audio Channels: " << audio_channels  << std::endl
    << "Audio rate: " << audio_rate << std::endl
    ;

#if 0
  /* We're going to be using a window onscreen to register keypresses
     in.  We don't really care what it has in it, since we're not
     doing graphics, so we'll just throw something up there. */
  screen = SDL_SetVideoMode(320, 240, 0, 0);

  while(!done) {
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_QUIT:
	done = 1;
	break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
	handleKey(event.key);
	break;
      }
    }

    /* So we don't hog the CPU */
    SDL_Delay(50);

  }
#endif
        /* Setup an "effect" here so that we can monitor the current playback time
         */
        //int channel = 3;
        MusicInfo* info = new MusicInfo;
        info->currentPosition = 0;
        Mix_SetPostMix(&musicLengthCallback, info);
        /*) {
            fprintf(stderr, "Mix_RegisterEffect: No such channel! (channel = %d)", channel);
            exit(1);
        }*/

        /* Actually loads up the music */
	music = Mix_LoadMUS("audio/moonlight-remix.ogg");
        if(!music) {
            fprintf(stderr, "Mix_LoadMUS(\"audio/moonlight.ogg\"): %s\n", Mix_GetError());
            exit(1);
            // this might be a critical error...
        }

	/* This begins playing the music - the first argument is a
	   pointer to Mix_Music structure, and the second is how many
	   times you want it to loop (use -1 for infinite, and 0 to
	   have it just play once) */
	Mix_PlayMusic(music, 0);

	/* We want to know when our music has stopped playing so we
	   can free it up and set 'music' back to NULL.  SDL_Mixer
	   provides us with a callback routine we can use to do
	   exactly that */
	Mix_HookMusicFinished(musicDone);

        std::cout << "Channels Playing: " << Mix_Playing(-1) << std::endl;

    return 0;

}

void StopAudio() {
    /* This is the cleaning up part */
    //Mix_UnregisterEffect(1, &musicLengthCallback);
    Mix_CloseAudio();
    SDL_Quit();
}

#if 0
void handleKey(SDL_KeyboardEvent key) {
  switch(key.keysym.sym) {
  case SDLK_m:
    if(key.state == SDL_PRESSED) {

      /* Here we're going to have the 'm' key toggle the music on and
	 off.  When it's on, it'll be loaded and 'music' will point to
	 something valid.  If it's off, music will be NULL. */

      if(music == NULL) {
	
	/* Actually loads up the music */
	music = Mix_LoadMUS("music.ogg");

	/* This begins playing the music - the first argument is a
	   pointer to Mix_Music structure, and the second is how many
	   times you want it to loop (use -1 for infinite, and 0 to
	   have it just play once) */
	Mix_PlayMusic(music, 0);

	/* We want to know when our music has stopped playing so we
	   can free it up and set 'music' back to NULL.  SDL_Mixer
	   provides us with a callback routine we can use to do
	   exactly that */
	Mix_HookMusicFinished(musicDone);
	
      } else {
	/* Stop the music from playing */
	Mix_HaltMusic();

	/* Unload the music from memory, since we don't need it
	   anymore */
	Mix_FreeMusic(music);
	
	music = NULL;
      }
      break;
    }
  }
}
#endif

/* This is the function that we told SDL_Mixer to call when the music
   was finished. In our case, we're going to simply unload the music
   as though the player wanted it stopped.  In other applications, a
   different music file might be loaded and played. */
void musicDone() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
  music = NULL;
  exit(0);
}


int TimeToBeat(float seconds) 
{
    return seconds * BPS;
}

int GetBeat(float seconds, Pattern* pat)
{
    int beat = TimeToBeat(seconds);

    // avoid retriggering a single beat, but
    // if there are multiple queries during the *exact* same
    // time, assum that they are different clients who all
    // need to know the beat was triggered

    // XXX: if the client perterbs the time, this logic will fail

    if (pat->lastTimeQuery != seconds and
        pat->lastBeatQuery == beat)
    {
        return false;
    }

    pat->lastTimeQuery = seconds;
    pat->lastBeatQuery = beat;

    if (pat->beats[beat]) {
        return true;
    }

    return false; 
}


void StampPattern(int beatOffset, Pattern* pat)
{
    for(int i = 0; i < pat->length; i++) {
        if (pat->beats[i+beatOffset]) {
            std::cerr << "Warning: stamp overlay at " 
                      << beatOffset 
                      << " for " << pat->name
                      << std::endl;
        }
        pat->beats[i+beatOffset] = pat->exemplar[i];
    }
}

void StampPatternRange(int startBeat, int endBeat, Pattern* pat) 
{
    for (int i = startBeat; i <= endBeat; i += pat->length) {
        StampPattern(i, pat);
    }
}

/* static */
Audio* Audio::_audio(NULL);

Audio::Audio() :
    _kickPat(4, "kick"),
    _snarePat(4, "snare")
{
    _kickPat[0] = _kickPat[1] = true;
    StampPatternRange(0, 4, &_kickPat);
}


void 
Audio::Update(float worldTime)
{

}

int 
Audio::GetKicks()  
{ 
    return GetBeat(_curTime, &_kickPat); 
}

int 
Audio::GetSnares() 
{ 
    return GetBeat(_curTime, &_snarePat);
}


void
Audio::Test() 
{
    for (float i = 0; i < 16; i++) {
        float seconds = i* (.5/BPS);
        int beat = TimeToBeat(seconds);
        std::cout << "beat: " << beat;
        std::cout << " -> " << GetBeat(seconds, &_kickPat);
        std::cout << " , " << GetBeat(seconds, &_kickPat);
        std::cout << std::endl;
    }
    exit(0);
}
