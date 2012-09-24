#include "common/sketchTess.h"
#include "common/init.h"
#include "common/vao.h"

using namespace sketch;
using namespace glm;

Tessellator::Tessellator(const sketch::Scene& scene) :
    _scene(&scene)
{
}

void
sketch::Tessellator::PullFromScene()
{
    _verts.clear();
    _tris.clear();

    float arcTessLength = 0;

    // TODO handle convex paths and honor IsHole using Poly2Tri
    // TODO we should simply use the _scene->_points as a VBO;
    //      this would allow point-sharing on corners
    
    FOR_EACH(p, _scene->_paths) {
        int n = (int) _verts.size();
        Vec3List rim = _scene->_WalkPath(*p, arcTessLength);
        int count = (int) rim.size();
        _verts.insert(_verts.end(), rim.begin(), rim.end());
        for (int i = 1; i < count - 1; ++i) {
            int j = (i+1) % count;
            _tris.push_back(ivec3(n, n+i, n+j));
        }
    }
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
