// Start the buildings radially
// Ridges on rooves, inset windows
// Sandstorms
// Camera work

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

static const int TerrainRes = 300;
static const float TerrainArea = 300;
static const float TerrainScale = 0.5;
static const float MinHeight = 5;
static const float MaxHeight = 15;
static const int NumRows = 24;
static const int NumCols = 64;
static const vec2 CellScale = vec2(0.9f, 0.7f);
static const float PopDuration = 1.0f;

// Params: int octaves, float freq, float amp, int seed
static Perlin HeightNoise(2, .5, 1, 3);
static Perlin PerturbNoiseY(2, .5, 1, 5);
static Perlin TerrainNoise(2, .1, 2, 0);

static vec3
GridTerrainFunc(vec2 v)
{
    float tx = v.x * TerrainScale;
    float tz = v.y * TerrainScale;
    float y = TerrainNoise.Get(tx, tz) + 20.0 * TerrainNoise.Get(tx/5.0, tz/5.0);
    vec3 p = vec3(v.x, y, v.y);

    float s = TerrainArea;
    p.x *= (s / TerrainRes);
    p.z *= (s / TerrainRes);
    p += vec3(-s/2.0, 0, -s/2.0);

    return p;
}

GridCity::GridCity()
{
    _currentBeat = 0;
}

GridCity::~GridCity()
{
    FOR_EACH(i, _cells) {
        GridCell& cell = *i;
        _FreeCell(&cell);
    }
}

// Perturb the endpoints of each horizontal line
vec2 GridCity::_CellSample(int row, int col)
{
    float s = TerrainArea;
    float x = float(col) / NumCols;
    float y = float(row) / NumRows;
    if (col < 1 || col == NumCols-1) {
        y += PerturbNoiseY.Get(float(row),float(col)) * 0.05;
        return vec2(-s/2 + x*s, -s/2 + y*s);
    }

    vec2 a = _CellSample(row, 0);
    vec2 b = _CellSample(row, NumCols-1);
    vec2 p = mix(a, b, x);

    return vec2(-s/2 + x*s, p.y);
}

void GridCity::Init()
{
    // Tessellate the ground
    FloatList ground;
    FloatList normals;
    IndexList indices;
    TerrainUtil::Smooth(
        TerrainRes, GridTerrainFunc,
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

    // "Unfloat" the quads and orient them onto the terrain
    FOR_EACH(i, _cells) {
        GridCell& cell = *i;
        cell.Visible = true;

        vec3 p0 = cell.Quad.p;
        vec2 p = vec2(p0.x, p0.z);
        vec2 coord = p / float(TerrainArea);
        vec2 domain = (coord + vec2(0.5)) * float(TerrainArea);
        cell.Quad.p.y = GridTerrainFunc(domain).y;

        vec3 p1 = cell.Quad.p + cell.Quad.u;
        p = vec2(p1.x, p1.z);
        coord = p / float(TerrainArea);
        domain = (coord + vec2(0.5)) * float(TerrainArea);
        p1.y = GridTerrainFunc(domain).y;
        cell.Quad.u = length(cell.Quad.u) * normalize(p1 - cell.Quad.p);

        vec3 p2 = cell.Quad.p + cell.Quad.v;
        p = vec2(p1.x, p1.z);
        coord = p / float(TerrainArea);
        domain = (coord + vec2(0.5)) * float(TerrainArea);
        cell.Quad.v = length(cell.Quad.v) * normalize(p2 - cell.Quad.p);
    }

    // Seed the sketch objects
    {
        int row = 0;
        int col = 0;
        FOR_EACH(i, _cells) {
            GridCell& cell = *i;
            _AllocCell(&cell);
            vec2 v = vec2(cell.Quad.p.x, cell.Quad.p.z);
            float d = length(v) / 2.0;
            cell.Anim.StartBeat = (int) d;
            col++;
            if (col >= NumCols) {
                col = 0;
                row++;
            }
        }
    }

    // Compile shaders
    Programs& progs = Programs::GetInstance();
    progs.Load("Buildings.Terrain", false);
    progs.Load("Sketch.Facets", true);

    // Set up camera
    _camera.far = 1000;
    _camera.up = vec3(0, 1, 0);
    _camera.center = vec3(0, 0, 0);
    _camera.eye = vec3(0, 300, 20);
}

void GridCity::_AllocCell(GridCell* cell)
{
    sketch::Scene* shape = new sketch::Scene;
    cell->Roof = shape->AddQuad(cell->Quad);
    vec3 n = cell->Roof->Plane->GetNormal();
    float flip = dot(n, vec3(0, 1, 0)) < 0 ? -1 : 1;
    cell->Anim.BeginW = cell->Roof->Plane->Eqn.w;
    shape->PushPath(cell->Roof, flip * cell->Height);
    cell->Anim.EndW = cell->Roof->Plane->Eqn.w;
    cell->Anim.StartTime = 0;
    cell->CpuShape = shape;
    cell->CpuTriangles = new sketch::Tessellator(*shape);
    cell->CpuTriangles->PullFromScene();
    cell->CpuTriangles->PushToGpu(cell->GpuTriangles);
    shape->SetPathPlane(cell->Roof, cell->Anim.BeginW);
    cell->CpuTriangles->PullFromScene();
    cell->CpuTriangles->PushToGpu(cell->GpuTriangles);
    cell->Visible = false;
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
    float time = GetContext()->elapsedTime;
    if (GetContext()->audio->GetHiHats()) {
        _currentBeat++;
    }

    tween::Elastic tweener;
    FOR_EACH(i, _cells) {
        GridCell& cell = *i;
        if (!cell.CpuTriangles) {
            continue;
        }
        if (cell.Anim.StartTime == 0) {
            // Check if the building hasn't been born yet:
            if (cell.Anim.StartBeat > _currentBeat) {
                cell.Visible = false;
                continue;
            }
            // At this point we're starting a new pop animation
            cell.Anim.StartTime = time;
            cell.Visible = true;
        }
        if (time > cell.Anim.StartTime + PopDuration) {
            // At this point we're ending a pop animation
            cell.CpuShape->SetPathPlane(cell.Roof, cell.Anim.EndW);
            cell.CpuTriangles->PullFromScene();
            cell.CpuTriangles->PushToGpu(cell.GpuTriangles);
            _FreeCell(&cell);
            continue;
        }
        // Update an in-progress animation
        float w = tweener.easeOut(
            time - cell.Anim.StartTime,
            cell.Anim.BeginW,
            cell.Anim.EndW - cell.Anim.BeginW,
            PopDuration);
        cell.CpuShape->SetPathPlane(cell.Roof, w);
        cell.CpuTriangles->PullFromScene();
        cell.CpuTriangles->PushToGpu(cell.GpuTriangles);
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