#pragma once

#include "glm/glm.hpp"
// specialized matrix ops
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

// abstract base 
class Camera {
public:
    mat4 projection;
    mat4 view;

    Camera() {}
    virtual ~Camera() {}

    void LookAt(vec3 eye, vec3 center, vec3 up);
};


class PerspCamera : public Camera {
public:
    float fov;
    float aspect;
    float near;
    float far;

    PerspCamera();
};


class OrthoCamera : public Camera {
public:
    // TODO: implement ortho cam
};
