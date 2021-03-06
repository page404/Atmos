﻿#include <sensors/a3Sensor.h>
#include <core/image/a3Film.h>
#include <core/log/a3Log.h>

a3Sensor::a3Sensor(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up,
                   float focalLength, float apretureWidth, float apretureHeight, float canvasDistance, 
                   float focalDistance, float lensRadius,
                   a3Film* image)
                   : origin(origin), up(up), 
                   focalLength(focalLength), apreture(apretureWidth, apretureHeight), canvasDistance(canvasDistance), 
                   focalDistance(focalDistance), lensRadius(lensRadius), 
                   image(image)
{
    // 这里不直接给出fov而是间接计算
    // 详情可见 http://www.scratchapixel.com/lessons/3d-basic-rendering/3d-viewing-pinhole-camera/virtual-pinhole-camera-model
    fov.x = 2.0f * t3Math::atanRad(focalLength / (apretureWidth / 2.0f));
    fov.y = 2.0f * t3Math::atanRad(focalLength / (apretureHeight / 2.0f));

    canvasSize.x = canvasDistance * t3Math::tanRad(fov.x / 2.0f);
    canvasSize.y = canvasDistance * t3Math::tanRad(fov.y / 2.0f);

    setCameraToWorld(origin, lookat, up);
}

float a3Sensor::castRay(const a3CameraSample* sample, a3Ray* ray) const
{
    a3Log::warning("Unimplemented a3Camera::generateRay() method called");

    return 0.0f;
}

void a3Sensor::setCameraToWorld(const t3Vector3f& origin, const t3Vector3f& lookat, const t3Vector3f& up)
{
    this->origin = origin;
    this->lookat = lookat;
    this->direction = (lookat - origin).getNormalized();

    if((up.getNormalized()).getCrossed(this->direction).length() == 0)
    {
        a3Log::error("\"up\" vector (%f, %f, %f) and viewing direction (%f, %f, %f) "
              "passed to LookAt are pointing in the same direction. Using "
              "the identity transformation.\n", up.x, up.y, up.z, direction.x, direction.y,
              direction.z);
        return;
    }

    this->right = (up.getNormalized()).getCrossed(this->direction).getNormalized();
    this->up = this->direction.getCrossed(this->right);

    // 平移矩阵直接可直接作用光线原点 减少矩阵计算量
    // world to camera
    //cameraToWorld._mat[0].set(this->right.x, this->right.y, this->right.z, 0);
    //cameraToWorld._mat[1].set(this->up.x, this->up.y, this->up.z, 0);
    //cameraToWorld._mat[2].set(this->direction.x, this->direction.y, this->direction.z, 0);
    //cameraToWorld._mat[3].set(this->origin.x, this->origin.y, this->origin.z, 1);
    cameraToWorld._mat[0].set(this->right.x, this->up.x, this->direction.x, this->origin.x);
    cameraToWorld._mat[1].set(this->right.y, this->up.y, this->direction.y, this->origin.y);
    cameraToWorld._mat[2].set(this->right.z, this->up.z, this->direction.z, this->origin.z);
    cameraToWorld._mat[3].set(0, 0, 0, 1);

    // camera to world
    //cameraToWorld = cameraToWorld.getInverse();
}

void a3Sensor::print()
{
    a3Log::debug("Camera position: [%f, %f, %f]\n", origin.x, origin.y, origin.z);
    a3Log::debug("Camera lookat: [%f, %f, %f]\n", lookat.x, lookat.y, lookat.z);
    t3Vector3f oritation = (lookat - origin).getNormalized();
    a3Log::debug("Camera oritation: [%f, %f, %f]\n", oritation.x, oritation.y, oritation.z);
    a3Log::debug("Camera up: [%f, %f, %f]\n", up.x, up.y, up.z);
    a3Log::debug("fov: %f, %f\n", t3Math::Rad2Deg(fov.x), t3Math::Rad2Deg(fov.y));
    a3Log::debug("focal distance: %f, lens radius: %f\n", focalDistance, lensRadius);
}
 