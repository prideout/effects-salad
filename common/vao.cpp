#include "vao.h"
#include "init.h"

Vao::Vao() :
    vertexCount(0),
    indexCount(0),
    vao(0) 
{
    /* nothing */
}

Vao::Vao(int componentCount, const FloatList& verts) :
    vertexCount(verts.size()),
    indexCount(0) 
{    
    vao = ::InitVao(componentCount, verts);
}

Vao::Vao(int componentCount, 
            const FloatList& verts, 
            const IndexList& indices) :
    vertexCount(verts.size()),
    indexCount(indices.size()) 
{
    vao = ::InitVao(componentCount, verts, indices);
}

Vao::Vao(int componentCount, 
            const float* verts,
            unsigned vertCount) : 
    vertexCount(vertCount),
    indexCount(0) 
{
    // TODO: we shouldn't be copying the buffer like this, we should just pass it raw
    vao = ::InitVao(componentCount, FloatList(verts, verts+vertCount));
}

Vao::Vao(int componentCount, 
            const float* verts, 
            unsigned vertCount,
            const unsigned* indices,
            unsigned indexCount) :
    vertexCount(vertCount),
    indexCount(0) 
{
    // TODO: we shouldn't be copying the buffer like this, we should just pass it raw
    vao = ::InitVao(componentCount, 
                    FloatList(verts, verts+vertCount),
                    IndexList(indices, indices+indexCount));
}

void 
Vao::AddVertexAttribute(GLuint attrib, 
                            int componentCount, 
                            const FloatList& values) {
    if (!vao) {
        std::cerr << "Array object was not initialized" << std::endl;
    }
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(values[0]) * values.size(), 
                 &values[0], 
                 GL_STATIC_DRAW);

    glVertexAttribPointer(attrib, componentCount, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrib);
    
    pezCheck(glGetError() == GL_NO_ERROR, "Failed to add vertex attribute");
}

void 
Vao::Bind() {
    glBindVertexArray(vao);
    pezCheck(vao != 0, "Invalid VAO in vao.bind");
    pezCheck(glGetError() == GL_NO_ERROR, "vao.bind failed");
}

