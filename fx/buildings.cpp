// Generate instances and templates with loops rather than unrolled code.
// Stack buildings for a slightly more interesting effect
// Build city using simplistic 2D packing of triangles, pentagons, circles, and squares
// Secondary explosion effect: some tets pop off the top, right before explosion
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

using namespace std;
using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec2;

static bool SingleBuilding = false;

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
    _emptyVao.InitEmpty();

    _templates.resize(SingleBuilding ? 1 : 4);
    _batches.resize(_templates.size());
     
    ThreadParams params0 = {0};
    params0.Thickness = 3;
    params0.TopRadius =  8.0f / 13.0f;
    params0.TetSize = 0.1f;
    params0.NumSides = 5;
    params0.Dest = &_templates[0];
    tthread::thread thread0(_GenerateBuilding, &params0);

    ThreadParams params1 = {0};
    params1.Thickness = 2.5f;
    params1.TopRadius =  1.0f;
    params1.TetSize = 0.1f;
    params1.NumSides = 4;
    params1.Dest = &_templates[1];
    tthread::thread thread1(_GenerateBuilding, &params1);

    ThreadParams params2 = {0};
    params2.Thickness = 2.5f;
    params2.TetSize = 0.1f;
    params2.TopRadius =  1.2f;
    params2.NumSides = 3;
    params2.Dest = &_templates[2];
    tthread::thread thread2(_GenerateBuilding, &params2);

    ThreadParams params3 = {0};
    params3.Thickness = 2.5f;
    params3.TetSize = 0.1f;
    params3.TopRadius = 1;
    params3.NumSides = 24;
    params3.Dest = &_templates[3];
    tthread::thread thread3(_GenerateBuilding, &params3);

     _batches[0].Template = &_templates[0];
     _batches[0].Instances.resize(1);
     _batches[0].Instances[0].EnableCullingPlane = false;
     _batches[0].Instances[0].CullingPlaneY = 10.0;
     _batches[0].Instances[0].GroundPosition = vec2(0, 0);
     _batches[0].Instances[0].Height = 1.25;
     _batches[0].Instances[0].Radius = 1;
     _batches[0].Instances[0].Hue = 0.1;
     _batches[0].Instances[0].ExplosionStart = 3.0;

     if (not SingleBuilding) {
         _batches[1].Template = &_templates[1];
         _batches[1].Instances.resize(2);
         _batches[1].Instances[0].EnableCullingPlane = false;
         _batches[1].Instances[0].GroundPosition = vec2(-20, 0);
         _batches[1].Instances[0].Height = 0.5;
         _batches[1].Instances[0].Radius = 1.0;
         _batches[1].Instances[0].Hue = 0.3;
         _batches[1].Instances[0].ExplosionStart = 5.0;
         _batches[1].Instances[1] = _batches[1].Instances[0];
         _batches[1].Instances[1].GroundPosition = vec2(12, 12);
         _batches[1].Instances[1].Height = 0.3;
         _batches[1].Instances[1].Radius = 1.2;

         _batches[2].Template = &_templates[2];
         _batches[2].Instances.resize(2);
         _batches[2].Instances[0].EnableCullingPlane = false;
         _batches[2].Instances[0].GroundPosition = vec2(-15, 30);
         _batches[2].Instances[0].Height = 1.3;
         _batches[2].Instances[0].Radius = 1.0;
         _batches[2].Instances[0].Hue = 0.1;
         _batches[2].Instances[0].ExplosionStart = 6.0;
         _batches[2].Instances[1] = _batches[2].Instances[0];
         _batches[2].Instances[1].GroundPosition = vec2(15, 30);
         _batches[2].Instances[1].Height = 2.0;

         _batches[3].Template = &_templates[3];
         _batches[3].Instances.resize(2);
         _batches[3].Instances[0].EnableCullingPlane = false;
         _batches[3].Instances[0].GroundPosition = vec2(0, -30);
         _batches[3].Instances[0].Height = 1.4;
         _batches[3].Instances[0].Radius = 0.8;
         _batches[3].Instances[0].Hue = 0.0;
         _batches[3].Instances[0].ExplosionStart = 7.0;
         _batches[3].Instances[1] = _batches[3].Instances[0];
         _batches[3].Instances[1].GroundPosition = vec2(13, -28);
         _batches[3].Instances[1].Radius = 0.4;
         _batches[3].Instances[1].Height = 0.4;
         _batches[3].Instances[1].Hue = 0.1;
         _batches[3].Instances[1].ExplosionStart = 7.5;
     }

     // Needs improvement:
     // This waits on thread0 to finish, which isn't necessarily the fastest!
     thread0.join();
     _UploadBuilding(params0);
     thread1.join();
     _UploadBuilding(params1);
     thread2.join();
     _UploadBuilding(params2);
     thread3.join();
     _UploadBuilding(params3);

     // Compile shaders
     Programs& progs = Programs::GetInstance();
     progs.Load("Tetra.Cracks", false);
     progs.Load("Tetra.Solid", false);
     progs.Load("Buildings.XZPlane", false);

     _cracks->Init();
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
    float y1 = 0;     float y2 = 20.0f;
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
    dest->HullVao.AddVertexAttribute(AttrPositionFlag,
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
    glUseProgram(progs["Tetra.Solid"]);
    surfaceCam.Bind(glm::mat4());
    FOR_EACH(batch, _batches) {
        FOR_EACH(instance, batch->Instances) {
            _DrawBuilding(*batch->Template, *instance);
        }
    }

    // Draw floor
    glDisable(GL_CULL_FACE);
    glUseProgram(progs["Buildings.XZPlane"]);
    surfaceCam.Bind(glm::mat4());
    _emptyVao.Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void
Buildings::_DrawBuilding(BuildingTemplate& templ, BuildingInstance& instance)
{
    Programs& progs = Programs::GetInstance();
    vec3 xlate = vec3(instance.GroundPosition.x, 0, instance.GroundPosition.y);
    vec3 scale = vec3(instance.Radius, instance.Height, instance.Radius);

    float time = GetContext()->elapsedTime;
    bool boundariesOnly = (time < instance.ExplosionStart);

    glUseProgram(progs["Tetra.Solid"]);
    templ.CentroidTexture.Bind(0, "CentroidTexture");
    templ.BuildingVao.Bind();
    glUniform1f(u("CullY"), instance.EnableCullingPlane ? instance.CullingPlaneY : 999);
    glUniform3fv(u("Translate"), 1, ptr(xlate));
    glUniform1f(u("Height"), instance.Height);
    glUniform1f(u("Time"), time);
    glUniform3fv(u("Scale"), 1, ptr(scale));
    glUniform1f(u("Hue"), instance.Hue);
    glUniform1f(u("ExplosionStart"), instance.ExplosionStart);

    int n = boundariesOnly ? templ.BoundaryTetCount : templ.TotalTetCount;
    if (not boundariesOnly) {
        glEnable(GL_BLEND);
    }

    // Hmm, explosions still look ok when I do this:
    n = templ.BoundaryTetCount;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    vec3 scale = vec3(instance.Radius, instance.Height, instance.Radius);
    glUseProgram(progs["Tetra.Cracks"]);
    glUniform3fv(u("Translate"), 1, ptr(xlate));
    glUniform1f(u("Height"), instance.Height);
    glUniform3fv(u("Scale"), 1, ptr(scale));
    glUniform1f(u("Time"), time);
    glUniform1f(u("DepthOffset"), -0.0001f);
    glUniform4f(u("Color"), 0, 10, 10, 10);
    templ.CentroidTexture.Bind(0, "CentroidTexture");
    templ.BuildingVao.Bind();
    templ.CracksVao.Bind();
    glDrawArrays(GL_LINES, 0, 2 * templ.NumCracks);
}
