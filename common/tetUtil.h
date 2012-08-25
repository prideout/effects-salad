#include "common/typedefs.h"
#include "tetgen/tetgen.h"
#include "glm/glm.hpp"

namespace TetUtil
{
    // Thin wrapper for tetgen's "tetrahedralize" function.
    void TetsFromHull(const tetgenio& hull,
                      tetgenio* dest,
                      float qualityBound,
                      float maxVolume,
                      bool quiet);

    // Creates a circular ribbon, composing the rim out of quads.
    // Each of the two caps is a single facet, and each quad is a facet.
    // (tetgen defines a facet as a coplanar set of polygons)
    void HullWheel(glm::vec3 center,
                   float radius,
                   float width,
                   int numQuads,
                   tetgenio* dest);

    // Flip the orientation of all facets in 'B'
    // and combine the result with the facets in 'A'
    void HullDifference(const tetgenio& hullA,
                        const tetgenio& hullB,
                        tetgenio* dest);

    // Copy all facets from "hull" to dest
    void HullCombine(const tetgenio& hull,
                     tetgenio* dest);

    // Add a volumetric "hole" to a tetgen structure
    void SubtractRegion(tetgenio* dest,
                        const tetgenio& emptiness);

    // Builds an index buffer for drawing the hull of a tetmesh with triangles.
    void TrianglesFromHull(const tetgenio& hull,
                           Blob* indices);

    // Builds an index buffer for drawing all tetrahedra with triangles.
    void TrianglesFromTets(const tetgenio& tets,
                           Blob* indices);

    // Builds a non-indexed, interleaved VBO from a set of tetrahedra.
    void PointsFromTets(const tetgenio& tets,
                        VertexAttribMask requestedAttribs,
                        Blob* vbo);

    // Add "regions", which are defined by seed points that flood until hitting a facet.
    // Regions annotate the resulting tets with region id's.
    void AddRegions(const Vec3List& points,
                    tetgenio* dest);

    // Add "holes", which are defined by seed points that flood until hitting a facet.
    // The tetgen implementation seem to handle holes more robustly than regions.
    void AddHoles(const Vec3List& points,
                  tetgenio* dest);

    // Averages the corners of each tet and dumps the result into an array.
    void ComputeCentroids(Vec3List* centroids,
                          const tetgenio& tets);

    // Computes centroids and neighbor counts, and re-orders the given tet list.
    void SortTetrahedra(Vec4List* tetData,
                        tetgenio& tets,
                        int* boundaryTets = 0);
}
