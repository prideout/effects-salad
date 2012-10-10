#include "tube.h"
#include "curve.h"
#include "demoContext.h"
#include "init.h"

glm::vec3 _Perp(glm::vec3 u) 
{
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

float
_Lerp(float v0, float v1, float t) {
    if (t > 1.0) t = 1.0;
    if (t < .0) t = .0;
    return (1-t) * v0 + t*v1;
}



void
Tube::Init()
{
    Vec3List spine = cvs;

    Vec3List centerline;
    Blob meshData;
    EvaluateBezier(spine, &centerline, lod);
    _segCount = centerline.size();

    VertexAttribMask attribs = AttrPositionFlag | AttrNormalFlag;

    Vec3List tangents, normals, binormals;
    ComputeFrames(centerline, &tangents, &normals, &binormals);
    Vec3List framesTmp(centerline.size() * 3, glm::vec3());
    FloatList scalesTmp(centerline.size() * 1, 0);
    for (unsigned i = 0; i < centerline.size(); i++) {
        pezCheck(i*3+2 < framesTmp.size(), "Out of bounds FRAMES access!");
        pezCheck(i < scalesTmp.size(), "Out of bounds SCALES access!");

        if (uniformWidth) { 
            scalesTmp[i] = 1.0f; 
        } else {
            scalesTmp[i] = _Lerp(1.0f, 0.0f, i / float(centerline.size() - 1.0f) ); 
        }

        framesTmp[i*3+0] = normals[i];
        framesTmp[i*3+1] = binormals[i];
        framesTmp[i*3+2] = tangents[i];
    }

    centers.Init(centerline);
    frames.Init(framesTmp);

    scales.drawType = GL_DYNAMIC_DRAW;
    scales.Init(scalesTmp);

    SweepPolygon(centerline, 
                 tangents, normals, binormals, 
                 &meshData, attribs, radius, sidesPerSlice);
    tube.Init();
    tube.AddInterleaved(attribs, meshData);
    GetIndices(centerline, sidesPerSlice, &tube);

    _drawCount = tube.indexCount;
 
    FloatList vpoints(centerline.size()*3,0);
    FloatList vnormals(centerline.size()*3,0);
    for(unsigned i = 0; i < centerline.size(); i++) {
        vpoints[i*3+0] = centerline[i].x;
        vpoints[i*3+1] = centerline[i].y;
        vpoints[i*3+2] = centerline[i].z;
        vnormals[i*3+0] = normals[i].x;
        vnormals[i*3+1] = normals[i].y;
        vnormals[i*3+2] = normals[i].z;
    }

    /* Disabled for performance

    normVis = NormalField(3, vpoints, 3, vnormals);
    normVis.Init();

    for(unsigned i = 0; i < centerline.size(); i++) {
        vnormals[i*3+0] = binormals[i].x;
        vnormals[i*3+1] = binormals[i].y;
        vnormals[i*3+2] = binormals[i].z;
    }
    binormVis = NormalField(3, vpoints, 3, vnormals);
    binormVis.Init();

    for(unsigned i = 0; i < centerline.size(); i++) {
        vnormals[i*3+0] = tangents[i].x;
        vnormals[i*3+1] = tangents[i].y;
        vnormals[i*3+2] = tangents[i].z;
    }
    tanVis = NormalField(3, vpoints, 3, vnormals);
    tanVis.Init();
    */
}

void
Tube::Update()
{
    // CPU based implementation, saved here as a fallback if we need to 
    // free up some GPU time

    #if 0
    float time = GetContext()->elapsedTime;
    FloatList scalesTmp(_segCount, 0);
    _drawCount = 0;
    float timeToGrow = 10;
    float segTime = timeToGrow / _segCount;
    float prev = 0;

    for(unsigned i = 0; i < _segCount; i++) {
        // Grow one segment at a time:
        /*
        float segStartTime = i * segTime;
        float segGrowth = (time - segStartTime) / segTime;
        float s = _Lerp(0, 1, segGrowth);
        */
        
        float segStartTime = i * segTime;
        float s = _Lerp(0, 1, (time - segStartTime) / timeToGrow);

        //float p = 1 - (i / float(_segCount * (fract(time/ 80) )));
        //float s = _Lerp(0, 1, p);
        scalesTmp[i] = s; 
        if (s > 0 or prev > 0) {
            _drawCount += sidesPerSlice*3*2;
        }
        prev = s;
    }

    // Send the scales out to the GPU
    scales.Rebuffer(scalesTmp);
    #endif
}

void
Tube::Draw()
{
    Draw(GetContext()->elapsedTime);
}

void
Tube::Draw(float time)
{
    if (startTime == 0) startTime = time;
    glPointSize(6);
    centers.Bind(0, "Centerline");
    frames.Bind(1, "Frames");
    scales.Bind(2, "Scales");
    tube.Bind();
    glUniform1i(u("Slices"), _segCount);
    glUniform1i(u("VertsPerSlice"), sidesPerSlice);
    glUniform1f(u("Time"), time - startTime);
    glUniform1f(u("TimeToGrow"), timeToGrow);
    glDrawElements(GL_TRIANGLES, _drawCount, GL_UNSIGNED_INT, NULL);
}

void
Tube::DrawFrames()
{
    pezCheck(false, "Uncomment the lines in Init and the lines below");
    /* 
    normVis.Draw();
    binormVis.Draw();
    tanVis.Draw();
    */
}


/*static*/ void
Tube::EvaluateBezier(const Vec3List& spine,
                     Vec3List* centerline,
                     int levelOfDetail)
{
    int segs = spine.size() - 1;
    float samples = levelOfDetail * segs;
    Bezier::EvalPiecewise(samples, spine, centerline);
}

// Sweeps an n-sided polygon along a given centerline.
// Populates a buffer with interleaved positions and/or normals.
/*static*/ void
Tube::SweepPolygon(const Vec3List& centerline,
                   const Vec3List& tangents,
                   const Vec3List& normals,
                   const Vec3List& binormals,
                   Blob* outputData,
                   VertexAttribMask requestedAttribs,
                   float polygonRadius,
                   int numPolygonSides)
{
    int n = numPolygonSides;
    const float TWOPI = 2*3.1415;

    unsigned count = centerline.size();
    outputData->resize(count * (n) * 6 * sizeof(float));
    //std::cout << "Max Index: " << count * (n) << std::endl;
    float* mesh = (float*)(&((*outputData)[0]));
    //FloatList mesh(count * (n+1) * 6, 0);
    //mesh = new Float32Array(count * (n+1) * 6)
    int i = 0;
    int m = 0;
    glm::vec3 p;
    float r = polygonRadius;

    pezCheck(normals.size() == centerline.size(), "Number of normals != number of center line points");
    while (i < int(count)) {
        int v = 0;

        glm::mat3 basis(normals[i], binormals[i], tangents[i]);
        
        float theta = 0;
        float dtheta = TWOPI / n;
        
        while (v < n) {
            float x = r*cos(theta);
            float y = r*sin(theta);
            float z = 0;

            // if GPU mode, don't transform
            //p = basis * glm::vec3(x,y,z);
            p = glm::vec3(x,y,z);

            //mat3.multiplyVec3(basis, [x,y,z], p)
            /*
            p.x += centerline[i].x;
            p.y += centerline[i].y;
            p.z += centerline[i].z;
            */

            // Stamp p into 'm', skipping over the normal:
            mesh[m+0] = p.x;
            mesh[m+1] = p.y;
            mesh[m+2] = p.z;

            m = m + 6;
            v++;
            theta += dtheta;
        }

        i++;
    }

    // Next, populate normals:
    i = 0;
    m = 0;
    glm::vec3 normal; 
    glm::vec3 center; 
    while (i < (int) count) {
      int v = 0;
      while (v < n) {
        pezCheck((m+3+2)*sizeof(float) < outputData->size(), "(TubeNormals) Invalid size: %d !< %d", (m+3+2)*sizeof(float), outputData->size());
        p.x = mesh[m+0];
        p.y = mesh[m+1];
        p.z = mesh[m+2];
        center = centerline[i];

        glm::vec3 normal = glm::normalize(p); // - center);

        // Stamp n into 'm', skipping over the position:
        mesh[m+3+0] = normal.x;
        mesh[m+3+1] = normal.y;
        mesh[m+3+2] = normal.z;

        m += 6;
        v++;
      }

      i++;
    }
}

/*static*/ void
Tube::GetIndices(const Vec3List& centerline,
                 int numPolygonSides,
                 Vao* target) 
{
    // Create the index buffer for the solid tube
    int faceCount = centerline.size() * numPolygonSides * 2;
    Blob rawBuffer(faceCount*3*sizeof(unsigned), 0u);

    // XXX(jcowles): this is a little too much of a transliteration of the
    //               original coffee script for my taste, I would like to 
    //               reduce the number of variables, etc.
    size_t i = 0;
    int ptr = 0;
    int v = 0;
    while (++i < centerline.size()) {
        int j = -1;
        while (++j < numPolygonSides) {
            int next = (j + 1) % numPolygonSides; 
            unsigned* tri = (unsigned*)(&rawBuffer[0]) + ptr;
            tri[0] = v+next+numPolygonSides;
            tri[1] = v+next;
            tri[2] = v+j;
            tri = (unsigned*)(&rawBuffer[0]) + ptr + 3;
            tri[0] = v+j;
            tri[1] = v+j+numPolygonSides;
            tri[2] = v+next+numPolygonSides;
            ptr += 6;
        }
      v += numPolygonSides;
    }
    
    target->AddIndices(rawBuffer);
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

    // Note: R -> Normals
    //       S -> Binormals
    //       T -> Tangents
    const int tangentSmoothness = 0;
    int count = centerline.size();
    
    // convenience refs from ptrs
    Vec3List& Ts = *tangents;
    Vec3List& Ns = *normals;
    Vec3List& Bs = *binormals;

    Ts.resize(count);
    Ns.resize(count);
    Bs.resize(count);

    // Obtain unit-length tangent vectors
    int i = -1;
    while(++i < count) {
        int ii = i;
        int j = (i+1+tangentSmoothness) % (count);
        if (j < i) {
            ii = j;
            j = i;
        }
        if (centerline[ii] == centerline[j]) {
            if (i > 0) 
                Ts[i] = Ts[i-1];
            else
                pezCheck(false, "Unhandled duplicate knots at 0"); 
        } else {
            Ts[i] = glm::normalize(centerline[ii] - centerline[j]);
        }
    }

    // Allocate some temporaries for vector math
    glm::vec3 n0, b0, t0,
              nj, bj, tj;

    // Create a somewhat-arbitrary initial frame (n0, b0, t0)
    t0 = Ts[0];
    n0 = _Perp(t0);
    b0 = glm::cross(t0, n0);
    n0 = glm::normalize(n0);
    b0 = glm::normalize(b0);
    Ns[0] = n0;
    Bs[0] = b0;

    // Use parallel transport to sweep the frame
    i = 0;
    int j = 1;
    glm::vec3 ni = n0, 
              bi = b0, 
              ti = t0;
    while(i < count-1) {
        j = i + 1;
        glm::vec3 ti = Ts[i];
        glm::vec3 tj = Ts[j];
        bj = glm::cross(tj, ni);
        bj = glm::normalize(bj);
        nj = glm::cross(bj, tj);
        Ns[j] = nj;
        Bs[j] = bj;
        ni = nj; 
        ++i;
    }
}
