//
// Building destruction
//   1 Silently change facets into tets
//   2 Grow glowing cracks along triangle edges from bottom up
//   3 Progressively peel away tets from the top
//   4 Make the cracks glow, perhaps with an HDR buffer
//
// Stack buildings for a slightly more interesting effect
//
// Buildings should have floors
//
// Build city using simplistic 2D packing of triangles, pentagons, circles, and squares
// Camera
// DOF effect
// Look at Akira references
// Smoke
//

#include "glm/gtc/type_ptr.hpp"
#include "fx/buildings.h"
#include "glm/gtx/rotate_vector.hpp"
#include "common/tetUtil.h"
#include "common/init.h"
#include "common/programs.h"
#include "common/camera.h"
#include "common/demoContext.h"

using namespace std;
using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec2;

static const bool SingleBuilding = true;

void
Buildings::Init()
{
    _emptyVao.InitEmpty();

    _templates.resize(SingleBuilding ? 1 : 4);
    _batches.resize(_templates.size());
     
    float thickness = 3;
    float topRadius =  8.0f / 13.0f;
    float tetSize = 0.1f;
    int nSides = 5;
    _GenerateBuilding(thickness, topRadius, tetSize, nSides, &_templates[0]);

     if (not SingleBuilding) {
         thickness = 2.5f;
         topRadius =  1.0f;
         tetSize = 0.2f;
         nSides = 4;
         _GenerateBuilding(thickness, topRadius, tetSize, nSides, &_templates[1]);
         thickness = 2.5f;
         topRadius =  1.2f;
         tetSize = 0.2f;
         nSides = 3;
         _GenerateBuilding(thickness, topRadius, tetSize, nSides, &_templates[2]);
         thickness = 4;
         topRadius =  1;
         tetSize = 0.3f;
         nSides = 32;
         _GenerateBuilding(thickness, topRadius, tetSize, nSides, &_templates[3]);
     }

     _batches[0].Template = &_templates[0];
     _batches[0].Instances.resize(1);
     _batches[0].Instances[0].BoundariesOnly = false;
     _batches[0].Instances[0].EnableCullingPlane = true;
     _batches[0].Instances[0].CullingPlaneY = 10.0;
     _batches[0].Instances[0].GroundPosition = vec2(0, 0);
     _batches[0].Instances[0].Height = 1;
     _batches[0].Instances[0].Radius = 1;
     _batches[0].Instances[0].Hue = 0.6;
     _batches[0].Instances[0].ShowWireframe = false;

     if (not SingleBuilding) {
         _batches[1].Template = &_templates[1];
         _batches[1].Instances.resize(2);
         _batches[1].Instances[0].BoundariesOnly = true;
         _batches[1].Instances[0].EnableCullingPlane = false;
         _batches[1].Instances[0].GroundPosition = vec2(-20, 0);
         _batches[1].Instances[0].Height = 0.5;
         _batches[1].Instances[0].Radius = 1.0;
         _batches[1].Instances[0].Hue = 0.3;
         _batches[1].Instances[1] = _batches[1].Instances[0];
         _batches[1].Instances[1].GroundPosition = vec2(12, 12);
         _batches[1].Instances[1].Height = 0.3;
         _batches[1].Instances[1].Radius = 1.2;
 
         _batches[2].Template = &_templates[2];
         _batches[2].Instances.resize(2);
         _batches[2].Instances[0].BoundariesOnly = true;
         _batches[2].Instances[0].EnableCullingPlane = false;
         _batches[2].Instances[0].GroundPosition = vec2(-15, 30);
         _batches[2].Instances[0].Height = 1.3;
         _batches[2].Instances[0].Radius = 1.0;
         _batches[2].Instances[0].Hue = 0.1;
         _batches[2].Instances[1] = _batches[2].Instances[0];
         _batches[2].Instances[1].GroundPosition = vec2(15, 30);

         _batches[3].Template = &_templates[3];
         _batches[3].Instances.resize(2);
         _batches[3].Instances[0].BoundariesOnly = true;
         _batches[3].Instances[0].EnableCullingPlane = false;
         _batches[3].Instances[0].GroundPosition = vec2(0, -30);
         _batches[3].Instances[0].Height = 1.4;
         _batches[3].Instances[0].Radius = 0.8;
         _batches[3].Instances[0].Hue = 0.0;
         _batches[3].Instances[1] = _batches[3].Instances[0];
         _batches[3].Instances[1].GroundPosition = vec2(13, -28);
         _batches[3].Instances[1].Radius = 0.4;
         _batches[3].Instances[1].Height = 0.4;
         _batches[3].Instances[1].Hue = 0.1;
     }

     // Compile shaders
     Programs& progs = Programs::GetInstance();
     progs.Load("Tetra.Simple", false);
     progs.Load("Tetra.Solid", false);
     progs.Load("Buildings.XZPlane", false);
}

void
Buildings::_GenerateBuilding(float thickness,
                             float topRadius,
                             float tetSize,
                             int nSides,
                             BuildingTemplate* dest)
{
    // Create the boundaries
    tetgenio in;
    float r1 = 10.0f;  float r2 = r1 * topRadius;
    float y1 = 0;     float y2 = 20.0f;
    TetUtil::HullFrustum(r1, r2, y1, y2, nSides, &in);
    y1 += thickness; y2 -= thickness;
    r1 -= thickness; r2 -= thickness;
    TetUtil::HullFrustum(r1, r2, y1, y2, nSides, &in);

    // Poke volumetric holes
    Vec3List holePoints;
    holePoints.push_back(vec3(0, 10.0, 0));
    TetUtil::AddHoles(holePoints, &in);

    // Tetrahedralize the boundary mesh
    tetgenio out;
    const float qualityBound = 1.414;
    const float maxVolume = tetSize;
    TetUtil::TetsFromHull(in, &out, qualityBound, maxVolume, true);
    dest->TotalTetCount = out.numberoftetrahedra;

    // Populate the per-tet texture data
    Vec4List centroids;
    TetUtil::SortTetrahedra(&centroids, out, &dest->BoundaryTetCount);
    dest->CentroidTexture.Init(centroids);

    // Create a flat list of non-indexed triangles
    Blob massive;
    VertexAttribMask attribs = AttrPositionFlag | AttrNormalFlag;
    TetUtil::PointsFromTets(out, attribs, &massive);
    dest->BuildingVao.Init();
    dest->BuildingVao.AddInterleaved(attribs, massive);

    // Create an index buffer for vertical crack lines
    Blob cracks;
    TetUtil::FindCracks(out, &cracks);
    glGenBuffers(1, &dest->CracksVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dest->CracksVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(cracks[0]) * cracks.size(),
                 &cracks[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void
Buildings::Update()
{
    if (SingleBuilding) {
        GetContext()->duration = std::numeric_limits<float>::infinity();
    }
    float time = GetContext()->elapsedTime;

    if (SingleBuilding) {
        PerspCamera* camera = &GetContext()->mainCam;
        camera->eye.x = 0;
        camera->eye.y = 30;
        camera->eye.z = 40;
        camera->center.y = 10;
        if (SingleBuilding) {
            camera->eye = glm::rotateY(camera->eye, time * 16);
        }
    }
}

void
Buildings::Draw()
{
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Programs& progs = Programs::GetInstance();
    PerspCamera surfaceCam = GetContext()->mainCam;

    // Draw buildings
    glUseProgram(progs["Tetra.Solid"]);
    surfaceCam.Bind(glm::mat4());
    glUseProgram(progs["Tetra.Simple"]);
    surfaceCam.Bind(glm::mat4());

    FOR_EACH(batch, _batches) {
        FOR_EACH(instance, batch->Instances) {
            _DrawBuilding(*batch->Template, *instance);
        }
    }

    // Draw floor
    glDisable(GL_CULL_FACE);
    glUseProgram(progs["Buildings.XZPlane"]);
    surfaceCam.Bind(glm::mat4());
    _emptyVao.Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void
Buildings::_DrawBuilding(BuildingTemplate& templ, BuildingInstance& instance)
{
    Programs& progs = Programs::GetInstance();
    vec3 xlate = vec3(instance.GroundPosition.x, 0, instance.GroundPosition.y);
    vec3 scale = vec3(instance.Radius, instance.Height, instance.Radius);

    glUseProgram(progs["Tetra.Solid"]);
    templ.CentroidTexture.Bind(0, "CentroidTexture");
    templ.BuildingVao.Bind();

    glUniform1f(u("CullY"), instance.EnableCullingPlane ? instance.CullingPlaneY : 999);
    glUniform3fv(u("Translate"), 1, ptr(xlate));
    glUniform1f(u("Height"), instance.Height);
    glUniform3fv(u("Scale"), 1, ptr(scale));
    glUniform1f(u("Hue"), instance.Hue);

    if (instance.BoundariesOnly) {
        glDrawArrays(GL_TRIANGLES, 0, templ.BoundaryTetCount * 4 * 3);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, templ.TotalTetCount * 4 * 3);
    }

    if (instance.ShowWireframe) {
        glUseProgram(progs["Tetra.Simple"]);
        templ.CentroidTexture.Bind(0, "CentroidTexture");
        templ.BuildingVao.Bind();
    
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1,12);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (instance.BoundariesOnly) {
            glDrawArrays(GL_TRIANGLES, 0, templ.BoundaryTetCount * 4 * 3);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, templ.TotalTetCount * 4 * 3);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_POLYGON_OFFSET_LINE);
    }
}
