#pragma once

#include "common/effect.h"
#include "common/sketchPlayback.h"
#include "common/sketchScene.h"
#include "common/vao.h"
#include "common/camera.h"
#include "common/halfBeat.h"
#include "common/tube.h"
#include "glm/glm.hpp"

struct GridAnim {
    float BeginW;
    float EndW;
    float StartTime;
    int StartBeat;
    sketch::CoplanarPath* Path;
};

struct GridCell {
    sketch::Quad Quad;
    float Height;
    sketch::Scene* Shape;
    sketch::Tessellator* CpuTriangles;
    GridAnim Anim;
    Vao GpuTriangles;
    bool Visible;
    int BuildingId;
};

typedef std::vector<GridCell> GridCells;
typedef std::vector<GridAnim> GridAnims;

struct GridRidges {
    GridAnims Anims;
    sketch::Scene* Shape;
    sketch::Tessellator* CpuTriangles;
    Vao GpuTriangles;
};

class GridCity : public Effect {
public:
    GridCity();
    virtual ~GridCity();
    virtual void Init();
    virtual void Update();
    virtual void Draw();
private:
    vec2 _CellSample(int row, int col);
    void _AllocCell(GridCell* cell);
    void _FreeCell(GridCell* cell);
    Vao _CreateCityWall();
    void _CreateVines();
    Tube* _CreateVine(float xmix, float zmix, float dirFactor, bool facingX, float radius=2, float lenght=10);
    float _GetHeight(vec3 p0);

    sketch::PathList
    _AddWindows(GridCell* cell, sketch::CoplanarPath* wall);

    vector<Tube*> _vines;

    HalfBeat _beats;
    GridCells _cells;
    Vao _terrainVao;
    PerspCamera _camera;
    int _currentBeat;
    Vao _cityWall;
    GridRidges _ridges;
};
