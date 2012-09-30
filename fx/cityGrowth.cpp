// TODO LIST
// ---------
// non-square rectangles
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

static const int TerrainSize = 150;
static const float TerrainScale = 0.5;
static const size_t CircleCount = 64;
static const float MinRadius = 3;
static const float MaxRadius = 7;

static const float MinHeight = 7;
static const float MaxHeight = 10;
static const float SkyscraperHeight = 60;

static const float CirclePadding = 1.25;

static const float BeatsPerMinute = 140;
static const float SecondsPerBeatInterval = 60.0 / BeatsPerMinute;
static const float BeatsPerBuilding = 1;
static const float BeatsPerFlight = 1;
static const float SecondsPerBuilding = BeatsPerBuilding * SecondsPerBeatInterval;
static const float SecondsPerFlight = BeatsPerFlight * SecondsPerBeatInterval;

CityGrowth::CityGrowth()
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

void CityGrowth::Init()
{
    srand(42);

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
    while (_elements.size() < CircleCount) {
        CityElement element;

        vec2 coord;
        coord.x = (rand() / float(RAND_MAX) - 0.5);
        coord.y = (rand() / float(RAND_MAX) - 0.5);

        vec2 domain = (coord + vec2(0.5)) * float(TerrainSize);

        element.Position.x = TerrainSize * coord.x;
        element.Position.y = MyTerrainFunc(domain).y;
        element.Position.z = TerrainSize * coord.y;

        element.ViewingAngleRadians = (rand() / float(RAND_MAX)) * 360;

        float shaper = rand() / float(RAND_MAX);
        if (shaper < 0.8) {
            element.NumSides = 4;
        } else if (shaper < 0.9) {
            element.NumSides = 20;
        } else {
            int b = rand() % 2;
            element.NumSides = b ? 3 : 5;
        }

        // Low-lying buildings vs Skyscrapers
        int tallUnluckiness = element.NumSides > 5 ? 2 : 3;
        if (rand() % tallUnluckiness != 0) {
            element.Height = MinHeight + (MaxHeight - MinHeight) *
                (rand() / float(RAND_MAX));
        } else {
            element.Height = MaxHeight + (SkyscraperHeight - MaxHeight) *
                (rand() / float(RAND_MAX));
        }

        element.Radius = MinRadius + (MaxRadius - MinRadius) *
            (rand() / float(RAND_MAX));

        if (_Collides(element)) {
            continue;
        }
        _elements.push_back(element);
    }

    // Create simple starting points for the buildings
    FOR_EACH(e, _elements) {
        sketch::Scene* shape = new sketch::Scene;

        const sketch::Plane* ground = shape->GroundPlane();

        e->RoofPath =
            shape->AddPolygon(e->Radius, ground->Eqn, vec2(0,0), e->NumSides);

        e->RoofBegin = e->RoofPath->Plane->Eqn.w;
        shape->PushPath(e->RoofPath, e->Height);
        e->RoofEnd = e->RoofPath->Plane->Eqn.w;
        e->CpuShape = shape;
        e->CpuTriangles = new sketch::Tessellator(*shape);
        e->CpuTriangles->PullFromScene();

        shape->SetPathPlane(e->RoofPath, e->RoofBegin);
        shape->PushPath(e->RoofPath, 2.0);
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
    _state = GROWTH;
    _UpdateFlight(2 * SecondsPerFlight);
}

void CityGrowth::_UpdateGrowth(float elapsedTime)
{
    CityElement& building = _elements[_currentBuildingIndex];
    if (elapsedTime > SecondsPerBuilding) {
        building.CpuShape->SetPathPlane(building.RoofPath, building.RoofEnd);
        building.CpuTriangles->PullFromScene();
        building.CpuTriangles->PushToGpu(building.GpuTriangles);

        // TODO finalize by freeing CPU memory

        _currentBuildingIndex++;
        _stateStartTime = GetContext()->elapsedTime;
        if (_state != DEBUG) {
            _state = FLIGHT;
        }

    } else {

        if (elapsedTime < 0) {
            return;
        }

        tween::Elastic tweener;
        float w = tweener.easeOut(
            elapsedTime,
            building.RoofBegin,
            building.RoofEnd,
            SecondsPerBuilding);
        building.CpuShape->SetPathPlane(building.RoofPath, w);
        building.CpuTriangles->PullFromScene();
        building.CpuTriangles->PushToGpu(building.GpuTriangles);
    }
}

void CityGrowth::_UpdateFlight(float elapsedTime)
{
    CityElement& building = _elements[_currentBuildingIndex];
    _camera.far = 400;
    _camera.up = vec3(0, 1, 0);

    float viewingDistance = 40.0 + building.Radius + building.Height * 2;
    vec3 center = building.Position + vec3(0, 10, 0);
    vec3 gaze = normalize(vec3(0, -0.5, 1));
    gaze = glm::rotateY(gaze, building.ViewingAngleRadians);
    vec3 eye = _camera.center - viewingDistance * gaze;

    if (elapsedTime > SecondsPerFlight) {

        _previousCamera = _camera;
        _camera.center = center;
        _camera.eye = eye;
        _stateStartTime = GetContext()->elapsedTime;
        _state = GROWTH;
        
    } else {

        tween::Quad tweener;
        float t = tweener.easeOut(
            elapsedTime,
            0,
            1,
            SecondsPerFlight);

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

    GetContext()->mainCam = _camera;

    if (_currentBuildingIndex >= _elements.size()) {
        return;
    }

    float elapsedTime = time - _stateStartTime;
    switch (_state) {
    case DEBUG:
    case GROWTH:
        _UpdateGrowth(elapsedTime);
        break;
    case FLIGHT:
        _UpdateFlight(elapsedTime);
        break;
    }
}

void CityGrowth::Draw()
{
    glEnable(GL_DEPTH_TEST);
    Programs& progs = Programs::GetInstance();
    PerspCamera surfaceCam = GetContext()->mainCam;

    // Draw terrain
    if (true) {
        glEnable(GL_CULL_FACE);
        glUseProgram(progs["Buildings.Terrain"]);
        surfaceCam.Bind(glm::mat4());
        _terrainVao.Bind();
        glDrawElements(GL_TRIANGLES, _terrainVao.indexCount, GL_UNSIGNED_INT, 0);
    }

    glDisable(GL_CULL_FACE);
    glUseProgram(progs["Sketch.Facets"]);
    glUniform3f(u("Scale"), 1, 1, 1);
    glUniform3f(u("Translate"), 0, 0, 0);

    FOR_EACH(e, _elements) {
        e->GpuTriangles.Bind();
        mat4 xlate = glm::translate(e->Position);
        surfaceCam.Bind(xlate);
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


