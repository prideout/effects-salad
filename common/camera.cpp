#include "camera.h"
#include "pez/pez.h"
#include "init.h"
#include "glm/gtc/type_ptr.hpp"


/*
 * CAMERA
 */
Camera::Camera() :
    eye(vec3(0,0,1)),
    center(vec3(0,0,0)),
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
    glUniformMatrix4fv(u("Projection"), 1, 0, ptr(projection));
    glUniformMatrix4fv(u("ViewMatrix"), 1, 0, ptr(view));
    glUniformMatrix4fv(u("ModelMatrix"), 1, 0, ptr(model));
    glUniformMatrix4fv(u("Modelview"), 1, 0, ptr(modelView));

    // Assume uniform scale for now to avoid expensive inverse:
    mat3 normalMatrix = mat3(modelView);
    glUniformMatrix3fv(u("NormalMatrix"), 1, 0, ptr(normalMatrix));
}

void
PerspCamera::Bind(const mat4& model) {
    Camera::Bind(model);
    glUniform1f(u("DistanceScale"), 1.0f / (far - near));
}


/*
 * PERSPECTIVE CAMERA
 */

PerspCamera::PerspCamera() :
    Camera(),
    fov(45),
    aspect(1.0),
    near(1.0),
    far(200)
{
}

mat4
PerspCamera::GetProjection() {
    return glm::perspective(fov, aspect, near, far);
}
