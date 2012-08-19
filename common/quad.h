#pragma once

#include "drawable.h"
#include "mesh.h"

class Quad : public Drawable {
    FloatList floats;
public:
    Mesh mesh;

    Quad();

    virtual void Init();
    virtual void Draw();

    // Update() is not used

}; 

