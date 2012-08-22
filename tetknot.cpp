#include "lib/pez/pez.h"
#include "jsoncpp/json.h"
#include "common/init.h"
#include "tetgen/tetgen.h"
#include "common/programs.h"
#include "common/mesh.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/constants.inl"
#include <iostream>
#include <fstream>
#include <streambuf>

using namespace std;
using glm::mat4;
using glm::vec3;

struct ContextType
{
    int PointCount;
    int PositionSlot;
    int TetCount;
    float Theta;
} Context;

PezConfig PezGetConfig()
{
    PezConfig config;
    config.Title = __FILE__;
    config.Width = 512*2;
    config.Height = 512*2;
    config.Multisampling = false;
    config.VerticalSync = true;
    return config;
}

// Creates a circular ribbon, composing the rim out of quads.
// Each of the two caps is a single "facet" and each quad is a "facet".
// (tetgen defines a facet as a coplanar set of polygons)
void GenerateWheel(glm::vec3 center, float radius, float width,
                   int numQuads, tetgenio* dest)
{
    dest->numberofpoints = numQuads * 2;
    dest->pointlist = new float[dest->numberofpoints * 3];
    const float twopi = 2 * glm::pi<float>();
    const float dtheta = twopi / numQuads;
    float* coord = dest->pointlist;
    const float z0 = -width / 2;
    const float z1 = width / 2;
    
    // Rim points:
    for (float theta = 0; theta < twopi - dtheta / 2; theta += dtheta) {
        float x = radius * std::cos(theta);
        float y = radius * std::sin(theta);
        *coord++ = x;
        *coord++ = y;
        *coord++ = z0;
        *coord++ = x;
        *coord++ = y;
        *coord++ = z1;
    }

    // Facet per rim face + 2 facets for the "caps"
    dest->numberoffacets = numQuads + 2;
    dest->facetlist = new tetgenio::facet[dest->numberoffacets];
    tetgenio::facet* facet = &dest->facetlist[0];

    // Rim faces:
    for (int n = 0; n < numQuads * 2; n += 2, ++facet) {
        facet->numberofpolygons = 1;
        facet->polygonlist = new tetgenio::polygon[facet->numberofpolygons];
        facet->numberofholes = 0;
        facet->holelist = NULL;
        tetgenio::polygon* poly = &facet->polygonlist[0];
        poly->numberofvertices = 4;
        poly->vertexlist = new int[poly->numberofvertices];
        poly->vertexlist[0] = n;
        poly->vertexlist[1] = n+1;
        poly->vertexlist[2] = (n+3) % (numQuads*2);
        poly->vertexlist[3] = (n+2) % (numQuads*2);
    }

    // Cap fans:
    for (int cap = 0; cap < 2; ++cap, ++facet) {
        facet->numberofpolygons = 1;
        facet->polygonlist = new tetgenio::polygon[facet->numberofpolygons];
        facet->numberofholes = 0;
        facet->holelist = NULL;
        tetgenio::polygon* poly = &facet->polygonlist[0];
        poly->numberofvertices = numQuads;
        poly->vertexlist = new int[poly->numberofvertices];
        for (int q = 0; q < numQuads; ++q) {
            poly->vertexlist[q] = q * 2 + cap;
        }
    }
}

void PezInitialize()
{
    Json::Value root;
    ::ReadJsonFile("data/knots.json", &root);

    Blob centerlines;
    ::ReadBinaryFile("data/centerlines.bin", &centerlines);

    tetgenio in;
    GenerateWheel(glm::vec3(0), 1.0f, 0.1f, 32, &in);

    cout << "Tetrahedralizing a hull defined by " << 
        in.numberofpoints << " points..." << endl;

    const float qualityBound = 15;
    const float maxVolume = 4.f;

    char configString[128];
    sprintf(configString, "Qpq%.3fa%.7f", qualityBound, maxVolume);

    tetgenio out;
    tetrahedralize(configString, &in, &out);

    //int* tets = out.tetrahedronlist;
    int numTets = out.numberoftetrahedra;
    int numPoints = out.numberofpoints;

    cout << numTets << " tets have been generated, defined by " <<
        numPoints << " points." << endl;

    Context.PositionSlot = 0;
    Context.Theta = 0;
    Context.TetCount = numTets;

    float* pts = out.pointlist;
    IndexList il;
    
    int* tetIt = out.tetrahedronlist;
    int* endIt = out.tetrahedronlist + (out.numberofcorners * numTets) + 1;
    il.reserve(numTets*4*3);
    std::cout << "# Corners: " << out.numberofcorners << std::endl;
    cout << "# tet attrs: " << out.numberoftetrahedronattributes<< endl;
    cout << "# trifaces: " << out.numberoftrifaces << endl;
    cout << "# expected tris: " << numTets * 4 << endl;

    #if 0
    for (int i = 0; i < out.numberoftrifaces*3; i+=3) {
        il.push_back(out.trifacelist[i]);
        il.push_back(out.trifacelist[i+1]);
        il.push_back(out.trifacelist[i+2]);
    }
    #endif

    #if 1
    int triCount = 0;
    while (tetIt < endIt) {
        triCount += 4;

        if (tetIt[0] > numPoints) {
            cout << float(tetIt[0]) << endl;
            cout << numPoints << endl;
            
        }
        pezCheck(tetIt[0] <= numPoints, "Error, index out of bounds");

        // FACE 1: bottom, back facing
        il.push_back(tetIt[1]/1);
        //il.push_back(tetIt[1]/1+1);
        //il.push_back(tetIt[1]/1+2);

        il.push_back(tetIt[0]/1);
        //il.push_back(tetIt[0]/1+1);
        //il.push_back(tetIt[0]/1+2);

        il.push_back(tetIt[2]/1);
        //il.push_back(tetIt[2]/1+1);
        //il.push_back(tetIt[2]/1+2);


        // FACE 2: Left, front
        il.push_back(tetIt[0]/1);
        //il.push_back(tetIt[0]+1);
        //il.push_back(tetIt[0]+2);

        il.push_back(tetIt[1]/1);
        //il.push_back(tetIt[1]+1);
        //il.push_back(tetIt[1]+2);

        il.push_back(tetIt[3]/1);
        //il.push_back(tetIt[3]+1);
        //il.push_back(tetIt[3]+2);

        // FACE 3: Right, front
        il.push_back(tetIt[1]/1);
        //il.push_back(tetIt[1]+1);
        //il.push_back(tetIt[1]+2);

        il.push_back(tetIt[2]/1);
        //il.push_back(tetIt[2]+1);
        //il.push_back(tetIt[2]+2);

        il.push_back(tetIt[3]/1);
        //il.push_back(tetIt[3]+1);
        //il.push_back(tetIt[3]+2);

        // FACE 4: rear center, back facing
        il.push_back(tetIt[2]/1);
        //il.push_back(tetIt[2]+1);
        //il.push_back(tetIt[2]+2);

        il.push_back(tetIt[0]/1);
        //il.push_back(tetIt[0]+1);
        //il.push_back(tetIt[0]+2);

        il.push_back(tetIt[3]/1);
        //il.push_back(tetIt[3]+1);
        //il.push_back(tetIt[3]+2);

        tetIt += 4;
    }
    cout << "TriCount: " << triCount << endl;
    #endif

    /*
    cout << "Facets: " << out.numberoffacets << endl;
    for (int faceti = 0; faceti < out.numberoffacets; faceti++) {
        tetgenio::facet& f = out.facetlist[faceti];
        pezCheck(f.numberofpolygons == 1, "Unexpected number of polys");
        tetgenio::polygon& poly = f.polygonlist[0];
        for (int i = 0; i < poly.numberofvertices; i++) {
            cout << poly.vertexlist[i] << endl;
            pezCheck(poly.vertexlist[i]+2 < numPoints*3, "Index out of bounds");
            il.push_back(poly.vertexlist[i]+0);
            il.push_back(poly.vertexlist[i]+1);
            il.push_back(poly.vertexlist[i]+2);
        }
    }
    */


    Mesh mesh(3, FloatList(out.pointlist, out.pointlist+(3*numPoints)),
                 il);
    mesh.Bind();

    /*
    GLuint vao;
    GLsizeiptr bufferSize = numPoints * sizeof(float) * 3;
    GLuint vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, out.pointlist, GL_STATIC_DRAW);
    glVertexAttribPointer(Context.PositionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(Context.PositionSlot);
    */

    Context.PointCount = numPoints;

    // Figure out how to represent the tets in GL:
    //   Static index buffer
    //   Static VBO of the "collapsed" mesh
    //   Texture buffer for per-tet transforms (?)

    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("Tetra.Simple"));

    float fov(60);
    float aspect(1.0);
    float near(1.0);
    float far(100);
    mat4 projection = glm::perspective(fov, aspect, near, far);
    glUniformMatrix4fv(u("Projection"), 1, 0, &projection[0][0]);

    pezCheck(glGetError() == GL_NO_ERROR, "OpenGL Error.");
}

void PezHandleMouse(int x, int y, int action)
{
}

void PezRender()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mat4 model;
    vec3 axis = glm::normalize(vec3(1, 1, 0));
    model = glm::rotate(model, Context.Theta, axis);

    vec3 eye = vec3(0,0,3);
    vec3 center = vec3(0,0,0);
    vec3 up = vec3(0,1,0);
    mat4 view = glm::lookAt(eye, center, up);
    mat4 modelview = view * model;
    glUniformMatrix4fv(u("Modelview"), 1, 0, &modelview[0][0]);

    //glDrawArrays(GL_POINTS, 0, Context.PointCount);
    // tetCount*4*3 : 4 faces per tet, 3 points per face
    glDrawElements(GL_TRIANGLES, Context.TetCount*4*3, GL_UNSIGNED_INT, NULL);

    // points for reference
    glDrawArrays(GL_POINTS, 0, Context.PointCount);
}

void PezUpdate(float seconds)
{
    Context.Theta += seconds * 100;
}
