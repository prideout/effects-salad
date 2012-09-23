#include "fx/buildings.h"

struct GpuParams {
    Blob HullIndices;
    Blob HullPoints;
    Vec4List Centroids;
    Blob FlattenedTets;
    Blob Cracks;
};

struct WindowParams {
    int Rows;
    int Columns;
    glm::vec2 Size;
};

struct ThreadParams {
    bool CanExplode;
    int NumSides;
    float Thickness;
    float TopRadius;
    float TetSize;
    float InsetDepth;
    WindowParams Windows;
    BuildingTemplate* Dest;
    GpuParams* GpuData;
};

/// Executes on a worker thread, performs no OpenGL calls.
void GenerateBuilding(void* params);

/// Executes on the main thread when a worker thread completes.
void UploadBuilding(ThreadParams& params);
