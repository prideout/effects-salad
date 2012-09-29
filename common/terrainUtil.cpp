#include "common/terrainUtil.h"

using namespace glm;

Perlin
TerrainUtil::GetNoise() 
{
    return Perlin(2, .1, 2, 0);
}


void
TerrainUtil::Tessellate(vec3 cent,
                        int SIZE,
                        float SCALE,
                        FloatList* ground,
                        IndexList* indices)
{
    Perlin noise = GetNoise();
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
    
    // Build up triangles for all the points
    for (int x = 0; x < SIZE - 1; x++) {
        for (int z =0; z < SIZE - 1; z++) {
            int idx = x*SIZE + z;
            indices->push_back(idx+SIZE);
            indices->push_back(idx);
            indices->push_back(idx+SIZE+1);

            indices->push_back(idx+SIZE+1);
            indices->push_back(idx);
            indices->push_back(idx+1);
        }
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
    vec3 cur, prev1, prev2;
    int curIdx=0, prev1Idx=0, prev2Idx=0;
    for(unsigned i = 0; i < indices.size(); i++) {
        prev2 = prev1;
        prev2Idx = prev1Idx;
        prev1 = cur;
        prev1Idx = curIdx;
        curIdx = indices[i];
        cur = vec3(ground[curIdx*4], ground[curIdx*4+1], ground[curIdx*4+2]);
        if (i < 2)
            continue;
        vec3 leg1 = (cur - prev1);
        vec3 leg2 = (prev2 - prev1);
        vec3 norm = (cross(leg1, leg2));
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
        vec3 v( normals[i], normals[i+1], normals[i+2] );
        v = normalize(v);
        normals[i+0] = v.x;
        normals[i+1] = v.y;
        normals[i+2] = v.z;
    }
}

vec3
TerrainUtil::SampleTerrain(Perlin& noise, int SIZE, float SCALE, float x, float z)
{
    float tx = x * SCALE;
    float tz = z * SCALE;
    float y = noise.Get(tx, tz);
    vec3 p = vec3(x, y, z);
    return p;
}

void
TerrainUtil::Smooth(int SIZE,
                    float SCALE,
                    FloatList* positions,
                    FloatList* normals,
                    IndexList* indices)
{
    Perlin noise = GetNoise();

    for (float x = 0; x < SIZE; x++) {
        for (float z = 0; z < SIZE; z++) {

            const float e = 0.1;

            vec3 p = SampleTerrain(noise, SIZE, SCALE, x, z);
            vec3 p1 = SampleTerrain(noise, SIZE, SCALE, x+e, z);
            vec3 p2 = SampleTerrain(noise, SIZE, SCALE, x, z+e);

            vec3 du = p1 - p;
            vec3 dv = p2 - p;
            vec3 n = normalize(cross(du, dv));

            p += vec3(-SIZE/2, 0, -SIZE/2);

            positions->push_back(p.x);
            positions->push_back(p.y);
            positions->push_back(p.z);
            normals->push_back(n.x);
            normals->push_back(n.y);
            normals->push_back(n.z);
        }
    } 

    // Build up triangles for all the points
    for (int x = 0; x < SIZE - 1; x++) {
        for (int z =0; z < SIZE - 1; z++) {
            int idx = x*SIZE + z;
            indices->push_back(idx+SIZE);
            indices->push_back(idx);
            indices->push_back(idx+SIZE+1);

            indices->push_back(idx+SIZE+1);
            indices->push_back(idx);
            indices->push_back(idx+1);
        }
    }
}
