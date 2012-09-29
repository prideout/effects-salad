#include "tree.h"

#include <glm/gtc/matrix_transform.hpp>

#include "common/camera.h"
#include "common/demoContext.h"
#include "common/programs.h"
#include "common/typedefs.h"

void Tree::Init() {
    name = "Tree";
    Effect::Init();

    Programs& progs = Programs::GetInstance();
    progs.Load("FireFlies.Tree", "FireFlies.Tree.FS", "FireFlies.Tube.VS");
    progs.Load("FireFlies.Blossom", "FireFlies.Blossom.FS", "FireFlies.Quad.VS");

    _treeSys.queue.push_back(new BranchDef());
    _treeSys.GrowAll();

    FloatList leaves;
    FloatList times;
    float leafSize = .02;

    FOR_EACH(branchIt, _treeSys.branches) {
        BranchDef* branch = *branchIt;

        int maxLevel = 6;
        float growTime = 15.0;

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
            tube->startTime = (.25 - (rand() / float(RAND_MAX))*.5) + (growTime / maxLevel) * (maxLevel - branch->levels - ((branch->levels > 0) ? .8 : 0));
            tube->timeToGrow = (growTime / maxLevel) * (branch->levels);

            _branches.push_back(tube);
            tube->Init();
        } else {
            // make a leaf instead
            times.push_back( (.5 - (rand() / float(RAND_MAX))*.5) 
                            + (growTime / maxLevel) 
                            * (maxLevel 
                                - branch->levels 
                                - ((branch->levels > 0) ? .8 : 0)
                                + branch->parentPercent)
                            );
            times.push_back((growTime / maxLevel) * (branch->levels));

            glm::vec3 start = branch->cvs[0];
            branch->norm /= 5.0f;
            leaves.push_back(start.x - leafSize);
            leaves.push_back(start.y - leafSize);
            leaves.push_back(start.z - leafSize);

            leaves.push_back(start.x + leafSize);
            leaves.push_back(start.y - leafSize);
            leaves.push_back(start.z - leafSize);

            leaves.push_back(start.x - leafSize);
            leaves.push_back(start.y + leafSize);
            leaves.push_back(start.z - leafSize);


            leaves.push_back(start.x + leafSize);
            leaves.push_back(start.y - leafSize);
            leaves.push_back(start.z - leafSize);

            leaves.push_back(start.x - leafSize);
            leaves.push_back(start.y + leafSize);
            leaves.push_back(start.z - leafSize);
            
            leaves.push_back(start.x + leafSize);
            leaves.push_back(start.y + leafSize);
            leaves.push_back(start.z - leafSize);



            leaves.push_back(start.x - leafSize);
            leaves.push_back(start.y - leafSize);
            leaves.push_back(start.z - leafSize);

            leaves.push_back(start.x - leafSize);
            leaves.push_back(start.y - leafSize);
            leaves.push_back(start.z + leafSize);

            leaves.push_back(start.x - leafSize);
            leaves.push_back(start.y + leafSize);
            leaves.push_back(start.z - leafSize);


            leaves.push_back(start.x - leafSize);
            leaves.push_back(start.y - leafSize);
            leaves.push_back(start.z + leafSize);

            leaves.push_back(start.x - leafSize);
            leaves.push_back(start.y + leafSize);
            leaves.push_back(start.z - leafSize);
            
            leaves.push_back(start.x - leafSize);
            leaves.push_back(start.y + leafSize);
            leaves.push_back(start.z + leafSize);
        }
    }
    _leaves = Vao(3, leaves);
    _leafData.Init(times);
}

void Tree::Update() {
    Effect::Update();
    FOR_EACH(tube, _branches) {
        (*tube)->Update();
    }
}

void Tree::Draw() {
    Effect::Draw();

    Programs& progs = Programs::GetInstance();
    PerspCamera cam = GetContext()->mainCam;

    glm::mat4 pos = glm::translate(glm::mat4(), glm::vec3(-3,-2.25,2));

    glUseProgram(progs["FireFlies.Tree"]);
    cam.Bind(pos);

    // brown tree color
    glUniform3f(u("MaterialColor"), 0.02, 0.01, 0.001);
    FOR_EACH(tube, _branches) {
        (*tube)->Draw();
    }

    glUseProgram(progs["FireFlies.Blossom"]);
    glUniform3f(u("MaterialColor"), 0.8, 0.1, 0.1);
    glUniform1f(u("Time"), GetContext()->elapsedTime);
    cam.Bind(pos);
    _leaves.Bind();
    _leafData.Bind(0, "LeafData");

    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glDrawArrays(GL_TRIANGLES, 0, _leaves.vertexCount);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);

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

