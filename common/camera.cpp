#include "camera.h"
#include "pez/pez.h"
#include "init.h"

Camera::Camera() :
    eye(vec3(0,0,0)),
    center(vec3(0,0,-1)),
    up(vec3(0,1,0)) { 
}

Camera::Camera(vec3 eye, vec3 center, vec3 up) :
    eye(eye),
    center(center),
    up(up) { 
}

mat4
Camera::GetView() {
    return glm::lookAt(eye, center, up);
}

void
Camera::Bind(const mat4& model) {
    // XXX: could cache this, but waiting until the need arises
    mat4 projection = GetProjection();
    mat4 view = GetView(); 
    mat4 modelView = view * model;
    glUniformMatrix4fv(u("Projection"), 1, 0, &projection[0][0]);
    glUniformMatrix4fv(u("ViewMatrix"), 1, 0, &view[0][0]);
    glUniformMatrix4fv(u("ModelMatrix"), 1, 0, &model[0][0]);
    glUniformMatrix4fv(u("Modelview"), 1, 0, &modelView[0][0]);
}


/*
 * PERSPECTIVE CAMERA
 */

PerspCamera::PerspCamera() :
    Camera(),
    fov(45),
    aspect(1.0),
    near(1.0),
    far(100)
{
}

mat4
PerspCamera::GetProjection() {
    return glm::perspective(fov, aspect, near, far);
}
