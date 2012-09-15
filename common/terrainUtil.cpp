#include "common/terrainUtil.h"
#include "noise/perlin.h"

void
TerrainUtil::Tessellate(glm::vec3 cent,
                        int SIZE,
                        float SCALE,
                        FloatList* ground,
                        IndexList* indices)
{
    Perlin noise(2, .1, 2, 0);
    // don't mess with Y because it isn't effected by SIZE
    cent.x = cent.x - float(SIZE/2.*SCALE);
    cent.z = cent.z - float(SIZE/2.*SCALE);

    for (float x = 0; x < SIZE; x++) {
        for (float z = 0; z < SIZE; z++) {
            ground->push_back(cent.x + x*SCALE + .6*(rand() / float(RAND_MAX)));
            ground->push_back(cent.y + noise.Get(x*SCALE,z*SCALE));
            ground->push_back(cent.z + z*SCALE + .6*(rand() / float(RAND_MAX)));
            ground->push_back(1.);
        }
    } 

    // build up a triangle strip over the surface
    int index = SIZE - 1;
    bool isFirstIndex = true;
    int di = -1;
    while (index < (SIZE*SIZE) - SIZE) {
        for (int i = 0; i < SIZE; i++) {
            if (i > 0 or isFirstIndex) {
                indices->push_back(index);
                isFirstIndex = false;
            }
            indices->push_back(index+SIZE);
            //std::cout << index + SIZE<< std::endl;
            index += di;
        }
        di *= -1;
        index += SIZE + di;
    }
}

void
TerrainUtil::ComputeNormals(const FloatList& ground,
                            const IndexList& indices,                    
                            FloatList* pNormals)
{
    // initialize the normals list
    pNormals->resize(ground.size());
    FloatList& normals = *pNormals;

    // calculate vertex normals
    // since we just generated the triangle indices, use them here
    glm::vec3 cur, prev1, prev2;
    int curIdx=0, prev1Idx=0, prev2Idx=0;
    for(unsigned i = 0; i < indices.size(); i++) {
        prev2 = prev1;
        prev2Idx = prev1Idx;
        prev1 = cur;
        prev1Idx = curIdx;
        curIdx = indices[i];
        cur = glm::vec3(ground[curIdx*4], ground[curIdx*4+1], ground[curIdx*4+2]);
        if (i < 2)
            continue;
        glm::vec3 leg1 = (cur - prev1);
        glm::vec3 leg2 = (prev2 - prev1);
        glm::vec3 norm = (glm::cross(leg1, leg2));
        if (i % 2 == 1)
            norm = -norm;

        normals[4*curIdx+0] += norm.x;
        normals[4*curIdx+1] += norm.y;
        normals[4*curIdx+2] += norm.z;
        normals[4*curIdx+3] = 1.0;

        normals[4*prev1Idx+0] += norm.x;
        normals[4*prev1Idx+1] += norm.y;
        normals[4*prev1Idx+2] += norm.z;
        normals[4*prev1Idx+3] = 1.0;

        normals[4*prev2Idx+0] += norm.x;
        normals[4*prev2Idx+1] += norm.y;
        normals[4*prev2Idx+2] += norm.z;
        normals[4*prev2Idx+3] = 1.0;
    }

    // normalize the, um, normals
    for (unsigned i = 0; i < normals.size(); i+=4) { 
        glm::vec3 v( normals[i], normals[i+1], normals[i+2] );
        v = glm::normalize(v);
        normals[i+0] = v.x;
        normals[i+1] = v.y;
        normals[i+2] = v.z;
    }
}
