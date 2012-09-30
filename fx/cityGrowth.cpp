// TODO LIST
// ---------
// sketch::Path should have a flag for smooth normals
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
#include "glm/gtx/constants.inl"

using namespace std;
using namespace glm;

static const int TerrainSize = 150;
static const float TerrainScale = 0.25;
static const size_t CircleCount = 150;
static const float MinRadius = 2;
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
        element.NumSides = 20;

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

        sketch::CoplanarPath* circle =
            shape->AddPolygon(e->Radius, ground->Eqn, vec2(0,0), e->NumSides);
        shape->PushPath(circle, 1.0);

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
    camera->far = 400;
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
    const float twopi = 2 * pi<float>();
    const float dtheta = twopi / a.NumSides;
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


