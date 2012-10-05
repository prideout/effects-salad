// TODO LIST
// --------
// Windows
// Restore Parabolas (see chipped branch)
// SSAO
// Camera shake
// White-out at end
// Viewport cracks

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "common/tetUtil.h"
#include "common/init.h"
#include "common/programs.h"
#include "common/camera.h"
#include "common/demoContext.h"
#include "tween/CppTweener.h"
#include "fx/buildings.h"
#include "fx/buildingThreads.h"

using namespace std;
using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec2;

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

Buildings::Buildings(bool explode) : Effect()
{
    _explode = explode;
    _cracks = new CracksEffect(this);
}

Buildings::~Buildings()
{
    delete _cracks;
}

void
Buildings::Init()
{
    if (!_batches.empty()) {
        return;
    }

    // Ensure that each instance of the city is the same by resetting the
    // random seed to a constant value.
    srand(42);

    // Populate the template parameters.  This should perhaps be moved to JSON.
    #include "fx/buildings.inl"
    
    // Kick off the threads that tetify the building templates
    for (size_t i = 0; i < _threadParams.size(); ++i) {
        _threads.push_back(new tthread::thread(GenerateBuilding,
            _threadParams[i]));
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

            if (!_explode) {
                inst.ExplosionStart = 1000.0f;
            }

            inst.Hue = 0.4 + 0.2 * (rand() % 100) / 100.0f;

            BuildingBatch& batch = _batches[templ];
            batch.Instances.push_back(inst);

            groundPos.y += cellSize.y;
        }
        groundPos.x += cellSize.x;
    }
    
    // Compile shaders if they haven't been compiled already
    Programs& progs = Programs::GetInstance();
    progs.Load("Tetra.Cracks", "Tetra.Cracks.FS", "Tetra.Solid.VS");
    progs.Load("Tetra.Solid", false);
    progs.Load("Buildings.XZPlane", false);
    progs.Load("Buildings.Facets", true);
    
    // Misc initialization
    _emptyVao.InitEmpty();
    if (_explode) {
        _cracks->Init();
    }
}

void
Buildings::Update()
{
    // Wait for the tetrahedralization to finish
    if (_threads.size()) {
        printf("Waiting for tet threads to join...\n");
        for (size_t i = 0; i < _threads.size(); ++i) {
            _threads[i]->join();
            if (_explode) {
                printf("Thread %lu of %lu has completed.\n", i+1, _threads.size());
            }
            UploadBuilding(*_threadParams[i]);
            delete _threads[i];
        }
        FOR_EACH(p, _threadParams) {
            delete *p;
        }
        _threads.clear();
        _threadParams.clear();
    }

    const bool Looping = true;
    if (not Looping) {
        GetContext()->duration = std::numeric_limits<float>::infinity();
    }

    float time = GetContext()->elapsedTime;
    PerspCamera* camera = &GetContext()->mainCam;

    if (_explode) {
        camera->eye.x = 0;
        camera->eye.y = 35;
        camera->eye.z = 70;
        camera->center.y = 20;
        camera->eye = glm::rotateY(camera->eye, time * 48);
    } else {
        tween::Quad tweener;

        camera->eye.x = 8 - time / 1.5f;
        camera->eye.y = tweener.easeOut(time, 40, -20, 5);
        camera->eye.z = 60;

        float starty = 60;
        float endy = 20;
        float swingDuration = 1;
        
        float t = time > swingDuration ? swingDuration : time;
        camera->center.y = tweener.easeOut(t, starty, endy-starty, swingDuration);
    }

    _cracks->Update();
}

void
Buildings::Draw()
{
    // Don't draw until the tet threads have joined.
    if (_threads.size()) {
        return;
    }

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
    if (true) {
        glDisable(GL_CULL_FACE);
        glUseProgram(progs["Buildings.XZPlane"]);
        surfaceCam.Bind(glm::mat4());
        _emptyVao.Bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
    // Don't draw if the tet threads haven't joined yet.
    if ( _buildings->_threads.size()) {
        return;
    }

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

    const float ExplosionDuration = 1.5;
    bool completelyDestroyed = (time > instance.ExplosionStart + ExplosionDuration);

    if (completelyDestroyed) {
        return;
    }

    // Near the end, put EVERYTHING on fire!
    float explosionStart = instance.ExplosionStart;
    float apocalypseTime = 6;
    if (time > apocalypseTime && explosionStart > 900.0f) {
        time -= apocalypseTime;
        explosionStart = 3;
    }


    Programs& progs = Programs::GetInstance();
    vec3 xlate = vec3(instance.GroundPosition.x, 0, instance.GroundPosition.y);
    vec3 scale = instance.Scale;
    glUseProgram(progs["Tetra.Cracks"]);
    glUniform1f(u("CullY"), 999);
    glUniform3fv(u("Translate"), 1, ptr(xlate));
    glUniform3fv(u("Scale"), 1, ptr(scale));
    glUniform1f(u("Time"), time);
    glUniform1f(u("DepthOffset"), -0.0001f);
    glUniform4f(u("Color"), 1, 0.2, 0.3, 10);
    glUniform1f(u("ExplosionStart"), explosionStart);
    templ.CentroidTexture.Bind(0, "CentroidTexture");
    templ.BuildingVao.Bind();
    templ.CracksVao.Bind();
    glDrawArrays(GL_LINES, 0, 2 * templ.NumCracks);
}
