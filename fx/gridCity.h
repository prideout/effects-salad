#pragma once

#include "common/effect.h"
#include "common/sketchPlayback.h"
#include "common/sketchScene.h"
#include "common/vao.h"
#include "common/camera.h"
#include "common/halfBeat.h"
#include "glm/glm.hpp"

struct GridCell {
    sketch::Quad Quad;
    float Height;
    sketch::Scene* CpuShape;
    sketch::Tessellator* CpuTriangles;
    sketch::CoplanarPath* Roof;
    Vao GpuTriangles;
    bool Visible;
 };

typedef std::vector<GridCell> GridCells;

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
