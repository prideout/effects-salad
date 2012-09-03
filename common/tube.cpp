#include "tube.h"

glm::vec3 _Perp(glm::vec3 u) {
    glm::vec3 dest;
    glm::vec3 v = glm::vec3(1,0,0);
    dest = glm::cross(u,v);
    float e = glm::dot(dest,dest);
    if (e < 0.01) {
        v = glm::vec3(0,1,0); 
        dest = glm::cross(u,v);
    }
    return glm::normalize(dest);
}


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

    // XXX(jcowles): WARNING: this *compiles* but has not been tested!

    const unsigned tangentSmoothness = 3.0f;
    unsigned count = centerline.size();
    
    // convenience refs from ptrs
    Vec3List& Ts = *tangents;
    Vec3List& Ns = *normals;
    Vec3List& Bs = *binormals;

    Ts.reserve(count);
    Ns.reserve(count);
    Bs.reserve(count);

    // Obtain unit-length tangent vectors
    int i = 0;
    int j = 0;
    FOR_EACH(p, centerline) {
        j = (i+1+tangentSmoothness) % (count - 1);
        Ts[j] = glm::normalize(centerline[i] - centerline[j]);
        i++;
    }

    // Allocate some temporaries for vector math
    glm::vec3 n0, b0, t0,
              nj, bj, tj;

    // Create a somewhat-arbitrary initial frame (n0, b0, t0)
    t0 = Ts[0];
    n0 = _Perp(t0);
    b0 = glm::cross(t0, n0);
    glm::normalize(n0);
    glm::normalize(b0);
    Ns[0] = n0;
    Bs[0] = b0;

    // Use parallel transport to sweep the frame
    i = 0;
    j = 1;
    glm::vec3 ni = n0, 
              si = b0, 
              ti = t0;
    while(i < (int) count-1) {
        j = i + 1;
        glm::vec3 xi = centerline[i];     //centerline.subarray(i*3, i*3+3)
        glm::vec3 xj = centerline[j];     //.subarray(j*3, j*3+3)
        glm::vec3 ti = Ts[i];             //frameT.subarray(i*3, i*3+3)
        glm::vec3 tj = Ts[j];             //frameT.subarray(j*3, j*3+3)
        bj = glm::cross(tj, ni);
        glm::normalize(bj);
        nj = glm::cross(bj, tj);
        Ns[j] = nj;
        Bs[j] = bj;
        ni = nj; 
        ++i;
    }
}
