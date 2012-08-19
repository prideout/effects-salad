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

    // Write a TubeGenerator class that generates verts in the "tetgen" format,
    // similar to GenerateWheel
    // Tetrahedralize the PLC. Switches are chosen to read a PLC (p),
    //   do quality mesh generation (q) with a specified quality bound
    //   (1.414), and apply a maximum volume constraint (a0.1).

    tetgenio out;
    tetrahedralize("pq1.414a0.1", &in, &out);

    // Figure out how to draw a wireframe
    
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
