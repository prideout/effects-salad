#include "tree.h"

#include <glm/gtc/matrix_transform.hpp>

#include "common/camera.h"
#include "common/demoContext.h"
#include "common/programs.h"

void Tree::Init() {
    name = "Tree";
    Effect::Init();

    //Programs& progs = Programs::GetInstance();
    //glUseProgram(progs.Load("Tree.Sky", true));

    _treeSys.queue.push_back(new BranchDef());
    _treeSys.GrowAll();

    FOR_EACH(branchIt, _treeSys.branches) {
        BranchDef* branch = *branchIt;
        //std::cout << "Branch: " << branch->name << std::endl;;
        Tube* tube = new Tube;
        
        // TODO: need to transfer color also
        // destructively transfer the CVs to avoid copies
        int maxLevel = 6;
        float growTime = 15.0;
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
    }
}

void Tree::Update() {
    Effect::Update();
    FOR_EACH(tube, _branches) {
        (*tube)->Update();
    }
}

void Tree::Draw() {
    //Programs& progs = Programs::GetInstance();
    Effect::Draw();


    PerspCamera cam = GetContext()->mainCam;
    
    cam.Bind(glm::translate(glm::mat4(), glm::vec3(-3,-2,2)));
    FOR_EACH(tube, _branches) {
        (*tube)->Draw();
    }

    //glUseProgram(progs["Tree.Sky"]);

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

