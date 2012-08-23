
#pragma once

#include "typedefs.h"
#include "pez/pez.h"
#include "jsoncpp/json.h"
#include "tetgen/tetgen.h"


using namespace std;

// Well known vertex attributes
enum VertexAttribs {
    AttrPosition,
    AttrNormal,
    AttrTexCoord
};

// Bit flags useful for argument passing
#define AttrPositionFlag   (1 << 0)
#define AttrNormalFlag     (1 << 1)
#define AttrTexCoordFlag   (1 << 2)

GLuint InitProgram(const char* fsKey, const char* vsKey, const char* gsKey);
GLuint InitVao(int componentCount, const FloatList& verts);
GLuint InitVao(int componentCount, const FloatList& verts, const IndexList& indices);

void ReadBinaryFile(string filename, Blob* destination);

void ReadJsonFile(string filename, Json::Value* root);

inline
GLuint CurrentProgram()
{
    GLuint p;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &p);
    return p;
}

#define u(x) glGetUniformLocation(CurrentProgram(), x)
#define p(x) glm::value_ptr(x)
#define offset(x) ((const GLvoid*)(x))
