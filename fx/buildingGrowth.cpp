// TODO
// ----
// Fix the leak seen with valgrind
// Create sketchPlayback and tween.h for animation
// Add a routine that cleans up dangling planes, edges, and points.
// Per-path dirty flags in PullFromScene would result in huge speedup
//    Might as well use DrawArrays instead of DrawElements
//    Keep a set of CPU-side vectors for the paths, but only a single VBO
//    Probably fine to re-upload the entire VBO if ANY path is dirty

#include "glm/gtx/rotate_vector.hpp"
#include "common/vao.h"
#include "common/programs.h"
#include "common/camera.h"
#include "common/demoContext.h"
#include "fx/buildingGrowth.h"
#include "glm/gtx/string_cast.hpp"

using namespace sketch;
using namespace std;
using namespace glm;

BuildingGrowth::BuildingGrowth() : _tess(0)
{
}

BuildingGrowth::~BuildingGrowth()
{
    delete _tess;
}

void
BuildingGrowth::Init()
{
    if (_tess) {
        return;
    }

    const Plane* ground = _sketch.GroundPlane();
    glm::vec2 offset(0, 0);
    const float width = 8;
    const float depth = 4;

    CoplanarPath* rect =
        _sketch.AddRectangle(width, depth, ground, offset);

    float height = 4;
    PathList walls;
    _sketch.PushPath(rect, height, &walls);
    _sketch.PushPath(rect, -1, &walls);
    _roof = rect;

    CoplanarPath* wall;
    wall = dynamic_cast<CoplanarPath*>(walls[0]);
    rect = _sketch.AddInscribedRectangle(1, 1, wall, vec2(0, 0));
    PathList walls2;
    _sketch.PushPath(rect, 1, &walls2);
    {
        wall = dynamic_cast<CoplanarPath*>(walls2[0]);
        rect = _sketch.AddInscribedRectangle(0.5, 0.5, wall, vec2(0, 0));
        _sketch.PushPath(rect, 0.5);
    }
    wall = dynamic_cast<CoplanarPath*>(walls[1]);
    rect = _sketch.AddInscribedRectangle(1, 1.5, wall, vec2(0, 0));
    _sketch.PushPath(rect, -0.5);

    _sketch.EnableHistory(false);

    _tess = new Tessellator(_sketch);
    _tess->PullFromScene();

    Programs& progs = Programs::GetInstance();
    progs.Load("Sketch.Facets", true);
}

void
BuildingGrowth::Update()
{
    float time = GetContext()->elapsedTime;
    _sketch.PushPath(_roof, 0.02 * sin(4 * time));
    _tess->PullFromScene();

    PerspCamera* camera = &GetContext()->mainCam;
    camera->eye.x = 2;
    camera->eye.y = 7;
    camera->eye.z = 15;
    camera->center.y = 2;
    camera->eye = glm::rotateY(camera->eye, time * -48);
}

void
BuildingGrowth::Draw()
{
    _tess->PushToGpu(_vao);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    Programs& progs = Programs::GetInstance();
    PerspCamera surfaceCam = GetContext()->mainCam;

    glUseProgram(progs["Sketch.Facets"]);
    surfaceCam.Bind(glm::mat4());
    
    _vao.Bind();
    glDrawElements(GL_TRIANGLES, _vao.indexCount, GL_UNSIGNED_INT, 0);
}
