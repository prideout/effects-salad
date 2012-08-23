#pragma once

#include <string>
#include <ctime>
#include <time.h>

#include "drawable.h"

// Not a high resolution timer, but it should get the job done

using std::string;

class Timer : Drawable {
    clock_t _start;
    unsigned _frameCount;
    float _fps;
    float _seconds;

public:
    static const unsigned MAX_FRAMES = 120;
    bool drawText;
    string name;
    
    Timer() :
        Drawable(),
        _start(clock()),
        _fps(0),
        drawText(true)
    { 
        /* nothing */
    }

    float GetFPS() { return _fps; }
    float GetSeconds() { return _seconds; }

    void Reset() {
        _start = clock();
    }

    virtual void Init();
    virtual void Draw();
    virtual void Update();
};

