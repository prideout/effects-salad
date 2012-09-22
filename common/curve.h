#pragma once

#include <vector>
#include <cmath>
#include <iostream>

#include "drawable.h"
#include "typedefs.h"
#include "vao.h"

//
// B-Spline
//

namespace BSpline {

    template<typename VEC> VEC
    _EvalHelper(VEC p1, VEC p2, VEC p3, VEC p4, float u) {
        // deCastlejau evaluation
        float u3 = u*u*u;
        float u2 = u*u;
        return (p1*(-u3 + 3*u2 - 3*u + 1)
                +  p2*(3*u3 - 6*u2 + 4)
                +  p3*(-3*u3 + 3*u2 + 3*u + 1)
                +  p4*(u3));
    }

    template <typename VEC>
    std::vector<VEC> Eval(float samplesPerCurve, std::vector<VEC> cvs) {
        pezCheck(samplesPerCurve > 1, "Error: numSamples must be 2 or greater");
        pezCheck(cvs.size() > 3, "Error: 2 or more CVs required");

        // the number of samples is internally 1-numSamples
        samplesPerCurve -= 1;

        typename std::vector<VEC> points;
        if (cvs.size() < 4) return points;

        typename std::vector<VEC>::iterator cvi = cvs.begin();
        while(cvi+3 != cvs.end()) {
            float scale = 1.0/6.0;

            for(float i = 0; i <= samplesPerCurve ; i += 1){
                // walk the parameter space based on LOD
                float u = 1.0 * i / samplesPerCurve;
                
                // Evaluate the X,Y,Z coords, convert to homogeneous coords
                VEC out(scale * _EvalHelper(cvi[0], cvi[1], cvi[2], cvi[3], u));
               
                /*
                std::cout << "Curve Point: " 
                     << out.x << ", "
                     << out.y << ", "
                     << out.z <<  std::endl;
                */
                points.push_back(out);
            }
            cvi++;
        }

        return points;
    }

}

//
// Bezier
//

namespace Bezier {
    inline
    double fact(int n){
        double res = 1;
        for (int i = 2; i <= n; i++) {
                res = res * i;
        }
        return res;
    }

    template <typename VEC> VEC
    _EvalHelper(VEC p, float u, float n, float k) {
        float b = (fact(n)/(fact(k)*fact(n - k))) 
                     * pow(1-u, n-k) * pow(u,k);
        return b * p;
    }

    template <typename VEC> 
    void
    Eval(float numSamples, const std::vector<VEC>& cvs, int cvStart, int cvCount, std::vector<VEC>* points)
    {
        pezCheck(numSamples > 1, "Error: numSamples must be 2 or greater");
        pezCheck(cvs.size() > 1, "Error: 2 or more CVs required");
        pezCheck(cvCount <= (int) cvs.size(), "Error: cvCount > cvs.size()");

        // the number of samples is internally 1-numSamples
        numSamples -= 1;

        int k;
        typename std::vector<VEC>::const_iterator p;
        
        int pcount = cvCount - 1;

        for(float i = 0; i <= numSamples; i++) {
            float u = 1.0 * (i / numSamples);
            k = 0;
            VEC pt;
            p = cvs.begin() + cvStart;
            for (int i = 0; i < cvCount; i++, p++) {
                pt += _EvalHelper(*p, u, pcount, k);
                k++;
            }
            points->push_back(pt);
        }
    }

    template <typename VEC> 
    void
    Eval(float numSamples, const std::vector<VEC>& cvs, std::vector<VEC>* points)
    {
        Eval(numSamples, cvs, 0, (int) cvs.size(), points);
    }

    template <typename VEC>
    void
    EvalPiecewise(float numSamples, const std::vector<VEC>& cvs, std::vector<VEC>* points)
    {
        unsigned i = 0;
        while (i + 4 <= cvs.size()) {
            Eval(numSamples, cvs, i, 4, points); 
            i += 3;
        }
    }
}

template<typename T>
class AnimCurve {
    // these can't be simpliy updated, so keep them private
    std::vector<T> _points;
    float _startTime;
    float _duration;

    unsigned _GetIndex(float time)
    {
        pezCheck(_duration > 0, "Invalid AnimCurve duration");
        pezCheck(_points.size() > 0, "Invalid number of curve points");
        if (time < _startTime) return 0;
        if (time > _startTime+_duration) return _points.size() -1;
        //std::cout << unsigned(((time - _startTime) / _duration) * _points.size()) << std::endl;

        // no interpolation, just grab the nearest sample
        unsigned index = round(((time - _startTime) / _duration) * (_points.size() - 1));
        pezCheck(index < _points.size(), "Invalid AnimCurve time index");
        return index;
    }

public:

    typedef std::vector<T> VecT;

    AnimCurve() :
        _startTime(0),
        _duration(0)
    {}

    AnimCurve(VecT cvs, float startTime, float duration) :
        _startTime(startTime),
        _duration(duration)
    {
        pezCheck(cvs.size() > 0, "Invalid number of curve CVs");
        Bezier::Eval(60*duration, cvs, &_points);    
    }

    bool IsEmpty() { return _points.size() == 0; }

    // Get the interpolated value at the given time
    T At(float time)
    {
        return _points[_GetIndex(time)];
    }

    // Get the next interpolated value after the given time
    T After(float time)
    {
        unsigned index = (_GetIndex(time)+1) % _points.size();
        return _points[index];
    }


};


/*
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
*/
