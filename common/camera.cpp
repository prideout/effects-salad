#include "camera.h"
#include "pez/pez.h"
#include "init.h"


void
Camera::LookAt(const vec3& eye, const vec3& center, const vec3& up) {
    view = glm::lookAt(eye, center, up);
}

void
Camera::Bind(const mat4& model) {
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
    fov(45),
    aspect(1.0),
    near(1.0),
    far(100)
{
    projection = glm::perspective(fov, aspect, near, far);
    LookAt(vec3(0,0,0), vec3(0,0,-1), vec3(0,1,0));
}

