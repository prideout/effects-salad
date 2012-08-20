#pragma once

#include "demoContext.h"
#include "pez/pez.h"
#include "glm/glm.hpp"

// specialized matrix ops
#include "glm/gtc/matrix_transform.hpp"


class Drawable {
public:

    const DemoContext* context;
    glm::mat4 modelMat;

    Drawable();

    virtual ~Drawable();
    
    virtual void Init();

    virtual void Draw();

    virtual void Update();


};


