#include "light.h"


Light::Light() : Drawable(),
    geometry(NULL)
{

}

void
Light::Init() {
    if (not geometry) return;
    geometry->Init();
}

void
Light::Update() {
    if (not geometry) return;
    geometry->Update();
}

void
Light::Draw() {
    if (not geometry) return;
    geometry->Draw();
}

