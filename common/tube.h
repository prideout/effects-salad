#pragma once

#include "drawable.h"
#include "normalField.h"
#include "texture.h"
#include "typedefs.h"
#include "vao.h"

class Tube : public Drawable {
    int _segCount;
    int _drawCount;

public:
    float startTime;
    float timeToGrow;
    int sidesPerSlice;
    int lod;
    float radius;

    Tube() : Drawable(), 
        _segCount(0), 
        _drawCount(0),
        startTime(0), 
        timeToGrow(9), 
        sidesPerSlice(8), 
        lod(3),
        radius(0.2f)
    {
    }

    Vec3List cvs;
    Vao tube;
    
    BufferTexture centers;
    BufferTexture frames;
    BufferTexture scales;

    NormalField normVis;
    NormalField binormVis;
    NormalField tanVis;

    virtual void Init();
    virtual void Draw();
    virtual void Update();
    virtual void DrawFrames();
    
    // Evaluate the spine as a piecewise curve set
    static void
    EvaluateBezier(const Vec3List& spine,
                   Vec3List* centerline,
                   int levelOfDetail);
    
    // Sweeps an n-sided polygon along a given centerline.
    // Populates a buffer with interleaved positions & normals.
    static void
    SweepPolygon(const Vec3List& centerline,
                 const Vec3List& tangents,
                 const Vec3List& normals,
                 const Vec3List& binormals,
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
