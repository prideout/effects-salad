#include "tree.h"

#include <glm/gtc/matrix_transform.hpp>

#include "common/camera.h"
#include "common/demoContext.h"
#include "common/programs.h"

void Tree::Init() {
    name = "Tree";
    Effect::Init();

    //Programs& progs = Programs::GetInstance();
    //glUseProgram(progs.Load("Tree.Sky", true));
}

void Tree::Update() {
    Effect::Update();
}

void Tree::Draw() {
    //Programs& progs = Programs::GetInstance();
    Effect::Draw();

    //glUseProgram(progs["Tree.Sky"]);

    //PerspCamera cam = GetContext()->mainCam;
    //cam.Bind(glm::mat4());
}

