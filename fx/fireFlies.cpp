#include "fireFlies.h"
#include "common/programs.h"
#include "common/demoContext.h"

#include "noise/perlin.h"
#include "glm/core/func_geometric.hpp"

#include <cstdlib>

void FireFlies::Init() 
{
    Perlin noise(2, .1, 2, 0);
    name = "FireFlies";
    Effect::Init();
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("FireFlies.Flies"));

    glUseProgram(progs.Load("FireFlies.Blur"));
    glUseProgram(progs.Load("FireFlies.Stars", "FireFlies.Stars.FS", "FireFlies.Flies.VS"));
    glUniform1i(u("Tex"), 1);

    _surface.Init();
    _quad.Init();
    _ground.Init();

    // --------------------------------------------------------------------- 
    // Fire Flies
    // --------------------------------------------------------------------- 

    // XXX: need to call srand in a more common location
    srand(0);
    for (int i = 0; i < 1000; i ++) {
        float x = 80*(rand() / float(RAND_MAX)) - 40; 
        float y = 3*(rand() / float(RAND_MAX)) - 2; 
        float z = 80*(rand() / float(RAND_MAX)) - 40; 
        //n = noise.Get(
        //std::cout << x << ", " << y << ", " << z << "\n";
        _fliesCpu.push_back(x);
        //_fliesCpu.push_back(noise.Get(x,z));
        _fliesCpu.push_back(y);
        _fliesCpu.push_back(z);
        _fliesCpu.push_back(1.0);
    }
    _fliesGpu = Vao(4, _fliesCpu); 


    // --------------------------------------------------------------------- 
    // Stars 
    // --------------------------------------------------------------------- 

    FloatList stars;
    for (int i = 0; i < 2000; i ++) {
        // Use spherical coordinates with fixed radius to simulate a sky dome
        float r = 80;
        float theta = 3.14*(rand() / float(RAND_MAX)); 
        float phi = 2*3.14*(rand() / float(RAND_MAX));
        stars.push_back(r*sin(theta)*cos(phi) - 10);
        stars.push_back(r*sin(theta)*sin(phi));
        stars.push_back(r*cos(theta) + 10);
        stars.push_back(1.0);
    }
    _stars = Vao(4, stars); 


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

         // bigger path, not super great
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
        //-9.5176629202812819, 0, 3.6461550396041615
        };
    Vec3List cvs;
    int cvCount = sizeof(cameraPath) / sizeof(float);
    for(unsigned i = 0; i < cvCount; i+=3) {
        cvs.push_back(vec3(cameraPath[i+0],cameraPath[i+1],cameraPath[i+2]));
    }
    _tube.cvs = cvs;
    _tube.Init();

    // XXX: this is time independent right now
    //      need to make it explicit in time if we're going to keep the camera motion
    cameraPoints = AnimCurve<glm::vec3>(cvs, 0, GetContext()->duration);
    counter = 0;
};

void FireFlies::Update() {
    Effect::Update();
    _ground.Update();
    _tube.Update();
};

void FireFlies::Draw() {
    //if (counter > cameraPoints.size() -1) counter = 0;
    Programs& progs = Programs::GetInstance();
    Effect::Draw();
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glUseProgram(progs["FireFlies.Flies"]);

        //_surface.Bind();
        PerspCamera oldCam = GetContext()->mainCam;
        PerspCamera& cam = GetContext()->mainCam;
        cam.eye = cameraPoints.At(GetContext()->elapsedTime); //[counter];
        //cam.eye.y = .0;
        //cam.center= vec3(0,1,-15); //cameraPoints[counter];
        //float t = GetContext()->elapsedTime;
        //cam.eye = vec3(-5*sin(t/2), 5+-5*cos(t/2), -5-5*sin(t/2)); //cameraPoints[counter];
        cam.eye.y += .5;

        // look where we are walking
        cam.center = cameraPoints.After(GetContext()->elapsedTime); //[counter+1 % cameraPoints.size()];
        cam.center.y += .5;
        cam.Bind(glm::mat4());
        _tube.Draw();
        
        /*
        glClearColor(.0,.0,.9,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        */
        glPointSize(1.5);
        _fliesGpu.Bind();
        glDrawArrays(GL_POINTS, 0, _fliesGpu.vertexCount);


        glUseProgram(progs["FireFlies.Stars"]);
        cam.Bind(glm::mat4());
        glPointSize(.5);
        _stars.Bind();
        glDrawArrays(GL_POINTS, 0, _stars.vertexCount);
        //_tube.DrawFrames();

        _ground.Draw();

    cam = oldCam;

    /*
    glUseProgram(progs["FireFlies.Blur"]);
    GetContext()->mainCam.Bind(glm::mat4());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _surface.texture);
    _quad.Draw();
    */
    counter++;
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
};

