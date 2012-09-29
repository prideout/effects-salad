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

CityGrowth::CityGrowth()
{
}

CityGrowth::~CityGrowth()
{
}

void CityGrowth::Init()
{
    srand(42);

    // Tessellate the ground
    if (true) {
        FloatList ground;
        FloatList normals;
        IndexList indices;
        const int SIZE = 150;
        const float SCALE = 0.35;
        TerrainUtil::Smooth(SIZE, SCALE, &ground, &normals, &indices);
        _terrainVao = Vao(3, ground, indices);
        _terrainVao.AddVertexAttribute(AttrNormal, 3, normals);
    }

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
        //glDisable(GL_DEPTH_TEST);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glUseProgram(progs["Buildings.Terrain"]);
        surfaceCam.Bind(glm::mat4());
        _terrainVao.Bind();
        glDrawElements(GL_TRIANGLE_STRIP, _terrainVao.indexCount, GL_UNSIGNED_INT, 0);
    }
}

