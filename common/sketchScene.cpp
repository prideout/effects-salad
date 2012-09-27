#include "common/sketchScene.h"
#include "common/sketchUtil.h"
#include "common/jsonUtil.h"
#include "glm/gtx/norm.hpp"
#include "pez/pez.h"
#include "glm/gtx/string_cast.hpp"

#include <algorithm>
#include <set>

using namespace sketch;
using namespace glm;
using namespace std;

Scene::Scene() : _threshold(0.001)
{
    _recording = true;
    Plane* ground = new Plane;
    ground->Eqn = vec4(0, 1, 0, 0);
    _planes.push_back(ground);
    _history.append(Json::Value(Json::arrayValue));
}

Scene::~Scene()
{
    FOR_EACH(p, _paths) { delete *p; }
    FOR_EACH(h, _holes) { delete *h; }
    FOR_EACH(p, _planes) { delete *p; }
    FOR_EACH(e, _edges) { delete *e; }
}

const Plane*
Scene::GroundPlane() const
{
    return _planes.front();
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

// Inscribe a path and create a hole in the outer path.
CoplanarPath*
Scene::AddInscribedRectangle(float width, float height,
                             sketch::CoplanarPath* outer, glm::vec2 pathOffset)
{
    vec3 outerCenter = _GetCentroid(outer);
    Plane* plane = outer->Plane;
    vec3 wsRectCenter = outerCenter + plane->GetCoordSys() * vec3(pathOffset.x, 0, pathOffset.y);
    mat3 inverseCoordSys = inverse(plane->GetCoordSys());
    vec3 planeOffset = inverseCoordSys * (wsRectCenter - plane->GetCenterPoint());
    vec2 offset = vec2(planeOffset.x, planeOffset.z);
    CoplanarPath* inner = AddRectangle(width, height, plane, offset);
    CoplanarPath* hole = new CoplanarPath();
    FOR_EACH(edge, inner->Edges) {
        _AppendEdge(hole, *edge);
    }
    hole->Plane = inner->Plane;
    outer->Holes.push_back(hole);
    _holes.push_back(hole);

    if (_recording) {
        appendJson(
            _history,
            "[ \"AddInscribedRectangle\", \"%8.8x\", %f, %f, \"%8.8x\", %s]",
            inner, width, height, outer, toString(pathOffset) );
    }

    return inner;
}


void
Scene::PushPath(CoplanarPath* path, float delta, PathList* pWalls)
{
    PathList walls;
    vec3 pushVector = delta * path->GetNormal();

    // Extrude edges into new coplanar paths
    EdgeList newEdges;
    bool alreadyExtruded = false;
    set<unsigned> vertsToPush;
    FOR_EACH(e, path->Edges) {
        FOR_EACH(f, (*e)->Faces) {
            if (*f == path) {
                continue;
            }
            CoplanarPath* cp = dynamic_cast<CoplanarPath*>(*f);
            if (!cp) {
                pezFatal("Non-coplanar paths aren't really supported yet.");
            }
            if (IsOrthogonal(path, cp, _threshold)) {
                uvec2 ends = (*e)->Endpoints;
                vertsToPush.insert(ends.x);
                vertsToPush.insert(ends.y);
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
            Edge* cd = _AppendEdge(f, c, d);
            Edge* da = _AppendEdge(f, d, a);
            vec3 vab = _GetEdgeVector(ab);
            vec3 vda = _GetEdgeVector(da);
            f->Plane = _GetPlane(_points[a], vab, -vda);
            _VerifyPlane(f, "Faulty path plane in PushPath 1.");
            _paths.push_back(f);
            walls.push_back(f);
            newEdges.push_back(cd);
        }
    }

    FOR_EACH(v, vertsToPush) {
        _points[*v] += pushVector;
    }

    vec4 eqn = path->Plane->Eqn;
    eqn.w += delta;

    if (!alreadyExtruded) {

        // Update the path's edges
        FOR_EACH(e, path->Edges) {
            PathList::iterator pe =
                std::find((*e)->Faces.begin(), (*e)->Faces.end(), path);
            (*e)->Faces.erase(pe);
        }
        path->Edges.clear();
        path->Edges.swap(newEdges);
        FOR_EACH(e, path->Edges) {
            (*e)->Faces.push_back(path);
        }

        // Update the path plane
        Plane* newPlane = const_cast<Plane*>(GetPlane(eqn));
        path->Plane = newPlane;

    } else {
        path->Plane->Eqn = eqn;
    }

    _VerifyPlane(path, "Faulty path plane in PushPath 2.");

    // Clean and consolidate
    bool finished = false;
    while (not finished) {
        bool mutated = false;
        mutated = mutated or _FinalizePath(path, _threshold);
        FOR_EACH(w, walls) {
            mutated = mutated or _FinalizePath(const_cast<Path*>(*w), _threshold);
        }
        finished = not mutated;
    }

    // Record for posterity
    if (_recording) {
        const char* handleList = toString((void**) &(walls[0]), walls.size());
        appendJson(
            _history,
            "[ \"PushPath\", \"%8.8x\", %f, %s]",
            path, delta, handleList );
    }

    // Return the extrusion walls if the client is interested
    if (pWalls) {
        pWalls->swap(walls);
    }
}

Edge*
Scene::_AppendEdge(Path* path, unsigned int a, unsigned int b)
{
    FOR_EACH(i, _edges) {
        if ((*i)->Endpoints == uvec2(a, b) ||
            (*i)->Endpoints == uvec2(b, a)) {
            _AppendEdge(path, *i);
            return *i;
        }
    }
    Edge* e = new Edge();
    e->Endpoints = uvec2(a, b);
    _AppendEdge(path, e);
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
    FOR_EACH(i, _points) {
        if (distance2(*i, p) < _threshold) {
            return i - _points.begin();
        }
    }
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
    float w = dot(p, n);
    vec4 eqn = vec4(n.x, n.y, n.z, w);

    if (eqn.w < 0) {
        eqn = -eqn;
    }

    Plane* plane = new Plane;
    plane->Eqn = eqn;
    _planes.push_back(plane);
    return plane;
}

// Canonicalize by scaling length of eqn.xyz with eqn.w.
const Plane*
Scene::GetPlane(vec4 eqn)
{
    vec3 n = vec3(eqn);
    float l = length(n);
    eqn = vec4(n / l, eqn.w * l);

    if (eqn.w < 0) {
        eqn = -eqn;
    }

    // TODO check if there's an existing plane within _threshold
    // If so, return that instead.

    Plane* p = new Plane;
    p->Eqn = eqn;
    _planes.push_back(p);
    return p;
}

Json::Value
Scene::Serialize() const
{
    Json::Value root;
    FOR_EACH(p, _paths) {
        Vec3List points;
        _WalkPath(*p, &points);
        appendJson(root, "%s", toString(points));
    }
    return root;
}

void
Scene::_WalkPath(const CoplanarPath* path, Vec2List* dest, float arcTessLength) const
{
    Vec2List vecs;
    if (path->Edges.empty()) {
        return;
    }

    mat3 planeInverse = inverse(path->Plane->GetCoordSys());
    vec3 planeCenter = path->Plane->GetCenterPoint();

    uvec2 previous = path->Edges.front()->Endpoints;
    FOR_EACH(e, path->Edges) {

        Arc* arc = dynamic_cast<Arc*>(*e);
        if (arc) {
            pezFatal("Arc walking isn't supported yet.");
        }

        // Edges can have inconsistent winding in our representation,
        // so swap the two points if needed.
        uvec2 xy = (*e)->Endpoints;
        if (xy.x != previous.y) {
            xy = uvec2(xy.y, xy.x);
        }

        // Poor man's way of returning only the outer path
        // for paths that have holes.
        if (xy.x != previous.x && xy.x != previous.y &&
            xy.y != previous.x && xy.y != previous.y) {
            break;
        }
        previous = xy;
        vec3 v = _points[xy.x];
        VerifyPlane(v, path->Plane, "Faulty path plane in WalkPath.");

        // Convert to the coordinate system of the plane.
        vec3 planeOffset = planeInverse * (v - planeCenter);
        vec2 v2 = vec2(planeOffset.x, planeOffset.z);

        vecs.push_back(v2);
    }

    dest->swap(vecs);
}

void
Scene::_WalkPath(const Path* path, Vec3List* dest, float arcTessLength) const
{
    Vec3List vecs;
    if (path->Edges.empty()) {
        return;
    }

    uvec2 previous = path->Edges.front()->Endpoints;
    FOR_EACH(e, path->Edges) {

        Arc* arc = dynamic_cast<Arc*>(*e);
        if (arc) {
            pezFatal("Arc walking isn't supported yet.");
        }

        // Edges can have inconsistent winding in our representation,
        // so swap the two points if needed.
        uvec2 xy = (*e)->Endpoints;
        if (xy.x != previous.y) {
            xy = uvec2(xy.y, xy.x);
        }

        // Poor man's way of returning only the outer path
        // for paths that have holes.
        if (xy.x != previous.x && xy.x != previous.y &&
            xy.y != previous.x && xy.y != previous.y) {
            break;
        }
        previous = xy;
        vec3 v = _points[xy.x];
        vecs.push_back(v);
    }

    dest->swap(vecs);
}

void
Scene::_VerifyPlane(const CoplanarPath* path, const char* msg) const
{
    FOR_EACH(e, path->Edges) {
        uvec2 xy = (*e)->Endpoints;
        vec3 x = _points[xy.x];
        vec3 y = _points[xy.y];
        VerifyPlane(x, path->Plane, msg);
        VerifyPlane(y, path->Plane, msg);
    }
}

vec3
Scene::_GetCentroid(const Path* path) const
{
    vec3 center;
    Vec3List vlist;
    _WalkPath(path, &vlist);
    FOR_EACH(p, vlist) {
        center += *p;
    }
    return center / float(vlist.size());
}
