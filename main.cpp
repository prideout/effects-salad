#include "lib/pez/pez.h"

#include "common/camera.h"
#include "common/demoContext.h"
#include "common/init.h"
#include "common/programs.h"
#include "common/surface.h"
#include "fx/all.h"
#include "jsoncpp/json.h"

DemoContext* context;

// TODO : make this function read from JSON
static void _constructScene()
{
    // Json::Value scene;
    // ReadJsonFile("data/scene.json", &scene);

    context->drawables.push_back(new Quads());

    Portal* portal = new Portal();
    portal->Init();
    portal->portalContext->viewport.x = 50;
    portal->portalContext->viewport.y = 250;
    portal->portalContext->viewport.width = 200;
    portal->portalContext->viewport.height = 100;
    portal->portalContext->drawables.push_back(new Background(vec4(0.6,.2,0.2,1)));
    portal->portalContext->drawables.push_back(new Quads());
    portal->portalContext->Init();
    context->drawables.push_back(portal);

    context->drawables.push_back(new FpsOverlay());
}

void PezInitialize()
{
    context = DemoContext::SetCurrent(DemoContext::New());
    PezConfig cfg = PezGetConfig();
    // add our shader path
    pezSwAddPath("", ".glsl");
    _constructScene();
    context->Init();
    context->mainCam.eye.z = 5;
    context->mainCam.aspect = cfg.Width / cfg.Height;
}

PezConfig PezGetConfig()
{
    PezConfig config;
    config.Title = __FILE__;
    config.Width = 2560/4;
    config.Height = 1600/4;
    config.Multisampling = false;
    config.VerticalSync = true;
    return config;
}

void PezHandleMouse(int x, int y, int action)
{
    if (action == PEZ_DOWN) {
    } else if (action == PEZ_UP) {
    }
}

void PezRender()
{
    context->Render();
}

void PezUpdate(float seconds)
{
    context->Update(seconds);
}
