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
    GridAnim Roof;
    Vao GpuTriangles;
    bool Visible;
    int BuildingId;
    GridAnim* Ridges[4];
};

typedef std::vector<GridCell> GridCells;
typedef std::vector<GridAnim*> GridAnims;

struct GridRidges {
    GridAnims Anims;
    sketch::Scene* Shape;
    sketch::Tessellator* CpuTriangles;
    Vao GpuTriangles;
};

class GridCity : public Effect {
public:
    
    bool centerVines;
    bool outerVines;
    bool trackBeat;
    bool diveCamera;
    bool centerpiece;
    bool pingpong;

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
    Tube* _CreateVine(float xmix, float zmix, float dirFactor, bool facingX,
                        float radius=2, float lenght=20);
    Tube* _CreateCenterVine(float xmix, float zmix, float radius=2, float lenght=50);
    float _GetHeight(vec3 p0);
    void _CreateCenterpiece();

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

    Vao _centerpieceVao;
    sketch::Scene* _centerpieceSketch;
    sketch::Scene* _historicalSketch;
    sketch::Tessellator* _centerpieceTess;
    sketch::Playback* _centerpiecePlayer;
    sketch::PathList _columns;
    sketch::PathList _hangingThings;
    vec3 _columnCenter;
    float _previousBump;
    bool _backwards;
};
