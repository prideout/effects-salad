#pragma once

#include "demoContext.h"
#include "pez/pez.h"
#include "glm/glm.hpp"


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


