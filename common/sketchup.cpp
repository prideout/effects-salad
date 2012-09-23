// TODO
// ----
// Ensure that _AddOffset and GetCoordSys make sense
// PushPath
// Rename sketchup to sketchScene, and
//    create sketchTess to prep for OpenGL rendering
// OpenGL drawing
// Create sketchPlayback and tween.h for animation

#include "common/sketchup.h"
#include "common/jsonUtil.h"
#include "glm/gtx/norm.hpp"

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
    FOR_EACH(p, _paths) { delete *p; }
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

    float hw = width / 2;
    float hh = height / 2;

    unsigned int a = _AppendPoint(_AddOffset(offset + vec2(-hw, -hh), plane));
    unsigned int b = _AppendPoint(_AddOffset(offset + vec2(+hw, -hh), plane));
    unsigned int c = _AppendPoint(_AddOffset(offset + vec2(+hw, +hh), plane));
    unsigned int d = _AppendPoint(_AddOffset(offset + vec2(-hw, +hh), plane));

    retval = new CoplanarPath();
    _AppendEdge(retval, a, b);
    _AppendEdge(retval, b, c);
    _AppendEdge(retval, c, d);
    _AppendEdge(retval, d, a);

    retval->IsHole = false;
    retval->Plane = const_cast<Plane*>(plane);

    _FinalizeCoplanarPath(retval, _threshold);

    _paths.push_back(retval);

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

Edge*
Scene::_AppendEdge(Path* path, unsigned int a, unsigned int b)
{
    Edge* e = new Edge();
    e->Endpoints = uvec2(a, b);
    e->Faces.push_back(path);
    path->Edges.push_back(e);
    return e;
}

// Snaps the edges, vertices, and plane equation of the given path with existing objects
// in the scene.  Updates everybody's adjacency information and shares pointers.
void
Scene::_FinalizePath(Path* path, float epsilon)
{
    // TODO check if the points can be snapped to "older" points.

    // TODO check if the edges can be combined with existing edges.
    // Note that a -> b is equivalent to b -> a.
}

// Snaps the edges, vertices, and plane equation of the given path with existing objects
// in the scene.  Updates everybody's adjacency information and shares pointers.
void
Scene::_FinalizeCoplanarPath(CoplanarPath* path, float epsilon)
{
    // TODO check if the plane can be snapped.  If so, adjust points accordingly.
    
    _FinalizePath(path, epsilon);
}

unsigned int
Scene::_AppendPoint(vec3 p)
{
    _points.push_back(p);
    return _points.size() - 1;
}

vec3
Scene::_AddOffset(vec2 p2, const Plane* plane)
{
    vec3 p3 = plane->GetCenterPoint();
    p3 += plane->GetCoordSys() * vec3(p2.x, 0, p2.y);
    return p3;
}

static vec3
_perp(vec3 a)
{
    vec3 c = vec3(1, 0, 0);
    vec3 b = cross(a, c);
    if (length2(b) < 0.01f) {
        c = vec3(0, 1, 0);
        b = cross(a, c);
    }
    return b;
}

mat3
Plane::GetCoordSys() const
{
    vec3 s = _perp(GetNormal());
    vec3 t = GetNormal();
    vec3 r = cross(s, t);
    return mat3(s, t, r);
}

vec3
Plane::GetCenterPoint() const
{
    return Eqn.w * GetNormal();
}

// Canonicalize by scaling length of eqn.xyz with eqn.w.
const Plane*
Scene::GetPlane(vec4 eqn)
{
    vec3 n = vec3(eqn);
    float l = length(n);
    eqn = vec4(n / l, eqn.w * l);

    // TODO check if there's an existing plane within _threshold
    // If so, return that instead.

    Plane p;
    p.Eqn = eqn;
    _planes.push_back(p);
    return &(_planes.back());
}
