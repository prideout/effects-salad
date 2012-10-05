#pragma once

#include <vector>

#include "drawable.h"
#include "glm/glm.hpp"

using glm::vec3;
using std::vector;

struct Particle {
    vec3 pos;
    float size;
    float ttl;
    float bornAt;
};


class ParticleSystem : public Drawable {
public:
    
    // this is to be set before calling init
    // and will cuase particles to be filled with that number
    // of particle instances
    int numParticles;

    // populated in init
    vector<Particle*> particles;

    ParticleSystem() : Drawable() {}
    
    virtual void Init();
    virtual void Update();
    virtual void Draw();

    // inherited classes can override these methods to control
    // particle creation and behavior
    virtual void Spawn(Particle* part);
    virtual void Update(Particle* part);
};
