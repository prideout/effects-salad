#include "lib/pez/pez.h"
#include "jsoncpp/json.h"
#include "common/init.h"
#include "tetgen/tetgen.h"
#include "glm/glm.hpp"
#include "glm/gtx/constants.inl"
#include <iostream>
#include <fstream>
#include <streambuf>

using namespace std;

PezConfig PezGetConfig()
{
    PezConfig config;
    config.Title = __FILE__;
    config.Width = 512;
    config.Height = 512;
    config.Multisampling = false;
    config.VerticalSync = true;
    return config;
}

// Creates a circular ribbon, composing it out of quads.
void GenerateWheel(glm::vec3 center, float radius, float width,
                   int numQuads, tetgenio* dest)
{
    dest->numberofpoints = numQuads * 2;
    dest->pointlist = new float[dest->numberofpoints * 3];
    const float twopi = 2 * glm::pi<float>();
    const float dtheta = twopi / numQuads;
    float* coord = dest->pointlist;
    for (float theta = 0; theta < twopi - dtheta / 2; theta += dtheta) {
        float x = radius * std::cos(theta);
        float y = radius * std::sin(theta);
        float z0 = -width / 2;
        float z1 = width / 2;
        *coord++ = x;
        *coord++ = y;
        *coord++ = z0;
        *coord++ = x;
        *coord++ = y;
        *coord++ = z1;
    }

    dest->numberoftrifaces = numQuads * 2;
    dest->trifacelist = new int[dest->numberoftrifaces * 3];
    int numPoints = dest->numberofpoints;
    int numTris = dest->numberoftrifaces;
    int* corner = dest->trifacelist;
    for (int n = 0; n < numTris; n += 2) {
        *corner++ = n;
        *corner++ = n+1;
        *corner++ = (n+2) % numPoints;
        *corner++ = (n+2) % numPoints;
        *corner++ = n+1;
        *corner++ = (n+3) % numPoints;
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

    // Write a TubeGenerator class that generates verts in the "tetgen" format
    // Tetrahedralize the PLC. Switches are chosen to read a PLC (p),
    //   do quality mesh generation (q) with a specified quality bound
    //   (1.414), and apply a maximum volume constraint (a0.1).

    tetgenio out;
    //tetrahedralize("pq1.414a0.1", &in, &out);
    tetrahedralize("q1.414a0.1", &in, &out);

    // Call tetgen
    
    // Figure out how to represent the tets in GL:
    //   Static index buffer
    //   Static VBO of the "collapsed" mesh
    //   Texture buffer for per-tet transforms (?)
}

void PezHandleMouse(int x, int y, int action)
{
}

void PezRender()
{
}

void PezUpdate(float seconds)
{
}
