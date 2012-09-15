#include "common/typedefs.h"
#include "glm/glm.hpp"

namespace TerrainUtil
{
    void Tessellate(glm::vec3 center,
                    int size,
                    float scale,
                    FloatList* points,
                    IndexList* indices);

    void ComputeNormals(const FloatList& points,
                        const IndexList& indices,                    
                        FloatList* normals);
}
