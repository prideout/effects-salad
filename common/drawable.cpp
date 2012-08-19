#include "drawable.h"


Drawable::Drawable() :
    context(DemoContext::GetCurrent())
{
}

/* virtual */
Drawable::~Drawable() {
}


/* virtual */
void
Drawable::Init() {

}

/* virtual */
void
Drawable::Draw() {
}

/* virtual */
void
Drawable::Update() {

}
