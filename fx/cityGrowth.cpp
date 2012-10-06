// TODO LIST
// ---------
// SSAO !!!
// tetra integration
// details per my handwritten notes
// See also TODO's in buildings.cpp

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "common/init.h"
#include "fx/cityGrowth.h"
#include "common/terrainUtil.h"
#include "common/programs.h"
#include "common/demoContext.h"
#include "common/sketchScene.h"
#include "common/sketchTess.h"
#include "glm/gtx/constants.inl"
#include "tween/CppTweener.h"

using namespace std;
using namespace glm;

static const bool Verbose = false;

static int TerrainSize = 1500;
static float RelativeCitySize = 0.05f;
static size_t NumBuildings = 32;

static const float TerrainScale = 0.5;
static const float MinRadius = 3;
static const float MaxRadius = 7;

static const float MinHeight = 7;
static const float MaxHeight = 10;
static const float SkyscraperHeight = 60;

static const float CirclePadding = 1.25;

static const float BeatsPerMinute = 140.0;
static const float SecondsPerBeatInterval = 60.0 / BeatsPerMinute;
static const float BeatsPerBuilding = 1;
static const float BeatsPerFlight = 1;
static const float SecondsPerBuilding = BeatsPerBuilding * SecondsPerBeatInterval;
static const float SecondsPerFlight = BeatsPerFlight * SecondsPerBeatInterval;

CityGrowth::CityGrowth(Config config) : _config(config)
{
}

CityGrowth::~CityGrowth()
{
    FOR_EACH(e, _elements) {
        delete e->CpuShape;
        delete e->CpuTriangles;
    }
}

Perlin noise(2, .1, 2, 0);

vec3
MyTerrainFunc(vec2 v)
{
    float tx = v.x * TerrainScale;
    float tz = v.y * TerrainScale;
    float y = noise.Get(tx, tz) + 20.0 * noise.Get(tx/5.0, tz/5.0);
    vec3 p = vec3(v.x, y, v.y);
    return p;
}


struct BuildingConfig {
    int NumSides;
    bool Skyscraper;
    float ViewingAngle;
};

const BuildingConfig BuildingScript[] = {
    {4, false, 138.931}, // 0
    {4, false, 262.647}, // 1
    {20, false, 110}, // 2
    {20, false, 0}, // 3
    {3, false, 280.763}, // 4
    {5, true, 273.603}, // 5
    {20, false, 95.2052}, // 6
    {4, false, 199.535}, // 7
    {3, true, 221.113}, // 8
    {4, false, 306.57}, // 9
    {5, false, 261.148}, // 10
    {20, false, 130.528}, // 11
    {4, false, 180}, // 12 -----
    {4, false, 231.419}, // 13
    {20, false, 45}, // 14 ---
    {20, true, 2.7568}, // 15
};

void CityGrowth::Init()
{
    srand(40);

    bool crappyMachine = PezGetConfig().Width < 2560 / 2;
    static bool first = true;
    if (crappyMachine && first) {
        TerrainSize /= 10;
        RelativeCitySize = 1.0f;
        NumBuildings /= 2;
        first = false;
    }

    vector<BuildingConfig> script(&BuildingScript[0], &BuildingScript[0] +
                                  sizeof(BuildingScript) / sizeof(BuildingScript[0]));

    // Tessellate the ground
    if (true) {
        FloatList ground;
        FloatList normals;
        IndexList indices;
        TerrainUtil::Smooth(TerrainSize, MyTerrainFunc,
                            &ground, &normals, &indices);
        _terrainVao = Vao(3, ground, indices);
        _terrainVao.AddVertexAttribute(AttrNormal, 3, normals);
    }

    // Pack some circles
    while (_elements.size() < NumBuildings) {
        CityElement element;

        element.Visible = true;

        vec2 coord;
        coord.x = (rand() / float(RAND_MAX) - 0.5);
        coord.y = (rand() / float(RAND_MAX) - 0.5);
        coord *= RelativeCitySize;

        vec2 domain = (coord + vec2(0.5)) * float(TerrainSize);

        element.Position.x = TerrainSize * coord.x;
        element.Position.y = MyTerrainFunc(domain).y;
        element.Position.z = TerrainSize * coord.y;

        element.Radius = MinRadius + (MaxRadius - MinRadius) *
            (rand() / float(RAND_MAX));

        if (_Collides(element)) {
            continue;
        }

        element.ViewingAngle = (rand() / float(RAND_MAX)) * 360;

        // Decide on shape; rectangles are most common, then cylinders,
        // then pyramids.
        float shaper = rand() / float(RAND_MAX);
        if (shaper < 0.7) {
            element.NumSides = 4;
        } else if (shaper < 0.9) {
            element.NumSides = 20;
        } else {
            int b = rand() % 2;
            element.NumSides = b ? 3 : 5;
        }

        bool skyscraper = (rand() % 6) == 0;

        if (_elements.size() < script.size()) {
            BuildingConfig cfg = script[_elements.size()];
            element.NumSides = cfg.NumSides;
            skyscraper = cfg.Skyscraper;
            element.ViewingAngle = cfg.ViewingAngle;
        }

        if (Verbose) {
            printf("    {%d, %s, %g}, // %d\n",
                   element.NumSides,
                   skyscraper ? "true" : "false",
                   element.ViewingAngle,               
                   int(_elements.size()));
        }

        element.HasWindows = element.NumSides <= 5;

        float heightFract = rand() / float(RAND_MAX);
        if (skyscraper) {
            float x = (MaxHeight + SkyscraperHeight) / 2;
            element.Height = x + (SkyscraperHeight - x) * heightFract;
        } else {
            float x = MinHeight;
            element.Height = x + (MaxHeight - x) * heightFract;
        }

        _elements.push_back(element);
    }

    // Create simple starting points for the buildings
    FOR_EACH(e, _elements) {
        sketch::Scene* shape = new sketch::Scene;
        const sketch::Plane* ground = shape->GroundPlane();

        if (e->NumSides == 4) {
            float quadrant = TwoPi / 4;
            float fract = rand() / float(RAND_MAX);
            float radians = quadrant / 4 + fract * quadrant / 2;
            float hw = e->Radius * cos(radians);
            float hh = e->Radius * sin(radians);
            e->Rect.Size = vec2(hw, hh);
            e->Rect.Offset = vec2(0, 0);
            e->Roof.Path = shape->AddRectangle(
                hw*2, hh*2,
                ground->Eqn,
                e->Rect.Offset);
        } else {
            e->Roof.Path =
                shape->AddPolygon(e->Radius, ground->Eqn, vec2(0,0), e->NumSides);
        }

        sketch::PathList walls;
        e->Roof.BeginW = e->Roof.Path->Plane->Eqn.w;
        shape->PushPath(e->Roof.Path, e->Height/2, &walls);

        // Occasionally extrude a sidewall.
        if (e->NumSides == 4 && !(rand() % 2)) {

            e->HasWindows = false;
            sketch::Path* wall = walls[2];
            e->Rect.SideWall.Path = shape->AddInscribedRectangle(
                e->Height / 2,
                e->Rect.Size.x * 0.9,
                dynamic_cast<sketch::CoplanarPath*>(wall),
                vec2(0, 0));
            e->Rect.SideWall.BeginW = e->Rect.SideWall.Path->Plane->Eqn.w;
            sketch::PathList secondaryWalls;

            shape->PushPath(
                e->Rect.SideWall.Path,
                2.5,
                &secondaryWalls);

            e->Rect.SideWallRoof.Path = 
                dynamic_cast<sketch::CoplanarPath*>(secondaryWalls[1]);

            e->Rect.SideWallRoof.EndW = e->Rect.SideWallRoof.Path->Plane->Eqn.w;
            e->Rect.SideWall.EndW = e->Rect.SideWall.Path->Plane->Eqn.w;

        } else {
            e->Rect.SideWall.Path = 0;
            e->Rect.SideWallRoof.Path = 0;
        }

        shape->PushPath(e->Roof.Path, e->Height/2, &walls);
        e->Roof.EndW = e->Roof.Path->Plane->Eqn.w;

        const float windowThickness = (rand() % 4) == 0 ? 2.0 : 0.1;

        if (e->HasWindows) {
            FOR_EACH(w, walls) {
                sketch::CoplanarPath* cop = dynamic_cast<sketch::CoplanarPath*>(*w);
                vec2 extent = shape->GetPathExtent(cop);
                float wallHeight = extent.x;
                float wallWidth = extent.y;
                int numRows = std::max(1, int(wallHeight / 4.0));
                int numCols = std::max(1, int(wallWidth / 3.0));
                vec2 padding(1, 1);
                float cellHeight = (wallHeight - (numRows + 1) * padding.y) / float(numRows);
                float cellWidth = (wallWidth - (numCols + 1) * padding.x) / float(numCols);
                float orientation = (cop->Plane->GetCoordSys() * vec3(1, 0, 0)).y;
                vec2 offset;
                offset.x = padding.x + cellWidth/2 - wallWidth/2;
                for (int col = 0; col < numCols; ++col) {
                    offset.y = padding.y + cellHeight/2 - wallHeight/2;
                    for (int row = 0; row < numRows; ++row) {
                        
                        sketch::CoplanarPath* winFrame;
                        sketch::CoplanarPath* win;

                        winFrame = shape->AddInscribedRectangle(
                            cellHeight,
                            cellWidth,
                            cop,
                            orientation * vec2(offset.y, offset.x));
                        e->WindowFrames.Paths.push_back(winFrame);
                        winFrame->Visible = false;

                        win = shape->AddInscribedRectangle(
                            cellHeight - 1.0,
                            cellWidth - 1.0,
                            winFrame,
                            vec2(0, 0));
                        e->Windows.Paths.push_back(win);
                        win->Visible = false;

                        shape->SetVisible(cop->Holes, false);
                        offset.y += cellHeight + padding.y;
                    }
                    offset.x += cellWidth + padding.x;
                }
            }
            shape->PushPaths(
                e->WindowFrames.Paths,
                windowThickness);
            shape->PushPaths(
                e->Windows.Paths,
                -windowThickness/2);
        }

        float srf = 5.0 + e->Height/2 * rand() / float(RAND_MAX);

        if (e->NumSides > 5 && _config == DETAIL) {
            sketch::CoplanarPath* secondRoof;
            vec2 ext = shape->GetPathExtent(e->Roof.Path);
            float radius = std::max(ext.x, ext.y) * 0.25f;
            secondRoof = shape->AddInscribedPolygon(
                radius,
                e->Roof.Path,
                vec2(0, 0),
                10);
            e->SecondaryRoof.Path = secondRoof;
            e->SecondaryRoof.BeginW = secondRoof->Plane->Eqn.w;
            shape->PushPath(secondRoof, srf);
            e->SecondaryRoof.EndW = secondRoof->Plane->Eqn.w;
        } else {
            e->SecondaryRoof.Path = 0;
        }

        // Tessellate the final form of the building before collapsing it
        e->CpuShape = shape;
        e->CpuTriangles = new sketch::Tessellator(*shape);
        e->CpuTriangles->PullFromScene();

        // Collapse the secondary roof
        if (e->SecondaryRoof.Path) {
            shape->PushPath(
                e->SecondaryRoof.Path,
                -srf);
        }

        // Collapse the window frames
        FOR_EACH(p, e->WindowFrames.Paths) {
            sketch::CoplanarPath* cop = dynamic_cast<sketch::CoplanarPath*>(*p);
            e->WindowFrames.EndW.push_back(cop->Plane->Eqn.w);
        }
        shape->PushPaths(
            e->WindowFrames.Paths,
            -windowThickness);
        FOR_EACH(p, e->WindowFrames.Paths) {
            sketch::CoplanarPath* cop = dynamic_cast<sketch::CoplanarPath*>(*p);
            e->WindowFrames.BeginW.push_back(cop->Plane->Eqn.w);
        }

        // Collapse the windows
        FOR_EACH(p, e->Windows.Paths) {
            sketch::CoplanarPath* cop = dynamic_cast<sketch::CoplanarPath*>(*p);
            e->Windows.EndW.push_back(cop->Plane->Eqn.w);
        }
        shape->PushPaths(
            e->Windows.Paths,
            windowThickness/2);
        FOR_EACH(p, e->Windows.Paths) {
            sketch::CoplanarPath* cop = dynamic_cast<sketch::CoplanarPath*>(*p);
            e->Windows.BeginW.push_back(cop->Plane->Eqn.w);
        }

        // Collapse the main building vertically
        if (_config == GROW) {
            shape->SetPathPlane(e->Roof.Path, e->Roof.BeginW);
            e->Visible = false;
        }

        // Collapse the side wall vertically
        if (e->Rect.SideWallRoof.Path) {
            shape->PushPath(e->Rect.SideWallRoof.Path,
                            -e->Height/2);
            e->Rect.SideWallRoof.BeginW = e->Rect.SideWallRoof.Path->Plane->Eqn.w;
        }

        // Collapse the side wall horizontally
        if (e->Rect.SideWall.Path) {
            shape->SetPathPlane(e->Rect.SideWall.Path,
                                e->Rect.SideWall.BeginW);
        }

        e->CpuTriangles->PullFromScene();
        e->CpuTriangles->PushToGpu(e->GpuTriangles);
    }

    // Compile shaders
    Programs& progs = Programs::GetInstance();
    progs.Load("Buildings.Terrain", false);
    progs.Load("Sketch.Facets", true);

    // Set up some growth state
    _stateStartTime = 0;
    _currentBuildingIndex = 0;
    _state = ENTER;
    _UpdateFlight(0);
}

static void
_finalize(sketch::Scene* shape, AnimElement anim)
{
    shape->SetPathPlane(anim.Path, anim.EndW);
}

void CityGrowth::_UpdateDetail(float elapsedTime)
{
    CityElement& building = _elements[_currentBuildingIndex];

    vector<AnimElement> anims;

    // Not sure why, but this prevents a crash:
    if (elapsedTime == 0) {
        elapsedTime = 0.01;
    }

    if (building.Rect.SideWall.Path) {
        anims.push_back(building.Rect.SideWall);
        anims.push_back(building.Rect.SideWallRoof);
    } else if (building.HasWindows) {
        AnimArray& frames = building.WindowFrames;
        for (size_t i = 0; i < frames.Paths.size(); ++i) {
            AnimElement anim;
            building.CpuShape->SetVisible(frames.Paths[i], true);
            anim.Path = dynamic_cast<sketch::CoplanarPath*>(frames.Paths[i]);
            anim.BeginW = frames.BeginW[i];
            anim.EndW = frames.EndW[i];
            anims.push_back(anim);
        }
        AnimArray& windows = building.Windows;
        for (size_t i = 0; i < windows.Paths.size(); ++i) {
            AnimElement anim;
            building.CpuShape->SetVisible(windows.Paths[i], true);
            anim.Path = dynamic_cast<sketch::CoplanarPath*>(windows.Paths[i]);
            anim.BeginW = windows.BeginW[i];
            anim.EndW = windows.EndW[i];
            anims.push_back(anim);
        }
    } else if (building.SecondaryRoof.Path) {
        anims.push_back(building.SecondaryRoof);
    } else {
        printf("Boring building detected!\n");
    }

    if (elapsedTime > SecondsPerBuilding) {
        elapsedTime = SecondsPerBuilding;
    }

    tween::Elastic tweener;
    FOR_EACH(a, anims) {
        float w = tweener.easeOut(
            elapsedTime,
            a->BeginW,
            a->EndW - a->BeginW,
            SecondsPerBuilding);
        building.CpuShape->SetPathPlane(a->Path, w);
    }

    building.CpuTriangles->PullFromScene();
    building.CpuTriangles->PushToGpu(building.GpuTriangles);
}

void CityGrowth::_UpdateGrowth(float elapsedTime)
{
    CityElement& building = _elements[_currentBuildingIndex];
    building.Visible = true;

    vector<AnimElement> anims;
    anims.push_back(building.Roof);
    if (elapsedTime > SecondsPerBuilding) {
        elapsedTime = SecondsPerBuilding;
    }

    tween::Elastic tweener;
    FOR_EACH(a, anims) {
        float w = tweener.easeOut(
            elapsedTime,
            a->BeginW,
            a->EndW - a->BeginW,
            SecondsPerBuilding);
        building.CpuShape->SetPathPlane(a->Path, w);
    }

    building.CpuTriangles->PullFromScene();
    building.CpuTriangles->PushToGpu(building.GpuTriangles);
}

PerspCamera CityGrowth::_InitialCamera()
{
    CityElement& building = _elements[_currentBuildingIndex];
    PerspCamera cam;
    cam.far = (_config == GROW) ? 2000 : 200;
    cam.up = vec3(0, 1, 0);
    float viewingDistance = (_config == GROW) ? 700 : 50;
    vec3 center = building.Position + vec3(0, 10, 0);
    vec3 gaze = normalize(vec3(0, -1, 1));
    gaze = glm::rotateY(gaze, building.ViewingAngle * 2);
    vec3 eye = _camera.center - viewingDistance * gaze;
    cam.center = center;
    cam.eye = eye;
    return cam;
}

void CityGrowth::_UpdateFlight(float elapsedTime)
{
    CityElement& building = _elements[_currentBuildingIndex];

    float viewingDistance = 40.0 + building.Radius + building.Height * 2;
    vec3 center = building.Position + vec3(0, 10, 0);
    vec3 gaze = normalize(vec3(0, -0.5, 1));
    gaze = glm::rotateY(gaze, building.ViewingAngle);
    vec3 eye = _camera.center - viewingDistance * gaze;

    float flightTime = SecondsPerFlight;
    float introDuration = 0;

    if (_state != FLIGHT && _config == GROW) {
        flightTime *= 5;
        introDuration = SecondsPerFlight * 4;
    }

    if (elapsedTime > flightTime + introDuration) {

        _previousCamera = _camera;
        _camera.center = center;
        _camera.eye = eye;

        bool beat = GetContext()->audio->GetKicks() || GetContext()->audio->GetSnares();
        if (beat) {
            _stateStartTime = GetContext()->elapsedTime;
            _state = GROWTH;
            if (Verbose) {
                printf("Building %d\n", (int) _currentBuildingIndex);
            }
        }
        
    } if (elapsedTime < introDuration) {

        gaze = normalize(vec3(0, -0.5, 1));
        gaze = glm::rotateY(gaze, building.ViewingAngle * 4);
        eye = _camera.center - viewingDistance * gaze;

        _camera = _previousCamera = _InitialCamera();
        tween::Quad tweener;
        float t = tweener.easeOut(
            elapsedTime,
            0,
            1,
            introDuration);
        _camera.center = mix(_previousCamera.center, center, t);
        _camera.eye = mix(_previousCamera.eye, eye, t);
        _previousCamera = _camera;

    } else {

        elapsedTime -= introDuration;
        tween::Quad tweener;
        float t = tweener.easeOut(
            elapsedTime,
            0,
            1,
            flightTime);
        _camera.center = mix(_previousCamera.center, center, t);
        _camera.eye = mix(_previousCamera.eye, eye, t);
    }
}

void CityGrowth::Update()
{
    float time = GetContext()->elapsedTime;

    if (_state == DEBUG) {
        _camera.far = 400;
        _camera.eye.x = 0;
        _camera.eye.y = 100;
        _camera.eye.z = 150;
        _camera.up = vec3(0, 1, 0);
    }

    if (_currentBuildingIndex >= _elements.size()) {
        return;
    }

    float elapsedTime = time - _stateStartTime;
    switch (_state) {
    case DEBUG:
    case GROWTH:
        if (elapsedTime < 0) {
            return;
        }
        if (elapsedTime > SecondsPerBuilding) {
            CityElement& building = _elements[_currentBuildingIndex];
            bool beat = GetContext()->audio->GetKicks() ||
                GetContext()->audio->GetSnares();
            if (not beat) {
                return;
            }
            building.CpuTriangles->PullFromScene();
            building.CpuTriangles->PushToGpu(building.GpuTriangles);
            delete building.CpuTriangles;
            delete building.CpuShape;
            _currentBuildingIndex++;
            _stateStartTime = GetContext()->elapsedTime;
            _state = FLIGHT;
            return;
        }
        if (_config == GROW)
            _UpdateGrowth(elapsedTime);
        else
            _UpdateDetail(elapsedTime);
        break;
    case ENTER:
    case EXIT:
    case FLIGHT:
        _UpdateFlight(elapsedTime);
        break;
    }
}

void CityGrowth::Draw()
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    Programs& progs = Programs::GetInstance();

    // Draw terrain
    if (true) {
        glEnable(GL_CULL_FACE);
        glUseProgram(progs["Buildings.Terrain"]);
        _camera.Bind(glm::mat4());
        _terrainVao.Bind();
        glDrawElements(GL_TRIANGLES, _terrainVao.indexCount, GL_UNSIGNED_INT, 0);
    }

    glDisable(GL_CULL_FACE);
    glUseProgram(progs["Sketch.Facets"]);
    glUniform3f(u("Scale"), 1, 1, 1);
    glUniform3f(u("Translate"), 0, 0, 0);

    FOR_EACH(e, _elements) {
        if (not e->Visible) {
            continue;
        }
        e->GpuTriangles.Bind();
        mat4 xlate = glm::translate(e->Position);
        _camera.Bind(xlate);
        glUniform1i(u("Smooth"), e->NumSides > 5 ? 1 : 0);
        glDrawElements(GL_TRIANGLES, e->GpuTriangles.indexCount, GL_UNSIGNED_INT, 0);
    }
}

bool CityGrowth::_Collides(const CityElement& a) const
{
    const float dtheta = TwoPi / a.NumSides;
    float theta = 0;
    vec2 center = vec2(a.Position.x, a.Position.z);
    for (int i = 0; i < a.NumSides; ++i, theta += dtheta) {
        vec2 p = center + a.Radius * vec2(sin(theta), cos(theta));
        if (p.x < -TerrainSize/2 + CirclePadding) return true;
        if (p.x > TerrainSize/2 - CirclePadding) return true;
        if (p.y < -TerrainSize/2 + CirclePadding) return true;
        if (p.y > TerrainSize/2 - CirclePadding) return true;

        vec2 coord = p / float(TerrainSize);
        vec2 domain = (coord + vec2(0.5)) * float(TerrainSize);

        float height = MyTerrainFunc(domain).y;
        const float threshold = 1.0;
        if (height > a.Position.y + threshold ||
            height < a.Position.y - threshold) {
            return true;
        }
    }

    FOR_EACH(b, _elements) {
        float r = a.Radius + b->Radius + CirclePadding;
        if (glm::distance2(a.Position, b->Position) < r * r) {
            return true;
        }
    }
    return false;
}


