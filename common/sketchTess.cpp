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
    _topologyHashPushToGpu = 0;
    _topologyHashDelaunay = 0;
}

void
sketch::Tessellator::PullFromScene()
{
    if (_topologyHashDelaunay == _scene->GetTopologyHash()) {
        return;
    }

    _tris.clear();

    float arcTessLength = 0;
    
    FOR_EACH(p, _scene->_paths) {

        CoplanarPath* coplanar = dynamic_cast<CoplanarPath*>(*p);
        pezCheck(coplanar != NULL, "Holes are not supported in non-coplanar paths.");

        Vec2List rim2d;
        IndexList indices;
        _scene->_WalkPath(coplanar, &rim2d, arcTessLength, &indices);

        if (rim2d.size() == 3) {
            _tris.push_back(ivec3(indices[0],indices[1], indices[2]));
            continue;
        }

        if (rim2d.size() < 3) {
            continue;
        }

        list<vector<p2t::Point*> > holes;
        vector<p2t::Point*> polyline;
        map<p2t::Point*, unsigned int> pointmap;
        FOR_EACH(p, rim2d) {
            p2t::Point* newp = new p2t::Point(p->x, p->y);
            pointmap[newp] = indices[p - rim2d.begin()];
            polyline.push_back(newp);
        }
        if (polyline.size() > 3) {
            p2t::CDT* cdt = new p2t::CDT(polyline);

            FOR_EACH(hole, coplanar->Holes) {

                CoplanarPath* copHole = dynamic_cast<CoplanarPath*>(*hole);
                pezCheck(copHole != NULL);

                Vec2List holePath;
                IndexList indices;
                _scene->_WalkPath(copHole, &holePath, arcTessLength, &indices);
                vector<p2t::Point*> polyline;
                FOR_EACH(p, holePath) {
                    p2t::Point* newp = new p2t::Point(p->x, p->y);
                    pointmap[newp] = indices[p - holePath.begin()];
                    polyline.push_back(newp);
                }
                cdt->AddHole(polyline);
                holes.push_back(polyline);
            }

            // We may also wish to add Steiner points here, but I see no
            // reason to do so at the moment:
            // http://www.cs.cmu.edu/~quake/triangle.defs.html

            cdt->Triangulate();
            vector<p2t::Triangle*> triangles = cdt->GetTriangles();

            FOR_EACH(t, triangles) {
                unsigned au = pointmap[(*t)->GetPoint(0)];
                unsigned bu = pointmap[(*t)->GetPoint(1)];
                unsigned cu = pointmap[(*t)->GetPoint(2)];
                _tris.push_back(ivec3(au, bu, cu));
            }

            delete cdt;
        }
        FOR_EACH(p, polyline) {
            delete *p;
        }
        FOR_EACH(h, holes) {
            FOR_EACH(p, *h) {
                delete *p;
            }
        }
    }
    _topologyHashDelaunay = _scene->GetTopologyHash();
}

void
sketch::Tessellator::PushToGpu(Vao& vao)
{
    Vec3List& verts = *(const_cast<Vec3List*>(&_scene->_points));
 
    if (!vao.vao) {
        vao = Vao(verts, _tris);
        vao.indexCount = 0;
    }

    vao.Bind();
    
    GLuint vbo = 0;
    glGetVertexAttribiv(AttrPosition,
                        GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                        (GLint*) &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(verts[0]) * verts.size(), 
                 &verts[0], 
                 GL_STATIC_DRAW);
    Vao::totalBytesBuffered += sizeof(verts[0]) * verts.size();

    if (_topologyHashPushToGpu != _scene->GetTopologyHash()) {
    
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                     sizeof(_tris[0]) * _tris.size(), 
                     &_tris[0], 
                     GL_STATIC_DRAW);
        Vao::totalBytesBuffered += sizeof(_tris[0]) * _tris.size();

        vao.indexCount = _tris.size() * 3;
        _topologyHashPushToGpu = _scene->GetTopologyHash();
    }
}
