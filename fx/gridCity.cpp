
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "common/init.h"
#include "fx/gridCity.h"
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
static const float MinHeight = 7;
static const float MaxHeight = 10;

GridCity::GridCity()
{
}

GridCity::~GridCity()
{
}

extern vec3 MyTerrainFunc(vec2 v);

void GridCity::Init()
{
    // Tessellate the ground
    FloatList ground;
    FloatList normals;
    IndexList indices;
    TerrainUtil::Smooth(
        TerrainSize, MyTerrainFunc,
        &ground, &normals, &indices);
    _terrainVao = Vao(3, ground, indices);
    _terrainVao.AddVertexAttribute(AttrNormal, 3, normals);

    // Compile shaders
    Programs& progs = Programs::GetInstance();
    progs.Load("Buildings.Terrain", false);
    progs.Load("Sketch.Facets", true);

    // Set up camera
    _camera.far = 1000;
    _camera.up = vec3(0, 1, 0);
    _camera.center = vec3(0, 0, 0);;
    _camera.eye = vec3(0, 200, 20);
}

void GridCity::Update()
{
}

void GridCity::Draw()
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    Programs& progs = Programs::GetInstance();

    // Draw terrain
    glEnable(GL_CULL_FACE);
    glUseProgram(progs["Buildings.Terrain"]);
    _camera.Bind(glm::mat4());
    _terrainVao.Bind();
    glDrawElements(GL_TRIANGLES, _terrainVao.indexCount, GL_UNSIGNED_INT, 0);

    // Draw buildings
    glDisable(GL_CULL_FACE);
    glUseProgram(progs["Sketch.Facets"]);
    glUniform3f(u("Scale"), 1, 1, 1);
    glUniform3f(u("Translate"), 0, 0, 0);

    #if 0
    FOR_EACH(e, _elements) {
        if (not e->Visible) {
            continue;
        }
        e->GpuTriangles.Bind();
        mat4 xlate = glm::translate(e->Position);
        _camera.Bind(xlate);
        glUniform1i(u("Smooth"), 0);
        glDrawElements(GL_TRIANGLES, e->GpuTriangles.indexCount, GL_UNSIGNED_INT, 0);
    }
    #endif
}
