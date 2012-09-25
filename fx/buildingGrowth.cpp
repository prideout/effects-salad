// TODO
// ----
// Inscribing a path should create a "hole" path; incorp poly2tri
// Create sketchPlayback and tween.h for animation
// Add a routine that cleans up dangling planes, edges, and points.

#include "glm/gtx/rotate_vector.hpp"
#include "common/vao.h"
#include "common/programs.h"
#include "common/camera.h"
#include "common/demoContext.h"
#include "fx/buildingGrowth.h"

using namespace sketch;
using namespace std;

BuildingGrowth::BuildingGrowth()
{
}

BuildingGrowth::~BuildingGrowth()
{
    delete _tess;
}

void
BuildingGrowth::Init()
{
    const Plane* ground = _sketch.GroundPlane();
    glm::vec2 offset(0, 0);
    const float width = 8;
    const float depth = 4;

    CoplanarPath* rect =
        _sketch.AddRectangle(width, depth, ground, offset);

    float height = 4;
    PathList walls;
    _sketch.PushPath(rect, height, &walls);

    CoplanarPath* wall;

    wall = dynamic_cast<CoplanarPath*>(walls[0]);
    rect = _sketch.AddRectangle(1, 1, wall, vec2(0, 0));
    PathList walls2;
    _sketch.PushPath(rect, 1, &walls2);
    {
        wall = dynamic_cast<CoplanarPath*>(walls2[0]);
        rect = _sketch.AddRectangle(0.5, 0.5, wall, vec2(0, 0));
        _sketch.PushPath(rect, 0.5);
    }

    wall = dynamic_cast<CoplanarPath*>(walls[1]);
    rect = _sketch.AddRectangle(1, 1.5, wall, vec2(0, 0));
    _sketch.PushPath(rect, -0.5);

    _tess = new Tessellator(_sketch);
    _tess->PullFromScene();

    Programs& progs = Programs::GetInstance();
    progs.Load("Sketch.Facets", true);
}

void
BuildingGrowth::Update()
{
    _tess->PullFromScene();

    float time = GetContext()->elapsedTime;
    PerspCamera* camera = &GetContext()->mainCam;

    camera->eye.x = 2;
    camera->eye.y = -7 + 14 * abs(sin(time / 2));
    camera->eye.z = 15;
    camera->center.y = 2;
    camera->eye = glm::rotateY(camera->eye, time * 48);
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
