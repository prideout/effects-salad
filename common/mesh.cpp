#include "mesh.h"
#include "init.h"

Mesh::Mesh() :
    vertexCount(0),
    indexCount(0),
    vao(0) {
    /* nothing */
}


Mesh::Mesh(int componentCount, const FloatList& verts) :
    vertexCount(verts.size()) {
    
    vao = ::InitVao(componentCount, verts);
}

Mesh::Mesh(int componentCount, 
            const FloatList& verts, 
            const IndexList& indices) :
        vertexCount(verts.size()),
        indexCount(indices.size()) {
    
    vao = ::InitVao(componentCount, verts, indices);
}


void Mesh::Bind() {
    glBindVertexArray(vao);
    pezCheck(vao != 0, "Invalid VAO in mesh.bind");
    pezCheck(glGetError() == GL_NO_ERROR, "mesh.bind failed");
}

