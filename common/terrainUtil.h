#include "common/typedefs.h"
#include "glm/glm.hpp"
#include "noise/perlin.h"

namespace TerrainUtil
{

    Perlin GetNoise();

    void Tessellate(glm::vec3 center,
                    int size,
                    float scale,
                    FloatList* points,
                    IndexList* indices);

    void ComputeNormals(const FloatList& points,
                        const IndexList& indices,                    
                        FloatList* normals);

    void Smooth(int size,
                float scale,
                FloatList* points,
                FloatList* normals,
                IndexList* indices);

    glm::vec3
    SampleTerrain(Perlin& noise, 
                  int SIZE, 
                  float SCALE, 
                  float x,
                  float z);
}
