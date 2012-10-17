#pragma once

#include <string>

#include "drawable.h"
#include "timer.h"

//
// An Effect is an aggregation of drawables, it has a name and timer for
// debugging / profiling
//
class Effect : public Drawable {
    static bool _enableDebug;
public:

    Timer timer;
    std::string name;

    Effect();
    virtual ~Effect();
   
    virtual void Init();
    virtual void Update();
    virtual void Draw();

    // Global debug state
    static void EnableDebug() { _enableDebug = true; }
    static void DisableDebug() { _enableDebug = false; }
};
