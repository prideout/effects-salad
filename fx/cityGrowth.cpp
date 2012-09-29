// TODO LIST
// ---------
// fix lighting
// push to ground
// add CityElement->Height
// camera work; add CityElement->ViewingAngle; add 'BirdsEye' mode for debugging
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

using namespace std;
using namespace glm;

static const int TerrainSize = 150;
static const float TerrainScale = 0.25;
static const size_t CircleCount = 100;
static const float MinRadius = 1;
static const float MaxRadius = 7;
static const float CirclePadding = 0.25;

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
    float y = noise.Get(tx, tz) + 20 * noise.Get(tx/5, tz/5);
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

        element.Position.x = TerrainSize * (rand() / float(RAND_MAX) - 0.5);
        element.Position.y = 0;
        element.Position.z = TerrainSize * (rand() / float(RAND_MAX) - 0.5);

        vec3 xlate = 0.5f * element.Position;
        xlate.x *= 1.75;

        xlate += vec3(0, -5, 30); // move towards the camera and push downwards
        element.Position = xlate;

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

        int numSides = 20;
        sketch::CoplanarPath* circle =
            shape->AddPolygon(e->Radius, ground->Eqn, vec2(0,0), numSides);
        shape->PushPath(circle, 5.5);

        e->CpuShape = shape;
        e->CpuTriangles = new sketch::Tessellator(*shape);
        e->CpuTriangles->PullFromScene();
        e->CpuTriangles->PushToGpu(e->GpuTriangles);
    }

    // Compile shaders
    Programs& progs = Programs::GetInstance();
    progs.Load("Buildings.Terrain", false);
    progs.Load("Sketch.Facets", true);
}

void CityGrowth::Update()
{
    //float time = GetContext()->elapsedTime;
    PerspCamera* camera = &GetContext()->mainCam;
    camera->eye.x = 0;
    camera->eye.y = 100;
    camera->eye.z = 150;
    camera->up = vec3(0, 1, 0);
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
    surfaceCam.Bind(glm::mat4());

    FOR_EACH(e, _elements) {
        e->GpuTriangles.Bind();
        glUniform3fv(u("Translate"), 1, ptr(e->Position));
        glDrawElements(GL_TRIANGLES, e->GpuTriangles.indexCount, GL_UNSIGNED_INT, 0);
    }
}

bool CityGrowth::_Collides(const CityElement& a) const
{
    FOR_EACH(b, _elements) {
        float r = a.Radius + b->Radius + CirclePadding;
        if (glm::distance2(a.Position, b->Position) < r * r) {
            return true;
        }
    }
    return false;
}


