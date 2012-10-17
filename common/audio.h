#pragma once

#include <bitset>
#include <vector>

//
// Static audio controller methods which pass commands to SDL_mixer
//
void StartAudio(void);
void StopAudio();
void SetAudioPosition(float seconds);

//
// Some globals describing the Beats per Minute and teh Beats per Second useful
// if client code wants to sync to the music but doesn't necessarily want to
// track a specific signal
//
const int BPM = 280;
const float BPS = BPM / 60.0f;

//
// A simple struct for building up musical signals, this emulates what has been
// autored in the music sequencer so the patterns can be transfered directly
//
struct Pattern 
{
    Pattern(int len, std::string name) : length(len), name(name) {}

    // The pattern flattened into one bit per beat in the song if the pattern
    // is present on the given beat, the bit is on if the pattern is not
    // present, the bit is off the number of entries is equal to BPM *
    // songLengthInSeconds
    std::bitset<1024> beats;

    // The unique pattern is actually only ~16 beats, this is the pattern that
    // is stamped into the beats bitset above each time the pattern is played
    std::bitset<64> exemplar;

    // two bits of state to avoid triggering the same beat multiple times it's
    // legit to return the same beat multiple times for the same time query,
    // but it's not OK to return the same beat for different times
    int lastBeatQuery;
    float lastTimeQuery;

    // the number of actual beats used in the pattern bitset
    int length;

    // a name, for debugging
    std::string name;

    // helper to make authoring less verbose 
    std::bitset<64>::reference operator[](int idx) {
        return exemplar[idx];
    }
};


//
// Helper methods used when building up patterns / signals, not needed by
// external clients
//
float TimeToBeat(float seconds);
int GetBeat(int beat, Pattern* pat);
void StampPattern(int beatOffset, Pattern* pat);


// A class providing audio signals for consumers who want to react to the music
class Audio 
{
    static Audio* _audio;

    Pattern _kickPat;
    Pattern _snarePat;
    Pattern _hihatPat;

    float _curTime;

    Audio();

public:
    
    static Audio& Get() 
    { 
        if (not _audio) {
            _audio = new Audio();
        }
        return *_audio;
    }

    void Test();

    void Update(float deltaSeconds);

    // 
    // Discrete Event Accessors
    //   These methods return the number of events that occured since the last
    //   frame to the current frame.
    // 
    bool GetKicks();
    bool GetSnares();
    bool GetHiHats();
    

    //
    // Continuous Level Accessors
    //   These methods return the 'amount' of the queried quantity that is
    //   currently happening.
    //
    float GetWind() { return 0.0f; };
};



