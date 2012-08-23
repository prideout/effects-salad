#pragma once

#include <string>

#include "drawable.h"
#include "timer.h"

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
