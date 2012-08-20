#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "pez/pez.h"
#include "typedefs.h"

using namespace std;

class Mesh {
public:
    

    int vertexCount;
    int indexCount;
    GLuint vao;

    Mesh();

    Mesh(int componentCount, const FloatList& verts);

    Mesh(int componentCount, 
        const FloatList& verts, 
        const IndexList& indices);


    void Bind();

    
};


