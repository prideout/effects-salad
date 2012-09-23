#include "common/sketchup.h"

using namespace Sketchup;
using namespace glm;

Scene::Scene() : _threshold(0.001)
{
    Plane ground;
    ground.Eqn = vec4(0, 1, 0, 0);
    _planes.push_back(ground);
}

Scene::~Scene()
{
    FOR_EACH(p, _polys) { delete *p; }
    FOR_EACH(e, _edges) { delete *e; }
}

const Plane*
Scene::GroundPlane() const
{
    return &(_planes.front());
}
