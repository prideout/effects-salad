#include "vao.h"
#include "init.h"

int Vao::totalBytesBuffered = 0;

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
    totalBytesBuffered += sizeof(verts[0]) * verts.size();
}

Vao::Vao(int componentCount, 
            const FloatList& verts, 
            const IndexList& indices) :
    vertexCount(verts.size() / componentCount),
    indexCount(indices.size()) 
{
    vao = ::InitVao(componentCount, verts, indices);
    totalBytesBuffered += sizeof(verts[0]) * verts.size();
    totalBytesBuffered += sizeof(indices[0]) * indices.size();
}

Vao::Vao(int componentCount, 
            const float* verts,
            unsigned vertCount) : 
    vertexCount(vertCount),
    indexCount(0) 
{
    // TODO: we shouldn't be copying the buffer like this, we should just pass it raw
    vao = ::InitVao(componentCount, FloatList(verts, verts+vertCount*componentCount));
    totalBytesBuffered += sizeof(verts[0]) * vertCount;
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
    totalBytesBuffered += sizeof(verts[0]) * vertCount;
    totalBytesBuffered += sizeof(indices[0]) * indexCount;
}

Vao::Vao(const Vec3List& verts, const TriList& indices) : 
    vertexCount(verts.size()),
    indexCount(indices.size())
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * verts.size(), &verts[0], GL_STATIC_DRAW);
    pezCheck(glGetError() == GL_NO_ERROR, "vao-vbo setup failed");
    totalBytesBuffered += sizeof(verts[0]) * verts.size();

    // indices
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
    pezCheck(glGetError() == GL_NO_ERROR, "vao-ibo setup failed");
    totalBytesBuffered += sizeof(indices[0]) * indices.size();

    // setup the "Position" attribute
    glVertexAttribPointer(AttrPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(AttrPosition);
}

Vao::Vao(const Vec3List& verts) :
    vertexCount(verts.size()),
    indexCount(0)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * verts.size(), &verts[0], GL_STATIC_DRAW);
    pezCheck(glGetError() == GL_NO_ERROR, "vao-vbo setup failed");
    totalBytesBuffered += sizeof(verts[0]) * verts.size();

    // setup the "Position" attribute
    glVertexAttribPointer(AttrPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(AttrPosition);
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

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(values[0]) * values.size(), 
                 &values[0], 
                 GL_STATIC_DRAW);
    totalBytesBuffered += sizeof(values[0]) * values.size();

    glVertexAttribPointer(attrib, componentCount, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrib);
    
    pezCheck(glGetError() == GL_NO_ERROR, "Failed to add vertex attribute");
}

void 
Vao::AddVertexAttribute(GLuint attrib, 
                        int componentCount, 
                        const float* values,
                        int vertexCount) {

    // XXX: This always makes a new VBO, which really isn't necessary...

    if (!vao) {
        std::cerr << "Array object was not initialized" << std::endl;
    }
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 
                 vertexCount * componentCount * sizeof(float), 
                 values, 
                 GL_STATIC_DRAW);
    totalBytesBuffered += sizeof(float) * vertexCount * componentCount;

    glVertexAttribPointer(attrib, componentCount, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrib);
    
    pezCheck(glGetError() == GL_NO_ERROR, "Failed to add vertex attribute");
}

void 
Vao::AddVertexAttribute(GLuint attrib, 
                        int componentCount, 
                        const Blob& values) {

    // XXX: This always makes a new VBO, which really isn't necessary...

    if (!vao) {
        std::cerr << "Array object was not initialized" << std::endl;
    }
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 
                 values.size(), 
                 &values[0], 
                 GL_STATIC_DRAW);
    totalBytesBuffered += sizeof(values[0]) * values.size();

    glVertexAttribPointer(attrib, componentCount, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrib);
    
    pezCheck(glGetError() == GL_NO_ERROR, "Failed to add vertex attribute");
}

void
Vao::AddIndices(const Blob& data) {
    if (!vao) {
        std::cerr << "Array object was not initialized" << std::endl;
    }
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 data.size(), 
                 &data[0], 
                 GL_STATIC_DRAW);
    this->indexCount = data.size() / sizeof(int);
    totalBytesBuffered += data.size();
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

// It is legal in modern OpenGL to have 0 enabled vertex attribs,
// which is convenient for ubersimple geometry (eg fullscreen
// triangles) but some Mac drivers have issues with this.
// Workaround is to add some dummy data into the VAO.
void
Vao::InitEmpty() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float dummy[4] = {0};
    glEnableVertexAttribArray(0);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(dummy), 
                 &dummy[0], 
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0);
    totalBytesBuffered += sizeof(dummy); 
}

void
Vao::AddInterleaved(VertexAttribMask attribs,
                    const Blob& data)
{
    pezCheck(vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size(), &data[0], GL_STATIC_DRAW);
    totalBytesBuffered += data.size(); 

    int stride = 0;
    if (attribs & AttrPositionFlag) stride += AttrPositionWidth;
    if (attribs & AttrNormalFlag)   stride += AttrNormalWidth;
    if (attribs & AttrTexCoordFlag) stride += AttrTexCoordWidth;
    if (attribs & AttrTetIdFlag)    stride += AttrTetIdWidth;
    if (attribs & AttrLengthFlag)   stride += AttrLengthWidth;
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
    if (attribs & AttrLengthFlag) {
        glVertexAttribPointer(AttrLength, 1, GL_FLOAT, GL_FALSE, stride, offset(p));
        glEnableVertexAttribArray(AttrLength);
        p += AttrLengthWidth;
    }
}
