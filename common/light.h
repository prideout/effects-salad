#pragma once

#include "drawable.h"

class Light;
typedef std::vector<Light*> LightList;

class Light : public Drawable {
public:
    
    // could make this more general and use a DrawableList
    Drawable* geometry;
    
    Light();

    virtual void Init();
    virtual void Update();
    virtual void Draw();
};
