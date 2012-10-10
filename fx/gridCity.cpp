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
static const float MinHeight = 20;
static const float MaxHeight = 40;
static const int NumRows = 10;
static const int NumCols = 20;
static const vec2 CellScale = vec2(0.9f, 0.7f);
static const float PopDuration = 1.0f;
static const float GrowthRate = 0.05f; // lower is faster

// Params: int octaves, float freq, float amp, int seed
static Perlin HeightNoise(2, .5, 1, 3);
static Perlin PerturbNoiseY(2, .5, 1, 5);
static Perlin TerrainNoise(2, .1, 2, 0);

// Terraces are orthogonal regions within a cell
struct Terrace {
    vec2 center;
    vec2 size;
    float height;
};

// Typically we want big terraces to come first
bool operator<(const Terrace& a, const Terrace& b)
{
    float area0 = a.size.x * a.size.y;
    float area1 = b.size.x * b.size.y;
    return area0 > area1;
}

static vec3
GridTerrainFunc(vec2 v)
{
    float tx = v.x * TerrainScale;
    float tz = v.y * TerrainScale;
    float y = TerrainNoise.Get(tx, tz) + 20.0 * TerrainNoise.Get(tx/5.0, tz/5.0);
    y *= 0.3;
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

            // Shrink buildings in the center:
            float h2 = length(p) * 0.01;
            h2 = h2*h2;
            h2 = std::min(h2, 3.0f);
            h *= h2;
            if (length(p) < 35) {
                h = -0.9;
            }
            
            // We now generate "terraces" which are orthogonal sub-cells
            // within each coplanar cell that are all adjacent to each other.
            // For example, here's a cell that's divided into 3 terraces:
            //
            //     +-----+-------+
            //     |     |       |
            //     |     |----+--+
            //     |     |    |
            //     +-----+    |
            //           +----+
            //
            float highTerraceHeight = MinHeight + h * (MaxHeight - MinHeight);
            float lowTerraceHeight = highTerraceHeight * 0.75;
            int numTerraces = 2 + (rand() % 3);
            int numLowTerraces = rand() % 3;
            float maxWidth = length(u) * 1.5;
            float minWidth = maxWidth * 0.25;
            float maxHeight = length(v) * 1.5;
            float minHeight = maxHeight * 0.25;
            vector<Terrace> terraces;
            terraces.resize(numTerraces);

            // Assign random width/heights; big terraces come first
            FOR_EACH(terrace, terraces) {
                terrace->size.x = minWidth + (maxWidth - minWidth) * float(rand()) / RAND_MAX;
                terrace->size.y = minHeight + (maxHeight - minHeight) * float(rand()) / RAND_MAX;
            }
            std::sort(terraces.begin(), terraces.end());

            // The largest terrace is stamped to (0,0)
            // Every other terrace is glued to one of its sides
            for (int terraceIndex = 0; terraceIndex < numTerraces; terraceIndex++) {
                Terrace& terrace = terraces[terraceIndex];
                terrace.height = highTerraceHeight;
                if (terraceIndex == 0) {
                    terrace.center = vec2(0, 0);
                    continue;
                }
                if (terraceIndex < numLowTerraces) {
                    terrace.height = lowTerraceHeight;
                }

                // Glue it to one of the sides of the "main" terrace
                int side = rand() % 4;
                float offset = float(rand()) / RAND_MAX - 0.5f;
                Terrace& main = terraces[0];
                switch (side) {
                case 0: // Glue to north wall
                    terrace.center.x = offset*main.size.x;
                    terrace.center.y = main.size.y/2 + terrace.size.y/2;
                    break;
                case 1: // Glue to south wall
                    terrace.center.x = offset*main.size.x;
                    terrace.center.y = -main.size.y/2 - terrace.size.y/2;
                    break;
                case 2: // Glue to west wall
                    terrace.center.x = -main.size.x/2 - terrace.size.x/2;
                    terrace.center.y = offset*main.size.x;
                    break;
                case 3: // Glue to east wall
                    terrace.center.x = main.size.x/2 + terrace.size.x/2;
                    terrace.center.y = offset*main.size.x;
                    break;
                }

                // TODO if it overlaps with an existing terrace, make a second attempt
                // by doing a terraceIndex--
            }

            // Translate the entire terrace group to center; reject any terrace
            // that overflows the cell border.  For each terrace, add a quad
            // to 'cells'

            // Diagnostics
            if (false) {
                FOR_EACH(terrace, terraces) {
                    float area = terrace->size.x * terrace->size.y;
                    std::cout << area << ' ' ;
                }
                std::cout << std::endl;
            }

            // Backwards compat
            if (true) {
                cell.Quad.p = vec3(p.x, 0, p.y);
                cell.Quad.u = vec3(u.x, 0, u.y);
                cell.Quad.v = vec3(v.x, 0, v.y);
                cell.Height = MinHeight + h * (MaxHeight - MinHeight);
            }

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
            float d = length(v) * GrowthRate;
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
    _camera.center = vec3(-10, 0, 0);
    _camera.eye = vec3(-150-10, 350, 350);
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
