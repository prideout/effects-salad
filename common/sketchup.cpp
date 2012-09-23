#include "common/sketchup.h"
#include "common/jsonUtil.h"

using namespace Sketchup;
using namespace glm;

Scene::Scene() : _threshold(0.001)
{
    _recording = true;
    Plane ground;
    ground.Eqn = vec4(0, 1, 0, 0);
    _planes.push_back(ground);
    _history.append(Json::Value(Json::arrayValue));
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

CoplanarPath*
Scene::AddRectangle(float width, float height, const Plane* plane, vec2 offset)
{
    CoplanarPath* retval = 0;

    // TODO

    if (_recording) {
        appendJson(
            _history,
            "[ \"AddRectangle\", \"%8.8x\", %f, %f, \"%8.8x\", %s]",
            retval, width, height, plane, toString(offset) );
    }
    return retval;
}

void
Scene::PushPath(Path* poly, float delta, ConstPathList* pWalls)
{
    ConstPathList walls;
    // TODO

    if (_recording) {
        const char* handleList = toString((void**) &(walls[0]), walls.size());
        appendJson(
            _history,
            "[ \"PushPath\", \"%8.8x\", %f, %s]",
            poly, delta, handleList );
    }

    if (pWalls) {
        pWalls->swap(walls);
    }
}
