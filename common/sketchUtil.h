#pragma once

#include "glm/glm.hpp"

namespace sketch
{

// Transform the given vector from the coordinate system defined
// by the given plane to world space.
glm::vec3
AddOffset(glm::vec2 p, const Plane* plane);

bool
IsOrthogonal(const CoplanarPath* p1, const CoplanarPath* p2, float epsilon);

bool
IsEquivDirections(glm::vec3 v1, glm::vec3 v2, float epsilon);

}
