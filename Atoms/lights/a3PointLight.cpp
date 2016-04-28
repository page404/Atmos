﻿#include <lights/a3PointLight.h>
#include <core/a3Warp.h>

a3PointLight::a3PointLight(const t3Matrix4x4& lightToWorld, const a3Spectrum& intensity)
:intensity(intensity), a3Light(lightToWorld)
{
    // 假定w项不为0
    position = lightToWorld.getTranslation();
}

a3Spectrum a3PointLight::sampleL(const t3Vector3f& normal, t3Vector3f& wo, 
                                 const t3Vector3f& p, float* pdf, 
                                 const a3LightSample& sample, a3VisibilityTester& vis) const
{
    t3Vector3f temp = position - p;

    wo = temp.getNormalized();

    // 满足delta分布
    *pdf = a3UniformSpherePdf();

    // --!误差较大需两重epsilon
    vis.setSegment(p, 0, position, 0.f);

    return intensity / temp.lengthSquared();
}

bool a3PointLight::isDeltaDistribution() const
{
    return true;
} 

float a3PointLight::pdf(const t3Vector3f&p, const t3Vector3f& wi)
{
    return 0.0f;
}

