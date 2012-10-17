#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;


// abstract base 
class Camera {
public:
    vec3 eye;
    vec3 center;
    vec3 up;

    Camera();
    Camera(vec3 eye, vec3 center, vec3 up);

    virtual ~Camera() {}

    virtual mat4 GetProjection() = 0;
    virtual mat4 GetView();
    virtual void Bind(const mat4& modelMatrix);
};


class PerspCamera : public Camera {
public:
    float fov;
    float aspect;
    float near;
    float far;

    PerspCamera();

    virtual mat4 GetProjection();
    virtual void Bind(const mat4& modelMatrix);
};




