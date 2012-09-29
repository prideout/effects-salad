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

    typedef glm::vec3(*TerrainFunc)(glm::vec2);

    void Smooth(int size,
                TerrainFunc func,
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
