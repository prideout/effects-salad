#pragma once

#include <vector>

#include "pez/pez.h"
#include "glm/glm.hpp"

class DemoContext;

class Drawable;
typedef std::vector<Drawable*> DrawableList;

class Drawable {
public:

    bool visible;
    const DemoContext* context;
    glm::mat4 modelMat;

    Drawable();

    virtual ~Drawable();
    
    virtual void Init();

    virtual void Draw();

    virtual void Update();


};


