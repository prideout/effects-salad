#include "grassIntro.h"
#include "common/programs.h"
#include "common/demoContext.h"

#include "noise/perlin.h"
#include "glm/core/func_geometric.hpp"

#include <cstdlib>

void GrassIntro::Init() 
{
    Perlin noise(2, .1, 2, 0);
    name = "GrassIntro";
    Effect::Init();
    Programs& progs = Programs::GetInstance();

    glUseProgram(progs.Load("FireFlies.Blur"));
    glUseProgram(progs.Load("FireFlies.Tube", "FireFlies.Tube.FS", "FireFlies.Tube.VS"));
    progs.Load("FireFlies.Sig", "FireFlies.Sig.FS", "FireFlies.Tube.VS");
    glUniform1i(u("Tex"), 1);

    _surface.Init();
    _quad.Init();
    _milkyway.Init();
    _fireFlies.Init();



    // --------------------------------------------------------------------- 
    // Camera Path
    // --------------------------------------------------------------------- 

    float cameraPath[] = {
        /* LOOP:
         0,    0,  4.04,
        -7.11, 0,  4.06,
        -8.11, 0, -4.10,
         0.05, 0, -3.99,
         8.22, 0, -3.88,
         7.35, 0,  4.06,
         0.01, 0,  4.04
         */
        
        // Acurve with discontinuous derrivitives for testing piecewise evaluator
        /*
        -6.7293233082706774, 0, -3.7468671679198229,
        -3.0200501253132837, 0, 0.56390977443606882,
        -3.796992481203008, 0, -4.9749373433584205,
        -2.1679197994987476, 0, -3.6716791979950112,
        -2.1177944862155398, 0, -1.6165413533834809,
        -2.1428571428571437, 0, -1.7919799498747087,
        -2.1679197994987476, 0, 0.81453634085210813,
        -5.8270676691729326, 0, 1.5413533834586246,
        -7.030075187969925, 0, 2.9448621553884502,
        -6.9799498746867172, 0, 0.93984962406012895,
        */

         // bigger path, not super great
         /*
        -9.483426722726314,  0, 3.6461550396041615,
        -7.4655181591348239, 0, 0.48813003348189232,
        -14.481911565751737, 0, -7.5490815608705866,
        -9.7573163031660641, 0, -4.6390047686982516,
        -5.032721040580391,  0, -1.7289279765259167,
        0.13694479021987505, 0, -10.938465118812484,
        6.5733499305539773,  0, -7.5490815608705875,
        13.00975507088808,   0, -4.1596980029286907,
        17.871295123693628,  0, -4.4335875833684417,
        17.802822728583692,  0, 0.73607824743182559,
        17.734350333473756,  0, 5.9057440782320931,
        12.256558724678776,  0, 11.31506329191714,
        5.8543897818996387,  0, 9.9798515872733624,
        -0.54777916087949841, 0, 8.6446398826295852,
        2.3622976312928383,  0, 0.39371627188213765,
        -5.5462640039049189, 0, 5.8715078806771226,
        -13.454825639102676, 0, 11.349299489472108,
        -11.948432946684054, 0, 7.3094281779858079,
        -9.483426722726314,  0, 3.6461550396041615,
        */


        // new camera
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
		/*
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
                */

        //-9.5176629202812819, 0, 3.6461550396041615
        /*
        0, -1.9873866943023757, 3.9720817637136108,
        0, -1.231492554543711, 0.76853040949831852,
        0, -0.94353288225469822, -0.059353648332600442,
        0, 0.01033353220266591, -0.041356168814537256,
        0, 0.96419994666003006, -0.023358689296474069,
        0, 0.60425035629876134, 2.568278361304662,
        0, 1.0181923852142203, 3.9720817637136108,
        0, 1.6301066888283759, 2.3523086070879016,
        0, 0.44227304063618833, -0.041356168814537145,
        0, 1.9720587996715817, -0.059353648332600442,
        0, 3.5018445587069751, -0.07735112785066374,
        0, 3.3578647225624669, 3.1981901444368832,
        0, 3.0339100912373258, 3.9540842841955484
        */
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
    //#include "sigCurve.h"
};

void GrassIntro::Update() {
    Effect::Update();
    _ground->Update();
    _tube.Update();
    FOR_EACH(tubeIt, _tubes) {
        tubeIt->Update();
    }
    _milkyway.Update();
};

void GrassIntro::Draw() {
    Programs& progs = Programs::GetInstance();
    Effect::Draw();
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    float time = GetContext()->elapsedTime;

    float fliesFadeTime = 14.0;
    float starsFadeTime = 8.0;
    float grassFadeTime = 5.0;
    float screenFadeTime = 10.0;

    _fireFlies.brightness = glm::smoothstep(0.0f, 1.0f, glm::min(1.0f, time / fliesFadeTime));
    _milkyway.brightness = glm::smoothstep(0.0f, 1.0f, glm::min(1.0f, time / starsFadeTime));
    _ground->brightness = glm::smoothstep(0.0f, 1.0f, glm::min(1.0f, time / grassFadeTime));
    
    //
    // Fade in
    //
    if (fullscreen) {
        fullscreen->brightness = glm::smoothstep(0.0f, 1.0f, glm::min(1.0f, time / screenFadeTime));
    }
    
    
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
    
    if (not fixedCam)
        cam.eye = cameraPoints.At(time);
    else {
        float t = time; 
        cam.eye = vec3(-4*sin(t/2), .5*(5+-5*cos(t/2)), -4*sin(t/2));
        //cam.eye = vec3(-5*sin(t/2), .5*(1+-1*cos(t/2)), -5*cos(t/2));
        cam.eye = cameraPoints.At(10);
        cam.eye.x = 20;
        cam.eye.z = 0;
        cam.center = glm::vec3(0.0f);
    }

    // look where we are walking
    //  cam.center = cameraPoints.After(0);
    //cam.center = cameraPoints.After(GetContext()->elapsedTime);

    // --------------------------------------------------------------------------------
    // Drawable stuff
    // --------------------------------------------------------------------------------
    glUseProgram(progs["FireFlies.Sig"]);
    cam.Bind(glm::translate(glm::mat4(), glm::vec3(0, -1.5, 0)));
    glUniform3f(u("Eye"), cam.eye.x, cam.eye.y, cam.eye.z);
    glUniform3f(u("MaterialColor"), .1, .9, .2);
    //glUniform3f(u("MaterialColor"), .35, .55, 1.0);
    glUniform1f(u("Time"), time);
    _tube.Draw();

    FOR_EACH(tubeIt, _tubes) {
        //tubeIt->Draw();
    }

    _fireFlies.Draw();
    _ground->Draw();
    _milkyway.Draw();

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
};

