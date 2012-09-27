#include "milkyway.h"

#include <glm/gtc/matrix_transform.hpp>

#include "common/camera.h"
#include "common/demoContext.h"
#include "common/programs.h"

void Milkyway::Init() {
    name = "Milkyway";
    Effect::Init();

    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("Milkyway.Sky", true));
    _skyQuad.Init();
}

void Milkyway::Update() {
    Effect::Update();
}

void Milkyway::Draw() {
    Programs& progs = Programs::GetInstance();
    Effect::Draw();

    glUseProgram(progs["Milkyway.Sky"]);

    PerspCamera cam = GetContext()->mainCam;
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

