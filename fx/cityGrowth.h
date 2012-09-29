#pragma once

#include "common/effect.h"
#include "common/sketchPlayback.h"
#include "common/vao.h"
#include "glm/glm.hpp"

struct CityElement {
    glm::vec2 Position;
    float Radius;
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
    CityElements _elements;
    sketch::Scene* _sketch;
    sketch::Scene* _historicalSketch;
    sketch::Tessellator* _tess;
    sketch::Playback* _player;
    Vao _vao;
};
