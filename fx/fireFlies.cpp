#include "fireFlies.h"
#include "common/programs.h"
#include "common/demoContext.h"
#include "noise/perlin.h"

#include <cstdlib>

void FireFlies::Init() 
{
    Perlin noise(2, .1, 2, 0);
    name = "FireFlies";
    Effect::Init();
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("FireFlies.Flies"));

    glUseProgram(progs.Load("FireFlies.Blur"));
    glUseProgram(progs.Load("FireFlies.Ground", "FireFlies.Ground.FS", "FireFlies.Flies.VS"));
    glUniform1i(u("Tex"), 1);

    _surface.Init();
    _quad.Init();

    FloatList ground;
    IndexList indices;
    const int SIZE = 150;
    const float SCALE = 1;
    glm::vec3 cent(0,-2,0);

    // don't mess with Y because it isn't effected by SIZE
    cent.x = cent.x - float(SIZE/2.*SCALE);
    cent.z = cent.z - float(SIZE/2.*SCALE);

    for (float x = 0; x < SIZE; x++) {
        for (float z = 0; z < SIZE; z++) {
            ground.push_back(cent.x + x*SCALE);
            ground.push_back(cent.y + noise.Get(x*SCALE,z*SCALE));
            ground.push_back(cent.z + z*SCALE);
            ground.push_back(1.);
        }
    }

    // build up a triangle strip over the surface
    int index = SIZE - 1;
    bool isFirstIndex = true;
    int di = -1;
    while (index < (SIZE*SIZE)) {
        for (int i = 0; i < SIZE; i++) {
            if (i > 0 or isFirstIndex) {
                indices.push_back(index);
                //std::cout << "F: " << index << std::endl;
                isFirstIndex = false;
            }
            indices.push_back(index+SIZE);
            //std::cout << index + SIZE<< std::endl;
            index += di;
        }
        di *= -1;
        index += SIZE + di;
    }

    _ground = Vao(4, ground, indices);
    

    // XXX: need to call srand in a more common location
    srand(0);
    for (int i = 0; i < 500; i ++) {
        float x = 20*(rand() / float(RAND_MAX)) - 10; 
        float y = 3*(rand() / float(RAND_MAX)) - 2; 
        float z = 20*(rand() / float(RAND_MAX)) - 10; 
        //n = noise.Get(
        //std::cout << x << ", " << y << ", " << z << "\n";
        _fliesCpu.push_back(x);
        //_fliesCpu.push_back(noise.Get(x,z));
        _fliesCpu.push_back(y);
        _fliesCpu.push_back(z);
        _fliesCpu.push_back(1.0);
    }
    _fliesGpu = Vao(4, _fliesCpu); 

    glPointSize(1.5);
    float cameraPath[] = {
         0,    0,  4.04,
        -7.11, 0,  4.06,
        -8.11, 0, -4.10,
         0.05, 0, -3.99,
         8.22, 0, -3.88,
         7.35, 0,  4.06,
         0.01, 0,  4.04};
    Vec3List cvs;
    for(int i = 0; i < sizeof(cameraPath) / sizeof(float); i+=3) {
        cvs.push_back( 2.f * vec3(cameraPath[i+0],cameraPath[i+1],cameraPath[i+2]));
    }

    // XXX: this is time independent right now
    //      need to make it explicit in time if we're going to keep the camera motion
    cameraPoints = Bezier::Eval(1000, cvs);
    counter = 0;
};

void FireFlies::Update() {
    Effect::Update();
};

void FireFlies::Draw() {
    if (counter > cameraPoints.size() -1) counter = 0;
    Programs& progs = Programs::GetInstance();
    Effect::Draw();
    glEnable(GL_CULL_FACE);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glUseProgram(progs["FireFlies.Flies"]);

        //_surface.Bind();
        PerspCamera surfaceCam = context->mainCam;
        surfaceCam.eye = cameraPoints[counter];
        //surfaceCam.eye.x = 0;
        surfaceCam.eye.y = .0;
        surfaceCam.center = vec3(0,.8,0); //cameraPoints[counter];
        surfaceCam.Bind(glm::mat4());
        
        /*
        glClearColor(.0,.0,.9,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        */
        _fliesGpu.Bind();
        glDrawArrays(GL_POINTS, 0, _fliesGpu.vertexCount*4);

        glUseProgram(progs["FireFlies.Ground"]);
        surfaceCam.Bind(glm::mat4());
        glUniform3f(u("Eye"), surfaceCam.eye.x, surfaceCam.eye.y, surfaceCam.eye.z);
        _ground.Bind();
        //glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, NULL);
        //glDrawElements(GL_TRIANGLE_STRIP, counter % 100, GL_UNSIGNED_INT, NULL);
        glDrawElements(GL_TRIANGLE_STRIP, _ground.indexCount, GL_UNSIGNED_INT, NULL);
        //_surface.Unbind();

    /*
    glUseProgram(progs["FireFlies.Blur"]);
    context->mainCam.Bind(glm::mat4());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _surface.texture);
    _quad.Draw();
    */
    counter++;
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDisable(GL_CULL_FACE);
};

