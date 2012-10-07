#include "milkyway.h"

#include <glm/gtc/matrix_transform.hpp>

#include "common/camera.h"
#include "common/demoContext.h"
#include "common/programs.h"

void Milkyway::Init() {
    name = "Milkyway";
    Effect::Init();

    Programs& progs = Programs::GetInstance();
    progs.Load("Milkyway.Sky", true);
    progs.Load("FireFlies.Stars", "FireFlies.Stars.FS", "FireFlies.Flies.VS"); 

    // --------------------------------------------------------------------- 
    // Sky background 
    // --------------------------------------------------------------------- 
    _skyQuad.Init();

    // --------------------------------------------------------------------- 
    // Stars 
    // --------------------------------------------------------------------- 

    FloatList stars;
    for (int i = 0; i < 4000; i ++) {
        // Use spherical coordinates with fixed radius to simulate a sky dome
        float r = 120;
        float theta = 3.14*(rand() / float(RAND_MAX)); 
        float phi = 3.14*(rand() / float(RAND_MAX));
        stars.push_back(r*sin(theta)*cos(phi) - 10);
        stars.push_back(r*sin(theta)*sin(phi));
        stars.push_back(r*cos(theta) + 10);
        stars.push_back(1.0);
    }
    _stars = Vao(4, stars); 

}

void Milkyway::Update() {
    Effect::Update();
}

void Milkyway::Draw() {
    Programs& progs = Programs::GetInstance();
    Effect::Draw();

    PerspCamera& cam = GetContext()->mainCam;

    // --------------------------------------------------------------------- 
    // Stars 
    // --------------------------------------------------------------------- 
    glUseProgram(progs["FireFlies.Stars"]);

    bool hihat = GetContext()->audio->GetHiHats();
    bool snare = GetContext()->audio->GetSnares();

    cam.Bind(glm::mat4());
    if (snare)
        glPointSize(5.5);
    else if (hihat)
        glPointSize(1.5);
    else
        glPointSize(.5);
    _stars.Bind();
    glDrawArrays(GL_POINTS, 0, _stars.vertexCount);

    // --------------------------------------------------------------------- 
    // Sky Quad
    // ---------------------------------------------------------------------     
    glUseProgram(progs["Milkyway.Sky"]);

    glUniform3f(u("Eye"), cam.eye.x, cam.eye.y, cam.eye.z);
    glUniform3f(u("Center"), cam.center.x, cam.center.y, cam.center.z);

    //cam.center = GetContext()->mainCam.eye;
    //cam.eye = glm::vec3(0,0,0);
    // draw the sky in clip coordinates
    // but the eye and center values of the camera are used
    // to position the sky backdrop
    glm::mat4 loc;
    vec3 dir = glm::normalize(cam.center - cam.eye);
    float azimuth = -atan2(dir.x, dir.z) / (3.1415); 
    float altitude = atan2(dir.y, dir.z) / (3.1415); 

    glUniform1f(u("Azimuth"), azimuth);
    glUniform1f(u("Altitude"), altitude);
    loc = glm::translate(glm::mat4(), glm::vec3(cam.eye.x, cam.eye.y, cam.eye.z-1.5));
    cam.Bind(loc);
    _skyQuad.Draw();
}

