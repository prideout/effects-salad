#pragma once

#include "glm/glm.hpp"

#include "camera.h"
#include "curve.h"
#include "typedefs.h"


//
// A camera path provides higher level camera control logic based on curves
//
class CameraPath {
public:
    AnimCurve<glm::vec3> eye;
    AnimCurve<glm::vec3> center;
    AnimCurve<glm::vec3> up;

    CameraPath() {}

    // Initialization helpers
    void InitEye(Vec3List cvs, float startTime, float duration);

    void InitCenter(Vec3List cvs, float startTime, float duration); 

    void InitUp(Vec3List cvs, float startTime, float duration);

    // Apply the available curves to the given camera at the specified time
    void Apply(float time, Camera* cam);

};

