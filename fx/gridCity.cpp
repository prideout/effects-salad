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

#include <algorithm>
#include <limits>

using namespace std;
using namespace glm;

static const int TerrainRes = 300;
static const float TerrainArea = 300;
static const float TerrainScale = 0.5;
static const float MaxHeight = 40;
static const vec2 CellScale = vec2(0.9f, 0.7f);
static const float PopDuration = 0.1f;
static const float GrowthRate = 0.3f; // lower is faster
static const float MinHeight = 20;
static const int NumRows = 10;
static const int NumCols = 20;
static const bool VisualizeCell = false;
static const bool PopBuildings = true;
static const bool HasWindows = false;

// Params: int octaves, float freq, float amp, int seed
static Perlin HeightNoise(2, .5, 1, 3);
static Perlin PerturbNoiseY(2, .5, 1, 5);
static Perlin TerrainNoise(2, .1, 2, 0);
static Perlin VineNoise(2, .1, 2, 0);

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
    vec3 p = vec3(v.x, y, v.y);
    float s = TerrainArea;
    p.x *= (s / TerrainRes);
    p.z *= (s / TerrainRes);
    p += vec3(-s/2.0, 0, -s/2.0);
    if (abs(p.x) < TerrainArea/2 && abs(p.z) < TerrainArea/2) {
        p.y *= 0.2;
    }
/*
    // Pathetic attempt to fix discontinuity:
    } else {
        float radius = 100.0f;
        float scale = 0.2 + 0.8 * std::min(radius, length(vec2(p.x, p.z))) / radius;
        p.y *= scale;
    }
*/
    return p;
}

GridCity::GridCity()
{
    centerVines = false;
    outerVines = true;
    trackBeat = false;
    diveCamera = false;
    _currentBeat = 0;
    centerpiece = false;
    pingpong = false;
    _backwards = false;
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

float
GridCity::_GetHeight(vec3 p0)
{
    vec2 p = vec2(p0.x, p0.z);
    vec2 coord = p / float(TerrainArea);
    vec2 domain = (coord + vec2(0.5)) * float(TerrainArea);
    return GridTerrainFunc(domain).y;
}

Tube* GridCity::_CreateCenterVine(float xmix, float zmix, float radius, float length)
{
    Tube* t = new Tube;
    Vec3List cvs;

    //
    // Build up CVs, make sure the total number equals 4 + 3n
    // or they won't draw correctly (cubic segments)
    //

    float curveAmp = 5;
    float curveLenght = length;
    float curvePeriod= 1; //10 / 7.0;

    float noise = TerrainNoise.Get(radius*2000*length+xmix, 2000*radius/length+zmix);
    float angle = (noise) * 2*3.1415f;
    //std::cout << (.5f + .5f*TerrainNoise.Get(2000*length+xmix, 2000*radius+zmix)) << std::endl;

    vec2 normal(cos(angle), sin(angle));
    vec2 center(cos(noise*20), sin(noise*20));

    float area = TerrainArea/2 - 2;
    vec2 min(-area, -area);
    vec2 max(area, area);

    //
    // Using cubic curves means they are easy to sample and spawn new curves
    //
    float numCvs = 4 + (3*10);
    for (int i = 0; i < numCvs; i++) {
        t->cvs.push_back(vec3(
            center.x+(normal.x * (i/4.0) * curveLenght) +
                                (curveAmp*sinf(noise*100+i*curvePeriod)), 
            1.0f, 
            center.y+(normal.y * (i/4.0) * curveLenght) +
                                (curveAmp*sinf(noise*100+i*curvePeriod))
             ));

        vec3& cv = t->cvs.back();

        // add some noise to the movement
        #if 0
        if (not facingX) 
            cv.x += curveAmp*TerrainNoise.Get(cv.x, cv.z);
        else
            cv.z += curveAmp*TerrainNoise.Get(cv.x, cv.z);
        #endif 

        // Fix up y value to match terrain
        cv.y = _GetHeight(cv) + t->radius*2;

        // force continuity between segments
        // this correction must be last, after applying the noise
        int cvCount = t->cvs.size();
        if (i > 3 and (i-4) % 3 == 0) {
            glm::vec3 dir = t->cvs[cvCount - 2] - 
                            t->cvs[cvCount - 3];
            cv = t->cvs[cvCount - 2] + dir;
        }

        if (i >= 0)
            cv.y += 20;
    }
    
    //
    // These values are consumed by Init, so set them first
    //
    vec3& cv = t->cvs.front();
    t->radius = radius;
    t->lod = 5;
    t->sidesPerSlice = 5;
    t->startTime = 10.0f + TerrainNoise.Get(cv.x, cv.z)*2.0f;
    t->timeToGrow = 15.0f + TerrainNoise.Get(cv.x, cv.z)*15.0f;

    //
    // Build sweep, build buffers, etc 
    // 
    t->Init();
    
    return t;
}


Tube* GridCity::_CreateVine(float xmix, float zmix, float dirFactor, bool facingX, float radius, float length)
{
    Tube* t = new Tube;
    Vec3List cvs;

    //
    // Build up CVs, make sure the total number equals 4 + 3n
    // or they won't draw correctly (cubic segments)
    //

    float curveAmp = 10;
    float curveLenght = length;
    float curvePeriod= 1; //10 / 7.0;

    float area = TerrainArea/2 - 2;
    vec2 min(-area, -area);
    vec2 max(area, area);


    //
    // Using cubic curves means they are easy to sample and spawn new curves
    //
    for (int i = 0; i < 4 + (3*4); i++) {
        if (facingX)
            t->cvs.push_back(vec3(
                                glm::mix(min.x, max.x, xmix) + dirFactor*i*curveLenght, 
                                1.0, 
                                (curveAmp*sin(i*curvePeriod)) + glm::mix(min.y, max.y, zmix)));

        else
            t->cvs.push_back(vec3(
                                (curveAmp*sin(i*curvePeriod)) + glm::mix(min.x, max.x, xmix), 
                                1.0, 
                                glm::mix(min.y, max.y, zmix) + dirFactor*i*curveLenght ));
        vec3& cv = t->cvs.back();




        // add some noise to the movement
        if (not facingX) 
            cv.x += curveAmp*TerrainNoise.Get(cv.x, cv.z);
        else
            cv.z += curveAmp*TerrainNoise.Get(cv.x, cv.z);

        // Fix up y value to match terrain
        cv.y = _GetHeight(cv) + t->radius*2;

        // force continuity between segments
        // this correction must be last, after applying the noise
        int cvCount = t->cvs.size();
        if (i > 3 and (i-4) % 3 == 0) {
            glm::vec3 dir = t->cvs[cvCount - 2] - 
                            t->cvs[cvCount - 3];
            cv = t->cvs[cvCount - 2] + dir;
        }

        //if (i >= 0)
        //    cv.y += 20;

    }
    
    //
    // These values are consumed by Init, so set them first
    //
    vec3& cv = t->cvs.front();
    t->radius = radius;
    t->lod = 5;
    t->sidesPerSlice = 5;
    t->startTime = 10.0f + TerrainNoise.Get(cv.x, cv.z)*5.0f;
    t->timeToGrow = 10.0f + TerrainNoise.Get(cv.x, cv.z)*5.0f;

    //
    // Build sweep, build buffers, etc 
    // 
    t->Init();
    
    return t;
}

void GridCity::_CreateVines() 
{
    bool edges = outerVines;
    bool centers = centerVines;
    if (edges) {
        float inc = .05;
        for (float a = 0; a < 1.0; a+= inc) {
            float radius = 2 + .5*TerrainNoise.Get(a, 0);
            Tube* t = _CreateVine(.0, a, -1, true, radius);
            _vines.push_back(t);
        }

        for (float a = 0; a < 1.0; a+= inc) {
            float radius = 2 + .5*TerrainNoise.Get(a, 0);
            Tube* t = _CreateVine(a, 1, 1, false, radius);
            _vines.push_back(t);
        }

        for (float a = 0; a < 1.0; a+= inc) {
            float radius = 2 + .5*TerrainNoise.Get(a, 0);
            Tube* t = _CreateVine(a, 0, -1, false, radius);
            _vines.push_back(t);
        }

        for (float a = 0; a < 1.0; a+= inc) {
            float radius = 2 + .5*TerrainNoise.Get(a, 0);
            Tube* t = _CreateVine(1, a, 1, true, radius);
            _vines.push_back(t);
        }
    } 
    if (centers) {
        float inc = .05;
        for (float a = 0; a < 1.0; a+= inc) {
            float radius = 2 + .5*TerrainNoise.Get(a, 0);
            Tube* t = _CreateCenterVine(.0, a, radius);
            _vines.push_back(t);
        }

        for (float a = 0; a < 1.0; a+= inc) {
            float radius = 2 + .5*TerrainNoise.Get(a, 0);
            Tube* t = _CreateCenterVine(a, 1, radius);
            _vines.push_back(t);
        }

        for (float a = 0; a < 1.0; a+= inc) {
            float radius = 2 + .5*TerrainNoise.Get(a, 0);
            Tube* t = _CreateCenterVine(a, 0, radius);
            _vines.push_back(t);
        }

        for (float a = 0; a < 1.0; a+= inc) {
            float radius = 2 + .5*TerrainNoise.Get(a, 0);
            Tube* t = _CreateCenterVine(1, a, radius);
            _vines.push_back(t);
        }
    }
}

Vao GridCity::_CreateCityWall()
{
    sketch::Scene shape;

    const float wallHeight = -30.0f;
    const float wallThickness = 2.0f;
    const float base = -10.0f;

    sketch::Quad southWallQuad;
    southWallQuad.p = vec3(0, base, TerrainArea/2);
    southWallQuad.u = vec3(TerrainArea/2+wallThickness, 0, 0);
    southWallQuad.v = vec3(0, 0, wallThickness);
    sketch::CoplanarPath* southWallRoof = shape.AddQuad(southWallQuad);
    shape.PushPath(southWallRoof, wallHeight);

    sketch::Quad northWallQuad;
    northWallQuad.p = vec3(0, base, -TerrainArea/2);
    northWallQuad.u = vec3(TerrainArea/2+wallThickness, 0, 0);
    northWallQuad.v = vec3(0, 0, wallThickness);
    sketch::CoplanarPath* northWallRoof = shape.AddQuad(northWallQuad);
    shape.PushPath(northWallRoof, wallHeight);

    sketch::Quad eastWallQuad;
    eastWallQuad.p = vec3(TerrainArea/2, base, 0);
    eastWallQuad.u = vec3(wallThickness, 0, 0);
    eastWallQuad.v = vec3(0, 0, TerrainArea/2+wallThickness);
    sketch::CoplanarPath* eastWallRoof = shape.AddQuad(eastWallQuad);
    shape.PushPath(eastWallRoof, wallHeight);

    sketch::Quad westWallQuad;
    westWallQuad.p = vec3(-TerrainArea/2, base, 0);
    westWallQuad.u = vec3(wallThickness, 0, 0);
    westWallQuad.v = vec3(0, 0, TerrainArea/2+wallThickness);
    sketch::CoplanarPath* westWallRoof = shape.AddQuad(westWallQuad);
    shape.PushPath(westWallRoof, wallHeight);

    sketch::Tessellator tess(shape);
    Vao vao;
    tess.PullFromScene();
    tess.PushToGpu(vao);

    return vao;
}

void GridCity::Init()
{
    _previousBump = 0;
    _cityWall = _CreateCityWall();
    _CreateVines();
    
    if (centerpiece) {
        _CreateCenterpiece();
    }

    _ridges.Shape = new sketch::Scene();
    _ridges.CpuTriangles = new sketch::Tessellator(*_ridges.Shape);

    // Tessellate the ground
    FloatList ground;
    FloatList normals;
    IndexList indices;
    TerrainUtil::Smooth(
        TerrainRes * 5, GridTerrainFunc,
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
            float maxWidth = length(u) * 1.0;
            float minWidth = maxWidth * 0.25;
            float maxHeight = length(v) * 1.0;
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

            // Compute the 2D bounding box
            const float inf = numeric_limits<float>::max();
            vec2 minCorner = vec2(inf, inf);
            vec2 maxCorner = vec2(-inf, -inf);
            FOR_EACH(terrace, terraces) {
                vec2 extent = terrace->size * 0.5f;
                minCorner = glm::min(minCorner, terrace->center - extent);
                maxCorner = glm::max(maxCorner, terrace->center + extent);
            }

            // Translate the entire terrace group to center; reject any terrace
            // that overflows the cell border.  For each terrace, add a quad
            // to 'cells'
            vec2 maxBound = vec2(length(u), length(v));
            vec2 minBound(-maxBound);
            vec2 translation = -mix(minCorner, maxCorner, 0.5f);
            vector<Terrace>::iterator terrace = terraces.begin();
            int numRejections = 0;
            while (terrace != terraces.end()) {
                terrace->center += translation;
                vec2 extent = terrace->size * 0.5f;
                vec2 minCorner = terrace->center - extent;
                vec2 maxCorner = terrace->center + extent;
                bool reject = false;
                if (minCorner.x < minBound.x) reject = true;
                if (minCorner.y < minBound.y) reject = true;
                if (maxCorner.x > maxBound.x) reject = true;
                if (maxCorner.y > maxBound.y) reject = true;
                if (reject) {
                    ++numRejections;
                    terrace = terraces.erase(terrace);
                } else {
                    ++terrace;
                }
            }

            // Diagnostics
            const bool Verbose = false;
            if (Verbose) {
                std::cout << terraces.size() << " terraces (" << numRejections << " rejections) ";
                FOR_EACH(terrace, terraces) {
                    float area = terrace->size.x * terrace->size.y;
                    std::cout << area << ' ' ;
                }
                std::cout << std::endl;
            }

            // Turn on "VisualizeCell" mode for simpler geometry
            cell.BuildingId = (int) _cells.size();
            if (VisualizeCell) {
                cell.Quad.p = vec3(p.x, 0, p.y);
                cell.Quad.u = vec3(u.x, 0, u.y);
                cell.Quad.v = vec3(v.x, 0, v.y);
                cell.Height = MinHeight + h * (MaxHeight - MinHeight);
                _cells.push_back(cell);
            } else {
                vec3 P = vec3(p.x, 0, p.y);
                vec3 U = normalize(vec3(u.x, 0, u.y));
                vec3 V = normalize(vec3(v.x, 0, v.y));
                FOR_EACH(terrace, terraces) {
                    cell.Quad.p = P +
                        terrace->center.x * U +
                        terrace->center.y * V ;
                    cell.Quad.u = U * terrace->size.x;
                    cell.Quad.v = V * terrace->size.y;
                    cell.Height = terrace->height;
                    _cells.push_back(cell);
                }
            }
        }
    }

    // "Unfloat" the quads and orient them onto the terrain
    FOR_EACH(i, _cells) {
        GridCell& cell = *i;
        cell.Visible = true;

        vec3 p0 = cell.Quad.p;
        cell.Quad.p.y = p0.y = _GetHeight(p0);

        vec3 p1 = cell.Quad.p + cell.Quad.u;
        p1.y = _GetHeight(p1);

        vec3 p2 = cell.Quad.p + cell.Quad.v;
        p2.y = _GetHeight(p2);

        cell.Quad.u = length(cell.Quad.u) * normalize(p1 - cell.Quad.p);
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
            cell.Roof.StartBeat = (int) d;
            col++;
            if (col >= NumCols) {
                col = 0;
                row++;
            }
        }
    }

    // Test the terrain sampling function
    bool TestGetHeight = false;
    if (TestGetHeight) {
        for (float x = -500; x < 500; x += 5.0) {
            printf("%f\n", 1000-(x+500));
            for (float z = -500; z < 500; z += 15.0) {
                sketch::Quad q;
                q.p.x = x;
                q.p.y = 0;
                q.p.z = z;
                q.p.y = _GetHeight(q.p);
                q.u = vec3(2, 0, 0);
                q.v = vec3(0, 0, 2);
                _ridges.Shape->AddQuad(q);
            }
        }
    }

    // Compile shaders
    Programs& progs = Programs::GetInstance();
    progs.Load("Buildings.Terrain", false);
    progs.Load("Sketch.Facets", true);
    progs.Load("FireFlies.Sig", "FireFlies.Sig.FS", "FireFlies.Tube.VS");

    // Set up camera
    _camera.far = 1000;
    _camera.up = vec3(0, 1, 0);
    _camera.center = vec3(-10, 0, 0);
    _camera.eye = vec3(-150-10, 350, 350);
}

sketch::PathList GridCity::_AddWindows(
    GridCell* cell,
    sketch::CoplanarPath* wall)
{
    sketch::Scene* shape = cell->Shape;
    sketch::PathList windows;

    sketch::Quad wallQuad = shape->ComputeQuad(wall);
    vec2 extent = 2.0f * vec2(length(wallQuad.u), length(wallQuad.v));
    float wallWidth = extent.x;
    float wallHeight = extent.y;

    const int numRows = 4;
    const int numCols = 10;
    const vec2 padding(1, 1);

    float holeWidth = (wallWidth - (numCols + 1) * padding.x) / float(numCols);
    float holeHeight = (wallHeight - (numRows + 1) * padding.y) / float(numRows);

    if (holeHeight < 1.0 || holeWidth < 1.0) {
        return windows;
    }

    vec3 U = normalize(wallQuad.u);
    vec3 V = normalize(wallQuad.v);

    vec2 offset;
    offset.x = padding.x + holeWidth/2 - wallWidth/2;
    for (int col = 0; col < numCols; ++col) {
        offset.y = padding.y + holeHeight/2 - wallHeight/2;
        for (int row = 0; row < numRows; ++row) {
            sketch::Quad holeQuad;
            holeQuad.p = wallQuad.p + U * offset.x + V * offset.y;
            holeQuad.u = 0.5f * holeWidth * U;
            holeQuad.v = 0.5f * holeHeight * V;
            sketch::CoplanarPath* hole = 0;
            hole = shape->AddHoleQuad(holeQuad, wall);
            windows.push_back(hole);
            offset.y += holeHeight + padding.y;
        }
        offset.x += holeWidth + padding.x;
    }
    return windows;
}

void GridCity::_AllocCell(GridCell* cell)
{
    cell->Ridges[0] = 0;
    cell->Ridges[1] = 0;
    cell->Ridges[2] = 0;
    cell->Ridges[3] = 0;

    sketch::Scene* shape = new sketch::Scene;
    cell->Roof.Path = shape->AddQuad(cell->Quad);
    vec3 n = cell->Roof.Path->Plane->GetNormal();
    float flip = dot(n, vec3(0, 1, 0)) < 0 ? -1 : 1;

    // Push up the roof
    sketch::PathList walls;
    cell->Roof.BeginW = cell->Roof.Path->Plane->Eqn.w;
    shape->PushPath(cell->Roof.Path, flip * cell->Height, &walls);
    cell->Roof.EndW = cell->Roof.Path->Plane->Eqn.w;

    // Create window holes
    if (HasWindows) {
        FOR_EACH(w, walls) {
            sketch::CoplanarPath* cop = dynamic_cast<sketch::CoplanarPath*>(*w);
            _AddWindows(cell, cop);
        }
    }

    // Add detail to rooftops
    const float ridgeHeight = 1.0f;
    const float ridgeThickness = 0.5f;
    sketch::Quad roofQuad = shape->ComputeQuad(cell->Roof.Path);
    vec3 U = normalize(roofQuad.u);
    vec3 V = normalize(roofQuad.v);
    if (_ridges.Shape) {

        // North ridge
        {
            GridAnim* anim = new GridAnim();
            sketch::Quad northRidgeQuad;
            northRidgeQuad.p = roofQuad.p + roofQuad.u - U * ridgeThickness;
            northRidgeQuad.u = U * ridgeThickness;
            northRidgeQuad.v = roofQuad.v;
            sketch::CoplanarPath* northRidge = 
                _ridges.Shape->AddQuad(northRidgeQuad);
            anim->BeginW = northRidge->Plane->Eqn.w;
            _ridges.Shape->PushPath(northRidge, ridgeHeight);
            anim->EndW = northRidge->Plane->Eqn.w;
            _ridges.Shape->SetPathPlane(northRidge, anim->BeginW);
            northRidge->Visible = false;
            anim->Path = northRidge;
            _ridges.Anims.push_back(anim);
            cell->Ridges[0] = anim;
        }
        // South ridge
        {
            GridAnim* anim = new GridAnim();
            sketch::Quad southRidgeQuad;
            southRidgeQuad.p = roofQuad.p - roofQuad.u - U * ridgeThickness;
            southRidgeQuad.u = U * ridgeThickness;
            southRidgeQuad.v = roofQuad.v;
            sketch::CoplanarPath* southRidge = 
                _ridges.Shape->AddQuad(southRidgeQuad);
            anim->BeginW = southRidge->Plane->Eqn.w;
            _ridges.Shape->PushPath(southRidge, ridgeHeight);
            anim->EndW = southRidge->Plane->Eqn.w;
            _ridges.Shape->SetPathPlane(southRidge, anim->BeginW);
            southRidge->Visible = false;
            anim->Path = southRidge;
            _ridges.Anims.push_back(anim);
            cell->Ridges[1] = anim;
        }
        // West ridge
        {
            GridAnim* anim = new GridAnim();
            sketch::Quad westRidgeQuad;
            westRidgeQuad.p = roofQuad.p + roofQuad.v - V * ridgeThickness;
            westRidgeQuad.u = roofQuad.u;
            westRidgeQuad.v = V * ridgeThickness;
            sketch::CoplanarPath* westRidge = 
                _ridges.Shape->AddQuad(westRidgeQuad);
            anim->BeginW = westRidge->Plane->Eqn.w;
            _ridges.Shape->PushPath(westRidge, ridgeHeight);
            anim->EndW = westRidge->Plane->Eqn.w;
            _ridges.Shape->SetPathPlane(westRidge, anim->BeginW);
            westRidge->Visible = false;
            anim->Path = westRidge;
            _ridges.Anims.push_back(anim);
            cell->Ridges[2] = anim;
        }
        // East ridge
        {
            GridAnim* anim = new GridAnim();
            sketch::Quad eastRidgeQuad;
            eastRidgeQuad.p = roofQuad.p - roofQuad.v + V * ridgeThickness;
            eastRidgeQuad.u = roofQuad.u;
            eastRidgeQuad.v = V * ridgeThickness;
            sketch::CoplanarPath* eastRidge = 
                _ridges.Shape->AddQuad(eastRidgeQuad);
            anim->BeginW = eastRidge->Plane->Eqn.w;
            _ridges.Shape->PushPath(eastRidge, ridgeHeight);
            anim->EndW = eastRidge->Plane->Eqn.w;
            _ridges.Shape->SetPathPlane(eastRidge, anim->BeginW);
            eastRidge->Visible = false;
            anim->Path = eastRidge;
            _ridges.Anims.push_back(anim);
            cell->Ridges[3] = anim;
        }
    }

    // Finalize the topology
    cell->CpuTriangles = new sketch::Tessellator(*shape);
    cell->CpuTriangles->PullFromScene();

    // Push the building back into the ground to allow it to pop up later
    if (PopBuildings) {
        shape->SetPathPlane(cell->Roof.Path, cell->Roof.BeginW);
    }

    // Test
    if (false) {
        for (float x = -500; x < 500; x += 50.0) {
            for (float z = -500; z < 500; z += 50.0) {
                sketch::Quad q;
                q.p.x = x;
                q.p.y = 0;
                q.p.z = z;
                q.p.y = _GetHeight(q.p);
                q.u = vec3(2, 0, 0);
                q.v = vec3(0, 0, 2);
                _ridges.Shape->AddQuad(q);
            }
        }
    }

    // Misc
    cell->Roof.StartTime = 0;
    cell->Shape = shape;
    cell->Visible = not PopBuildings;
    cell->CpuTriangles->PullFromScene();
    cell->CpuTriangles->PushToGpu(cell->GpuTriangles);

    if (not PopBuildings) {
        _FreeCell(cell);
    }
}

void GridCity::_FreeCell(GridCell* cell)
{
    delete cell->Shape;
    delete cell->CpuTriangles;
    cell->Shape = 0;
    cell->CpuTriangles = 0;
}

void GridCity::Update()
{
    float time = GetContext()->elapsedTime;
    if (GetContext()->audio->GetHiHats()) {
        _currentBeat++;
    }

    int numAnimating = 0;
    tween::Elastic tweener;
    FOR_EACH(i, _cells) {
        GridCell& cell = *i;
        if (!cell.CpuTriangles) {
            continue;
        }
        if (cell.Roof.StartTime == 0) {
            // Check if the building hasn't been born yet:
            if (cell.Roof.StartBeat > _currentBeat) {
                cell.Visible = false;
                continue;
            }
            // At this point we're starting a new pop animation
            cell.Roof.StartTime = time;
            cell.Visible = true;
        }
        if (time > cell.Roof.StartTime + PopDuration) {
            // At this point we're ending a pop animation
            cell.Shape->SetPathPlane(cell.Roof.Path, cell.Roof.EndW);
            cell.CpuTriangles->PullFromScene();
            cell.CpuTriangles->PushToGpu(cell.GpuTriangles);

            // Show ridges
            for (int i = 0; i < 4; ++i) {
                if (!cell.Ridges[i]) {
                    continue;
                }
                _ridges.Shape->SetPathPlane(
                    cell.Ridges[i]->Path,
                    cell.Ridges[i]->EndW);
                _ridges.Shape->SetVisible(cell.Ridges[i]->Path, true);
            }

            if (!pingpong) {
                _FreeCell(&cell);
            }
            continue;
        }
        // Update an in-progress animation
        numAnimating++;
        float w = tweener.easeOut(
            time - cell.Roof.StartTime,
            cell.Roof.BeginW,
            cell.Roof.EndW - cell.Roof.BeginW,
            PopDuration);
        cell.Shape->SetPathPlane(cell.Roof.Path, w);
        cell.CpuTriangles->PullFromScene();
        cell.CpuTriangles->PushToGpu(cell.GpuTriangles);
    }

    if (numAnimating == 0 && pingpong) {
        // TBD prideout
    }

    // update vines
    FOR_EACH(tubeIt, _vines) {
        (*tubeIt)->Update();
    }

    // update ridges
    _ridges.CpuTriangles->PullFromScene();
    _ridges.CpuTriangles->PushToGpu(_ridges.GpuTriangles);

    // update centerpiece
    if (centerpiece && time > 2.0f) {

        bool bump = false;
        if (time - _previousBump > 0.5 || time > 10.0) {
            bump = GetContext()->audio->GetKicks() || GetContext()->audio->GetSnares();
            if (time > 10.0) {
                //_centerpiecePlayer->SetCommandDuration(1.0 / 6.0); // prideout
                bump = bump || GetContext()->audio->GetHiHats();
            }
            if (bump) {
                _previousBump = time;
            }
        }
        _centerpiecePlayer->Update(true, bump);
        _centerpieceTess->PullFromScene();

        if (false && time > 4.0f) {
            static bool cw = false;
            if (GetContext()->audio->GetKicks() || GetContext()->audio->GetSnares()) {
                vec3 axis = vec3(0, 1, 0);
                FOR_EACH(c, _columns) {
                    _centerpieceSketch->RotatePath(
                        *c,
                        axis,
                        _columnCenter,
                        cw ? 15.0f : -15.0f);
                }
                cw = !cw;
            }
        }
    }

    // update camera
    _camera.eye = glm::rotate(_camera.eye, 
                        (trackBeat and (GetContext()->audio->GetSnares() or GetContext()->audio->GetKicks())) ? 1.0f : .1f, 
                        glm::vec3(0,1,0));
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
    glCullFace(GL_FRONT);
    glUseProgram(progs["Sketch.Facets"]);
    glUniform3f(u("Scale"), 1, 1, 1);
    glUniform3f(u("Translate"), 0, 0, 0);
    glUniform1i(u("HasWindows"), 1);
    _camera.Bind(glm::mat4());

    FOR_EACH(cell, _cells) {
        glUniform1i(u("BuildingId"), cell->BuildingId);
        if (not cell->Visible) {
            continue;
        }
        cell->GpuTriangles.Bind();
        glDrawElements(GL_TRIANGLES, cell->GpuTriangles.indexCount, GL_UNSIGNED_INT, 0);
    }

    // Draw roof ridges
    glUniform1i(u("HasWindows"), 0);
    glUniform1i(u("BuildingId"), 0);
    _ridges.GpuTriangles.Bind();
    glDrawElements(GL_TRIANGLES, _ridges.GpuTriangles.indexCount, GL_UNSIGNED_INT, 0);

    // Add city wall
    glUniform1i(u("HasWindows"), 0);
    _cityWall.Bind();
    glDrawElements(GL_TRIANGLES, _cityWall.indexCount, GL_UNSIGNED_INT, 0);

    // Draw the centerpiece
    if (centerpiece) {
        glDisable(GL_CULL_FACE);
        _centerpieceTess->PushToGpu(_centerpieceVao);
        _centerpieceVao.Bind();
        glDrawElements(GL_TRIANGLES, _centerpieceVao.indexCount, GL_UNSIGNED_INT, 0);
    }

    // Restore culling to normal
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // Grow vines
    glUseProgram(progs["FireFlies.Sig"]);
    _camera.Bind(glm::mat4());
    glUniform3f(u("Eye"), _camera.eye.x, _camera.eye.y, _camera.eye.z);
    glUniform3f(u("MaterialColor"), .5, .5, .2);
    glUniform1f(u("Time"), GetContext()->elapsedTime);

    FOR_EACH(tubeIt, _vines) {
        (*tubeIt)->Draw();
    }
}

void GridCity::_CreateCenterpiece()
{
    using namespace sketch;

    _centerpieceSketch = new Scene();
    _historicalSketch = new Scene();

    const Plane* ground = _centerpieceSketch->GroundPlane();
    glm::vec2 off(0, 0);

    vec4 groundEqn = ground->Eqn;

    CoplanarPath* circleRoof = _centerpieceSketch->AddPolygon(30, groundEqn, off, 96);
    _centerpieceSketch->PushPath(circleRoof, 80);

    CoplanarPath* lowerInset = _centerpieceSketch->AddInscribedPolygon(29, circleRoof, vec2(0,0), 128);
    _centerpieceSketch->PushPath(lowerInset, -2);

    int numColumns = 12;
    vec4 roofEqn = groundEqn; roofEqn.w = 78;
    for (int i = 0; i < numColumns; ++i) {
        float theta = 6.28 * i / numColumns;
        off.x = 10.0f * cos(theta);
        off.y = 10.0f * sin(theta);
        CoplanarPath* column = _centerpieceSketch->AddPolygon(2, roofEqn, off, 32);
        _columns.push_back(column);
    }
    _columnCenter = vec3(0, roofEqn.w, 0);
    _centerpieceSketch->PushPaths(_columns, 30);
    roofEqn.w = 80;

    off = vec2(0, 0);
    CoplanarPath* circle2Roof = _centerpieceSketch->AddPolygon(7, roofEqn, off, 5);
    _centerpieceSketch->PushPath(circle2Roof, 80);

    off = vec2(0, 0);
    roofEqn.w += 80;
    CoplanarPath* triRoof = _centerpieceSketch->AddPolygon(40, roofEqn, off, 128);
    PathList triWalls;
    PathList bumps;
    _centerpieceSketch->PushPath(triRoof, 4, &triWalls);
    roofEqn.w -= 3;

    int numHangingThings = 32;
    for (int i = 0; i < numHangingThings; ++i) {
        Quad hangingThingyQ;

        float theta = 6.28 * i / numHangingThings;
        off.x = 15.0f * cos(theta);
        off.y = 15.0f * sin(theta);

        theta = -theta;

        hangingThingyQ.p = vec3(off.x, roofEqn.w, off.y);
        hangingThingyQ.u = 0.78f * vec3(sin(theta), 0, cos(theta));
        hangingThingyQ.v = 0.25f * vec3(sin(theta+3.14/2.0), 0, cos(theta+3.14/2.0));
        CoplanarPath* hangingThingy = _centerpieceSketch->AddQuad(hangingThingyQ);
        _hangingThings.push_back(hangingThingy);

        vec4 r = roofEqn; r.w += 3.0;
        CoplanarPath* bump = _centerpieceSketch->AddPolygon(1, r, off, 16);
        bumps.push_back(bump);
    }
    _centerpieceSketch->PushPaths(_hangingThings, -20);

    for (int i = 0; i < numHangingThings; ++i) {
        float theta = 6.28 * i / numHangingThings;
        off.x = 20.0f * cos(theta);
        off.y = 20.0f * sin(theta);
        vec4 r = roofEqn; r.w += 3.0;
        CoplanarPath* bump = _centerpieceSketch->AddPolygon(1, r, off, 16);
        bumps.push_back(bump);
    }

    _centerpieceSketch->PushPaths(bumps, 6.0);

    CoplanarPath* topInset = _centerpieceSketch->AddInscribedPolygon(37, triRoof, vec2(0,0), 128);
    _centerpieceSketch->PushPath(topInset, -2);

    int numDents = 30;
    PathList dents;
    for (int i = 0; i < numDents; i++) {
        float theta = 6.28 * i / numDents;
        off.x = 20.0f * cos(theta);
        off.y = 20.0f * sin(theta);
        CoplanarPath* dent = _centerpieceSketch->AddInscribedPolygon(1.5f, lowerInset, off, 16);
        dents.push_back(dent);
    }
    _centerpieceSketch->PushPaths(dents, -2);
    _centerpieceSketch->PushPath(lowerInset, -4);

    for (int repeats = 0; repeats < 5; ++repeats) {
        for (int i = 0; i < numDents;) {
            sketch::CoplanarPath* cop = dynamic_cast<sketch::CoplanarPath*>(dents[i]);
            _centerpieceSketch->PushPath(cop, 6);
            i += (repeats + 1);
        }
        for (int i = 0; i < numDents;) {
            sketch::CoplanarPath* cop = dynamic_cast<sketch::CoplanarPath*>(dents[i]);
            _centerpieceSketch->PushPath(cop, -6);
            i += (repeats + 1);
        }
    }

    const Json::Value& history = _centerpieceSketch->GetHistory();
    std::swap(_historicalSketch, _centerpieceSketch);

    _centerpieceTess = new Tessellator(*_centerpieceSketch);
    _centerpiecePlayer = new Playback(history, _centerpieceSketch, _centerpieceTess);
    _centerpieceSketch->EnableHistory(false);
    _historicalSketch->EnableHistory(false);

    _centerpieceTess->PullFromScene();
}

