#include "fx/buildings.h"
#include "common/tetUtil.h"
#include "common/init.h"
#include "common/programs.h"
#include "common/camera.h"
#include "common/demoContext.h"

using namespace std;
using glm::mat4;
using glm::mat3;
using glm::vec3;

void
Buildings::Init()
{
    // Create the boundaries
    tetgenio in;
    float r1 = 13;    float r2 = 8;
    float y1 = 0;     float y2 = 20;
    TetUtil::HullFrustum(r1, r2, y1, y2, 4, &in);
    y1 += 3; y2 -= 3;
    r1 -= 3; r2 -= 3;
    TetUtil::HullFrustum(r1, r2, y1, y2, 4, &in);

    TetUtil::HullTranslate(27, 0, 0, &in);

    r1 = 13;    r2 = 8;
    y1 = 0;     y2 = 20;
    TetUtil::HullFrustum(r1, r2, y1, y2, 5, &in);
    y1 += 3; y2 -= 3;
    r1 -= 3; r2 -= 3;
    TetUtil::HullFrustum(r1, r2, y1, y2, 5, &in);

    // Poke volumetric holes
    Vec3List holePoints;
    holePoints.push_back(vec3(0, 10, 0));
    holePoints.push_back(vec3(27, 10, 0));
    TetUtil::AddHoles(holePoints, &in);

    // Tetrahedralize the boundary mesh
    tetgenio out;
    const float qualityBound = 1.414;
    const float maxVolume = 0.1f;
    TetUtil::TetsFromHull(in, &out, qualityBound, maxVolume, false);
    _totalTetCount = out.numberoftetrahedra;

    // Populate the per-tet texture data
    Vec4List centroids;
    TetUtil::SortTetrahedra(&centroids, out, &_boundaryTetCount);
    _centroidTexture.Init(centroids);

    // Create a flat list of non-indexed triangles
    Blob massive;
    VertexAttribMask attribs = AttrPositionFlag | AttrNormalFlag;
    TetUtil::PointsFromTets(out, attribs, &massive);
    _buildingVao.Init();
    _buildingVao.AddInterleaved(attribs, massive);

    // Compile shaders
    Programs& progs = Programs::GetInstance();
    progs.Load("Tetra.Simple", false);
    progs.Load("Tetra.Solid", false);
}

void 
Buildings::Update()
{
}

void
Buildings::Draw()
{
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    Programs& progs = Programs::GetInstance();

    bool cuttingPlane = true;
    float cullY = 9;
    glUseProgram(progs["Tetra.Solid"]);
    glUniform1f(u("CullY"), cuttingPlane ? cullY : 999);

    PerspCamera surfaceCam = context->mainCam;
    surfaceCam.Bind(glm::mat4());

    _centroidTexture.Bind(0, "CentroidTexture");
    _buildingVao.Bind();

    // Optimization: don't draw interior tets after completely filling the volume
    if (false) {
        glDrawArrays(GL_TRIANGLES, 0, _boundaryTetCount * 4 * 3);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, _totalTetCount * 4 * 3);
    }
}
