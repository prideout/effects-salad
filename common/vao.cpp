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
    vertexCount(verts.size() / componentCount),
    indexCount(0) 
{    
    vao = ::InitVao(componentCount, verts);
}

Vao::Vao(int componentCount, 
            const FloatList& verts, 
            const IndexList& indices) :
    vertexCount(verts.size() / componentCount),
    indexCount(indices.size()) 
{
    vao = ::InitVao(componentCount, verts, indices);
}

Vao::Vao(int componentCount, 
            const float* verts,
            unsigned vertCount) : 
    vertexCount(vertCount / componentCount),
    indexCount(0) 
{
    // TODO: we shouldn't be copying the buffer like this, we should just pass it raw
    vao = ::InitVao(componentCount, FloatList(verts, verts+vertCount*componentCount));
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
                    FloatList(verts, verts+vertCount*componentCount),
                    IndexList(indices, indices+indexCount));
}

void 
Vao::AddVertexAttribute(GLuint attrib, 
                            int componentCount, 
                            const FloatList& values) {

    // XXX: This always makes a new VBO, which really isn't necessary...

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

void
Vao::Init() {
    glGenVertexArrays(1, &vao);
}

void
Vao::AddInterleaved(VertexAttribMask attribs,
                    const Blob& data)
{
    pezCheck(vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size(), &data[0], GL_STATIC_DRAW);

    int stride = 0;
    if (attribs & AttrPositionFlag) stride += AttrPositionWidth;
    if (attribs & AttrNormalFlag)   stride += AttrNormalWidth;
    if (attribs & AttrTexCoordFlag) stride += AttrTexCoordWidth;
    if (attribs & AttrTetIdFlag)    stride += AttrTetIdWidth;
    this->vertexCount = data.size() / stride;

    int p = 0;
    if (attribs & AttrPositionFlag) {
        glVertexAttribPointer(AttrPosition, 3, GL_FLOAT, GL_FALSE, stride, offset(p));
        glEnableVertexAttribArray(AttrPosition);
        p += AttrPositionWidth;
    }
    if (attribs & AttrNormalFlag) {
        glVertexAttribPointer(AttrNormal, 3, GL_FLOAT, GL_FALSE, stride, offset(p));
        glEnableVertexAttribArray(AttrNormal);
        p += AttrNormalWidth;
    }
    if (attribs & AttrTexCoordFlag) {
        glVertexAttribPointer(AttrTexCoord, 2, GL_FLOAT, GL_FALSE, stride, offset(p));
        glEnableVertexAttribArray(AttrTexCoord);
        p += AttrTexCoordWidth;
    }
    if (attribs & AttrTetIdFlag) {
        glVertexAttribIPointer(AttrTetId, 1, GL_UNSIGNED_INT, stride, offset(p));
        glEnableVertexAttribArray(AttrTetId);
        p += AttrTetIdWidth;
    }
}
