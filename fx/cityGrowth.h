#pragma once

#include "common/effect.h"
#include "common/sketchPlayback.h"
#include "common/sketchScene.h"
#include "common/vao.h"
#include "common/camera.h"
#include "glm/glm.hpp"

struct AnimElement {
    sketch::CoplanarPath* Path;
    float BeginW;
    float EndW;
};

struct RectElement {
    glm::vec2 Size;
    glm::vec2 Offset;
    AnimElement SideWall;
    AnimElement SideWallRoof;
};

struct CityElement {
    glm::vec3 Position;
    float Radius;
    float Height;
    int NumSides;
    AnimElement Roof;
    sketch::Scene* CpuShape;
    sketch::Tessellator* CpuTriangles;
    Vao GpuTriangles;
    float ViewingAngle;
    RectElement Rect;
    bool HasWindows;
    sketch::PathList WindowFrames;
    sketch::PathList Windows;
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
