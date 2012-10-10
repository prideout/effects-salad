#include "tree.h"

#include <glm/gtc/matrix_transform.hpp>

#include "noise/perlin.h"

#include "common/camera.h"
#include "common/demoContext.h"
#include "common/programs.h"
#include "common/typedefs.h"

void 
Tree::Spawn(Particle* part)
{
    int idx = (part->id * 74792) % _leafPoints.size();
    assert(idx < _leafPoints.size());
    part->pos = _leafPoints[idx];

    //part->pos -= off;
    
    part->startPos = part->pos;

    part->ttl = 3.0;

}

void
Tree::Update(Particle* part, float age)
{
    static Perlin noise(2,      /* octaves */ 
                        .1,     /* freq */
                        .02,    /* amp */ 
                        0       /* seed */
                        );

    glm::vec3 rndOff(
                    .5 - (float(rand()) / RAND_MAX),
                    .5 - (float(rand()) / RAND_MAX),
                    .5 - (float(rand()) / RAND_MAX)
                );

    float off = noise.Get(part->pos.x, part->pos.y);
    part->pos.x += 2*off;
    part->pos.y = off + part->startPos.y - age;
    part->pos.z += 2*off;

    part->pos += .05f*rndOff;

}

void Tree::Init() 
{
    name = "Tree";
    Effect::Init();

    Programs& progs = Programs::GetInstance();
    progs.Load("FireFlies.Tree", "FireFlies.Tree.FS", "FireFlies.Tube.VS");
    progs.Load("FireFlies.Blossom", "FireFlies.Blossom.FS", "FireFlies.Blossom.VS");
    progs.Load("FireFlies.FallingLeafsTmp");

    // leaf particle system
    _leafParticles.numParticles = 1000;
    _leafParticles.spawnPerSec = 10.0;
    _leafParticles.controller = this;
    _leafParticles.Init();

    BranchDef* trunk = new BranchDef();
    trunk->isTrunk = true;
    _treeSys.queue.push_back(trunk);
    _treeSys.GrowAll();

    FloatList leaves;
    FloatList leafData;
    Vec3List leafNormals;
    float leafSize = .02;
    int leafCount = 3;

    FOR_EACH(branchIt, _treeSys.branches) {
        BranchDef* branch = *branchIt;

        int maxLevel = 6;
        float startTime = 9.0;
        float growTime = 15.0;
        //float growTime = 30.0 - startTime;
        if (grown) {
            startTime = -1;
            growTime = 0;
        }

        //std::cout << "Branch: " << branch->name << std::endl;;
        if (not branch->isLeaf) {
            Tube* tube = new Tube;
            
            // TODO: need to transfer color also
            // destructively transfer the CVs to avoid copies
            tube->cvs.swap(branch->cvs);
            tube->radius = branch->width;
            if (branch->levels > 3) {
                tube->sidesPerSlice = 7;
                tube->lod = 2;
            } else {
                tube->sidesPerSlice = 3;
                tube->lod = 1;
            }
            tube->startTime = startTime 
                                + (.25 - (rand() / float(RAND_MAX))*.5) 
                                + (growTime / maxLevel) 
                                * (maxLevel - branch->levels - ((branch->levels > 0) ? .8 : 0));
                                
            tube->timeToGrow = (growTime / maxLevel) * (branch->levels);

            if (branch->isTrunk) {
                _branches.insert(_branches.begin(), tube);
            } else {
                _branches.push_back(tube);
            }

            tube->Init();
        } else {
            for (int leaf_i = 0; leaf_i < leafCount; leaf_i++) {
                _leafPoints.push_back(branch->cvs[0]);
                // make a leaf instead
                // start time
                leafData.push_back( startTime
                                + (.1 - (float(rand()) / float(RAND_MAX))*.1) 
                                + (.1 + growTime / maxLevel) 
                                * (maxLevel 
                                    - branch->levels 
                                    - ((branch->levels > 0) ? .8 : 0)
                                    + 3*branch->parentPercent)
                                );
                // grow time
                leafData.push_back(2*(growTime / maxLevel) * (branch->levels));
                // color variation
                leafData.push_back(branch->parentPercent);

                // 
                // Setup the leaf quad and give it some random orientation
                // would be nice to orient the leaf to the branch here, which
                // we could do by using the basis vector the tube would have used
                // 
                glm::vec3 leafQuad[4];
                glm::mat3 basis;

                glm::vec3 tangent(.5 - (rand() / float(RAND_MAX)),
                                  .5 - (rand() / float(RAND_MAX)),
                                  .5 - (rand() / float(RAND_MAX)));
                tangent = glm::normalize(tangent);
                basis[2] = tangent;

                glm::vec3 norm = glm::cross(tangent,glm::vec3(1,0,0));
                float e = glm::dot(norm,norm);
                if (e < 0.01) {
                    norm = glm::cross(tangent,glm::vec3(0,1,0));
                }
                basis[0] = glm::normalize(norm);
                basis[1] = glm::normalize(glm::cross(tangent, norm));

                glm::vec3 n  = basis * glm::vec3(0,0,-1);
                leafNormals.push_back(n);
                // --, +-, ++, -+
                for (int i = 0; i < 4; i++) {
                    leafQuad[i].x = (2*(i & 1) - 1.0) * leafSize;
                    leafQuad[i].y = (2*(i & 2) - 1.0) * leafSize;
                    leafQuad[i].z = 0.0;
                    leafQuad[i] = leafQuad[i] * basis;
                }



                glm::vec3 start = branch->cvs[0];
                branch->norm /= 5.0f;
                for (int q = 0; q < 3; q++) {
                    leaves.push_back(start.x + leafQuad[q].x);
                    leaves.push_back(start.y + leafQuad[q].y);
                    leaves.push_back(start.z + leafQuad[q].z);
                }

                for (int q = 3; q > 0; q--) {
                    leaves.push_back(start.x + leafQuad[q].x);
                    leaves.push_back(start.y + leafQuad[q].y);
                    leaves.push_back(start.z + leafQuad[q].z);
                }
            }
        }
    }
    _leaves = Vao(3, leaves);
    _leafData.Init(leafData);
    _leafNormals.Init(leafNormals);
}

void Tree::Update() 
{
    Effect::Update();
    FOR_EACH(tube, _branches) {
        (*tube)->Update();
    }
    _leafParticles.Update();
}

void Tree::Draw() 
{
    Draw(GetContext()->elapsedTime);
}

void Tree::Draw(float time) 
{
    Effect::Draw();

    Programs& progs = Programs::GetInstance();
    PerspCamera cam = GetContext()->mainCam;

    glm::mat4 xf = glm::translate(glm::mat4(), pos);

    glUseProgram(progs["FireFlies.Tree"]);
    cam.Bind(xf);

    // brown tree color
    glUniform3f(u("MaterialColor"), 0.3, 0.2, 0.15);
    glUniform1f(u("MinOcc"), 0.0);
    FOR_EACH(tube, _branches) {
        (*tube)->Draw();
        glUniform1f(u("MinOcc"), 1.0);
    }

    glUseProgram(progs["FireFlies.Blossom"]);
    glUniform3f(u("Eye"), cam.eye.x, cam.eye.y, cam.eye.z);
    glUniform3f(u("MaterialColor"), 0.8, 0.1, 0.1);
    glUniform1f(u("Time"), time);
    cam.Bind(xf);
  
    _leaves.Bind();
    _leafData.Bind(0, "LeafData");
    _leafNormals.Bind(1, "LeafNormals");

    //glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glDrawArrays(GL_TRIANGLES, 0, _leaves.vertexCount);
    glDisable(GL_BLEND);

    FOR_EACH(partIt, _leafParticles.particles) {
        Particle* part = *partIt;
        if (not part->alive)
            continue;

        glm::mat4 xfl = glm::translate(xf, part->pos - part->startPos);

        cam.Bind(xfl);
        int idx = (part->id * 74792) % _leafPoints.size();

        glDrawArrays(GL_TRIANGLES, idx*6, 6);
    }
 
    //glEnable(GL_CULL_FACE);

    //
    // Draw leaf particles for debugging
    //
    //glUseProgram(progs["FireFlies.FallingLeafsTmp"]);
    //glUniform3f(u("Eye"), cam.eye.x, cam.eye.y, cam.eye.z);
    //cam.Bind(xf);
    //_leafParticles.Draw();


    //PerspCamera cam = GetContext()->mainCam;
    //cam.Bind(glm::mat4());
}

/* -------------------------------------------------------------------------------- 
    Tree growth algorithm
   -------------------------------------------------------------------------------- */


void 
MakeBranches(glm::vec3 start, glm::vec3 end, float width)
{
    //glm::vec3 bdir = start + (end - start); 
    //float endWidth = .015 + .015*(random() / RAND_MAX);
}

