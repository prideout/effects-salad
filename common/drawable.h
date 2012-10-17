#pragma once

#include <vector>

#include "pez/pez.h"
#include "glm/glm.hpp"

class DemoContext;

class Drawable;
typedef std::vector<Drawable*> DrawableList;

//
// Drawable is a base class that provides a basic facilities interface
// (Init,Draw,Update) and a built in model matrix.
//
// Drawable objects should be self-contained and should not aggregate other
// drawables
//
class Drawable {
public:

    bool visible;
    glm::mat4 modelMat;

    Drawable();

    virtual ~Drawable();
    
    virtual void Init();

    virtual void Draw();

    virtual void Update();

    DemoContext* GetContext();

};


