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
}
