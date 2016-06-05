﻿#include <core/a3Intersection.h>
#include <lights/a3AreaLight.h>
#include <shapes/a3Shape.h>

bool a3Intersection::isValid() const
{
    return t != FLT_MAX;
}

t3Vector3f a3Intersection::getNormal() const
{
    if(shape)
        return shape->getNormal(p, u, v);
    else
        return t3Vector3f::zero();
}

bool a3Intersection::isEmitter() const
{
    if(shape)
        return shape->isEmitter();
    else
        return false;
}

a3Spectrum a3Intersection::getEmission(const t3Vector3f& wo)
{
    a3AreaLight* areaLight = shape->getAreaLight();
    
    t3Vector3f normal = shape->getNormal(p, u, v);

    float costheta = normal.dot(-wo);

    return costheta > 0.0f ? areaLight->getEmission() : a3Spectrum::zero();
}
 