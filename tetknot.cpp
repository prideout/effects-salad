#include "lib/pez/pez.h"
#include "jsoncpp/json.h"
#include "common/init.h"
#include "tetgen/tetgen.h"
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



void PezInitialize()
{
    Json::Value root;
    ::ReadJsonFile("data/knots.json", &root);

    Blob centerlines;
    ::ReadBinaryFile("data/centerlines.bin", &centerlines);

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
