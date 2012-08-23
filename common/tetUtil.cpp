#include "common/tetUtil.h"
#include "glm/gtx/constants.inl"

// Thin wrapper for tetgen's "tetrahedralize" function.
void
TetUtil::TetsFromHull(const tetgenio& hull,
                      tetgenio* dest,
                      float qualityBound,
                      float maxVolume)
{
    char configString[128];
    sprintf(configString, "Qpq%.3fa%.7f", qualityBound, maxVolume);
    tetrahedralize(configString, (tetgenio*) &hull, dest);
}

// Creates a circular ribbon, composing the rim out of quads.
// Each of the two caps is a single facet, and each quad is a facet.
// (tetgen defines a facet as a coplanar set of polygons)
void
TetUtil::HullWheel(glm::vec3 center,
                   float radius,
                   float width,
                   int numQuads,
                   tetgenio* dest)
{
    dest->numberofpoints = numQuads * 2;
    dest->pointlist = new float[dest->numberofpoints * 3];
    const float twopi = 2 * glm::pi<float>();
    const float dtheta = twopi / numQuads;
    float* coord = dest->pointlist;
    const float z0 = -width / 2;
    const float z1 = width / 2;
    
    // Rim points:
    for (float theta = 0; theta < twopi - dtheta / 2; theta += dtheta) {
        float x = radius * std::cos(theta);
        float y = radius * std::sin(theta);
        *coord++ = x;
        *coord++ = y;
        *coord++ = z0;
        *coord++ = x;
        *coord++ = y;
        *coord++ = z1;
    }

    // Facet per rim face + 2 facets for the "caps"
    dest->numberoffacets = numQuads + 2;
    dest->facetlist = new tetgenio::facet[dest->numberoffacets];
    tetgenio::facet* facet = &dest->facetlist[0];

    // Rim faces:
    for (int n = 0; n < numQuads * 2; n += 2, ++facet) {
        facet->numberofpolygons = 1;
        facet->polygonlist = new tetgenio::polygon[facet->numberofpolygons];
        facet->numberofholes = 0;
        facet->holelist = NULL;
        tetgenio::polygon* poly = &facet->polygonlist[0];
        poly->numberofvertices = 4;
        poly->vertexlist = new int[poly->numberofvertices];
        poly->vertexlist[0] = n;
        poly->vertexlist[1] = n+1;
        poly->vertexlist[2] = (n+3) % (numQuads*2);
        poly->vertexlist[3] = (n+2) % (numQuads*2);
    }

    // Cap fans:
    for (int cap = 0; cap < 2; ++cap, ++facet) {
        facet->numberofpolygons = 1;
        facet->polygonlist = new tetgenio::polygon[facet->numberofpolygons];
        facet->numberofholes = 0;
        facet->holelist = NULL;
        tetgenio::polygon* poly = &facet->polygonlist[0];
        poly->numberofvertices = numQuads + 1;
        poly->vertexlist = new int[poly->numberofvertices];
        for (int q = 0; q < numQuads + 1; ++q) {
            poly->vertexlist[q] = (q % numQuads) * 2 + cap;
        }
    }
}

// Flip the orientation of all facets in 'B'
// and combine the result with the facets in 'A'
void
TetUtil::HullDifference(const tetgenio& hullA,
                        const tetgenio& hullB,
                        tetgenio* dest)
{
}

// Builds an index buffer for drawing the hull of a tetmesh with triangles.
void
TetUtil::TrianglesFromHull(const tetgenio& hull,
                           Blob* points)
{
}

// Builds an index buffer for drawing all tetrahedra with triangles.
void
TetUtil::TrianglesFromTets(const tetgenio& hull,
                           Blob* indices)
{
    int numTets = hull.numberoftetrahedra;
    indices->resize(sizeof(int) * numTets * 4 * 3);
    int* index = reinterpret_cast<int*>(&indices->front());
    int* currentTet = hull.tetrahedronlist;
    for (int i = 0; i < numTets; ++i, currentTet += 4) {
        *index++ = currentTet[1];
        *index++ = currentTet[0];
        *index++ = currentTet[2];
    
        *index++ = currentTet[0];
        *index++ = currentTet[1];
        *index++ = currentTet[3];

        *index++ = currentTet[1];
        *index++ = currentTet[2];
        *index++ = currentTet[3];
    
        *index++ = currentTet[2];
        *index++ = currentTet[0];
        *index++ = currentTet[3];
    }
}
