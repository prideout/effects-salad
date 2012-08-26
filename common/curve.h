#pragma once

#include <vector>
#include <cmath>

#include "drawable.h"
#include "typedefs.h"
#include "vao.h"

class Curve : public Drawable {
protected:
    Vec3List _cvs;
    unsigned _lod;

    // given the current curve CVs and LOD,
    // produces a vector of points to approximate the curve 
    virtual void _Eval(FloatList* points) = 0;


public:

    Vao vao;

    Curve() : _lod(0) {}

    // LOD is the number of subdivisions over the curve parameter
    Curve(float lod, const Vec3List& cvs);
    virtual ~Curve() {};
    
    // Access to these needs to be protected, to ensure consistency
    const Vec3List& GetCVs() { return _cvs; }
    const unsigned GetLOD() { return _lod; }

    // TODO:
    // void GetPoints(Vec4List* points);

    // calling INIT will evaluate the curve, generate a vertex buffer
    // for the given control verts
    virtual void Init(float lod, const Vec3List& cvs);
    virtual void Init();
    virtual void Update();
    virtual void Draw();

};

//
// BSpline: a very simple b-spline evaluator
//
class BSpline : public Curve {

    // Vectorized evaluator
    template <typename VEC>
    VEC _EvalHelper(VEC p1, VEC p2, VEC p3, VEC p4, float u);

protected:

    virtual void _Eval(FloatList* points);

public:

    BSpline() : Curve() {}
    BSpline(float lod, const Vec3List& cvs);

};


template <typename VEC> VEC
BSpline::_EvalHelper(VEC p1, VEC p2, VEC p3, VEC p4, float u) {
    // deCastlejau evaluation
    float u3 = u*u*u;
    float u2 = u*u;
    return (p1*(-u3 + 3*u2 - 3*u + 1)
            +  p2*(3*u3 - 6*u2 + 4)
            +  p3*(-3*u3 + 3*u2 + 3*u + 1)
            +  p4*(u3));
}


//
// Bezier: a very simple bezier evaluator
//
class Bezier : public Curve {

    // Vectorized evaluator
    template <typename VEC>
    VEC _EvalHelper(VEC p, float u, float n, float k);

protected:

    virtual void _Eval(FloatList* points);

public:

    Bezier() : Curve() {}
    Bezier(float lod, const Vec3List& cvs);

};

double fact(int n); 

template <typename VEC> VEC
Bezier::_EvalHelper(VEC p, float u, float n, float k) {
    float b = (fact(n)/(fact(k)*fact(n - k))) 
                 * pow(1-u, n-k) * pow(u,k);
    return b * p;
}
