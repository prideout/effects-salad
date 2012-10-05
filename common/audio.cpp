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



/* Mix_Music holds the music information.  */
Mix_Music *music = NULL;

float AudioPlaybackTime = 0;

void MusicDone();


void musicLengthCallback(void *udata, Uint8 *stream, int len)
{
    AudioPlaybackTime += len / 22050.0 / 4.0;
}

void StartAudio(void) 
{
    // 
    // Setup audio format, rate, channels and buffers.
    // Setting a larger number of buffers reduces CPU time, but also means 
    // our AudioPlaybackTime is less accurate
    //
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
    int audio_channels = 2;
    int audio_buffers = 1024; //4096;

    SDL_Init(SDL_INIT_AUDIO);

    //
    // Open the audio device.  Mix_OpenAudio takes as its parameters the audio
    // format we'd *like* to have.
    //
    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
        std::cerr << "Unable to open audio!\n" << std::endl;
        exit(1);
    }

    // 
    // These are the audio settings we actually got, which may differ from the
    // requested paramaters set above
    //
    Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
    std::cout << "Audio Channels: " << audio_channels  << std::endl
              << "Audio rate: " << audio_rate << std::endl ;

    //
    // Setup an "effect" here so that we can monitor the current playback time
    //
    Mix_SetPostMix(&musicLengthCallback, NULL);
    
    // 
    // Load the music from source file
    //
    music = Mix_LoadMUS("audio/moonlight-remix.ogg");
    if(!music) {
        fprintf(stderr, "Mix_LoadMUS(\"audio/moonlight.ogg\"): %s\n", Mix_GetError());
        exit(1);
    }

    // 
    // This begins playing the music - the second argument is how many times
    // you want it to loop (-1 for infinite, and 0 to play once)
    //
    Mix_PlayMusic(music, 0);

    // 
    // Setup a callback to kill the demo when the music finishes playing 
    //
    Mix_HookMusicFinished(MusicDone);
}

void StopAudio() 
{
    /* This is the cleaning up part */
    //Mix_UnregisterEffect(1, &musicLengthCallback);
    Mix_CloseAudio();
    SDL_Quit();
}

void SetAudioPosition(float seconds) 
{
    if(Mix_SetMusicPosition(seconds)==-1) {
        printf("Mix_SetMusicPosition: %s\n", Mix_GetError());
    }
    AudioPlaybackTime = seconds;
    Audio::Get().Update(seconds);
}


//
// SDL_mixer will call this when the music stops, we terminate the demo here
//
void MusicDone() 
{
  Mix_HaltMusic();
  Mix_FreeMusic(music);
  music = NULL;
  exit(0);
}


float TimeToBeat(float seconds) 
{
    return seconds * BPS;
}

int GetBeat(float seconds, Pattern* pat)
{
    float beatf = TimeToBeat(seconds);
    int beat = int(beatf); // + .5);
    //float delta = beat - int(beatf);
    //if (delta > .1)
    //    return false;

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
        //std::cout << "BEAT AT (" << pat->name << "): " << seconds << " -- " << (beat - int(beat)) << std::endl;
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
    _kickPat(8, "kick"),
    _snarePat(8, "snare"),
    _hihatPat(4, "hihat"), 
    _curTime(0)
{
    _kickPat[0] = true;
    StampPatternRange(20*4, 138*4, &_kickPat);
    _snarePat[4] = true;
    StampPatternRange(20*4, 138*4, &_snarePat);
    _hihatPat[0] = _hihatPat[1] = _hihatPat[2] = _hihatPat[3] = true;
    StampPatternRange(20*4, 135*4, &_hihatPat);
}


void 
Audio::Update(float deltaSeconds)
{
    // world time is too sloppy, use 
    //_curTime += deltaSeconds;
    _curTime = AudioPlaybackTime;

    //std::cout << _curTime - AudioPlaybackTime << std::endl;
}

bool 
Audio::GetKicks()  
{ 
    return GetBeat(_curTime, &_kickPat); 
}

bool
Audio::GetSnares() 
{ 
    return GetBeat(_curTime, &_snarePat);
}

bool
Audio::GetHiHats()
{
    return GetBeat(_curTime, &_hihatPat);
}


void
Audio::Test() 
{
    for (float i = 0; i < 16; i++) {
        float seconds = i* (.5/BPS);
        float beat = TimeToBeat(seconds);
        std::cout << "beat: " << beat;
        std::cout << " -> " << GetBeat(seconds, &_kickPat);
        std::cout << " , " << GetBeat(seconds, &_kickPat);
        std::cout << std::endl;
    }
    exit(0);
}
