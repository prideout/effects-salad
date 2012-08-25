#include "fx/fpsOverlay.h"
#include "common/programs.h"

const Glyph FpsOverlay::NumeralGlyphs[] = {
    {{ 0, 0 }, { 2, -19, 16, 19, 19, 0 }},
    {{ 19, 0 }, { 1, -20, 14, 20, 16, 0 }},
    {{ 37, 0 }, { 1, -19, 17, 19, 20, 0 }},
    {{ 57, 0 }, { 0, -18, 16, 23, 17, 0 }},
    {{ 77, 0 }, { 0, -19, 21, 24, 22, 0 }},
    {{ 102, 0 }, { 0, -18, 18, 23, 17, 0 }},
    {{ 123, 0 }, { 3, -23, 19, 23, 21, 0 }},
    {{ 146, 0 }, { 2, -18, 18, 23, 18, 0 }},
    {{ 168, 0 }, { 2, -23, 15, 23, 18, 0 }},
    {{ 186, 0 }, { 0, -17, 19, 24, 20, 0 }},
};

void
FpsOverlay::Init()
{
    name = "FpsOverlay";
    Effect::Init();
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("Fps"));
    _numerals.Init("numerals.png");
}

void
FpsOverlay::Update()
{
    Effect::Update();
}

void
FpsOverlay::Draw()
{
    Effect::Draw();
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("Fps"));
}
