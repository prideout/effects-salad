#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "pez/pez.h"
#include "typedefs.h"

using namespace std;

class Vao {
public:
    

    int vertexCount;
    int indexCount;
    GLuint vao;

    Vao();

    Vao(int componentCount, const FloatList& verts);

    Vao(int componentCount, 
        const FloatList& verts, 
        const IndexList& indices);


    void AddVertexAttribute(GLuint attrib, 
                            int componentCount, 
                            const FloatList& values);


    void Bind();

    
};


