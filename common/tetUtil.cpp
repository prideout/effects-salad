#include "common/tetUtil.h"
#include "glm/gtx/constants.inl"

// Thin wrapper for tetgen's "tetrahedralize" function.
void
TetUtil::TetsFromHull(const tetgenio& hull,
                      tetgenio* dest,
                      float qualityBound,
                      float maxVolume,
                      bool quiet)
{
    const char* formatString = quiet ? "Qpq%.3fa%.7f" : "pq%.3fa%.7f";
    char configString[128];
    sprintf(configString, formatString, qualityBound, maxVolume);
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
    // If the destination already has facets, append to it:
    if (dest->numberofpoints) {
        static tetgenio freshHull;
        HullWheel(center, radius, width, numQuads, &freshHull);
        //HullCombine(freshHull, dest);
        return;
    }

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
        *coord++ = z0;
        *coord++ = y;
        *coord++ = x;
        *coord++ = z1;
        *coord++ = y;
        *coord++ = x;
    }

    // Facet per rim face + 2 facets for the "caps"
    dest->numberoffacets = numQuads + 2;
    dest->facetlist = new tetgenio::facet[dest->numberoffacets];
    tetgenio::facet* facet = dest->facetlist;

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
        poly->numberofvertices = numQuads;
        poly->vertexlist = new int[poly->numberofvertices];
        int nq = numQuads;
        if (cap) {
            for (int q = 0; q < nq; ++q) poly->vertexlist[q] = q*2;
        } else {
            for (int q = 0; q < nq; ++q) poly->vertexlist[nq-1-q] = q*2+1;
        }
    }
}

static void
_CopyPolygons(const tetgenio::facet& source,
              tetgenio::facet* dest,
              int offset,
              bool flipWinding)
{
    dest->numberofpolygons = source.numberofpolygons;
    dest->polygonlist = new tetgenio::polygon[dest->numberofpolygons];
    dest->numberofholes = 0;
    dest->holelist = NULL;
    tetgenio::polygon* destPoly = dest->polygonlist;
    const tetgenio::polygon* srcPoly = source.polygonlist;
    for (int pi = 0; pi < dest->numberofpolygons; ++pi, ++destPoly, ++srcPoly) {
        destPoly->numberofvertices = srcPoly->numberofvertices;
        destPoly->vertexlist = new int[destPoly->numberofvertices];
        for (int vi = 0; vi < destPoly->numberofvertices; ++vi) {
            int vj = flipWinding ? (destPoly->numberofvertices - 1 - vi) : vi;
            destPoly->vertexlist[vi] = offset + srcPoly->vertexlist[vj];
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
    dest->numberofpoints = hullA.numberofpoints + hullB.numberofpoints;
    dest->pointlist = new float[dest->numberofpoints * 3];
    for (int i = 0; i < hullA.numberofpoints * 3; i++) {
        dest->pointlist[i] = hullA.pointlist[i];
    }
    for (int i = 0; i < hullB.numberofpoints * 3; i++) {
        dest->pointlist[i + hullA.numberofpoints * 3] = hullB.pointlist[i];
    }

    dest->numberoffacets = hullA.numberoffacets + hullB.numberoffacets;
    dest->facetlist = new tetgenio::facet[dest->numberoffacets];
    for (int i = 0; i < hullA.numberoffacets; i++) {
        _CopyPolygons(hullA.facetlist[i],
                      &dest->facetlist[i],
                      0,
                      false);
    }
    for (int i = 0; i < hullB.numberoffacets; i++) {
        _CopyPolygons(hullB.facetlist[i],
                      &dest->facetlist[i + hullA.numberoffacets],
                      hullA.numberofpoints,
                      true);
    }
}

// Copy all facets from "hull" to dest
void
TetUtil::HullCombine(const tetgenio& second,
                     tetgenio* dest)
{
    float* firstPoints = dest->pointlist;
    int firstPointCount = dest->numberofpoints;
    tetgenio::facet* firstFacets = dest->facetlist;
    int firstFacetCount = dest->numberoffacets;

    dest->numberofpoints += second.numberofpoints;
    dest->numberoffacets += second.numberoffacets;
    dest->pointlist = new float[dest->numberofpoints];
    dest->facetlist = new tetgenio::facet[dest->numberoffacets];

    for (int i = 0; i < firstPointCount * 3; i++) {
        dest->pointlist[i] = firstPoints[i];
    }
    for (int i = 0; i < second.numberofpoints * 3; i++) {
        dest->pointlist[i + firstPointCount * 3] = second.pointlist[i];
    }
    for (int i = 0; i < firstFacetCount; i++) {
        _CopyPolygons(firstFacets[i],
                      dest->facetlist + i,
                      0,
                      false);
    }
    for (int i = 0; i < second.numberoffacets; i++) {
        _CopyPolygons(second.facetlist[i],
                      dest->facetlist + i + firstFacetCount,
                      firstPointCount,
                      false);
    }
    //delete[] firstPoints;
    //delete[] firstFacets; // TODO <-- leaky: needs to be deeper
}

// Add a volumetric "hole" to a tetgen structure
void
TetUtil::SubtractRegion(tetgenio* dest,
                        const tetgenio& emptiness)
{
    dest->numberofholes = emptiness.numberofpoints;
    dest->holelist = new float[emptiness.numberofpoints * 3];
    memcpy(dest->holelist,
           emptiness.pointlist,
           sizeof(float) * emptiness.numberofpoints * 3);
}

// Builds an index buffer for drawing the hull of a tetmesh with triangles.
void
TetUtil::TrianglesFromHull(const tetgenio& hull,
                           Blob* indices)
{
    std::vector<int> dest;
    int numFacets = hull.numberoffacets;
    const tetgenio::facet* facet = &hull.facetlist[0];
    for (; numFacets; ++facet, --numFacets) {
        int numPolys = facet->numberofpolygons;
        const tetgenio::polygon* poly = &facet->polygonlist[0];
        for (; numPolys; ++poly, --numPolys) {
            int numTriangles = poly->numberofvertices - 2;
            int n = 1;
            for (; numTriangles; ++n, --numTriangles) {
                int p = (n+1) % (poly->numberofvertices);
                dest.push_back(poly->vertexlist[0]);
                dest.push_back(poly->vertexlist[n]);
                dest.push_back(poly->vertexlist[p]);
            }
        }
    }
    indices->resize(sizeof(int) * dest.size());
    Blob& blob = *indices;
    unsigned char* front = &(blob[0]);
    memcpy(front, &(dest[0]), blob.size());
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
