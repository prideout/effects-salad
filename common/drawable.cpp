#include "drawable.h"
#include "demoContext.h"


Drawable::Drawable() :
    visible(true)
{
}

/* virtual */
Drawable::~Drawable() {
}

DemoContext*
Drawable::GetContext() {
    return DemoContext::GetCurrent();
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
