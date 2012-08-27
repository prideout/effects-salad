#include "glm/gtc/type_ptr.hpp"
#include "fx/background.h"
#include "common/programs.h"
#include "common/init.h"

using namespace std;
using namespace glm;

void
Background::Init()
{
    name = "Background";
    Effect::Init();
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("Background"));
    glGenVertexArrays(1, &_vao);
}

void
Background::Update()
{
    Effect::Update();
}

void
Background::Draw()
{
    Effect::Draw();
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs["Background"]);
    glUniform4fv(u("Color"), 1, ptr(_color));
    glBindVertexArray(_vao);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
