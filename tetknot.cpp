#include "lib/pez/pez.h"
#include "jsoncpp/json.h"
#include "tetgen/tetgen.h"
#include "glm/glm.hpp"
#include "glm/gtx/constants.inl"
#include <iostream>
#include <fstream>
#include <streambuf>

using namespace std;

typedef std::vector<unsigned char> Blob;

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

void ReadBinaryFile(string filename, Blob* destination)
{
    ifstream binFile(filename.c_str(), ios::binary);
    vector<char> blob((istreambuf_iterator<char>(binFile)), 
                      (istreambuf_iterator<char>()));
    destination->resize(blob.size());
    memcpy(&destination[0], &blob[0], blob.size());
}

void ReadJsonFile(string filename, Json::Value* root)
{
    ifstream jsonFile(filename.c_str());
    string jsonString((istreambuf_iterator<char>(jsonFile)),
                      istreambuf_iterator<char>());
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonString.c_str(), *root);
    if (!parsingSuccessful) {
        cerr  << "Failed to parse knot metadata \n"
              << reader.getFormatedErrorMessages();
        exit(1);
    }
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
    ReadJsonFile("data/knots.json", &root);

    Blob centerlines;
    ReadBinaryFile("data/centerlines.bin", &centerlines);

    tetgenio in;
    GenerateWheel(glm::vec3(0), 1.0f, 0.1f, 32, &in);

    // Write a TubeGenerator class that generates verts in the "tetgen" format

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
