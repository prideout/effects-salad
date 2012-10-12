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
    //if (bloomMode) 
    //    _tree.grown = true;

    _surface.Init();
    _quad.Init();
    _milkyway.Init();
    
    if (bloomMode) {
        _tree.leafDropMultiplier = 2.0f;
    }
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
    cvs[6] = _tree.pos;
    // XXX: this is time independent right now
    //      need to make it explicit in time if we're going to keep the camera motion
    cameraPoints = AnimCurve<glm::vec3>(cvs, 0, GetContext()->duration);


    // Placeholder for generated curves
    

    {
        // intro cam
        float cameraPath[] = {
            // intro camera
            -73.889963783203328, 1.9022898114153644, -15.880825318011672,
            -70.139848876016615, 1.9022898114153644, -14.874696928278611,
            -62.68034894083236, 1.757119043952587, -8.9160731012576164,
            -58.020746383103905, 1.6045803485703456, -7.8094230808761287,
            -53.36114382537545, 1.4520416531881042, -6.7027730604946401,
            -46.219941113223165, 1.1596638655462193, -7.1143489800664916,
            -40.00023834032784, 1.1596638655462193, -6.0167543730849786,
            -33.780535567432516, 1.1596638655462193, -4.9191597661034656,
            -29.398405178043745, 0.3691749981521088, -3.3890212359114953,
            -22.081107798166876, 0.3691749981521088, -3.9378185394022491,
        };

        Vec3List cvs;
        size_t cvCount = sizeof(cameraPath) / sizeof(float);
        for(size_t i = 0; i < cvCount; i+=3) {
            cvs.push_back(vec3(cameraPath[i+0],cameraPath[i+1],cameraPath[i+2]));
        }

        _tube.cvs = cvs;
        // under stop time
        _tube.timeToGrow = GetContext()->duration - (13.0f + 5.0f);
        _tube.startTime = 0.0f;
        std::cout << "Grow Time: " << _tube.timeToGrow << std::endl;
        _tube.Init();

        introCameraPoints = AnimCurve<glm::vec3>(cvs, 0, GetContext()->duration);
    }
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
    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _ground->brightness = 1.0;

    static float stickyTime = 0.0f;
    float time = GetContext()->elapsedTime;
    float rtime = GetContext()->duration - time;

    stickyTime = int(time*10.f) / 10.f;
    float SPB = .5f * (1.0f / BPS);
    stickyTime = int(time / SPB) * SPB;

    static float hhCount = 0;
    static float flash = 0;

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
        cam.eye = cameraPoints.At(time);
        //cam.eye.x += cos(stickyTime);
        //cam.eye.z += sin(stickyTime);
    }
    else {
        cam.eye = cameraPoints.At(10);
        //cam.eye = vec3(-4*sin(t/2), .5*(5+-5*cos(t/2)), -4*sin(t/2));
        //cam.eye = vec3(-5*sin(t/2), .5*(1+-1*cos(t/2)), -5*cos(t/2));
    }

    float underStart = 11.f;
    float underStop = 14.5f;
    float overStop = 36.f;
    if (bloomMode) {
        underStart = 3.0f + 5.0f;
        underStop = 13.0f + 5.0f;

        //
        // Fade out 
        //
        if (fullscreen) {
            float screenFadeTime = 4.0;
            fullscreen->brightness = glm::smoothstep(0.0f, 1.0f, glm::min(1.0f, rtime / screenFadeTime));
            fullscreen->_mask |= Fullscreen::BrightnessFlag;
        }

    }
    if (time > underStart and time < underStop) {
        if (bloomMode) {
            fullscreen->_mask |= Fullscreen::ScanLinesFlag;
        }
        if (time > 13.3 and time < 14.5) {
            flash = 0.f;//.5f;//1.f + sinf(time*240);
        }
        if (time > 13.3 and GetContext()->audio->GetHiHats()) {
            hhCount += 1.0f;
            fullscreen->_mask |= Fullscreen::ScanLinesFlag;
        }

        fullscreen->brightness = 1.0 - flash;
        flash *= .9;

        glm::vec3 orig = cam.eye;
        cam.eye = _tree.pos;
        cam.eye.x += 3.0 * cos(hhCount*.25 + time / 4);// + 5.0*glm::smoothstep(underStart, underStop, time);
        cam.eye.y += 0.9; // + (2 + sin(hhCount*.25 + time));
        cam.eye.z -= 3.0 * sin(hhCount*.25 + time / 4);
        //cam.eye = glm::mix(cam.eye, orig, glm::smoothstep(underStart, underStop, time));
    } else if (bloomMode and time > underStop) {
        fullscreen->_mask &= ~Fullscreen::ScanLinesFlag;
        cam.center= vec3(0,1,-15);
        cam.center.x = -3;
        cam.center.y = 0;
        cam.center.z = 2;
        cam.eye.x = -9;
        cam.eye.y = 1.5;
        cam.eye.z = 8;
    
        cam.eye = introCameraPoints.At(rtime);

    } else if (time > underStop and time < overStop) {
        fullscreen->brightness = 1.0;
        fullscreen->_mask &= ~Fullscreen::ScanLinesFlag;
        fullscreen->solidColor = vec4(0,0,0,1);
        _ground->vibration = 0.0f; //.5+.5*sin(time); 
        if (
            (GetContext()->audio->GetSnares() 
             or GetContext()->audio->GetKicks())) {
            hhCount += 1.0f;
            flash = .1f;
        } else if (GetContext()->audio->GetHiHats()) {
            hhCount += .1f;
        }
        flash = .5*sin(hhCount+time*240);
        fullscreen->brightness = 1.0 - flash;
        flash *= .9;
        cam.eye = _tree.pos;
        cam.eye.x += 7.0 * cos(hhCount*.25 + time / 4);// + 5.0*glm::smoothstep(underStart, underStop, time);
        cam.eye.y += 2.7; // + (2 + sin(hhCount*.25 + time));
        cam.eye.z -= 7.0 * sin(hhCount*.25 + time / 4);
    } else {
        _ground->vibration = 0.0f; 
        fullscreen->brightness = 1.0f;
        fullscreen->_mask &= ~Fullscreen::ScanLinesFlag;
    }

    // crazy camera shake
    //cam.eye += .5f+.5f*sinf(time) * .3f*glm::vec3(0.0, sin(time*140), 0.);

    // look where we are walking
    //  cam.center = cameraPoints.After(0);
    //cam.center = cameraPoints.After(GetContext()->elapsedTime);

    // --------------------------------------------------------------------------------
    // Drawable stuff
    // --------------------------------------------------------------------------------
    glUseProgram(progs["FireFlies.Tube"]);
    cam.Bind(glm::translate(glm::mat4(), glm::vec3(0, -1.5, 0)));
    //cam.Bind(glm::mat4());
    glUniform3f(u("Eye"), cam.eye.x, cam.eye.y, cam.eye.z);
    //glUniform3f(u("HazardCenter"), _tree.pos.x, _tree.pos.y, _tree.pos.z);
    glUniform1f(u("Time"), time);

    if (bloomMode and time > underStop) {
        // no clue why this doesn't work :(
        _tube.Draw(rtime) ;//rtime*200); //(GetContext()->duration + underStop) - (time - underStop));
    }

    FOR_EACH(tubeIt, _tubes) {
        tubeIt->Draw();
    }

    if (bloomMode) {
        _tree.Draw(GetContext()->duration - (time+9.6)); //15 + 15*sin(time));
    } else {
        _tree.Draw(time); //15 + 15*sin(time));
    }
    _fireFlies.Draw();
    _ground->Draw();

    _milkyway.Draw();

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
};

