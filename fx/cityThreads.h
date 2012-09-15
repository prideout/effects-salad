#include "common/typedefs.h"
#include "common/texture.h"
#include "common/vao.h"

class BuildingCreator;

struct BuildingTemplate {
    BufferTexture CentroidTexture;
    int TotalTetCount;
    int BoundaryTetCount;
    Vao BuildingVao;
    Vao CracksVao;
    Vao HullVao;
    int NumCracks;
};

struct GpuParams {
    Blob HullIndices;
    Blob HullPoints;
    Vec4List Centroids;
    Blob FlattenedTets;
    Blob Cracks;
};

struct ThreadParams {
    float Thickness;
    float TopRadius;
    float TetSize;
    int NumSides;
    BuildingTemplate* Dest;
    GpuParams* GpuData;
    //BuildingCreator* Creator;
};

void StartBuildingCreator(ThreadParams* params);
void WaitForBuildingCreators();
