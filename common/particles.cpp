#include "particles.h"

#include "typedefs.h"


/* virtual */
void
ParticleSystem::Init() 
{
    for (int i = 0; i < numParticles; i++) {
        Particle* p = new Particle;
        Spawn(p);
        particles.push_back(p);
    }
}

/* virtual */
void
ParticleSystem::Draw() 
{
}

/* virtual */
void
ParticleSystem::Update() 
{
    FOR_EACH(p, particles) {
        Update(*p);
    }
}

/* virtual */
void
ParticleSystem::Spawn(Particle* part) 
{
}

/* virtual */
void
ParticleSystem::Update(Particle* part) 
{
}
