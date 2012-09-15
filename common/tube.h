#pragma once

#include "drawable.h"
#include "normalField.h"
#include "typedefs.h"
#include "vao.h"

class Tube : public Drawable {
public:
    // The curve parameter in [0..1] indicating what percentage
    // of the curve to draw. Setting this value will cause update
    // to apply changes to the widths of this curve.
    float t;

    Tube() : Drawable(), t(0) {}

    Vec3List cvs;
    Vao tube;
    NormalField normVis;
    NormalField binormVis;
    NormalField tanVis;

    virtual void Init();
    virtual void Draw();
    virtual void Update();
    virtual void DrawFrames();
    
    // Quick and dirty CPU-based 2D Bézier evaluator,
    // solely for purpose of converting coarse knot data
    // loaded from disk (the "spine") into a smooth curve
    // in 3-space (the "centerline")
    static void
    EvaluateBezier(const Vec3List& spine,
                   Vec3List* centerline,
                   int levelOfDetail);
    
    // Sweeps an n-sided polygon along a given centerline.
    // Populates a buffer with interleaved positions & normals.
    static void
    SweepPolygon(const Vec3List& centerline,
                 Blob* outputData,
                 VertexAttribMask requestedAttribs,
                 float polygonRadius,
                 int numPolygonSides);

    static void
    GetIndices(const Vec3List& centerline, int numPolygonSides, Vao* target);

    // Generates reasonable orthonormal basis vectors for a
    // curve in R3.  See "Computation of Rotation Minimizing Frame"
    // by Wang and Jüttler.
    static void
    ComputeFrames(const Vec3List& centerline,
                  Vec3List* tangents,
                  Vec3List* normals,
                  Vec3List* binormals);
}; 
