#pragma once

#include <vector>

#include "common/camera.h"
#include "common/effect.h"
#include "common/init.h"
#include "common/normalField.h"
#include "common/particles.h"
#include "common/quad.h"
#include "common/tube.h"
#include "common/treeGen.h"


// simple effect used to test framework features

class Tree : public Effect, private ParticleController {
    std::vector<Tube*> _branches;

    TreeSystem _treeSys;
    Vao _leaves;
    BufferTexture _leafData;
    BufferTexture _leafNormals;
    Vec3List _leafPoints;

    ParticleSystem _leafParticles;

    virtual void Spawn(Particle* part);
    virtual void Update(Particle* part, float age);

public:
    glm::vec3 pos;
    bool grown;

    Tree() : Effect(),
        pos(glm::vec3(-3,-2.25,2)),
        grown(false)
    { 
    }
   
    virtual void Init();

    virtual void Update();

    virtual void Draw();
    virtual void Draw(float time);
};

