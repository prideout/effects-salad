#include "fireFlies.h"
#include "common/programs.h"
#include "common/demoContext.h"

#include <cstdlib>

void FireFlies::Init() 
{
    name = "FireFlies";
    Effect::Init();
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("FireFlies.Flies"));

    glUseProgram(progs.Load("FireFlies.Blur"));
    glUniform1i(u("Tex"), 1);

    _surface.Init();
    _quad.Init();

    // XXX: need to call srand in a more common location
    srand(0);
    for (int i = 0; i < 500; i ++) {
        float x = 10*(rand() / float(RAND_MAX)) - 5; 
        float y = 10*(rand() / float(RAND_MAX)) - 5; 
        float z = 10*(rand() / float(RAND_MAX)) - 5; 
        //std::cout << x << ", " << y << ", " << z << "\n";
        _fliesCpu.push_back(x);
        _fliesCpu.push_back(y);
        _fliesCpu.push_back(z);
        _fliesCpu.push_back(1.0);
    }
    _fliesGpu = Vao(4, _fliesCpu); 

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

    glUseProgram(progs["FireFlies.Flies"]);

        //_surface.Bind();
        PerspCamera surfaceCam = context->mainCam;
        surfaceCam.eye = cameraPoints[counter];
        surfaceCam.center = vec3(0,0,0); //cameraPoints[counter];
        surfaceCam.Bind(glm::mat4());
        
        /*
        glClearColor(.0,.0,.9,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        */
        _fliesGpu.Bind();
        glDrawArrays(GL_POINTS, 0, _fliesGpu.vertexCount*4);
        //_surface.Unbind();

    /*
    glUseProgram(progs["FireFlies.Blur"]);
    context->mainCam.Bind(glm::mat4());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _surface.texture);
    _quad.Draw();
    */
    counter++;
};

