#pragma once

#include "pez/pez.h"
#include "drawable.h"
#include "mesh.h"
#include <vector>

using std::vector;

class Instancer : public Drawable {
public:
    
    unsigned primCount;
    Mesh mesh;

    Instancer();
    virtual ~Instancer();

    virtual void Init();
    virtual void Draw();
    virtual void Update();
};

