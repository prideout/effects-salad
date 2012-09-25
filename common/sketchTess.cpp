#include "poly2tri/poly2tri.h"
#include "common/sketchUtil.h"
#include "common/sketchTess.h"
#include "common/init.h"
#include "common/vao.h"
#include <list>

using namespace sketch;
using namespace glm;
using namespace std;

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

        const bool usePoly2Tri = true;
        if (usePoly2Tri) {

            CoplanarPath* coplanar = dynamic_cast<CoplanarPath*>(*p);
            pezCheck(coplanar != NULL, "Holes are not supported in non-coplanar paths.");

            Vec2List rim2d;
            _scene->_WalkPath(coplanar, &rim2d, arcTessLength);

            vector<p2t::Point*> polyline;
            FOR_EACH(p, rim2d) {
                polyline.push_back(new p2t::Point(p->x, p->y));
            }
            if (polyline.size() > 3) {
                p2t::CDT* cdt = new p2t::CDT(polyline);

                // TODO Add holes; see http://code.google.com/p/poly2tri/source/browse/testbed/main.cc#167
                // We may also wish to add Steiner points but I see no reason to do so at the moment:
                // http://www.cs.cmu.edu/~quake/triangle.defs.html

                cdt->Triangulate();
                vector<p2t::Triangle*> triangles = cdt->GetTriangles();

                // TODO optimize this retarded code:

                FOR_EACH(t, triangles) {
                    int n = (int) _verts.size();
                    _tris.push_back(ivec3(n,n+1,n+2));
                    vec2 a2 = vec2((*t)->GetPoint(0)->x, (*t)->GetPoint(0)->y);
                    vec2 b2 = vec2((*t)->GetPoint(1)->x, (*t)->GetPoint(1)->y);
                    vec2 c2 = vec2((*t)->GetPoint(2)->x, (*t)->GetPoint(2)->y);
                    vec3 a3 = AddOffset(a2, coplanar->Plane);
                    vec3 b3 = AddOffset(b2, coplanar->Plane);
                    vec3 c3 = AddOffset(c2, coplanar->Plane);
                    _verts.push_back(a3);
                    _verts.push_back(b3);
                    _verts.push_back(c3);
                }

                delete cdt;
            }
            FOR_EACH(p, polyline) {
                delete *p;
            }
        } else {
            Vec3List rim;
            _scene->_WalkPath(*p, &rim, arcTessLength);
            int count = (int) rim.size();
            int n = (int) _verts.size(); // <-- triangle fan center
            _verts.insert(_verts.end(), rim.begin(), rim.end());
            for (int i = 1; i < count - 1; ++i) {
                int j = (i+1) % count;
                _tris.push_back(ivec3(n, n+i, n+j));
            }
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
