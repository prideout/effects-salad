#include "common/typedefs.h"
#include "glm/glm.hpp"

namespace Sketchup
{
    struct Polygon;
    struct Edge;

    typedef std::vector<Polygon*> PolyList;
    typedef std::vector<Edge*> EdgeList;
    typedef std::vector<glm::vec4> PlaneList;

    struct Edge
    {
        glm::uvec2 Endpoints;
        PolyList Faces;
    };

    struct Polygon
    {
        EdgeList Edges;
        PolyList Holes;
    };

    struct FloatingPolygon : Polygon
    {
        glm::vec4* Plane;
    };

    struct InscribedPolygon : Polygon
    {
        Polygon* Outer;
    };

    class Scene
    {
    public:
        static glm::vec4 Y()
        {
            return glm::vec4(0, 1, 0, 0);
        }
        FloatingPolygon* AddRegularPolygon(int numSides, float radius, glm::vec4 plane, glm::vec2 offset);
        InscribedPolygon* AddRegularPolygon(int numSides, float radius, Polygon* outer,  glm::vec2 offset);
        void PushPolygon(Polygon* poly, float delta, PolyList* walls = 0);
        ~Scene()
        {
            // delete everything in _polys
            // delete everything in _edges
        }
    private:
        PolyList _polys;
        EdgeList _edges;
        Vec3List _points;
        PlaneList _planes;
    };
}
