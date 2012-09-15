#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "common/tetUtil.h"
#include "common/init.h"
#include "fx/cityThreads.h"
#include "tthread/tinythread.h"
#include <list>

using namespace tthread;
using namespace glm;

class BuildingCreator {
public:
    BuildingCreator(ThreadParams*);
    ~BuildingCreator();

    // Performs CPU-heavy work that executes in the worker thread.
    // No OpenGL calls are allowed here!
    void Run();

    // Executes in the main thread after completion; makes OpenGL calls.
    void Epilogue();

private:
    void _signalCompletion();
    ThreadParams* _params;
    thread* _thread;
};

// BEGIN poor man's singleton class declaration

void StartBuildingCreator(ThreadParams* params);
void WaitForBuildingCreators();

typedef std::list<BuildingCreator*> ThreadList;

static ThreadList _runningThreads;
static ThreadList _completedThreads;
static mutex _completionMutex;
static condition_variable _completionCond;

// END poor man's singleton class declaration

void
StartBuildingCreator(ThreadParams* params)
{
    lock_guard<mutex> guard(_completionMutex);
    BuildingCreator* creator = new BuildingCreator(params);
    _runningThreads.push_back(creator);
}

void
BuildingCreator::_signalCompletion()
{
    lock_guard<mutex> guard(_completionMutex);
    _completedThreads.push_back(this);
    _completionCond.notify_all();
}

void
WaitForBuildingCreators()
{
    size_t numRunningThreads = _runningThreads.size();
    printf("Waiting for %d threads...\n", (int) numRunningThreads);
    while (numRunningThreads > 0) {
        lock_guard<mutex> guard(_completionMutex);
        while (_completedThreads.empty()) {
            _completionCond.wait(_completionMutex);
        }
        //lock_guard<mutex> guard(_completionMutex);
        while (_completedThreads.size() > 0) {
            BuildingCreator* completed = _completedThreads.front();
            _completedThreads.pop_front();
            completed->Epilogue();
            delete completed;
            --numRunningThreads;
        }
    }
}

void _threadFunc(void* arg)
{
    BuildingCreator* creator = (BuildingCreator*) arg;
    creator->Run();
}

BuildingCreator::BuildingCreator(ThreadParams* params)
{
    _params = params;
    _thread = new thread(_threadFunc, (void*) this);
}

BuildingCreator::~BuildingCreator()
{
    delete _thread;
}

void
BuildingCreator::Run()
{
    float thickness = _params->Thickness;
    float topRadius = _params->TopRadius;
    float tetSize = _params->TetSize;
    int nSides = _params->NumSides;
    BuildingTemplate* dest = _params->Dest;
    GpuParams* gpuData = new GpuParams;

    // Create the outer skin
    tetgenio in;
    float r1 = 10.0f;  float r2 = r1 * topRadius;
    float y1 = 0;     float y2 = 20.0f;
    TetUtil::HullFrustum(r1, r2, y1, y2, nSides, &in);

    // Create a cheap Vao for buildings that aren't self-destructing
    TetUtil::TrianglesFromHull(in, &gpuData->HullIndices);
    gpuData->HullPoints.resize(sizeof(float) * 3 * in.numberofpoints);
    memcpy(&gpuData->HullPoints[0], in.pointlist, gpuData->HullPoints.size());

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

    _params->GpuData = gpuData;
    _signalCompletion();
}

void
BuildingCreator::Epilogue()
{
    GpuParams* src = _params->GpuData;
    BuildingTemplate* dest = _params->Dest;

    // Cheap Vao for buildings that aren't self-destructing
    dest->HullVao.Init();
    dest->HullVao.AddVertexAttribute(AttrPositionFlag,
                                     3,
                                     src->HullPoints);
    dest->HullVao.AddIndices(src->HullIndices);

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

    // Free CPU memory
    delete src;
    _params->GpuData = 0;
}
