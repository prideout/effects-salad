#include "lib/pez/pez.h"
#include "jsoncpp/json.h"
#include "tetgen/tetgen.h"
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

void PezInitialize()
{
    Json::Value root;
    ReadJsonFile("data/knots.json", &root);

    Blob centerlines;
    ReadBinaryFile("data/centerlines.bin", &centerlines);

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
