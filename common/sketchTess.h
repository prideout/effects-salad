#pragma once
#include "common/sketchScene.h"
#include "common/typedefs.h"

class Vao;

namespace sketch
{
    class Tessellator
    {
    public:
        Tessellator(const sketch::Scene& scene);
        void PullFromScene();
        void PushToGpu(Vao& vao);
    private:
        const sketch::Scene* _scene;
        Vec3List _verts;
        TriList _tris;
    };
}
