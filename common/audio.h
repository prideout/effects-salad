#pragma once

int StartAudio(void);
void StopAudio();

// A class providing audio signals for consumers who want to react to the music
class Audio {
    static Audio* _audio;

    Audio() {

    }

public:
    

    static Audio& GetAudio() { 
        if (not _audio) {
            _audio = new Audio();
        }
        return *_audio;
    }

    void Update(float worldTime) {};

    // 
    // Discrete Event Accessors
    //   These methods return the number of events that occured since the last
    //   frame to the current frame.
    // 
    int GetKicks()  { return 0; };
    int GetSnares() { return 0; };
    

    //
    // Continuous Level Accessors
    //   These methods return the 'amount' of the queried quantity that is
    //   currently happening.
    //
    float GetWind() { return 0.0f; };
};

