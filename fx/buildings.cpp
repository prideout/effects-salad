// Add "composite" support building instance: cube-on-cube, cylinder-on-cube
// Use long parabolas for the falling tets
// Secondary explosion effect: some tets pop off the top, right before explosion

// Cracks on window, with viewport shattering?
// Viewport with "map view"?  Could help with path-finding...

// Voronoi divisions of the city
//
// Better radial blur?
// Street lamps, ground-level detail, low-flying camera
// Dual viewports for street-level view?
//
// We're only rendering boundary tets right now, but we're not exploiting
//   the massive memory savings.  Maybe buildings should have floors?
//
// Camera
// Radial blur
// DOF effect
// Look at Akira references
// Smoke

#include "glm/gtc/type_ptr.hpp"
#include "fx/buildings.h"
#include "glm/gtx/rotate_vector.hpp"
#include "tthread/tinythread.h"
#include "common/tetUtil.h"
#include "common/init.h"
#include "common/programs.h"
#include "common/camera.h"
#include "common/demoContext.h"
#include "common/terrainUtil.h"

using namespace std;
using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec2;

struct GpuParams {
    Blob HullIndices;
    Blob HullPoints;
    Vec4List Centroids;
    Blob FlattenedTets;
    Blob Cracks;
};

struct ThreadParams {
    float Thickness;
    float TopRadius;
    float TetSize;
    int NumSides;
    BuildingTemplate* Dest;
    GpuParams* GpuData;
};

void _GenerateBuilding(void* params);
void _UploadBuilding(ThreadParams& params);

class CracksEffect : public Effect {
public:
    CracksEffect(Buildings* buildings) : Effect(), _buildings(buildings) {}
    ~CracksEffect() {} 
    void Init();
    void Update();
    void Draw();
private:
    void _DrawBuilding(BuildingTemplate& templ, BuildingInstance& instance);
    Buildings* _buildings;
};

Buildings::Buildings() : Effect()
{
    _cracks = new CracksEffect(this);
}

Buildings::~Buildings()
{
    delete _cracks;
}

void
Buildings::Init()
{
    // Kick off the threads that tetify the building templates
    vector<tthread::thread*> threads;
    vector<ThreadParams> params;
    #include "fx/buildings.inl"

    // Tessellate the ground
    if (true) {
        FloatList ground;
        FloatList normals;
        IndexList indices;
        const int SIZE = 150;
        const float SCALE = 0.25;
        TerrainUtil::Smooth(SIZE, SCALE, &ground, &normals, &indices);
        _terrainVao = Vao(3, ground, indices);
        _terrainVao.AddVertexAttribute(AttrNormal, 3, normals);
    }

    // Allocate batches for each template
    _batches.resize(_templates.size());
    for (size_t i = 0; i < _templates.size(); ++i) {
        _batches[i].Template = &_templates[i];
    }

    // Stamp down the buildings in a grid
    const vec2 extent(80, 80);
    const int numCols = 12;
    const int numRows = 12;
    const vec2 cellSize(extent.x / float(numCols),
                        extent.y / float(numRows));
    vec2 groundPos;
    BuildingInstance inst;
    groundPos.x = -0.5 * extent.x + 0.5 * cellSize.y;
    for (int col = 0; col < numCols; ++col) {
        groundPos.y = -0.5 * extent.y + 0.5 * cellSize.y;
        for (int row = 0; row < numRows; ++row) {

            inst.GroundPosition = groundPos;
            inst.EnableCullingPlane = false;
            inst.ExplosionStart = 1000.0f; // don't explode
            size_t templ;

            // Low-lying buildings
            if (rand() % 3 != 0) {
                float height = 0.2 + 0.5 * (rand() % 10) / 10.0f;
                float radiusx = 0.1f + 0.3 * (rand() % 10) / 10.0f;
                float radiusz = 0.1f + 0.3 * (rand() % 10) / 10.0f;
                inst.Scale = vec3(radiusx, height, radiusz);
                templ = 1;

            // Skyscrapers
            } else {
                templ = rand() % _batches.size();
                float height = 0.5 + 1.0 * (rand() % 100) / 100.0f;
                if (rand() % 10 == 0) {
                    height += 0.5;
                }
                float radius = 0.5f - 0.5 * (rand() % 10) / 10.0f;
                inst.Scale = vec3(radius, height, radius);

                // Only half of them actually explode
                inst.ExplosionStart = 3.0f + 10.0f * (rand() % 100) / 100.0f;
            }

            inst.Hue = 0.4 + 0.2 * (rand() % 100) / 100.0f;

            BuildingBatch& batch = _batches[templ];
            batch.Instances.push_back(inst);

            groundPos.y += cellSize.y;
        }
        groundPos.x += cellSize.x;
    }
    
    // Compile shaders
    Programs& progs = Programs::GetInstance();
    progs.Load("Tetra.Cracks", false);
    progs.Load("Tetra.Solid", false);
    progs.Load("Buildings.XZPlane", false);
    progs.Load("Buildings.Facets", true);
    progs.Load("Buildings.Terrain", false);
    
    // Misc initialization
    _emptyVao.InitEmpty();
    _cracks->Init();

    // Wait for the tetrahedralization to finish
    for (size_t i = 0; i < threads.size(); ++i) {
        threads[i]->join();
        _UploadBuilding(params[i]);
        delete threads[i];
    }
}

void
_GenerateBuilding(void* vParams)
{
    ThreadParams* params = (ThreadParams*) vParams;
    float thickness = params->Thickness;
    float topRadius = params->TopRadius;
    float tetSize = params->TetSize;
    int nSides = params->NumSides;
    BuildingTemplate* dest = params->Dest;
    GpuParams* gpuData = new GpuParams;

    // Create the outer skin
    tetgenio in;
    float r1 = 10.0f;  float r2 = r1 * topRadius;
    float y1 = 0;      float y2 = 20.0f;
    TetUtil::HullFrustum(r1, r2, y1, y2, nSides, &in);

    // Create a cheap Vao for buildings that aren't self-destructing
    TetUtil::TrianglesFromHull(in, &gpuData->HullIndices);
    gpuData->HullPoints.resize(sizeof(float) * 3 * in.numberofpoints);
    memcpy(&gpuData->HullPoints[0], in.pointlist, gpuData->HullPoints.size());

    // Add inner walls
    y1 += thickness; y2 -= thickness;
    r1 -= thickness; r2 -= thickness;
    TetUtil::HullFrustum(r1, r2, y1, y2, nSides, &in);

    // Poke volumetric holes
    Vec3List holePoints;
    holePoints.push_back(vec3(0, 10.0, 0));
    TetUtil::AddHoles(holePoints, &in);

    // Tetrahedralize the boundary mesh
    tetgenio out;
    const float qualityBound = 1.414;
    const float maxVolume = tetSize;
    TetUtil::TetsFromHull(in, &out, qualityBound, maxVolume, true);
    dest->TotalTetCount = out.numberoftetrahedra;

    // Populate the per-tet texture data and move boundary tets to the front
    TetUtil::SortTetrahedra(&gpuData->Centroids, out, &dest->BoundaryTetCount);

    // Create a flat list of non-indexed triangles
    VertexAttribMask attribs = AttrPositionFlag | AttrNormalFlag;
    TetUtil::PointsFromTets(out, attribs, &gpuData->FlattenedTets);

    // Non-indexed vertical crack lines
    TetUtil::FindCracks(out, gpuData->Centroids, &gpuData->Cracks);

    params->GpuData = gpuData;
}

void
_UploadBuilding(ThreadParams& params)
{
    GpuParams* src = params.GpuData;
    BuildingTemplate* dest = params.Dest;

    // Cheap Vao for buildings that aren't self-destructing
    dest->HullVao.Init();
    dest->HullVao.AddVertexAttribute(AttrPosition,
                                     3,
                                     src->HullPoints);
    dest->HullVao.AddIndices(src->HullIndices);

    // Texture buffer with centroids
    dest->CentroidTexture.Init(src->Centroids);

    // Huge buffer of non-indexed triangles
    dest->BuildingVao.Init();
    VertexAttribMask attribs = AttrPositionFlag | AttrNormalFlag;
    dest->BuildingVao.AddInterleaved(attribs, src->FlattenedTets);
    pezCheckGL("Bigass VBO for tets");

    // Non-indexed vertical crack lines
    dest->CracksVao.Init();
    dest->CracksVao.AddInterleaved(AttrPositionFlag | AttrLengthFlag, src->Cracks);
    dest->NumCracks = (src->Cracks.size() / sizeof(vec4)) / 2;
    pezCheckGL("Bigass VBO for cracks");

    // Free CPU memory
    delete src;
    params.GpuData = 0;
}

void
Buildings::Update()
{
    const bool Looping = true;
    if (not Looping) {
        GetContext()->duration = std::numeric_limits<float>::infinity();
    }

    float time = GetContext()->elapsedTime;
    PerspCamera* camera = &GetContext()->mainCam;
    camera->eye.x = 0;
    camera->eye.y = 35;
    camera->eye.z = 70;
    camera->center.y = 20;
    camera->eye = glm::rotateY(camera->eye, time * 48);

    _cracks->Update();
}

void
Buildings::Draw()
{
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Programs& progs = Programs::GetInstance();
    PerspCamera surfaceCam = GetContext()->mainCam;

    // Draw buildings
    glUseProgram(progs["Buildings.Facets"]);
    surfaceCam.Bind(glm::mat4());
    glUseProgram(progs["Tetra.Solid"]);
    surfaceCam.Bind(glm::mat4());
    FOR_EACH(batch, _batches) {
        FOR_EACH(instance, batch->Instances) {
            _DrawBuilding(*batch->Template, *instance);
        }
    }

    // Draw floor
    if (false) {
        glDisable(GL_CULL_FACE);
        glUseProgram(progs["Buildings.XZPlane"]);
        surfaceCam.Bind(glm::mat4());
        _emptyVao.Bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // Draw terrain
    if (true) {
        glDisable(GL_CULL_FACE);
        glUseProgram(progs["Buildings.Terrain"]);
        surfaceCam.Bind(glm::mat4());
        _terrainVao.Bind();
        glDrawElements(GL_TRIANGLE_STRIP, _terrainVao.indexCount, GL_UNSIGNED_INT, 0);
    }
}

void
Buildings::_DrawBuilding(BuildingTemplate& templ, BuildingInstance& instance)
{
    const float ExplosionDuration = 1.5;
    const float BulgeDuration = 1.0;

    Programs& progs = Programs::GetInstance();
    vec3 xlate = vec3(instance.GroundPosition.x, 0, instance.GroundPosition.y);
    vec3 scale = instance.Scale;

    float time = GetContext()->elapsedTime;
    bool boundariesOnly = time < (instance.ExplosionStart - BulgeDuration);
    bool completelyDestroyed = (time > instance.ExplosionStart + ExplosionDuration);

    if (completelyDestroyed) {
        return;
    }

    if (boundariesOnly) {
        glUseProgram(progs["Buildings.Facets"]);
    } else {
        glUseProgram(progs["Tetra.Solid"]);
        templ.CentroidTexture.Bind(0, "CentroidTexture");
    }

    glUniform1f(u("CullY"), instance.EnableCullingPlane ? instance.CullingPlaneY : 999);
    glUniform3fv(u("Translate"), 1, ptr(xlate));
    glUniform1f(u("Time"), time);
    glUniform3fv(u("Scale"), 1, ptr(scale));
    glUniform1f(u("Hue"), instance.Hue);
    glUniform1f(u("ExplosionStart"), instance.ExplosionStart);

    if (boundariesOnly) {
        templ.HullVao.Bind();
        glDrawElements(GL_TRIANGLES, templ.HullVao.indexCount, GL_UNSIGNED_INT, 0);
        return;
    }

    templ.BuildingVao.Bind();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    int n = templ.BoundaryTetCount;
    glDrawArrays(GL_TRIANGLES, 0, n * 4 * 3);
    glDisable(GL_BLEND);
}

Effect*
Buildings::Cracks()
{
    return _cracks;
}

void
CracksEffect::Init()
{
}

void
CracksEffect::Update()
{
}

void
CracksEffect::Draw()
{
    Programs& progs = Programs::GetInstance();
    PerspCamera surfaceCam = GetContext()->mainCam;

    glUseProgram(progs["Tetra.Cracks"]);
    surfaceCam.Bind(glm::mat4());
    FOR_EACH(batch, _buildings->_batches) {
        FOR_EACH(instance, batch->Instances) {
            _DrawBuilding(*batch->Template, *instance);
        }
    }
}

void
CracksEffect::_DrawBuilding(BuildingTemplate& templ, BuildingInstance& instance)
{
    float time = GetContext()->elapsedTime;
    if (time > instance.ExplosionStart) {
        return;
    }

    Programs& progs = Programs::GetInstance();
    vec3 xlate = vec3(instance.GroundPosition.x, 0, instance.GroundPosition.y);
    vec3 scale = instance.Scale;
    glUseProgram(progs["Tetra.Cracks"]);
    glUniform3fv(u("Translate"), 1, ptr(xlate));
    glUniform3fv(u("Scale"), 1, ptr(scale));
    glUniform1f(u("Time"), time - instance.ExplosionStart + 3.0);
    glUniform1f(u("DepthOffset"), -0.0001f);
    glUniform4f(u("Color"), 0, 10, 10, 10);
    templ.CentroidTexture.Bind(0, "CentroidTexture");
    templ.BuildingVao.Bind();
    templ.CracksVao.Bind();
    glDrawArrays(GL_LINES, 0, 2 * templ.NumCracks);
}
