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


unsigned shotIndex;
std::vector<DemoContext*> sequence;


static std::string _getShotName() 
{
    int argc;
    const char** argv;
    pezGetArgs(&argc, &argv);
    if (argc < 2) return "";
    return std::string(argv[1]);
}

static void _constructScene()
{
    typedef std::map<std::string,DemoContext*> ShotMap;
    ShotMap shotMap;

    //
    // Check to see if a specific shot was pass on the command line
    //
    std::string shot = _getShotName();

    //
    // Build up the shot map, this idea is evolving...
    //

    {   // Grass Intro 
        DemoContext* ctx = DemoContext::New("GrassIntro");
        DemoContext::SetCurrent(ctx);
        shotMap[ctx->name] = ctx;

        ctx->mainCam.eye.z = 5;
        ctx->drawables.push_back(new FireFlies());
        ctx->drawables.push_back(new FpsOverlay());
    }


    {   // City Intro
        DemoContext* ctx = DemoContext::New("CityIntro");
        DemoContext::SetCurrent(ctx);
        shotMap[ctx->name] = ctx;

        ctx->mainCam.eye.z = 5;
        ctx->mainCam.eye.x = 50;
        ctx->mainCam.eye.y = 50;

        // Instance the effects, but do not place them into the scene graph:
        Fullscreen* fullscreen1 = new Fullscreen(Fullscreen::VignetteFlag);
        fullscreen1->clearColor = vec4(0.1,0.9,1,1);
        Buildings* buildings = new Buildings(false);
        FpsOverlay* fps = new FpsOverlay();

        // Now, insert the effects into our poor man's "scene graph":
        ctx->drawables.push_back(fullscreen1);
        fullscreen1->AddChild(buildings);
        ctx->drawables.push_back(fps);
    }


    {   // City Explosion
        DemoContext* ctx = DemoContext::New("CityExplosion");
        DemoContext::SetCurrent(ctx);
        shotMap[ctx->name] = ctx;

        ctx->mainCam.eye.z = 5;
        ctx->mainCam.eye.x = 50;
        ctx->mainCam.eye.y = 50;

        // Instance the effects, but do not place them into the scene graph:
        Fullscreen* fullscreen1 = new Fullscreen(Fullscreen::VignetteFlag |
                                                 Fullscreen::TeleLinesFlag);
        fullscreen1->clearColor = vec4(0.1,0.9,1,1);
        Fullscreen* fullscreen2 = new Fullscreen("RadialBlur",
                                                 Fullscreen::BlendFlag |
                                                 Fullscreen::MipmapsFlag);
        fullscreen2->ShareDepth(fullscreen1);
        Buildings* buildings = new Buildings(true);
        FpsOverlay* fps = new FpsOverlay();

        // Now, insert the effects into our poor man's "scene graph":
        ctx->drawables.push_back(fullscreen1);
          fullscreen1->AddChild(buildings);
        ctx->drawables.push_back(fullscreen2);
          fullscreen2->AddChild(buildings->Cracks());
        ctx->drawables.push_back(fps);
    }

    {   // Test 
        DemoContext* ctx = DemoContext::New("Test");
        DemoContext::SetCurrent(ctx);
        shotMap[ctx->name] = ctx;

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
    }

    Json::Value script;
    ReadJsonFile("data/script.json", &script);
    FOR_EACH(element, script) {
        Json::Value cur = *element;
        if (shotMap.find(cur[0u].asString()) != shotMap.end()) {
            // XXX: can currently only use a context once, because duration is shared :(
            string curShot = cur[0u].asString();
            DemoContext::SetCurrent(shotMap[curShot]);
            shotMap[curShot]->duration = cur[1u].asDouble();
            sequence.push_back(shotMap[curShot]);
            if (shot.empty())  {
                shotMap[curShot]->Init();
            }
            //std::cout << "Added " << curShot << " duration: " << cur[1u].asDouble() << std::endl;
        } else {
            std::cerr << "WARNING: shot not found '" << cur[0u] << "'" << std::endl;
        }
    }

    if (sequence.size() > 1) {
        std::cout << "Click the viewport to jump to next shot.\n";
    }

    if (not shot.empty()) {
        if (shotMap.find(shot) == shotMap.end()) {
            std::cerr << "ERROR: Shot not found '" << shot << "', aborting" << std::endl;
            exit(1);
        }

        sequence.clear();

        // run forever
        //shotMap[shot]->duration = -1;

        // the shot is lazily initialized to avoid having to load everything
        // so call Init here
        DemoContext::SetCurrent(shotMap[shot]);
        shotMap[shot]->Init();

        sequence.push_back(shotMap[shot]);
    }

    shotIndex = 0;

    // TODO:
    //  - allow shots to be reused

    DemoContext::SetCurrent(sequence[0]);
}

void PezInitialize()
{
    // add our shader path
    pezSwAddPath("", ".glsl");
    _constructScene();
}

PezConfig PezGetConfig()
{
    PezConfig config;
    config.Title = __FILE__;
    config.Width = 2560/2;
    config.Height = 1600/2;

    // Accomodate my puny MacBook resolution if need be:
    if (config.Height > pezGetDesktopHeight() * 2 /3) {
        config.Width = config.Width * 2 / 3;
        config.Height = config.Height * 2 / 3;
    }

    config.Multisampling = false;
    config.VerticalSync = true;
    return config;
}

static void _nextShot()
{
    shotIndex = (shotIndex+1) % sequence.size();
    DemoContext::SetCurrent(sequence[shotIndex]);
    DemoContext* ctx = DemoContext::GetCurrent();
    // XXX: by setting this to 0, we may be messing up the audio sync
    ctx->elapsedTime = 0;
    printf("Switching to shot %s\n", ctx->name.c_str());
}

void PezHandleMouse(int x, int y, int action)
{
    if (action == PEZ_DOWN) {
    } else if (action == PEZ_UP) {
        if (sequence.size() > 1) {
            _nextShot();
        }
    }
}

void PezRender()
{
    DemoContext::GetCurrent()->Render();
}

void PezUpdate(float seconds)
{
    //
    // The firest couple frames may be jumpy, so try to account for that by freezing time
    // XXX: how will this impact audio? 
    // 
    static int prime = 2;
    if (prime) {
        prime--;
        seconds = 0.0f;
    }
    DemoContext* ctx = DemoContext::GetCurrent();
    ctx->Update(seconds);
    //std::cout << "seconds: " << seconds << " elapsed: " << ctx->elapsedTime << " dur: " << ctx->duration << std::endl;
    if (ctx->elapsedTime > ctx->duration) {
        _nextShot();
    }
}
