#include "fireFlies.h"
#include "common/programs.h"
#include "common/demoContext.h"

#include "noise/perlin.h"

void FireFlies::Init() 
{
    Perlin noise(2, .1, 2, 0);
    name = "FireFlies";
    Effect::Init();
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("FireFlies.Flies"));

    glUseProgram(progs.Load("FireFlies.Blur"));

    // --------------------------------------------------------------------- 
    // Fire Flies
    // --------------------------------------------------------------------- 

    // XXX: need to call srand in a more common location
    srand(0);
    for (int i = 0; i < 1000; i ++) {
        float x = 150*(rand() / float(RAND_MAX)) - 75; 
        float y = 3*(rand() / float(RAND_MAX)) - 2; 
        float z = 150*(rand() / float(RAND_MAX)) - 75; 
        //n = noise.Get(
        //std::cout << x << ", " << y << ", " << z << "\n";
        _fliesCpu.push_back(x);
        //_fliesCpu.push_back(noise.Get(x,z));
        _fliesCpu.push_back(y);
        _fliesCpu.push_back(z);

        // set firefly size multiplier
        _fliesCpu.push_back(1.5);
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

    PerspCamera& cam = GetContext()->mainCam;
    glUseProgram(progs["FireFlies.Flies"]);
    cam.Bind(glm::mat4());
        
    bool kick = GetContext()->audio->GetKicks();
    bool hihat = GetContext()->audio->GetHiHats();

    if (kick)
        glUniform1f(u("SizeMult"), 3.0);
    else
        glUniform1f(u("SizeMult"), 1.0);

    glUniform3f(u("Eye"), cam.eye.x, cam.eye.y, cam.eye.z);
    glUniform1f(u("Time"), GetContext()->elapsedTime);
    glUniform1f(u("Brightness"), brightness); 


    if (kick)
        glPointSize(5.5);
    else if (hihat)
        glPointSize(3);
    else
        glPointSize(1.5);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    _fliesGpu.Bind();
    glDrawArrays(GL_POINTS, 0, _fliesGpu.vertexCount);
};

