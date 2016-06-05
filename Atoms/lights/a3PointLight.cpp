﻿#include <lights/a3PointLight.h>
#include <core/a3Warp.h>

a3PointLight::a3PointLight(const t3Matrix4x4& lightToWorld, const a3Spectrum& intensity)
:intensity(intensity), a3Light(lightToWorld)
{
    // 假定w项不为0
    position = lightToWorld.getTranslation();
}

a3PointLight::a3PointLight(const t3Vector3f& position, const a3Spectrum& intensity) :position(position), intensity(intensity)
{
    lightToWorld = t3Matrix4x4::newTranslationMatrix(position);
    worldToLight = lightToWorld.getInverse();
}

a3Spectrum a3PointLight::sampleL(t3Vector3f& wo, const t3Vector3f& p, 
                                 float* pdf, const a3LightSample& sample, a3VisibilityTester& vis) const
{
    t3Vector3f temp = position - p;

    wo = temp.getNormalized();

    // 满足dirac分布
    *pdf = a3UniformSpherePdf();

    // --!误差较大需两重epsilon
    vis.setSegment(p, A3_RAY_BIAS, position, 0.f);

    return intensity / temp.lengthSquared();
}

bool a3PointLight::isDiracDistribution() const
{
    return true;
} 

float a3PointLight::pdf(const t3Vector3f&p, const t3Vector3f& wi) const
{
    return 0.0f;
}

