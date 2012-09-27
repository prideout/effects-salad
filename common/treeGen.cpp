
#include "treeGen.h"
#include "curve.h"

#include <sstream>

glm::vec3
GetAxis(float theta, float phi) {
    // return unit vector using spherical coords
    return glm::vec3(sin(theta) * cos(phi),
                     cos(theta),
                     sin(theta) * sin(phi));
}

void
TreeSystem::GrowAll() { 
    // could easily multi-thread this if it becomes a bottleneck
    while(queue.size()) {
        GrowBranch();
    }
}

void
TreeSystem::GrowBranch() { 
    float length;
    unsigned curIndex = queue.size() - 1;
    BranchDef* branch = queue[curIndex];

    //std::cout << branch->level << std::endl;

    if (branch->level < 2) {
        branches.push_back(branch);
        queue.pop_back();
    }

    if (branch->isLeaf) {
        //length = 4;
        length = lengths[0] / 4;
    } else {
        length = lengths[branch->level - 1];
    }

    glm::vec3 pos = branch->pos;
    glm::vec3 end = branch->pos + branch->norm*length;

    int numCvs = 3;
    if (branch->cvs.size() == 0)
        numCvs++;

    float curWidth = 0;
    float nextWidth = 0;

    if (branch->isLeaf) {
        curWidth = 1;
        nextWidth = 1;
    } else {
        curWidth = Lerp(0.0f, branch->width, branch->level / float(branch->levels));
        nextWidth = Lerp(0.0f, branch->width, (branch->level-1) / float(branch->levels));
    }

    // force some continuity between segments
    int cvCount = branch->cvs.size();
    if (cvCount > 0) {
        glm::vec3 dir = branch->cvs[cvCount - 1] - 
                        branch->cvs[cvCount - 2];
        branch->cvs.push_back(branch->cvs[cvCount - 1] + dir);
    }

    float curveAmt = .09;
    for (int i = (cvCount > 0 ? 1 : 0); i < numCvs; i++) {
        float z = float(i) / (numCvs - 1.0);

        // Use a linear transition to guide the overall growth
        // but mix in some noise with the CVs to add some curviness 
        glm::vec3 cv = Lerp(pos, end, z);
        if (cvCount != 0 or i > 0) {
            cv = (1-curveAmt) * cv + curveAmt * (glm::vec3(.5f, .5f, .5f) - glm::vec3(float(rand()) / RAND_MAX, 
                                           float(rand()) / RAND_MAX, 
                                           float(rand()) / RAND_MAX));
        }
        branch->cvs.push_back(cv);
        //_pvec3(branch->cvs[branch->cvs.size()-1]);
    }

    const float PI = 3.1415;
    int childCount = childCounts[branch->level - 1];
    if (not branch->isLeaf and childCount > 0) {
        // randomly spawn more branch defs
        //float inc = childCount / 2.0*PI;
        //float i2 = 1. / childCount;
        glm::vec2 chDist = childDist[branch->level -1 ];
        glm::vec2 chAngle = childAngles[branch->level - 1];
        for (int i = 0; i < childCount; i++) {
            float a = chDist.x + chDist.y * (float(rand()) / RAND_MAX);

            BranchDef* child = new BranchDef;

            std::stringstream name;
            name << branch->name << "." << branch->level << "_" << i;
            child->name = name.str();
            
            child->parentPercent = a;

            //std::cout << child->name << std::endl;

            child->pos = Bezier::EvalAt(a, branch->cvs, branch->cvs.size() - 4, 4);  //Lerp(pos, end, a);
            
            //child->norm = branch->norm*.5f + glm::vec3(float(rand())/RAND_MAX, float(rand())/RAND_MAX, float(rand())/RAND_MAX)*.5f;
            //child->norm = glm::normalize(child->norm);
            //child->norm = GetAxis(glm::smoothstep(chAngle.x, chAngle.y, float(rand())/RAND_MAX), 
            //                     glm::smoothstep(0.0f, 2.0f*PI, float(rand())/RAND_MAX));
            child->norm = GetAxis(Lerp(chAngle.x, chAngle.y, float(rand())/RAND_MAX), 
                                  Lerp(0.0f, 2.0f*PI, float(rand())/RAND_MAX));

            child->width = Lerp(curWidth, nextWidth, a);

            child->level = child->levels = branch->level - 1;

            if (branch->level == 1) {
                child->pos = Bezier::EvalAt(a, branch->cvs, branch->cvs.size() - 4, 4)  //Lerp(pos, end, a) 
                            + .35f
                               *(.25f-.5f*glm::vec3(float(rand())/RAND_MAX, float(rand())/RAND_MAX, float(rand())/RAND_MAX));
                child->name += "_LEAF";
                child->level = 0;
                child->levels = 1;
                child->color = glm::vec3(.8, .1, .1);
                child->isLeaf = true;
            }

            queue.push_back(child);
        }

    }

    branch->norm = branch->norm + .5f*glm::vec3(float(rand())/RAND_MAX, float(rand())/RAND_MAX, float(rand())/RAND_MAX);
    branch->norm = glm::normalize(branch->norm);
    branch->pos = end;

    branch->level -= 1;
}


