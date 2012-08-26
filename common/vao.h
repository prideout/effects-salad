#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "pez/pez.h"
#include "typedefs.h"

using namespace std;

class Vao {
public:
    
    unsigned vertexCount;
    unsigned indexCount;
    GLuint vao;

    Vao();

    Vao(int componentCount, const FloatList& verts);

    Vao(int componentCount, 
        const FloatList& verts, 
        const IndexList& indices);

    Vao(int componentCount, 
        const float* verts,
        unsigned vertCount);

    Vao(int componentCount, 
        const float* verts, 
        unsigned vertCount,
        const unsigned* indices,
        unsigned indexCount);

    void Init();

    void AddVertexAttribute(GLuint attrib, 
                            int componentCount, 
                            const FloatList& values);

    void AddInterleaved(VertexAttribMask attribs,
                        const Blob& data);

    void Bind();
};


