#pragma once

#include "common/vao.h"
#include "common/texture.h"
#include "common/effect.h"

struct BuildingTemplate {
    BufferTexture CentroidTexture;
    int TotalTetCount;
    int BoundaryTetCount;
    Vao BuildingVao;
    Vao CracksVao;
    Vao HullVao;
    int NumCracks;
};

struct BuildingInstance {
    bool EnableCullingPlane;
    float CullingPlaneY;
    glm::vec2 GroundPosition;
    glm::vec3 Scale;
    float Hue;
    float ExplosionStart;
};

class CracksEffect;

class Buildings : public Effect {
public:
    Buildings(bool explode);
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
    Vao _terrainVao;
    CracksEffect* _cracks;
    bool _explode;

    friend class CracksEffect;
};
