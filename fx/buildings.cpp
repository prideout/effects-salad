// Render hulls for pre-exploded bldgs,
// Skip draw calls for dead buildings
// Generate instances and templates with loops rather than unrolled code.
// Stack buildings for a slightly more interesting effect
// Build city using simplistic 2D packing of triangles, pentagons, circles, and squares
// Secondary explosion effect: some tets pop off the top, right before explosion
//
// We're only rendering boundary tets right now, but we're not exploiting
// the massive memory savings.  Maybe buildings should have floors?
//
// Camera
// Radial blur
// DOF effect
// Look at Akira references
// Smoke

#include "glm/gtc/type_ptr.hpp"
#include "fx/buildings.h"
#include "glm/gtx/rotate_vector.hpp"
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
    StartBuildingCreator(&params0);

    ThreadParams params1 = {0};
    params1.Thickness = 2.5f;
    params1.TopRadius =  1.0f;
    params1.TetSize = 0.1f;
    params1.NumSides = 4;
    params1.Dest = &_templates[1];
    StartBuildingCreator(&params1);

    ThreadParams params2 = {0};
    params2.Thickness = 2.5f;
    params2.TetSize = 0.1f;
    params2.TopRadius =  1.2f;
    params2.NumSides = 3;
    params2.Dest = &_templates[2];
    StartBuildingCreator(&params2);

    ThreadParams params3 = {0};
    params3.Thickness = 2.5f;
    params3.TetSize = 0.1f;
    params3.TopRadius = 1;
    params3.NumSides = 24;
    params3.Dest = &_templates[3];
    StartBuildingCreator(&params3);

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

     WaitForBuildingCreators();

     // Compile shaders
     Programs& progs = Programs::GetInstance();
     progs.Load("Tetra.Cracks", false);
     progs.Load("Tetra.Solid", false);
     progs.Load("Buildings.XZPlane", false);

     _cracks->Init();
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
