#pragma once

#include <vector>

#include "common/camera.h"
#include "common/effect.h"
#include "common/init.h"
#include "common/normalField.h"
#include "common/quad.h"
#include "common/tube.h"
#include "common/treeGen.h"


// simple effect used to test framework features

class Tree : public Effect {
    std::vector<Tube*> _branches;

    TreeSystem _treeSys;
    Vao _leaves;
    BufferTexture _leafData;

public:
    glm::vec3 pos;

    Tree() : Effect(),
        pos(glm::vec3(-3,-2.25,2))
    { 
    }
   
    virtual void Init();

    virtual void Update();

    virtual void Draw();
};

