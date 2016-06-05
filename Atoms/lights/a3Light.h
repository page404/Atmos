﻿#ifndef A3_LIGHT_H
#define A3_LIGHT_H

#include <t3Math/core/t3Matri4x4.h>
#include <core/a3Ray.h>
#include <core/a3Spectrum.h>

class a3Scene;
class a3LightSample;
class a3VisibilityTester;

class a3Light
{
public:
    a3Light();

    a3Light(const t3Matrix4x4& lightToWorld);

    virtual ~a3Light();

    // 从摄像机出发的正向光路上，给定相交平面信息返回光照辐射度
    virtual a3Spectrum sampleL(t3Vector3f& wo,const t3Vector3f& p, 
                               float* pdf, const a3LightSample& sample, a3VisibilityTester& vis) const = 0;

    // a function for infinite area light
    virtual a3Spectrum le(const a3Ray& ray) const;

    virtual float pdf(const t3Vector3f&p, const t3Vector3f& wi) const;

    // light power distribution is delta?
    virtual bool isDiracDistribution() const = 0;

protected:
    t3Matrix4x4 lightToWorld, worldToLight;
};

// from pbrt
class a3VisibilityTester
{
public:
    void setSegment(const t3Vector3f &p1, float eps1, 
                    const t3Vector3f &p2, float eps2);

    void setRay(const t3Vector3f& p, const t3Vector3f& direction, float eps);

    bool occluded(const a3Scene *scene) const;

    a3Ray ray;
};

#endif 