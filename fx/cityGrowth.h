#pragma once

#include "common/effect.h"
#include "common/sketchPlayback.h"
#include "common/sketchScene.h"
#include "common/vao.h"
#include "glm/glm.hpp"

struct CityElement {
    glm::vec3 Position;
    float Radius;
    float Height;
    int NumSides;
    sketch::CoplanarPath* RoofPath;
    float RoofBegin;
    float RoofEnd;
    sketch::Scene* CpuShape;
    sketch::Tessellator* CpuTriangles;
    Vao GpuTriangles;
    float ViewingAngleRadians;
};

typedef std::vector<CityElement> CityElements;

class CityGrowth : public Effect {
public:
    CityGrowth();
    virtual ~CityGrowth();
    virtual void Init();
    virtual void Update();
    virtual void Draw();
protected:
    void _UpdateGrowth(float elapsedTime);
    void _UpdateFlight(float elapsedTime);
private:
    bool _Collides(const CityElement& e) const;
    CityElements _elements;
    sketch::Tessellator* _tess;
    sketch::Playback* _player;
    Vao _terrainVao;

    enum StateMachine {
        GROWTH,
        FLIGHT,
        DEBUG,
    };
    StateMachine _state;
    float _stateStartTime;

    size_t _currentBuildingIndex;
};
