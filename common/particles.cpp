#include "particles.h"
#include "demoContext.h"

#include "typedefs.h"


/* virtual */
void
ParticleSystem::Init() 
{
    deadList.reserve(numParticles);

    for (int i = 0; i < numParticles; i++) {
        Particle* p = new Particle;
        //p->bornOn = now;
        p->alive = false;
        //Spawn(p);
        deadList.push_back(i);
        particles.push_back(p);
    }

    Vec3List partsTemp;
    partsTemp.resize(numParticles);
    _particles = Vao(partsTemp);
    pezCheckGL("failed to initialize particle data");
}

/* virtual */
void
ParticleSystem::Draw() 
{
    // this is slow!
    Vec3List parts;
    parts.reserve(numParticles);
    FOR_EACH(pIt, particles) {
        Particle* p = *pIt;
        if (not p->alive) 
            continue;

        parts.push_back(p->pos);
    }
    if (parts.empty()) return;
    _particles.Bind();
    pezCheck(glGetError() == GL_NO_ERROR, "failed to bind particles");

    glBindBuffer(GL_ARRAY_BUFFER, _particles.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*parts.size(), &parts[0].x, GL_DYNAMIC_DRAW);
    pezCheckGL("failed to rebuffer particle data");

    glDrawArrays(GL_POINTS, 0, _particles.vertexCount);
    pezCheckGL("failed to draw particles");

}

/* virtual */
void
ParticleSystem::Update() 
{
    float now = GetContext()->elapsedTime;

    //
    // Update all the living particles
    //
    for (unsigned i = 0; i < particles.size(); i++) {
        Particle* p = particles[i];
        if (not p->alive)
            continue;

        Update(p, now - p->bornOn);

        if (now - p->bornOn > p->ttl) {
            p->alive = false;
            deadList.push_back(i);
        }
    }

    //
    // Main entry point for new particles 
    //
    float timeToSpawn = now - lastSpawned;
    float secPerSpawn = 1.0f / spawnPerSec;

    while (not deadList.empty() and timeToSpawn >= secPerSpawn) {
        timeToSpawn -= secPerSpawn;
        Particle* p = particles[deadList.back()];
        deadList.pop_back();

        p->bornOn = now;
        p->alive = true;
        Spawn(p);
        lastSpawned = now;
    }
    pezCheckGL("failed to update particles");
}

/* virtual */
void
ParticleSystem::Spawn(Particle* part) 
{
    // alive and bornOn are automatic, since they
    // are unlikely to be interesting to set here
    part->pos.x = -3;
    part->pos.y =  5;
    part->pos.z =  4;

    part->startPos.x = -3;
    part->startPos.y =  5;
    part->startPos.z =  4;

    part->ttl = 3.0;
}

/* virtual */
void
ParticleSystem::Update(Particle* part, float age) 
{
    // no need to kill particles here, just update their
    // living properties
    part->pos.x = part->startPos.x + age;
    part->pos.y = part->startPos.y - age;
}


void
ParticleSystem::Print() 
{
    std::cout   <<   "TIME: " << GetContext()->elapsedTime << std::endl;
    std::cout   <<   "ParticleSystem:" << std::endl;
    std::cout   <<   "  numParticles: " << numParticles << std::endl;
    std::cout   <<   "  lastSpawned: " << lastSpawned << std::endl;
    std::cout   <<   "  particles.size(): " << particles.size() << std::endl;
    std::cout   <<   "  deadList.size(): " << deadList.size() << std::endl;
    std::cout   <<   "  Particles:" << std::endl;
    int i = 0;
    FOR_EACH(pIt, particles) {
        Particle* p = *pIt; 
        std::cout << "   PART: " << i << std::endl;
        std::cout << "    pos:      " << p->pos.x << ", " << p->pos.y << ", " << p->pos.z << std::endl;
        std::cout << "    startPos: " << p->startPos.x << ", " << p->startPos.y << ", " << p->startPos.z << std::endl;
        std::cout << "    size:     "    << p->size << std::endl;
        std::cout << "    ttl:      "     << p->ttl << std::endl;
        std::cout << "    bornOn:   "  << p->bornOn << std::endl;
        std::cout << "    alive:    "   << p->alive << std::endl;
        std::cout << "    age:      "   << GetContext()->elapsedTime - p->bornOn << std::endl;
        i++;
    }

}

/* static */
void
ParticleSystem::Test()
{
    ParticleSystem sys;
    DemoContext::GetCurrent()->elapsedTime = 0.0;
    sys.numParticles = 2;
    sys.spawnPerSec = 1.0;
    sys.Init();
    sys.Print();

    DemoContext::GetCurrent()->elapsedTime = 1.0;
    sys.Update();
    sys.Print();

    DemoContext::GetCurrent()->elapsedTime = 2.0;
    sys.Update();
    sys.Print();
    
    DemoContext::GetCurrent()->elapsedTime = 3.0;
    sys.Update();
    sys.Print();
    
    DemoContext::GetCurrent()->elapsedTime = 4.0;
    sys.Update();
    sys.Print();
 
    DemoContext::GetCurrent()->elapsedTime = 5.0;
    sys.Update();
    sys.Print();
 
    exit(0);
}
