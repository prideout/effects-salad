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

    void LookAt(const vec3& eye, const vec3& center, const vec3& up);

    void Bind(const mat4& modelMatrix);
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
