#include <string>
#include <map>

#include "lib/pez/pez.h"

#include "common/camera.h"
#include "common/demoContext.h"
#include "common/init.h"
#include "common/programs.h"
#include "common/surface.h"
#include "fx/all.h"
#include "jsoncpp/json.h"

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
    typedef std::map<std::string,DemoContext*> ShotMap;
    ShotMap shotMap;
     
    //
    // Build up the shot map, this idea is evolving...
    //

    {   // Grass Intro 
        DemoContext* ctx = DemoContext::New();
        DemoContext::SetCurrent(ctx);
        ctx->mainCam.eye.z = 5;
        ctx->drawables.push_back(new FireFlies());
        ctx->drawables.push_back(new FpsOverlay());
        shotMap["GrassIntro"] = ctx;
    }


    {   // City Intro
        DemoContext* ctx = DemoContext::New();
        DemoContext::SetCurrent(ctx);
        ctx->mainCam.eye.z = 5;
        ctx->mainCam.eye.x = 50;
        ctx->mainCam.eye.y = 50;
        ctx->drawables.push_back(new Buildings());
        ctx->drawables.push_back(new FpsOverlay());
        shotMap["CityIntro"] = ctx;

    }

    {   // Test 
        DemoContext* ctx = DemoContext::New();
        DemoContext::SetCurrent(ctx);
        ctx->mainCam.eye.z = 5;
        ctx->drawables.push_back(new Quads());

        Portal* portal = new Portal();
        portal->Init();
        portal->portalContext->viewport.x = 10;
        portal->portalContext->viewport.y = 316;
        portal->portalContext->viewport.width = 150;
        portal->portalContext->viewport.height = 75;
        portal->portalContext->drawables.push_back(new Background(vec4(0.6,.2,0.2,1)));
        portal->portalContext->drawables.push_back(new Quads());
        portal->portalContext->Init();
        portal->portalContext->mainCam = ctx->mainCam;
        ctx->drawables.push_back(portal);
        
        // ordering is important here... need to fix this
        DemoContext::SetCurrent(ctx);
        ctx->drawables.push_back(new FpsOverlay());
        shotMap["Test"] = ctx;
    }


    std::vector<DemoContext*> sequence;
    Json::Value script;
    ReadJsonFile("data/script.json", &script);
    FOR_EACH(element, script) {
        Json::Value cur = *element;
        if (shotMap.find(cur[0u].asString()) != shotMap.end()) {
            sequence.push_back(shotMap[cur[0u].asString()]);
            std::cerr << "Added " << cur[0u] << std::endl;
        } else {
            std::cerr << "WARNING: shot not found '" << cur[0u] << "'" << std::endl;
        }
    }

    //
    // Check to see if a specific shot was pass on the command line
    //
    std::string shot = _getShotName();

    if (shot != "") {
        if (shotMap.find(shot) == shotMap.end()) {
            std::cerr << "ERROR: Shot not found '" << shot << "', aborting" << std::endl;
            exit(1);
        }

        sequence.clear();
        sequence.push_back(shotMap[shot]);
    }

    // TODO:
    //  - save duration 
    //  - make sequence some sort of global state
    //  - make shot map global state
    //  - iterate over the sequence at update time

    DemoContext::SetCurrent(sequence[0]);
}

void PezInitialize()
{
    // add our shader path
    pezSwAddPath("", ".glsl");
    _constructScene();
    DemoContext::GetCurrent()->Init();
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
    DemoContext::GetCurrent()->Render();
}

void PezUpdate(float seconds)
{
    DemoContext::GetCurrent()->Update(seconds);
}
