#include "audio.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include <SDL_mixer/SDL_mixer.h>

/* Mix_Music actually holds the music information.  */
Mix_Music *music = NULL;

void handleKey(SDL_KeyboardEvent key);
void musicDone();

int TestAudio(void) {

  SDL_Surface *screen;
  SDL_Event event;
  int done = 0;

  /* We're going to be requesting certain things from our audio
     device, so we set them up beforehand */
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
  int audio_channels = 2;
  int audio_buffers = 4096;

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

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

  /* This is the cleaning up part */
  Mix_CloseAudio();
  SDL_Quit();

}

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

/* This is the function that we told SDL_Mixer to call when the music
   was finished. In our case, we're going to simply unload the music
   as though the player wanted it stopped.  In other applications, a
   different music file might be loaded and played. */
void musicDone() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
  music = NULL;
}

