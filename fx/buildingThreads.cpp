#include "fx/buildingThreads.h"
#include "common/tetUtil.h"
#include "common/init.h"
#include "glm/gtx/constants.inl"

using namespace std;
using namespace glm;

static void
_CreateExteriorWall(
    float r1,
    float r2,
    float y1,
    float y2,
    int numQuads,
    const WindowParams& windows,
    tetgenio* dest)
{
    // If the destination already has facets, append to it:
    if (dest->numberofpoints) {
        tetgenio freshHull;
        _CreateExteriorWall(r1, r2, y1, y2, numQuads,
                            windows, &freshHull);
        TetUtil::HullCombine(freshHull, dest);
        return;
    }

    dest->numberofpoints = numQuads * 2;
    dest->pointlist = new float[dest->numberofpoints * 3];
    const float twopi = 2 * pi<float>();
    const float dtheta = twopi / numQuads;
    float* coord = dest->pointlist;
    
    // Rim points:
    for (float theta = 0; theta < twopi - dtheta / 2; theta += dtheta) {
        float x1 = r1 * cos(theta);
        float z1 = r1 * sin(theta);
        *coord++ = x1; *coord++ = y1; *coord++ = z1;
        float x2 = r2 * cos(theta);
        float z2 = r2 * sin(theta);
        *coord++ = x2; *coord++ = y2; *coord++ = z2;
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
        tetgenio::polygon* poly = facet->polygonlist;
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
        tetgenio::polygon* poly = facet->polygonlist;
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

void
GenerateBuilding(void* vParams)
{
    ThreadParams* params = (ThreadParams*) vParams;
    float thickness = params->Thickness;
    float topRadius = params->TopRadius;
    float tetSize = params->TetSize;
    int nSides = params->NumSides;
    BuildingTemplate* dest = params->Dest;
    GpuParams* gpuData = new GpuParams;

    // Create the outer skin
    tetgenio in;
    float r1 = 10.0f;  float r2 = r1 * topRadius;
    float y1 = 0;      float y2 = 20.0f;
    _CreateExteriorWall(r1, r2, y1, y2, nSides, params->Windows, &in);

    // Create a cheap Vao for buildings that aren't self-destructing
    TetUtil::TrianglesFromHull(in, &gpuData->HullIndices);
    gpuData->HullPoints.resize(sizeof(float) * 3 * in.numberofpoints);
    memcpy(&gpuData->HullPoints[0], in.pointlist, gpuData->HullPoints.size());

    if (!params->CanExplode) {
        params->GpuData = gpuData;
        return;
    }

    // Add inner walls
    y1 += thickness; y2 -= thickness;
    r1 -= thickness; r2 -= thickness;
    TetUtil::HullFrustum(r1, r2, y1, y2, nSides, &in);

    // Poke volumetric holes
    Vec3List holePoints;
    holePoints.push_back(vec3(0, 10.0, 0));
    TetUtil::AddHoles(holePoints, &in);

    // Tetrahedralize the boundary mesh
    tetgenio out;
    const float qualityBound = 1.414;
    const float maxVolume = tetSize;
    TetUtil::TetsFromHull(in, &out, qualityBound, maxVolume, true);
    dest->TotalTetCount = out.numberoftetrahedra;

    // Populate the per-tet texture data and move boundary tets to the front
    TetUtil::SortTetrahedra(&gpuData->Centroids, out, &dest->BoundaryTetCount);

    // Create a flat list of non-indexed triangles
    VertexAttribMask attribs = AttrPositionFlag | AttrNormalFlag;
    TetUtil::PointsFromTets(out, attribs, &gpuData->FlattenedTets);

    // Non-indexed vertical crack lines
    TetUtil::FindCracks(out, gpuData->Centroids, &gpuData->Cracks);

    params->GpuData = gpuData;
}

void
UploadBuilding(ThreadParams& params)
{
    GpuParams* src = params.GpuData;
    BuildingTemplate* dest = params.Dest;

    // Cheap Vao for buildings that aren't self-destructing
    dest->HullVao.Init();
    dest->HullVao.AddVertexAttribute(AttrPosition,
                                     3,
                                     src->HullPoints);
    dest->HullVao.AddIndices(src->HullIndices);

    if (params.CanExplode) {
        // Texture buffer with centroids
        dest->CentroidTexture.Init(src->Centroids);

        // Huge buffer of non-indexed triangles
        dest->BuildingVao.Init();
        VertexAttribMask attribs = AttrPositionFlag | AttrNormalFlag;
        dest->BuildingVao.AddInterleaved(attribs, src->FlattenedTets);
        pezCheckGL("Bigass VBO for tets");

        // Non-indexed vertical crack lines
        dest->CracksVao.Init();
        dest->CracksVao.AddInterleaved(AttrPositionFlag | AttrLengthFlag, src->Cracks);
        dest->NumCracks = (src->Cracks.size() / sizeof(vec4)) / 2;
        pezCheckGL("Bigass VBO for cracks");
    }

    // Free CPU memory
    delete src;
    params.GpuData = 0;
}
