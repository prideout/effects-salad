#pragma once

#include <string>

#include "drawable.h"

class Effect : public Drawable {
public:

    std::string name;

    Effect();
    virtual ~Effect();
   
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
};
