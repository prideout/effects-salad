#include "lib/pez/pez.h"
#include "jsoncpp/json.h"
#include "common/init.h"
#include "common/jsonUtil.h"
#include "common/camera.h"
#include "common/programs.h"
#include "common/tube.h"
#include "common/vao.h"
#include "common/tetUtil.h"
#include "common/texture.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"
#include <iostream>
#include <fstream>
#include <streambuf>

using namespace std;
using glm::mat4;
using glm::mat3;
using glm::vec3;

struct ContextType
{
    int PointCount;
    int TetCount;
    float Theta;
    PerspCamera Cam;
    mat4 ModelMatrix;
    int CurrentTet;
    float ElapsedTime;
    Vao ExpandedVao;
    GLsizei HullTriCount;
    BufferTexture CentroidTexture;
    int BoundaryTets;
} Context;

PezConfig PezGetConfig()
{
    PezConfig config;
    config.Title = __FILE__;
    config.Width = 700;
    config.Height = 700;
    config.Multisampling = true;
    config.VerticalSync = false;
    return config;
}

void PezInitialize()
{
    Json::Value root;
    ::ReadJsonFile("data/knots.json", &root);

    Blob centerlines;
    ::ReadBinaryFile("data/centerlines.bin", &centerlines);
 
    tetgenio in;
    TetUtil::HullWheel(glm::vec3(0), 25.0f, 13.0f, 32, &in);
    TetUtil::HullWheel(glm::vec3(0), 20.0f, 8.5f, 32, &in);

    Vec3List holePoints;
    holePoints.push_back(vec3(0, 0, 0));
    TetUtil::AddHoles(holePoints, &in);

    cout <<
        "Tetrahedralizing a hull defined by " << 
        in.numberofpoints << " points and " <<
        in.numberoffacets << " facets." << endl;

    tetgenio out;
    const float qualityBound = 1.414;
    const float maxVolume = 0.5f;
    TetUtil::TetsFromHull(in, &out, qualityBound, maxVolume, false);

    int numTets = Context.TetCount = out.numberoftetrahedra;
    int numPoints = Context.PointCount = out.numberofpoints;
    Context.CurrentTet = 0;
    Context.ElapsedTime = 0;
    Context.Theta = 0;

    cout <<
        numTets << " tets have been generated, defined by " <<
        numPoints << " points." << endl;

    // Populate the per-tet texture data
    Vec4List centroids;
    TetUtil::SortTetrahedra(&centroids, out, &Context.BoundaryTets);
    Context.CentroidTexture.Init(centroids);

    // Create the 'Expanded' VAO
    Blob massive;
    VertexAttribMask attribs = AttrPositionFlag | AttrNormalFlag;
    TetUtil::PointsFromTets(out, attribs, &massive);
    
    Context.ExpandedVao.Init();
    Context.ExpandedVao.AddInterleaved(attribs, massive);

    Programs& progs = Programs::GetInstance();
    progs.Load("Tetra.Simple", false);
    progs.Load("Tetra.Solid", false);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Context.Cam.fov = 60;
    Context.Cam.aspect = 1.0;
    Context.Cam.near = 1.0;
    Context.Cam.far = 100;
    Context.Cam.eye = vec3(0,0,60);
    Context.Cam.center = vec3(0,0,0);
    Context.Cam.up = vec3(0,1,0);
}

void PezHandleMouse(int x, int y, int action)
{
}

void PezRender()
{
    Programs& progs = Programs::GetInstance();

    glClearColor(0.1,0.2,0.3,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);

    const bool cuttingPlane = true;
    const bool drawSolidTriangles = true;
    if (drawSolidTriangles) {
        GLsizei tetCount = cuttingPlane ? Context.TetCount : Context.CurrentTet;
        float cullY = -25 + 50 * (float) Context.CurrentTet / Context.TetCount;
        glUseProgram(progs["Tetra.Solid"]);
        glUniform1f(u("CullY"), cuttingPlane ? cullY : 999);
        Context.Cam.Bind(Context.ModelMatrix);
        Context.CentroidTexture.Bind(0);
        glUniform1i(u("CentroidTexture"), 0);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        Context.ExpandedVao.Bind();

        // Optimization: don't draw interior tets after completely filling the volume
        if (Context.CurrentTet >= Context.TetCount) {
            glDrawArrays(GL_TRIANGLES, 0, Context.BoundaryTets * 4 * 3);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, tetCount * 4 * 3);
        }
    }
}

void PezUpdate(float seconds)
{
    const float TetAppearanceRate = 0.1f;
    const float RotationRate = 10;

    Context.Theta += seconds * RotationRate;
    Context.ElapsedTime += seconds;

    float percentage = Context.ElapsedTime * TetAppearanceRate;
    percentage = percentage > 1.0 ? 1.0 : percentage;
    Context.CurrentTet = (int) (percentage * Context.TetCount);

    mat4 model;
    vec3 axis = glm::normalize(vec3(1, 1, 0));
    Context.ModelMatrix = glm::rotate(model, Context.Theta, axis);
}
