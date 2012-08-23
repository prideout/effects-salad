#pragma once

#include "drawable.h"
#include "typedefs.h"

class Tube : public Drawable {
public:
    virtual void Init() {}
    virtual void Draw() {}
    
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


    // Generates reasonable orthonormal basis vectors for a
    // curve in R3.  See "Computation of Rotation Minimizing Frame"
    // by Wang and Jüttler.
    static void
    ComputeFrames(const Vec3List& centerline,
                  Vec3List* tangents,
                  Vec3List* normals,
                  Vec3List* binormals);
}; 
