#include "tube.h"

// Quick and dirty CPU-based 2D Bézier evaluator,
// solely for purpose of converting coarse knot data
// loaded from disk (the "spine") into a smooth curve
// in 3-space (the "centerline")
/*static*/ void
Tube::EvaluateBezier(const Vec3List& spine,
                     Vec3List* centerline,
                     int levelOfDetail)
{
}

// Sweeps an n-sided polygon along a given centerline.
// Populates a buffer with interleaved positions and/or normals.
/*static*/ void
Tube::SweepPolygon(const Vec3List& centerline,
                   Blob* outputData,
                   VertexAttribMask requestedAttribs,
                   float polygonRadius,
                   int numPolygonSides)
{
}


// Generates reasonable orthonormal basis vectors for a
// curve in R3.  See "Computation of Rotation Minimizing Frame"
// by Wang and Jüttler.
/*static*/ void
Tube::ComputeFrames(const Vec3List& centerline,
                  Vec3List* tangents,
                  Vec3List* normals,
                  Vec3List* binormals)
{
}
