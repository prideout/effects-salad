#pragma once

#include <bitset>
#include <vector>

int StartAudio(void);
void StopAudio();

const int BPM = 140;
const float BPS = BPM / 60.0f;

struct Pattern 
{
    Pattern(int len, std::string name) : length(len), name(name) {}

    // The pattern flattened into one bit per beat in the song
    // if the pattern is present on the given beat, the bit is on
    // if the pattern is not present, the bit is off
    // the number of entries is equal to BPM * songLengthInSeconds
    std::bitset<1024> beats;

    // The unique pattern is actually only ~16 beats, this is the
    // pattern that is stamped into the beats bitset above each time
    // the pattern is played
    std::bitset<64> exemplar;

    // two bits of state to avoid triggering the same beat multiple times
    // it's legit to return the same beat multiple times for the same 
    // time query, but it's not OK to return the same beat for different
    // times
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

int TimeToBeat(float seconds);
int GetBeat(int beat, Pattern* pat);
void StampPattern(int beatOffset, Pattern* pat);


// A class providing audio signals for consumers who want to react to the music
class Audio 
{
    static Audio* _audio;

    Pattern _kickPat;
    Pattern _snarePat;

    float _curTime;

    Audio();

public:
    

    static Audio& GetAudio() 
    { 
        if (not _audio) {
            _audio = new Audio();
        }
        return *_audio;
    }

    void Test();

    void Update(float worldTime);

    // 
    // Discrete Event Accessors
    //   These methods return the number of events that occured since the last
    //   frame to the current frame.
    // 
    int GetKicks();
    int GetSnares();
    

    //
    // Continuous Level Accessors
    //   These methods return the 'amount' of the queried quantity that is
    //   currently happening.
    //
    float GetWind() { return 0.0f; };
};



