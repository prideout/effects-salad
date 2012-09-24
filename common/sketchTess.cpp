#include "common/sketchTess.h"
#include "common/init.h"
#include "common/vao.h"

using namespace sketch;

Tessellator::Tessellator(const sketch::Scene& scene) :
    _scene(&scene)
{
}

void
sketch::Tessellator::PullFromScene()
{
    
}

void
sketch::Tessellator::PushToGpu(Vao& vao)
{
    if (!vao.vao) {
        vao = Vao(_verts, _tris);
        return;
    }

    vao.Bind();
    
    GLuint vbo = 0;
    glGetVertexAttribiv(AttrPosition,
                        GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                        (GLint*) &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(_verts[0]) * _verts.size(), 
                 &_verts[0], 
                 GL_STATIC_DRAW);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 sizeof(_tris[0]) * _tris.size(), 
                 &_tris[0], 
                 GL_STATIC_DRAW);

    vao.indexCount = _tris.size() * 3;
}
