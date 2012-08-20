#include "camera.h"

void
Camera::LookAt(vec3 eye, vec3 center, vec3 up) {
    view = glm::lookAt(eye, center, up);
}

PerspCamera::PerspCamera() :
    fov(45),
    aspect(1.0),
    near(1.0),
    far(1000)
{
    projection = glm::perspective(fov, aspect, near, far);
    LookAt(vec3(0,0,0), vec3(0,0,-1), vec3(0,1,0));
}
