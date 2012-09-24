#include "fx/buildingGrowth.h"

using namespace sketch;
using namespace std;

BuildingGrowth::BuildingGrowth()
{
}

BuildingGrowth::~BuildingGrowth()
{
}

void
BuildingGrowth::Init()
{
    const Plane* ground = _sketch.GroundPlane();
    glm::vec2 offset(0, 0);
    const float width = 2;
    const float depth = 1;

    CoplanarPath* rect =
        _sketch.AddRectangle(width, depth, ground, offset);

    float height = 1;
    ConstPathList walls;
    _sketch.PushPath(rect, height, &walls);

    if (false) {
        Json::Value root = _sketch.Serialize();
        Json::StyledWriter writer;
        cout << writer.write(root) << endl;
    }
}

void
BuildingGrowth::Update()
{
}

void
BuildingGrowth::Draw()
{
}
