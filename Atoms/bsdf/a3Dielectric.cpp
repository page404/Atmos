﻿#include <bsdf/a3Dielectric.h>
#include <shapes/a3Shape.h>

a3Dieletric::a3Dieletric(a3Spectrum R) :R(R)
{

}

a3Spectrum a3Dieletric::sample(const t3Vector3f& wi, t3Vector3f& wo, float* pdf, const a3Intersection& its) const
{
    static a3Random random;
    t3Vector3f normal = its.getNormal();

    float cosTheta1 = wi.dot(normal);

    // 是否在光密介质内部
    bool into = cosTheta1 < 0;

    // n = 入射光所在材质折射率 / 出射材质折射率
    float n1 = 1.0f, n2 = its.shape->refractiveIndex;
    float n = into ? n1 / n2 : n2 / n1;

    float cosTheta2 = 1 - n * n * (1 - cosTheta1 * cosTheta1);

    *pdf = 1.0f;

    // 全反射
    if(cosTheta2 < 0)
    {
        wo = (wi - normal * 2 * (wi.dot(normal))).normalize();

        return a3Spectrum(R) / t3Math::Abs(wo.dot(normal));
    }

    // Schlick的近似方程
    float R0 = (1.0f - n) / (1.0 + n);
    R0 = R0 * R0;

    // 折射与反射概率可求 通过[0, 1)之间生成随机数 根据大小间接完成折射反射选择
    // --!详情可见 http://www.kevinbeason.com/smallpt/
    t3Vector3f _wo, _wr;

    _wo = n * (wi - normal * cosTheta1) - (into ? normal : -normal) * t3Math::sqrt(cosTheta2);

    // 反射光线方向
    _wr = wi - 2 * (wi.dot(normal)) * normal;

    // 发生反射的概率
    float probablity = R0 + (1 - R0) * t3Math::pow((1 - t3Math::Abs(cosTheta1)), 5);

    if(random.randomFloat() > probablity)
    {
        wo = _wo;
        // 二项分布无需计算cosTheta, 先除则在外部累乘时抵消
        return a3Spectrum(R) / t3Math::Abs(_wo.dot(normal));
    }
    else
    {
        wo = _wr;
        // 二项分布无需计算cosTheta, 先除则在外部累乘时抵消
        return a3Spectrum(R) / t3Math::Abs(_wr.dot(normal));
    }
}

a3Spectrum a3Dieletric::eval(const t3Vector3f& wi, const t3Vector3f& wo, const a3Intersection& its) const
{
    // delta分布在给定出射方向上几率为0
    return a3Spectrum(0.0f);
}

bool a3Dieletric::isDeltaDistribution() const
{
    return true;
}

float a3Dieletric::pdf(const t3Vector3f& wi, const t3Vector3f& wo, const a3Intersection& its) const
{
    return 0.0f;
}

//bool a3Dieletric::isReflection() const
//{
//    return false;
//}

 