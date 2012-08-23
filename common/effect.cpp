#include "effect.h"

bool Effect::_enableDebug(false);

Effect::Effect() : Drawable(),
    name("ERROR")
{

}

Effect::~Effect() {
}

void
Effect::Init() {
    timer.Init();
    timer.name = name;
}

void
Effect::Update() {
    if (_enableDebug)
        timer.Update();
}

void
Effect::Draw() {
    if (_enableDebug)
        timer.Draw();
}
