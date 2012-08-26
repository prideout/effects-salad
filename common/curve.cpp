#include "curve.h"
#include <iostream>

#if 0
//
// Curve, abstract base class
//
Curve::Curve(float lod, const Vec3List& cvs) : Drawable() {
    Init(lod, cvs);
}

void
Curve::Init(float lod, const Vec3List& cvs) {
    _lod = lod;
    // make a copy of the CVs
    _cvs = cvs;
    Init();
}

void 
Curve::Init() {
    // build up a point vector for drawing
    // XXX: we now have two copies of the points, this really isn't necessary
    FloatList points;
    _Eval(&points);
    vao = Vao(4, points);
    std::cout << vao.vertexCount << std::endl;
    pezCheckGL("Create curve VAO failed");
}

void
Curve::Update() {
    // nothing to do here
}

void 
Curve::Draw() {
    vao.Bind();
    glDrawArrays(GL_LINE_STRIP, 0, vao.vertexCount);
    pezCheckGL("Draw lines failed");
}


//
// B-Spline
//
BSpline::BSpline(float lod, const Vec3List& cvs) : Curve(lod, cvs) {

}


void
BSpline::_Eval(FloatList* points) 
{
    points->clear();
    if (_cvs.size() < 4) return;

    Vec3List::iterator cvi = _cvs.begin();
    while(cvi+3 != _cvs.end()) {
	float scale = 1.0/6.0;

	for(float i = 0; i <= _lod; i += 1){
            // walk the parameter space based on LOD
            float u = 1.0 * i / _lod;
            
            // Evaluate the X,Y,Z coords, convert to homogeneous coords
            glm::vec3 out(scale * _EvalHelper(cvi[0], cvi[1], cvi[2], cvi[3], u));
           
            std::cout << "Curve Point: " 
                 << out.x << ", "
                 << out.y << ", "
                 << out.z <<  std::endl;
            points->push_back(out.x);
            points->push_back(out.y);
            points->push_back(out.z);
            points->push_back(1.0);
	}
        cvi++;
    }
}


//
// Bezier
//
Bezier::Bezier(float lod, const Vec3List& cvs) : Curve(lod, cvs) {

}


void
Bezier::_Eval(FloatList* points) {
    points->clear();
    if (_cvs.size() < 2) return;

    int k;
    Vec3List::iterator p;
    
    int pcount = _cvs.size() - 1;

    for(float i = 0; i <= _lod; i++) {
            float u = 1.0 * (i / _lod);
            k = 0;
            glm::vec3 pt;
            for(p = _cvs.begin(); p != _cvs.end(); ++p) {
                    pt += _EvalHelper(*p, u, pcount, k);
                    k++;
            }
            points->push_back(pt.x);
            points->push_back(pt.y);
            points->push_back(pt.z);
            points->push_back(1.0);

            std::cout << "Curve Point: " 
                 << pt.x << ", "
                 << pt.y << ", "
                 << pt.z <<  std::endl;
    }
}

double fact(int n){
    double res = 1;
    for (int i = 2; i <= n; i++) {
            res = res * i;
    }
    return res;
}
#endif
