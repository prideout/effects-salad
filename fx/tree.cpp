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
        std::cout << "Branch: " << branch->name << std::endl;;
        Tube* tube = new Tube;
        
        // TODO: need to transfer color also
        // destructively transfer the CVs to avoid copies
        tube->cvs.swap(branch->cvs);
        tube->sidesPerSlice = 3;
        tube->lod = 1;
        tube->startTime = (10.0 / 6) * (6 - branch->levels);
        tube->timeToGrow = (10.0 / 6) * (branch->levels);

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

