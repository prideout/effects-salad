#pragma once

#include "common/effect.h"
#include "common/sketchPlayback.h"
#include "common/vao.h"
#include "glm/glm.hpp"

struct CityElement {
    glm::vec3 Position;
    float Radius;
    int NumSides;
    sketch::Scene* CpuShape;
    sketch::Tessellator* CpuTriangles;
    Vao GpuTriangles;
};

typedef std::vector<CityElement> CityElements;

class CityGrowth : public Effect {
public:
    CityGrowth();
    virtual ~CityGrowth();
    virtual void Init();
    virtual void Update();
    virtual void Draw();
private:
    bool _Collides(const CityElement& e) const;
    CityElements _elements;
    sketch::Tessellator* _tess;
    sketch::Playback* _player;
    Vao _terrainVao;
};
