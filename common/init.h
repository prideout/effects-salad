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
    AttrTexCoord,
    AttrTetId,
    AttrLength,
};

// Bit flags useful for argument passing
enum VertexAttribFlags {
    AttrPositionFlag    = (1 << AttrPosition),
    AttrNormalFlag      = (1 << AttrNormal),
    AttrTexCoordFlag    = (1 << AttrTexCoord),
    AttrTetIdFlag       = (1 << AttrTetId),
    AttrLengthFlag      = (1 << AttrLength),
};

// Byte Counts for attributes
enum VertexAttribWidths {
    AttrPositionWidth   = 12,
    AttrNormalWidth     = 12,
    AttrTexCoordWidth   = 8,
    AttrTetIdWidth      = 4,
    AttrLengthWidth     = 4,
};

// Some helper methods for initializing shaders and vertex buffers
// XXX: feels like these belong in program and vao classes, rather than here
GLuint InitProgram(const char* fsKey, const char* vsKey, const char* gsKey);
GLuint InitVao(int componentCount, const FloatList& verts);
GLuint InitVao(int componentCount, const FloatList& verts, const IndexList& indices);

void ReadBinaryFile(string filename, Blob* destination);

inline
GLuint CurrentProgram()
{
    GLuint p;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &p);
    return p;
}

// A couple commonly used helper macros
#define u(x) glGetUniformLocation(CurrentProgram(), x)
#define ptr(x) glm::value_ptr(x)
#define offset(x) ((const GLvoid*)(x))
