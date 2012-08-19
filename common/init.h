
#pragma once

#include "typedefs.h"
#include "pez/pez.h"
#include "jsoncpp/json.h"
#include "tetgen/tetgen.h"


using namespace std;


GLuint InitProgram(const char* fsKey, const char* vsKey, const char* gsKey);
GLuint InitVao(int componentCount, const FloatList& verts);
//GLuint InitVao(const Vec4List& verts, const IndexList& indices);

void ReadBinaryFile(string filename, Blob* destination);

void ReadJsonFile(string filename, Json::Value* root);


