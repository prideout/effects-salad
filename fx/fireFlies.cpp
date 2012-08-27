#include "fireflies.h"
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
        float z = 10*(rand() / float(RAND_MAX)) - 20; 
        //std::cout << x << ", " << y << ", " << z << "\n";
        _fliesCpu.push_back(x);
        _fliesCpu.push_back(y);
        _fliesCpu.push_back(z);
        _fliesCpu.push_back(1.0);
    }
    _fliesGpu = Vao(4, _fliesCpu); 
};

void FireFlies::Update() {
    Effect::Update();
};

void FireFlies::Draw() {
    Programs& progs = Programs::GetInstance();
    Effect::Draw();

    glUseProgram(progs["FireFlies.Flies"]);

        _surface.Bind();
        PerspCamera surfaceCam = context->mainCam;
        /*
        surfaceCam.eye.y += 1.9;
        surfaceCam.center.y += 1.9;
        surfaceCam.eye.x += 4.5;
        surfaceCam.center.x += 4.5;
        surfaceCam.fov = 90;
        */
        surfaceCam.Bind(glm::mat4());

        glClearColor(.0,.0,.9,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _fliesGpu.Bind();
        glDrawArrays(GL_POINTS, 0, _fliesGpu.vertexCount*4);
        _surface.Unbind();

    glUseProgram(progs["FireFlies.Blur"]);
    context->mainCam.Bind(glm::mat4());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _surface.texture);
    _quad.Draw();
        
};

