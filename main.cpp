#include <string>

#include "lib/pez/pez.h"

#include "common/camera.h"
#include "common/demoContext.h"
#include "common/init.h"
#include "common/programs.h"
#include "common/surface.h"
#include "fx/all.h"
#include "jsoncpp/json.h"

DemoContext* context;

static std::string _getShotName() 
{
    int argc;
    const char** argv;
    pezGetArgs(&argc, &argv);
    if (argc < 2) return "";
    return std::string(argv[1]);
}

// TODO : make this function read from JSON
static void _constructScene()
{
    
    // Json::Value scene;
    // ReadJsonFile("data/scene.json", &scene);

    //
    // Check to see if a specific shot was pass on the command line
    //
    std::string shot = _getShotName();

    if (shot == "grassIntro") {
        context->drawables.push_back(new FireFlies());
        context->drawables.push_back(new FpsOverlay());
        DemoContext::SetCurrent(context);
        return;

    } else if (shot == "cityIntro") {
        context->mainCam.eye.x = 50;
        context->mainCam.eye.y = 50;
        context->drawables.push_back(new Buildings());
        context->drawables.push_back(new FpsOverlay());
        DemoContext::SetCurrent(context);
        return;

    }


    context->drawables.push_back(new Quads());

    Portal* portal = new Portal();
    portal->Init();
    portal->portalContext->viewport.x = 10;
    portal->portalContext->viewport.y = 316;
    portal->portalContext->viewport.width = 150;
    portal->portalContext->viewport.height = 75;
    portal->portalContext->drawables.push_back(new Background(vec4(0.6,.2,0.2,1)));
    portal->portalContext->drawables.push_back(new Quads());
    portal->portalContext->Init();
    portal->portalContext->mainCam = context->mainCam;
    context->drawables.push_back(portal);
    DemoContext::SetCurrent(context);

    context->drawables.push_back(new FpsOverlay());
}

void PezInitialize()
{
    //PezConfig cfg = PezGetConfig();

    context = DemoContext::SetCurrent(DemoContext::New());
    context->mainCam.eye.z = 5;

    // add our shader path
    pezSwAddPath("", ".glsl");
    _constructScene();
    context->Init();
}

PezConfig PezGetConfig()
{
    PezConfig config;
    config.Title = __FILE__;
    config.Width = 2560/3;
    config.Height = 1600/3;
    config.Multisampling = true;
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
