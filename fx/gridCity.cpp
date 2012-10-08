
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "common/init.h"
#include "fx/gridCity.h"
#include "common/terrainUtil.h"
#include "common/programs.h"
#include "common/demoContext.h"
#include "common/sketchScene.h"
#include "common/sketchTess.h"
#include "glm/gtx/constants.inl"
#include "tween/CppTweener.h"

using namespace std;
using namespace glm;

static const int TerrainSize = 150;
static const float TerrainScale = 0.5;
static const float MinHeight = 5;
static const float MaxHeight = 15;
static const int NumRows = 10;
static const int NumCols = 32;
static const vec2 CellScale = vec2(0.9f, 0.7f);

// Params: int octaves, float freq, float amp, int seed
static Perlin HeightNoise(2, .5, 1, 3);

extern vec3 MyTerrainFunc(vec2 v);

GridCity::GridCity()
{
}

GridCity::~GridCity()
{
}

vec2 GridCity::_CellSample(int row, int col)
{
    float s = TerrainSize;
    float x = float(col) / NumCols;
    float y = float(row) / NumRows;
    return vec2(-s/2 + x*s, -s/2 + y*s);
}

void GridCity::Init()
{
    // Tessellate the ground
    FloatList ground;
    FloatList normals;
    IndexList indices;
    TerrainUtil::Smooth(
        TerrainSize, MyTerrainFunc,
        &ground, &normals, &indices);
    _terrainVao = Vao(3, ground, indices);
    _terrainVao.AddVertexAttribute(AttrNormal, 3, normals);

    // Form the grid
    _cells.reserve(NumRows * NumCols);
    GridCell cell;
    for (int row = 0; row < NumRows; ++row) {
        for (int col = 0; col < NumCols; ++col) {
            vec2 nw = _CellSample(row, col);
            vec2 sw = _CellSample(row+1, col);
            vec2 ne = _CellSample(row, col+1);
            vec2 se = _CellSample(row+1, col+1);
            vec2 n = (nw + ne) / 2.0f;
            vec2 s = (sw + se) / 2.0f;
            vec2 w = (nw + sw) / 2.0f;
            vec2 e = (se + ne) / 2.0f;
            vec2 p1 = (n + s) / 2.0f;
            vec2 p2 = (e + w) / 2.0f;
            vec2 p = (p1 + p2) / 2.0f;
            vec2 u = CellScale.x * (e - p);
            vec2 v = CellScale.y * (s - p);
            float h = std::max(0.0f, HeightNoise.Get(p.x,p.y) + 0.5f);
            cell.Quad.p = vec3(p.x, 0, p.y);
            cell.Quad.u = vec3(u.x, 0, u.y);
            cell.Quad.v = vec3(v.x, 0, v.y);
            cell.Height = MinHeight + h * (MaxHeight - MinHeight);
            _cells.push_back(cell);
        }
    }

    // "Unfloat" the quads
    FOR_EACH(i, _cells) {
        GridCell& cell = *i;
        cell.Visible = true;

        vec3 p0 = cell.Quad.p;
        vec2 p = vec2(p0.x, p0.z);
        vec2 coord = p / float(TerrainSize);
        vec2 domain = (coord + vec2(0.5)) * float(TerrainSize);
        cell.Quad.p.y = MyTerrainFunc(domain).y;

        vec3 p1 = cell.Quad.p + cell.Quad.u;
        p = vec2(p1.x, p1.z);
        coord = p / float(TerrainSize);
        domain = (coord + vec2(0.5)) * float(TerrainSize);
        p1.y = MyTerrainFunc(domain).y;
        cell.Quad.u = length(cell.Quad.u) * normalize(p1 - cell.Quad.p);

        vec3 p2 = cell.Quad.p + cell.Quad.v;
        p = vec2(p1.x, p1.z);
        coord = p / float(TerrainSize);
        domain = (coord + vec2(0.5)) * float(TerrainSize);
        cell.Quad.v = length(cell.Quad.v) * normalize(p2 - cell.Quad.p);
    }

    // Seed the sketch objects
    FOR_EACH(i, _cells) {
        GridCell& cell = *i;
        _AllocCell(&cell);
        cell.CpuTriangles->PullFromScene();
        cell.CpuTriangles->PushToGpu(cell.GpuTriangles);
        _FreeCell(&cell);
    }

    // Compile shaders
    Programs& progs = Programs::GetInstance();
    progs.Load("Buildings.Terrain", false);
    progs.Load("Sketch.Facets", true);

    // Set up camera
    _camera.far = 1000;
    _camera.up = vec3(0, 1, 0);
    _camera.center = vec3(0, 0, 0);;
    _camera.eye = vec3(0, 200, 20);
}

void GridCity::_AllocCell(GridCell* cell)
{
    sketch::Scene* shape = new sketch::Scene;
    cell->Roof = shape->AddQuad(cell->Quad);
    vec3 n = cell->Roof->Plane->GetNormal();
    float flip = dot(n, vec3(0, 1, 0)) < 0 ? -1 : 1;
    shape->PushPath(cell->Roof, flip * cell->Height);
    cell->CpuShape = shape;
    cell->CpuTriangles = new sketch::Tessellator(*shape);
}

void GridCity::_FreeCell(GridCell* cell)
{
    delete cell->CpuShape;
    delete cell->CpuTriangles;
    cell->CpuShape = 0;
    cell->CpuTriangles = 0;
}

void GridCity::Update()
{
    bool beat = GetContext()->audio->GetKicks() ||
        GetContext()->audio->GetSnares();
    bool halfBeat = _beats.Update(beat, 2.0f);
    if (halfBeat) {
    }
}

void GridCity::Draw()
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    Programs& progs = Programs::GetInstance();

    // Draw terrain
    glEnable(GL_CULL_FACE);
    glUseProgram(progs["Buildings.Terrain"]);
    _camera.Bind(glm::mat4());
    _terrainVao.Bind();
    glDrawElements(GL_TRIANGLES, _terrainVao.indexCount, GL_UNSIGNED_INT, 0);

    // Draw buildings
    glDisable(GL_CULL_FACE);
    glUseProgram(progs["Sketch.Facets"]);
    glUniform3f(u("Scale"), 1, 1, 1);
    glUniform3f(u("Translate"), 0, 0, 0);
    glUniform1i(u("Smooth"), 0);
    _camera.Bind(glm::mat4());
    FOR_EACH(cell, _cells) {
        if (not cell->Visible) {
            continue;
        }
        cell->GpuTriangles.Bind();
        glDrawElements(GL_TRIANGLES, cell->GpuTriangles.indexCount, GL_UNSIGNED_INT, 0);
    }
}
