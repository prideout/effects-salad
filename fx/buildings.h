#pragma once

#include "common/vao.h"
#include "common/texture.h"
#include "common/effect.h"
#include "fx/cityThreads.h"

struct BuildingInstance {
    bool EnableCullingPlane;
    float CullingPlaneY;
    glm::vec2 GroundPosition;
    float Height;
    float Radius;
    float Hue;
    float ExplosionStart;
};

class CracksEffect;

class Buildings : public Effect {
public:
    Buildings();
    virtual ~Buildings();
    virtual void Init();
    virtual void Update();
    virtual void Draw();

    Effect* Cracks();

private:

    void _DrawBuilding(BuildingTemplate& templ,
                       BuildingInstance& instance);


    typedef std::vector<BuildingTemplate> TemplateList;
    typedef std::vector<BuildingInstance> InstanceList;

    struct BuildingBatch {
        BuildingTemplate* Template;
        InstanceList Instances;
    };

    typedef std::vector<BuildingBatch> BatchList;

    TemplateList _templates;
    BatchList _batches;
    Vao _emptyVao;
    CracksEffect* _cracks;

    friend class CracksEffect;
};
