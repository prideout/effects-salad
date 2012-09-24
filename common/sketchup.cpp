// TODO
// ----
// Does GLM already have stringify functions?
// Sanity check by serializing Scene to JSON
//     Pretty-print from buildingGrowth.cppp
//
// Rename sketchup to sketchScene, and
//    create sketchTess to prep for OpenGL rendering
//    expose the path list!
// OpenGL drawing
// Create sketchPlayback and tween.h for animation

#include "common/sketchup.h"
#include "common/sketchUtil.h"
#include "common/jsonUtil.h"
#include "glm/gtx/norm.hpp"
#include "pez/pez.h"

using namespace sketch;
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

    unsigned int a = _AppendPoint(AddOffset(offset + vec2(-hw, -hh), plane));
    unsigned int b = _AppendPoint(AddOffset(offset + vec2(+hw, -hh), plane));
    unsigned int c = _AppendPoint(AddOffset(offset + vec2(+hw, +hh), plane));
    unsigned int d = _AppendPoint(AddOffset(offset + vec2(-hw, +hh), plane));

    retval = new CoplanarPath();
    _AppendEdge(retval, a, b);
    _AppendEdge(retval, b, c);
    _AppendEdge(retval, c, d);
    _AppendEdge(retval, d, a);
    retval->IsHole = false;
    retval->Plane = const_cast<Plane*>(plane);
    _FinalizePath(retval, _threshold);
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
Scene::PushPath(CoplanarPath* path, float delta, ConstPathList* pWalls)
{
    ConstPathList walls;
    vec3 pushVector = delta * path->GetNormal();

    FOR_EACH(e, path->Edges) {
        bool alreadyExtruded = false;
        FOR_EACH(f, (*e)->Faces) {
            if (*f == path) {
                continue;
            }
            if (_IsOrthogonal(path, *f, *e)) {
                pezFatal("Re-pushing a face isn't supported yet.");
                walls.push_back(*f);
                alreadyExtruded = true;
            }
        }
        if (!alreadyExtruded) {
            Arc* arc = dynamic_cast<Arc*>(*e);
            if (arc) {
                pezFatal("Arc extrusion isn't supported yet.");
            }

            Edge* ab = *e;
            unsigned int a = ab->Endpoints.x;
            unsigned int b = ab->Endpoints.y;
            unsigned int c = _AppendPoint(_points[b] + pushVector);
            unsigned int d = _AppendPoint(_points[a] + pushVector);
            CoplanarPath* f = new CoplanarPath();
            _AppendEdge(f, ab);
            _AppendEdge(f, b, c);
            _AppendEdge(f, c, d);
            Edge* da = _AppendEdge(f, d, a);
            f->IsHole = false;
            vec3 vab = _GetEdgeVector(ab);
            vec3 vda = _GetEdgeVector(da);
            f->Plane = _GetPlane(_points[a], vab, -vda);
            _paths.push_back(f);
        }
    }

    bool finished = false;
    while (not finished) {
        bool mutated = false;
        mutated = mutated or _FinalizePath(path, _threshold);
        FOR_EACH(w, *pWalls) {
            mutated = mutated or _FinalizePath(const_cast<Path*>(*w), _threshold);
        }
        finished = not mutated;
    }

    if (_recording) {
        const char* handleList = toString((void**) &(walls[0]), walls.size());
        appendJson(
            _history,
            "[ \"PushPath\", \"%8.8x\", %f, %s]",
            path, delta, handleList );
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

void
Scene::_AppendEdge(Path* path, Edge* e)
{
    e->Faces.push_back(path);
    path->Edges.push_back(e);
}

// Snaps the edges, vertices, and plane equation of the given path with existing objects
// in the scene.  Updates everybody's adjacency information and shares pointers.
// Returns true if the scene was mutated in any way.
bool
Scene::_FinalizePath(Path* path, float epsilon)
{
    CoplanarPath* coplanar = dynamic_cast<CoplanarPath*>(path);
    if (coplanar) {
        // TODO check if the plane can be snapped.  If so, adjust points accordingly.
    }

    // TODO check if the points can be snapped to "older" points.
    // TODO check if the edges can be combined with existing edges.
    // Note that a -> b is equivalent to b -> a.

    return false;
}

unsigned int
Scene::_AppendPoint(vec3 p)
{
    _points.push_back(p);
    return _points.size() - 1;
}

static vec3
_perp(vec3 a)
{
    vec3 c = vec3(0, 0, -1);
    vec3 b = cross(a, c);
    if (length2(b) < 0.01f) {
        c = vec3(1, 0, 0);
        b = cross(a, c);
    }
    return b;
}

mat3
Plane::GetCoordSys() const
{
    vec3 s = _perp(GetNormal());
    vec3 t = GetNormal();
    vec3 r = cross(t, s);
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

// Since the second path is potentially non-coplanar, we examine
// incident edges rather than plane normals.
bool
Scene::_IsOrthogonal(const CoplanarPath* p1, const Path* p2, const Edge* e)
{
    EdgeList e0 = _FindAdjacentEdges(e->Endpoints.x, p2);
    FOR_EACH(pe, e0) {
        if (*pe == e) {
            continue;
        }
        if (dot(_GetEdgeVector(*pe), p1->GetNormal()) > _threshold) {
            return false;
        }
    }

    EdgeList e1 = _FindAdjacentEdges(e->Endpoints.y, p2);
    FOR_EACH(pe, e1) {
        if (*pe == e) {
            continue;
        }
        if (dot(_GetEdgeVector(*pe), p1->GetNormal()) > _threshold) {
            return false;
        }
    }
    return true;
}

EdgeList
Scene::_FindAdjacentEdges(unsigned int p, const Path* path)
{
    EdgeList edges;
    FOR_EACH(e, path->Edges) {
        if ((*e)->Endpoints.x == p ||
            (*e)->Endpoints.y == p) {
            edges.push_back(*e);
        }
    }
    return edges;
}

vec3
Scene::_GetEdgeVector(Edge* e)
{
    return _points[e->Endpoints.y] - _points[e->Endpoints.x];
}


Plane*
Scene::_GetPlane(vec3 p, vec3 u, vec3 v)
{
    u = normalize(u);
    v = normalize(v);
    vec3 n = cross(u, v);
    float w = -dot(p, n);
    vec4 eqn = vec4(n.x, n.y, n.z, w);

    Plane plane;
    plane.Eqn = eqn;
    _planes.push_back(plane);
    return &(_planes.back());
}

Json::Value
Scene::Serialize() const
{
    Json::Value root;
    FOR_EACH(p, _paths) {
        Vec3List points = _WalkPath(*p);
        appendJson(root, "%s", toString(points));
    }
    return root;
}

Vec3List
Scene::_WalkPath(const Path* path, float arcTessLength) const
{
    Vec3List vecs;
    FOR_EACH(e, path->Edges) {
        vec3 v = _points[(*e)->Endpoints.x];
        vecs.push_back(v);
    }
    return vecs;
}
