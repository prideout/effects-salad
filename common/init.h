
#pragma once

#include "pez/pez.h"
#include "jsoncpp/json.h"
#include "tetgen/tetgen.h"

using namespace std;

typedef std::vector<unsigned char> Blob;

GLuint InitProgram(const char* fsKey, const char* vsKey, const char* gsKey);

void ReadBinaryFile(string filename, Blob* destination);

void ReadJsonFile(string filename, Json::Value* root);


