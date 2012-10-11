#include "grassTreeGrow.h"
#include "common/programs.h"
#include "common/demoContext.h"

#include "noise/perlin.h"
#include "glm/core/func_geometric.hpp"

#include <cstdlib>

void GrassTreeGrow::Init() 
{
    Perlin noise(2, .1, 2, 0);
    name = "GrassTreeGrow";
    Effect::Init();
    Programs& progs = Programs::GetInstance();

    glUseProgram(progs.Load("FireFlies.Blur"));
    glUseProgram(progs.Load("FireFlies.Tube", "FireFlies.Tube.FS", "FireFlies.Tube.VS"));
    glUniform1i(u("Tex"), 1);

    // this must be set before Tree::Init
    if (bloomMode) 
        _tree.grown = true;

    _surface.Init();
    _quad.Init();
    _milkyway.Init();
    _tree.Init();
    _fireFlies.Init();


    // --------------------------------------------------------------------- 
    // Camera Path
    // --------------------------------------------------------------------- 

    float cameraPath[] = {
        // new camera
        -29.398405178043745, 0.3691749981521088, -3.3890212359114953,
        -22.081107798166876, 0.3691749981521088, -3.9378185394022491,
        -14.763810418290007, 0.3691749981521088, -4.4866158428930021,
        -11.369377169338659, 0, -6.8509163042400134,
        -4.8752757446979409, 0, -6.9423825214884758,
        1.6188256799427769, 0, -7.0338487387369382,
        7.5641298010927152, 0, -3.6495987005438955,
        7.4726635838442625, 0, 1.4725094653699053,
        7.3811973665958099, 0, 6.5946176312837075,
        1.6188256799427769, 0, 9.61300280048291,
        -4.7838095274494883, 0, 8.8812730624952252,
        -11.186444734841754, 0, 8.1495433245075333,
        -13.47310016605325, 0, -0.63121353134468361,
        -8.3509920001394491, 0, -3.9239973522892759,
        };
    Vec3List cvs;
    size_t cvCount = sizeof(cameraPath) / sizeof(float);
    for(size_t i = 0; i < cvCount; i+=3) {
        cvs.push_back(vec3(cameraPath[i+0],cameraPath[i+1],cameraPath[i+2]));
    }
    _tube.cvs = cvs;
    _tube.Init();

    // XXX: this is time independent right now
    //      need to make it explicit in time if we're going to keep the camera motion
    cameraPoints = AnimCurve<glm::vec3>(cvs, 0, GetContext()->duration);


    // Placeholder for generated curves

};

void GrassTreeGrow::Update() {
    Effect::Update();
    _ground->Update();
    _tube.Update();
    FOR_EACH(tubeIt, _tubes) {
        tubeIt->Update();
    }
    _milkyway.Update();
    _tree.Update();
};

void GrassTreeGrow::Draw() {
    Programs& progs = Programs::GetInstance();
    Effect::Draw();
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    static float stickyTime = 0.0f;
    float time = GetContext()->elapsedTime;

    stickyTime = int(time*10.f) / 10.f;
    float SPB = .5f * (1.0f / BPS);
    stickyTime = int(time / SPB) * SPB;

    // --------------------------------------------------------------------------------
    // Camera
    // --------------------------------------------------------------------------------
    bool fixedCam = false;

    PerspCamera& cam = GetContext()->mainCam;
    cam.center= vec3(0,1,-15);
    cam.center.x = -3;
    cam.center.y = 0;
    cam.center.z = 2;
    cam.eye.x = -9;
    cam.eye.y = 1.5;
    cam.eye.z = 8;
    
    if (not fixedCam)// and not bloomMode)
    {
        cam.eye = cameraPoints.At(GetContext()->elapsedTime);
        //cam.eye.x += cos(stickyTime);
        //cam.eye.z += sin(stickyTime);
    }
    else {
        cam.eye = cameraPoints.At(10);
        //cam.eye = vec3(-4*sin(t/2), .5*(5+-5*cos(t/2)), -4*sin(t/2));
        //cam.eye = vec3(-5*sin(t/2), .5*(1+-1*cos(t/2)), -5*cos(t/2));
    }

    // look where we are walking
    //  cam.center = cameraPoints.After(0);
    //cam.center = cameraPoints.After(GetContext()->elapsedTime);

    // --------------------------------------------------------------------------------
    // Drawable stuff
    // --------------------------------------------------------------------------------
    glUseProgram(progs["FireFlies.Tube"]);
    cam.Bind(glm::translate(glm::mat4(), glm::vec3(0, -1.5, 0)));
    glUniform3f(u("Eye"), cam.eye.x, cam.eye.y, cam.eye.z);
    //glUniform3f(u("HazardCenter"), _tree.pos.x, _tree.pos.y, _tree.pos.z);
    glUniform1f(u("Time"), time);
    _tube.Draw();

    FOR_EACH(tubeIt, _tubes) {
        tubeIt->Draw();
    }

    _tree.Draw(time); //15 + 15*sin(time));
    _fireFlies.Draw();
    _ground->Draw();

    _milkyway.Draw();

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
};

