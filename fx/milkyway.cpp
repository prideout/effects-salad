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
    for (int i = 0; i < 19000; i ++) {
        // Use spherical coordinates with fixed radius to simulate a sky dome
        float r = 120;
        float theta = 3.14*(rand() / float(RAND_MAX)); 
        float phi = 3.14*(rand() / float(RAND_MAX));
        stars.push_back(r*sin(theta)*cos(phi));// - 10);
        stars.push_back(r*sin(theta)*sin(phi));
        stars.push_back(r*cos(theta));// + 10);

        // set star size multiplier
        float starSize = rand() / float(RAND_MAX);
        if (starSize < .7) {
            stars.push_back(1.0 - starSize);
        } else if (starSize >= .7 and starSize < .99) {
            stars.push_back(1.0 + starSize / 2.0);
        } else { 
            stars.push_back(2.0);
        }
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

    glm::mat4 loc;

    // --------------------------------------------------------------------- 
    // Stars 
    // --------------------------------------------------------------------- 
    glUseProgram(progs["FireFlies.Stars"]);

    glUniform1f(u("Time"), GetContext()->elapsedTime);

    bool hihat = GetContext()->audio->GetHiHats();
    bool snare = GetContext()->audio->GetSnares();

    if (snare)
        glUniform1f(u("SizeMult"), 3.0);
    else
        glUniform1f(u("SizeMult"), 1.0);

    // 
    // Move stars down just a bit to hide the seam where they stop spawning
    //
    loc = glm::translate(glm::mat4(), glm::vec3(0.0f, -.75f, 0.0f));
    cam.Bind(loc);
    if (snare)
        glPointSize(5.5);
    else if (hihat)
        glPointSize(1.5);
    else
        glPointSize(.5);
    _stars.Bind();
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
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
    vec3 dir = glm::normalize(cam.center - cam.eye);
    float azimuth = -atan2(dir.x, dir.z) / (3.1415); 
    float altitude = atan2(dir.y, dir.z) / (3.1415); 

    glUniform1f(u("Azimuth"), azimuth);
    glUniform1f(u("Altitude"), altitude);
    loc = glm::translate(glm::mat4(), glm::vec3(cam.eye.x, cam.eye.y, cam.eye.z-1.5));
    cam.Bind(loc);
    _skyQuad.Draw();
}

