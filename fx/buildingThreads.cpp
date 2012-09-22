#include "tthread/tinythread.h"
#include "fx/buildingThreads.h"
#include "common/tetUtil.h"
#include "common/init.h"

using namespace std;
using glm::mat4;
using glm::mat3;
using glm::vec4;
using glm::vec3;
using glm::vec2;

tthread::mutex gts_locker;

void
_GenerateBuilding(void* vParams)
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

    // Either the gts library isn't thread safe, or we're not building it to be thread-safe.
    // Anyway, it's not slow like tetgen so just put it around a lock for now.
    //gts_locker.lock();
    TetUtil::HullFrustum(r1, r2, y1, y2, nSides, &in);
    //gts_locker.unlock();

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
_UploadBuilding(ThreadParams& params)
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
