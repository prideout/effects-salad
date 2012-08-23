#pragma once

#include "drawable.h"
#include "vao.h"

class Quad : public Drawable {
    FloatList floats;
public:
    Vao vao;

    Quad();

    virtual void Init();
    virtual void Draw();

    // Update() is not used

}; 

