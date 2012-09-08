#include "milkyway.h"

#include "common/camera.h"
#include "common/demoContext.h"
#include "common/programs.h"

void Milkyway::Init() {
    name = "Milkyway";
    Effect::Init();

    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("Milkyway.Sky"));
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
    // We draw the sky in clip coordinates
    // but the eye and center values of the camera are used
    // to position the sky backdrop
    glUniform3f(u("Eye"), cam.eye.x, cam.eye.y, cam.eye.z);
    glUniform3f(u("Center"), cam.center.x, cam.center.y, cam.center.z);
    _skyQuad.Draw();
}

