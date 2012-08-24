#include "lib/pez/pez.h"
#include "jsoncpp/json.h"
#include "common/init.h"
#include "common/programs.h"
#include "common/tube.h"
#include "common/tetUtil.h"
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
    int PositionSlot;
    int TetCount;
    float Theta;
    mat4 Projection;
    mat4 Modelview;
    mat3 NormalMatrix;
    int CurrentTet;
    float ElapsedTime;
    GLuint TetsVao;
    GLuint HullVao;
    GLsizei HullTriCount;
} Context;

PezConfig PezGetConfig()
{
    PezConfig config;
    config.Title = __FILE__;
    config.Width = 700;
    config.Height = 700;
    config.Multisampling = true;
    config.VerticalSync = true;
    return config;
}

void PezInitialize()
{
    Json::Value root;
    ::ReadJsonFile("data/knots.json", &root);

    Blob centerlines;
    ::ReadBinaryFile("data/centerlines.bin", &centerlines);
 
    tetgenio in;
    TetUtil::HullWheel(glm::vec3(0), 1.0f, 0.3f, 16, &in);

    tetgenio innerWheel;
    TetUtil::HullWheel(glm::vec3(0), 0.25f, 0.15f, 16, &innerWheel);

    cout <<
        "Tetrahedralizing a hull defined by " << 
        in.numberofpoints << " points and " <<
        in.numberoffacets << " facets." << endl;

    tetgenio out;
    const float qualityBound = 15;
    const float maxVolume = 0.00005f;
    TetUtil::TetsFromHull(in, &out, qualityBound, maxVolume, false);

    int numTets = out.numberoftetrahedra;
    int numPoints = out.numberofpoints;
    Context.PointCount = numPoints;
    Context.TetCount = numTets;
    Context.CurrentTet = 0;
    Context.ElapsedTime = 0;

    cout <<
        numTets << " tets have been generated, defined by " <<
        numPoints << " points." << endl;

    Context.PositionSlot = 0;
    Context.Theta = 0;

    // Create the Tets VAO
    glGenVertexArrays(1, &Context.TetsVao);
    glBindVertexArray(Context.TetsVao);
    GLuint pointsVbo;
    {
        GLsizeiptr bufferSize = numPoints * sizeof(float) * 3;
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, bufferSize, out.pointlist, GL_STATIC_DRAW);
        glVertexAttribPointer(Context.PositionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(Context.PositionSlot);
        pointsVbo = vbo;
    }
    {
        Blob triIndices;
        TetUtil::TrianglesFromTets(out, &triIndices);
        GLsizeiptr bufferSize = triIndices.size();
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, &triIndices[0], GL_STATIC_DRAW);
    }

    // Create the Hull VAO
    glGenVertexArrays(1, &Context.HullVao);
    glBindVertexArray(Context.HullVao);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVbo);
    glVertexAttribPointer(Context.PositionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(Context.PositionSlot);
    {
        Blob triIndices;
        TetUtil::TrianglesFromHull(in, &triIndices);
        GLsizeiptr bufferSize = triIndices.size();
        Context.HullTriCount = (triIndices.size() / sizeof(int)) / 3;
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, &triIndices[0], GL_STATIC_DRAW);
    }

    Programs& progs = Programs::GetInstance();
    progs.Load("Tetra.Simple", false);
    progs.Load("Tetra.Solid", true);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float fov(60);
    float aspect(1.0);
    float near(1.0);
    float far(100);
    Context.Projection = glm::perspective(fov, aspect, near, far);

    pezCheck(glGetError() == GL_NO_ERROR, "OpenGL Error.");
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
        float cullY = -1 + 2 * (float) Context.CurrentTet / Context.TetCount;
        glUseProgram(progs["Tetra.Solid"]);
        glUniform1f(u("CullY"), cuttingPlane ? cullY : 999);
        glUniformMatrix4fv(u("Modelview"), 1, 0, ptr(Context.Modelview));
        glUniformMatrix4fv(u("Projection"), 1, 0, ptr(Context.Projection));
        glUniformMatrix3fv(u("NormalMatrix"), 1, 0, ptr(Context.NormalMatrix));
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(Context.TetsVao); 
        glDrawElements(GL_TRIANGLES, tetCount * 4 * 3, GL_UNSIGNED_INT, 0);
    }

    const bool drawPointCloud = false;
    if (drawPointCloud) {
        glUseProgram(progs["Tetra.Simple"]);
        glUniformMatrix4fv(u("Modelview"), 1, 0, ptr(Context.Modelview));
        glUniformMatrix4fv(u("Projection"), 1, 0, ptr(Context.Projection));
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(Context.TetsVao);
        glDrawArrays(GL_POINTS, 0, Context.PointCount);
    }

    const bool drawHull = false;
    if (drawHull) {
        glUseProgram(progs["Tetra.Solid"]);
        glUniform1f(u("CullY"), 999);
        glUniformMatrix4fv(u("Modelview"), 1, 0, ptr(Context.Modelview));
        glUniformMatrix4fv(u("Projection"), 1, 0, ptr(Context.Projection));
        glUniformMatrix3fv(u("NormalMatrix"), 1, 0, ptr(Context.NormalMatrix));
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(Context.HullVao);
        glDrawElements(GL_TRIANGLES, Context.HullTriCount * 3, GL_UNSIGNED_INT, 0);
    }
}

void PezUpdate(float seconds)
{
    const float TetAppearanceRate = 0.2f;
    const float RotationRate = 100;

    Context.Theta += seconds * RotationRate;
    Context.ElapsedTime += seconds;

    float percentage = Context.ElapsedTime * TetAppearanceRate;
    percentage = percentage > 1.0 ? 1.0 : percentage;
    Context.CurrentTet = (int) (percentage * Context.TetCount);

    mat4 model;
    vec3 axis = glm::normalize(vec3(1, 1, 0));
    model = glm::rotate(model, Context.Theta, axis);

    vec3 eye = vec3(0,0,3);
    vec3 center = vec3(0,0,0);
    vec3 up = vec3(0,1,0);
    mat4 view = glm::lookAt(eye, center, up);
    Context.Modelview = view * model;

    Context.NormalMatrix = mat3(Context.Modelview);
}
