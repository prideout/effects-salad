// TODO LIST
// ---------
// terrain
// x y height radius
// pop up simple cylinders for now
// camera work
// tetra integration
// details per my handwritten notes
// See also TODO's in buildings.cpp

#include "common/init.h"
#include "fx/cityGrowth.h"
#include "common/terrainUtil.h"
#include "common/programs.h"
#include "common/demoContext.h"

static const int TerrainSize = 150;
static const float TerrainScale = 0.25;
static const int CircleCount = 10;
static const float MinRadius = 1;
static const float MaxRadius = 3;
static const float CirclePadding = 0.25;

CityGrowth::CityGrowth()
{
}

CityGrowth::~CityGrowth()
{
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
    for (int i = 0; i < CircleCount; ++i) {
        CityElement element;
        element.Position.x = TerrainSize * (rand() / float(RAND_MAX) - 0.5);
        element.Position.y = TerrainSize * (rand() / float(RAND_MAX) - 0.5);
        element.Radius = MinRadius + (MaxRadius - MinRadius) *
            (rand() / float(RAND_MAX));
        if (_Collides(element)) {
            --i;
            continue;
        }
        _elements.push_back(element);
    }

    // Compile shaders
    Programs& progs = Programs::GetInstance();
    progs.Load("Buildings.Terrain", false);
}

void CityGrowth::Update()
{
    float time = GetContext()->elapsedTime;
    PerspCamera* camera = &GetContext()->mainCam;
    camera->eye.x = 0;
    camera->eye.y = 100;
    camera->eye.z = 150;
    camera->up = vec3(0, 1, 0);
}

void CityGrowth::Draw()
{
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
}

bool CityGrowth::_Collides(const CityElement& e) const
{
    return false;
}

