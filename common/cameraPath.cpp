#include "cameraPath.h"

void 
CameraPath::InitEye(Vec3List cvs, float startTime, float duration) 
{ 
    eye = AnimCurve<glm::vec3>(cvs, startTime, duration); 
}

void 
CameraPath::InitCenter(Vec3List cvs, float startTime, float duration) 
{ 
    center = AnimCurve<glm::vec3>(cvs, startTime, duration); 
}

void 
CameraPath::InitUp(Vec3List cvs, float startTime, float duration) 
{ 
    up = AnimCurve<glm::vec3>(cvs, startTime, duration); 
} 


void
CameraPath::Apply(float time, Camera* cam) 
{
    if (not eye.IsEmpty()) 
        cam->eye = eye.At(time);

    if (not center.IsEmpty()) 
        cam->center = center.At(time);

    if (not up.IsEmpty()) 
        cam->up = up.At(time);
}


