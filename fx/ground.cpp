#include "ground.h"

#include <cstdlib>

#include "noise/perlin.h"

#include "common/demoContext.h"
#include "common/programs.h"

void Ground::Init() {
    name = "Ground";
    Effect::Init();

    Perlin noise(2, .1, 2, 0);

    Programs& progs = Programs::GetInstance();
    
    glUseProgram(progs.Load("FireFlies.Ground", "FireFlies.Ground.FS", "FireFlies.Flies.VS"));
    glUseProgram(progs.Load("FireFlies.Grass"));


    // --------------------------------------------------------------------- 
    // Ground plane
    // --------------------------------------------------------------------- 
    FloatList ground;
    IndexList indices;
    const int SIZE = 150;
    const float SCALE = 1;
    const int GRASS_COUNT = SIZE*SIZE*10;
    glm::vec3 cent(0,-2,0);


    // don't mess with Y because it isn't effected by SIZE
    cent.x = cent.x - float(SIZE/2.*SCALE);
    cent.z = cent.z - float(SIZE/2.*SCALE);

    for (float x = 0; x < SIZE; x++) {
        for (float z = 0; z < SIZE; z++) {
            ground.push_back(cent.x + x*SCALE + .6*(rand() / float(RAND_MAX)));
            ground.push_back(cent.y + noise.Get(x*SCALE,z*SCALE));
            ground.push_back(cent.z + z*SCALE + .6*(rand() / float(RAND_MAX)));
            ground.push_back(1.);
        }
    } 

    // build up a triangle strip over the surface
    int index = SIZE - 1;
    bool isFirstIndex = true;
    int di = -1;
    while (index < (SIZE*SIZE) - SIZE) {
        for (int i = 0; i < SIZE; i++) {
            if (i > 0 or isFirstIndex) {
                indices.push_back(index);
                isFirstIndex = false;
            }
            indices.push_back(index+SIZE);
            //std::cout << index + SIZE<< std::endl;
            index += di;
        }
        di *= -1;
        index += SIZE + di;
    }

    // initialize the normals list
    FloatList normals;
    normals.resize(ground.size());

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
        //std::cout << i << " - " << curIdx << " -- index.size: " << indices.size() << "  std::endl;
        if (i < 2)
            continue;
        glm::vec3 leg1 = (cur - prev1);
        glm::vec3 leg2 = (prev2 - prev1);
        glm::vec3 norm = (glm::cross(leg1, leg2));
        if (i % 2 == 1)
            norm = -norm;


        /*
        std::cout << "XXX: " << cur.x << ", " << cur.y << ", " << cur.z << std::endl;
        std::cout << "XXX: " << prev1.x << ", " << prev1.y << ", " << prev1.z << std::endl;
        std::cout << "XXX: " << prev2.x << ", " << prev2.y << ", " << prev2.z << std::endl;
        std::cout << "leg1: " << leg1.x << ", " << leg1.y << ", " << leg1.z << std::endl;
        std::cout << "leg2: " << leg2.x << ", " << leg2.y << ", " << leg2.z << std::endl;
        std::cout << "NORM: " << norm.x << ", " << norm.y << ", " << norm.z << std::endl;
        */

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
    if (1)
    for (unsigned i = 0; i < normals.size(); i+=4) { 
        glm::vec3 v( normals[i], normals[i+1], normals[i+2] );
        v = glm::normalize(v);
        normals[i+0] = v.x;
        normals[i+1] = v.y;
        normals[i+2] = v.z;
    }
    //FOR_EACH(n, normals) 
    //    std::cout << *n << std::endl;
    FloatList grass(GRASS_COUNT*8, 0);
    for (int i = 0; i < GRASS_COUNT; i++) {
        int index = rand() % (ground.size()/4); 
        index *= 4;
        pezCheck(index >= 0, "Grass index must be >= 0 (%d)", index);
        pezCheck(index+2 < ground.size(), "Grass index must be < ground.size()");
        pezCheck(i*4+3 < grass.size());

        float x = ground[index+0] + 1*(rand() / float(RAND_MAX));
        float y = ground[index+1];
        float z = ground[index+2] + 1*(rand() / float(RAND_MAX));
        grass[i*8+0+0] = x;
        grass[i*8+0+1] = y;
        grass[i*8+0+2] = z;
        grass[i*8+0+3] = ground[index+3];

        grass[i*8+4+0] = x+normals[index+0];
        grass[i*8+4+1] = y+normals[index+1];
        grass[i*8+4+2] = z+normals[index+2];
        grass[i*8+4+3] = ground[index+3];
    }

    _ground = Vao(4, ground, indices);
    _ground.AddVertexAttribute(AttrNormal, 4, normals);

    _grass = Vao(4, grass);

    _normals = NormalField(4, ground, 4, normals, 1);
    _normals.Init();

}

void Ground::Update() {
    Effect::Update();
    _normals.Update();
}

void Ground::Draw() {
    Programs& progs = Programs::GetInstance();
    Effect::Draw();

    glUseProgram(progs["FireFlies.Ground"]);
    Camera& cam = GetContext()->mainCam;
    cam.Bind(glm::mat4());
    glUniform3f(u("Eye"), cam.eye.x, cam.eye.y, cam.eye.z);
    _ground.Bind();
    glDrawElements(GL_TRIANGLE_STRIP, _ground.indexCount, GL_UNSIGNED_INT, NULL);


    glUseProgram(progs["FireFlies.Grass"]);
    cam.Bind(glm::mat4());
    _normals.Draw();

    glPointSize(4);
    _grass.Bind();
    // Progressively add grass (for debugging)
    int t = int(GetContext()->elapsedTime*100000) % (_grass.vertexCount+1);
    t = _grass.vertexCount;
    //glDrawArrays(GL_POINTS, 0, t);
    glDrawArrays(GL_LINES,0, t);
}

