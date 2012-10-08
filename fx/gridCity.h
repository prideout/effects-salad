#pragma once

#include "common/effect.h"
#include "common/sketchPlayback.h"
#include "common/sketchScene.h"
#include "common/vao.h"
#include "common/camera.h"
#include "glm/glm.hpp"

// Orientable rectangle in 3-space define by a point and two vectors:
// p ... center point
// u ... half-width vector
// v ... half-height vector
struct GridQuad {
    glm::vec3 p;
    glm::vec3 u;
    glm::vec3 v;
};

struct GridCell {
    GridQuad Quad;
    float Height;
    sketch::Scene* CpuShape;
    sketch::Tessellator* CpuTriangles;
    sketch::CoplanarPath* Roof;
    Vao GpuTriangles;
    bool Visible;
 };

typedef std::vector<GridCell> GridCells;

struct HalfBeat {
    HalfBeat();
    bool Update(bool beat, float time);
    float _previousBeatTime;
    float _currentBeatInterval;
    bool _downBeat;
    float _multiplier;
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
    HalfBeat _beats;
    GridCells _cells;
    Vao _terrainVao;
    PerspCamera _camera;
};
