#pragma once

#include "pez/pez.h"
#include "drawable.h"
#include "vao.h"
#include <vector>

using std::vector;

//
// Hardware instancing support
//
class Instancer : public Drawable {
public:
    
    unsigned primCount;
    Vao vao;

    Instancer();
    virtual ~Instancer();

    virtual void Init();
    virtual void Draw();
    virtual void Update();
};

