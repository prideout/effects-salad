#include <string>
#include <map>

#include "lib/pez/pez.h"

#include "common/audio.h"
#include "common/camera.h"
#include "common/demoContext.h"
#include "common/init.h"
#include "common/jsonUtil.h"
#include "common/programs.h"
#include "common/surface.h"
#include "fx/all.h"
#include "jsoncpp/json.h"


int byteMark = 0;
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

    //Audio::Get().Test();

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
        Fullscreen* fullscreen1 = new Fullscreen(Fullscreen::VignetteFlag);
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

    {   // prideout credits
        DemoContext* ctx = DemoContext::New("prideout_credits");
        DemoContext::SetCurrent(ctx);
        shotMap[ctx->name] = ctx;

        ctx->mainCam.eye.z = 5;
        ctx->mainCam.eye.x = 50;
        ctx->mainCam.eye.y = 50;

        // Instance the effects, but do not place them into the scene graph:
        Fullscreen::Mask mask = Fullscreen::VignetteFlag;
        mask |= Fullscreen::SupersampleFlag;
        mask |= Fullscreen::TeleLinesFlag;
        Fullscreen* fullscreen = new Fullscreen(mask);
        vec4 hotPink(1.000, 0.078, 0.576, 1);
        fullscreen->clearColor = vec4(hotPink);
        //fullscreen->clearColor = vec4(0.1,0.9,0.7,1);

        // Now, insert the effects into our poor man's "scene graph":
        ctx->drawables.push_back(fullscreen);
        fullscreen->AddChild(new BuildingGrowth());

        ctx->drawables.push_back(fullscreen);
        ctx->drawables.push_back(new FpsOverlay());
    }

    {   // City Grow
        DemoContext* ctx = DemoContext::New("CityGrow");
        DemoContext::SetCurrent(ctx);
        shotMap[ctx->name] = ctx;
        Fullscreen::Mask mask = Fullscreen::VignetteFlag;
        mask |= Fullscreen::SupersampleFlag;
        mask |= Fullscreen::TeleLinesFlag;
        Fullscreen* fullscreen = new Fullscreen(mask);
        fullscreen->AddChild(new CityGrowth(CityGrowth::GROW));
        ctx->drawables.push_back(fullscreen);
    }

    {   // City Detail
        DemoContext* ctx = DemoContext::New("CityDetail");
        DemoContext::SetCurrent(ctx);
        shotMap[ctx->name] = ctx;
        Fullscreen::Mask mask = Fullscreen::VignetteFlag;
        mask |= Fullscreen::SupersampleFlag;
        mask |= Fullscreen::AmbientOcclusionFlag;
        Fullscreen* fullscreen = new Fullscreen(mask);
        fullscreen->AddChild(new CityGrowth(CityGrowth::DETAIL));
        ctx->drawables.push_back(fullscreen);
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
    float startTime = 0;
    FOR_EACH(element, script) {
        Json::Value cur = *element;
        if (shotMap.find(cur[0u].asString()) != shotMap.end()) {
            // XXX: can currently only use a context once, because duration is shared :(
            string curShot = cur[0u].asString();
            DemoContext::SetCurrent(shotMap[curShot]);
            shotMap[curShot]->startTime = startTime;
            shotMap[curShot]->duration = cur[1u].asDouble();
            startTime += shotMap[curShot]->duration;
            sequence.push_back(shotMap[curShot]);
            if (shot.empty())  {
                shotMap[curShot]->Init();
                std::cout << "Init [" << curShot << "] GPU memory: " 
                          << (Vao::totalBytesBuffered - byteMark) / 1024 
                          << " KiB" << std::endl;
                byteMark = Vao::totalBytesBuffered;
            }
            //std::cout << "Added " << curShot << " duration: " << cur[1u].asDouble() << std::endl;
        } else {
            std::cerr << "WARNING: shot not found '" << cur[0u] << "'" << std::endl;
        }
    }

    if (sequence.size() > 1) {
        std::cout << "Click the viewport to jump to next shot.\n";
    }

    float audioOffset = 0;
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
        std::cout << "Init [" << shot << "] GPU memory: " 
                  << (Vao::totalBytesBuffered - byteMark) / 1024 
                  << " KiB" << std::endl;
        byteMark = Vao::totalBytesBuffered;

        audioOffset = shotMap[shot]->startTime;
        sequence.push_back(shotMap[shot]);
    }

    shotIndex = 0;

    // TODO:
    //  - allow shots to be reused

    DemoContext::SetCurrent(sequence[0]);
    StartAudio();
    if (audioOffset > 0) {
        SetAudioPosition(audioOffset);
    }
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
    std::cout << "    Consumed GPU memory: " 
              << (Vao::totalBytesBuffered - byteMark) / 1024 
              << " KiB" << std::endl;
    byteMark = Vao::totalBytesBuffered;
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
            SetAudioPosition(DemoContext::GetCurrent()->startTime);
        }
    }
}

void PezRender()
{
    DemoContext::GetCurrent()->Render();
}

void PezUpdate(float seconds)
{
    // sync up the audio sequencer
    Audio::Get().Update(seconds);

    //
    // The firest couple frames may be jumpy, so try to account for that by freezing time
    // XXX: how will this impact audio? 
    // 
    static int prime = 2;
    
    if (prime) {
        prime--;
        seconds = 0.0f;
    }

    DemoContext::totalTime += seconds;

    DemoContext* ctx = DemoContext::GetCurrent();
    ctx->Update(seconds);
    //std::cout << "seconds: " << seconds << " elapsed: " << ctx->elapsedTime << " dur: " << ctx->duration << std::endl;
    if (ctx->elapsedTime > ctx->duration) {
        _nextShot();
    }
}
