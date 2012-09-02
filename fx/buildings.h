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
    int NumCracks;
};

struct BuildingInstance {
    bool BoundariesOnly;
    bool EnableCullingPlane;
    float CullingPlaneY;
    glm::vec2 GroundPosition;
    float Height;
    float Radius;
    float Hue;
};

class Buildings : public Effect {

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

public:
    Buildings() : Effect() {}
    virtual ~Buildings() {} 
    virtual void Init();
    virtual void Update();
    virtual void Draw();

private:
    void _GenerateBuilding(float thickness,
                           float topRadius,
                           float tetSize,
                           int nSides,
                           BuildingTemplate* dest);

    void _DrawBuilding(BuildingTemplate& templ,
                       BuildingInstance& instance);
};
