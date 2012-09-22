#include "fx/buildings.h"

struct GpuParams {
    Blob HullIndices;
    Blob HullPoints;
    Vec4List Centroids;
    Blob FlattenedTets;
    Blob Cracks;
};

struct ThreadParams {
    bool CanExplode;
    float Thickness;
    float TopRadius;
    float TetSize;
    int NumSides;
    BuildingTemplate* Dest;
    GpuParams* GpuData;
};

/// Executes on a worker thread, performs no OpenGL calls.
void _GenerateBuilding(void* params);

/// Executes on the main thread when a worker thread completes.
void _UploadBuilding(ThreadParams& params);
