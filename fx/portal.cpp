#include "fx/portal.h"
#include "common/demoContext.h"
#include "common/typedefs.h"
#include "common/drawable.h"

Portal::Portal()
{
    portalContext = 0;
}

void
Portal::Init()
{
    if (portalContext) {
        return;
    }
    name = "Portal";
    Effect::Init();
    portalContext = DemoContext::SetCurrent(DemoContext::New());
}

void
Portal::Update()
{
    Effect::Update();
    // save off the time delta before switching context
    float dt = GetContext()->deltaTime;
    DemoContext* previous = DemoContext::GetCurrent();
    DemoContext::SetCurrent(portalContext);
    portalContext->Update(dt);
    DemoContext::SetCurrent(previous);
}

void Portal::Draw()
{
    Effect::Draw();
    DemoContext* previous = DemoContext::GetCurrent();
    DemoContext::SetCurrent(portalContext);

    // Don't call portalContext->Render() because
    // doing so will clear the entire backbuffer
    FOR_EACH(drawable, portalContext->drawables) {
        (*drawable)->Draw();
    }

    DemoContext::SetCurrent(previous);
}
