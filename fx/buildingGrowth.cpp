// TODO
// ----
// OpenGL drawing
// Create sketchPlayback and tween.h for animation

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
    const float width = 2;
    const float depth = 1;

    CoplanarPath* rect =
        _sketch.AddRectangle(width, depth, ground, offset);

    float height = 1;
    ConstPathList walls;
    _sketch.PushPath(rect, height, &walls);

    if (false) {
        Json::Value root = _sketch.Serialize();
        Json::StyledWriter writer;
        cout << writer.write(root) << endl;
    }

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

    camera->eye.x = 10;
    camera->eye.y = 30 - time / 10.0f;
    camera->eye.z = 60;
    camera->center.y = 20;
}

void
BuildingGrowth::Draw()
{
    _tess->PushToGpu(_vao);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Programs& progs = Programs::GetInstance();
    PerspCamera surfaceCam = GetContext()->mainCam;

    glUseProgram(progs["Sketch.Facets"]);
    surfaceCam.Bind(glm::mat4());
    
    _vao.Bind();
    glDrawElements(GL_TRIANGLES, _vao.indexCount, GL_UNSIGNED_INT, 0);
}
