#include "drawable.h"
#include "demoContext.h"


Drawable::Drawable() :
    visible(true),
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
