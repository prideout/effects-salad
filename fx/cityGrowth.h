#pragma once

#include "common/effect.h"
#include "common/sketchPlayback.h"
#include "common/sketchScene.h"
#include "common/vao.h"
#include "common/camera.h"
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
    float ViewingAngle;
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
    bool _Collides(const CityElement& e) const;
    PerspCamera _InitialCamera();
private:
    CityElements _elements;
    sketch::Tessellator* _tess;
    sketch::Playback* _player;
    Vao _terrainVao;

    enum StateMachine {
        ENTER,
        GROWTH,
        FLIGHT,
        DEBUG,
        EXIT,
    };
    StateMachine _state;
    float _stateStartTime;

    size_t _currentBuildingIndex;
    PerspCamera _camera;
    PerspCamera _previousCamera;
};
