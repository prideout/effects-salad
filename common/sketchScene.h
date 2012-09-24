#pragma once
#include "common/typedefs.h"
#include "glm/glm.hpp"
#include "jsoncpp/json.h"

namespace sketch
{
    // For convenience, data structures of Scene such as Plane, Path,
    // and Edge are exposed in the public API.  However, clients should
    // take care to never create them from scratch or modify them directly.

    class Tessellator;
    struct Path;
    struct Edge;
    struct Plane;

    typedef std::vector<Path*> PathList;
    typedef std::vector<const Path*> ConstPathList;
    typedef std::vector<Edge*> EdgeList;
    typedef std::vector<Plane*> PlaneList;

    struct Plane
    {
        glm::vec4 Eqn;
        glm::vec3 GetNormal() const { return glm::vec3(Eqn); }
        glm::mat3 GetCoordSys() const;
        glm::vec3 GetCenterPoint() const;
    };

    // Closed path in 3-space consisting of arcs and line segments.  Cannot self-intersect.
    struct Path
    {
        EdgeList Edges;
        PathList Holes;
        bool IsHole;
        virtual ~Path() {}
    };

    // Path where all points lie in a plane.  This is the common case; the only way
    // to create non-coplanar paths is via arc extrusion.
    struct CoplanarPath : Path
    {
        sketch::Plane* Plane;
        glm::vec3 GetNormal() const { return Plane->GetNormal(); }
    };

    struct Edge
    {
        glm::uvec2 Endpoints;
        PathList Faces;
        virtual ~Edge() {}
    };

    // Cross the plane normal with the edge direction to determine which side
    // of the edge the arc lies on.  Arcs cannot be greater than 180 degrees.
    struct Arc : Edge
    {
        float Radius;
        sketch::Plane* Plane;
    };

    class Scene
    {
    public:

        const sketch::Plane* GroundPlane() const;

        // Create an axis-aligned rectangle.
        // Edge-sharing and point-sharing with existing paths occurs automatically.
        // This is the most common starting point for a building.
        CoplanarPath*
        AddRectangle(float width, float height, const sketch::Plane* plane, glm::vec2 offset);

        // Create an extrusion or change an existing extrusion, optionally returning the walls of the extrusion.
        // When creating a new extrusion, a hole is automatically created in the enclosing polygon.
        // Walls are automatically deleted when pushing an extrusion back to its original location.
        // When extruding causes the path to "meet" with an existing path in some way
        // (eg, sharing the edges or the same plane), their adjacency information is updated
        // accordingly and pointer-sharing occurs automatically.
        void
        PushPath(CoplanarPath* path, float delta, ConstPathList* walls = 0);

        // Useful if you have an array of window sills that you want extrude simultaneously.
        void
        PushPaths(PathList paths, float delta);

    public:

        // Sometimes you want to extrude in a custom direction; eg, a chimney from a slanted roof.
        void
        PushPath(CoplanarPath* path, glm::vec3 delta, ConstPathList* walls = 0);

        // Pushing a non-coplanar path is tricky because you have to push it past 
        // a certain point to be valid, and we need a reasonable heuristic for
        // figuring out which incident faces are existing extrusions.
        // Maybe we'll require existing extrusions to be 1:1 with edges.
        // That won't let you push a non-coplanar path after manually deleting
        // an extrusion face, but hopefully that's a corner case.
        bool
        PushTricky(Path* path, glm::vec3 delta, ConstPathList* walls = 0);

        // Attempts to find a path with two edges that contain the given points and split it.
        // If successful, returns the new edge that is common to the two paths.
        Edge*
        SplitPath(glm::vec3 a, glm::vec3 b);

        // This is useful for creating a slanted rooftop.  All incident coplanar paths are
        // adjusted automatically. As always, if translation causes the edge to "meet"
        // with existing paths or edges, pointer-sharing occurs automatically.
        void
        TranslateEdge(Edge* e, glm::vec3 delta);

    public:

        CoplanarPath*
        AddCircle(float radius, const sketch::Plane* plane, glm::vec2 offset);

        CoplanarPath*
        AddRegularPolygon(int numSides, float radius, const sketch::Plane* plane, glm::vec2 offset);

        // Finds or creates a new frame-of-reference.
        const sketch::Plane*
        GetPlane(glm::vec4 eqn);

        // Ditto.
        const sketch::Plane*
        GetPlane(float x, float y, float z, float w) { return GetPlane(glm::vec4(x, y, z, w)); }

        Scene();
        ~Scene();

    public:

        void
        ClearHistory();

        const Json::Value &
        GetHistory() const { return _history; }

        void
        RunCommands(const Json::Value& commandList);

        Json::Value
        Serialize() const;

    private:

        // Snaps the edges, vertices, and plane equation of the given path with existing objects
        // in the scene.  Updates everybody's adjacency information and shares pointers.
        // Returns true if the scene was mutated in any way.
        bool
        _FinalizePath(Path* path, float epsilon);

        // Create a new edge a push it into the given path.
        Edge*
        _AppendEdge(Path* path, unsigned int a, unsigned int b);

        // Push an existing edge into the given path.
        void
        _AppendEdge(Path* path, Edge* e);

        // Create a new point and return its index.
        unsigned int
        _AppendPoint(glm::vec3 p);

        // Ditto.
        unsigned int
        _AppendPoint(float x, float y, float z)  { return _AppendPoint(glm::vec3(x, y, z)); }

        // Returns true if the two paths meet at the given edge at ninety degrees.
        bool
        _IsOrthogonal(const CoplanarPath* p1, const Path* p2, const Edge* e);

        EdgeList
        _FindAdjacentEdges(unsigned int p, const Path*);

        glm::vec3
        _GetEdgeVector(Edge* e);

        sketch::Plane*
        _GetPlane(glm::vec3 p, glm::vec3 u, glm::vec3 v);

        Vec3List
        _WalkPath(const Path* p, float arcTessLength = 0) const;

        void
        _AdjustPathPlane(CoplanarPath* path, glm::vec4 newPlane);

        PathList _paths;
        EdgeList _edges;
        Vec3List _points;
        PlaneList _planes;
        const float _threshold;
        Json::Value _history;
        bool _recording;

        friend class Tessellator;
    };
}
