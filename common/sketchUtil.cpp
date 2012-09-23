#include "common/sketchup.h"
#include "common/sketchUtil.h"

using namespace sketch;
using namespace glm;

vec3
sketch::AddOffset(vec2 p2, const Plane* plane)
{
    vec3 p3 = plane->GetCenterPoint();
    p3 += plane->GetCoordSys() * vec3(p2.x, 0, p2.y);
    return p3;
}

bool
sketch::IsOrthogonal(const CoplanarPath* p1, const CoplanarPath* p2, float epsilon)
{
    float dp = dot(p1->GetNormal(), p2->GetNormal());
    return dp < epsilon;
}
