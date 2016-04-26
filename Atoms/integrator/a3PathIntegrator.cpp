﻿#include <integrator/a3PathIntegrator.h>

#include <core/a3Ray.h>
#include <core/a3Intersection.h>
#include <core/a3Settings.h>
#include <core/a3Scene.h>
#include <core/a3Spectrum.h>

#include <bsdf/a3BSDF.h>
#include <lights/a3Light.h>

a3PathIntegrator::a3PathIntegrator()
{
    this->maxDepth = A3_DEFAULT_MAX_DEPTH;
}

a3PathIntegrator::a3PathIntegrator(int maxDepth) 
{
    this->maxDepth = maxDepth;
}

// its = first ray intersection
//
// while depth < maxDepth
//      if !its.valid
//           evironment lighting
//
//      Le += self emission
//      
//      Li += estimateDirect()
//      
//      throughput *= bsdf * cos
//
//      trace new ray direction
//
//      depth++
//  
//      [optional]
//      throughput /= russian rulette
// end
//
// return Li
a3Spectrum a3PathIntegrator::li(const a3Ray& _ray, const a3Scene& scene) const
{
    a3Ray ray(_ray);
    // 直接光项(光源, 自发光)
    a3Spectrum Li, Le;
    a3Intersection its;

    // 第一次相交光线出射方向
    t3Vector3f wo;

    // 路径上所有brdf * cosTheta * reflectivity/transmission累乘
    a3Spectrum throughput(1.0f);

    /* ==================================================================== */
    /*                          First Ray Intersection                      */
    /* ==================================================================== */
    // 执行第一次求交测试
    if(!scene.intersect(ray, &its))
    {
        // 添加可能有的无限远区域光照
        Li += scene.le(ray);

        return Li;
    }

    // 当前深度
    int depth = 0;

    // 是否是镜面反射/折射
    bool isDelta = false;

    /* ==================================================================== */
    /*                                 Path Tracing                         */
    /* ==================================================================== */
    while(depth <= maxDepth)
    {
        // 第一次相交到了面积光光源 / 上一帧求交bsdf为delta分布形式
        if(depth == 0 || isDelta)
        { /*缺省的AreaLight计算*/ }

        a3BSDF* bsdf = its.shape->getBSDF();

        // 自发光统计贡献
        Le += its.shape->emission;

        /* ==================================================================== */
        /*                             Estimate Direct                          */
        /* ==================================================================== */
        float bsdfPdf = 0.0f;

        // 初始化出射光线方向信息
        a3Spectrum bsdfValue = bsdf->sample(ray.direction, wo, &bsdfPdf, its);

        //if(!bsdf->isDeltaDistribution())
        Li += throughput * estimateDirect(ray.direction, wo, isDelta, bsdfValue, bsdfPdf, *bsdf, scene, its);

        float cosTheta = t3Math::Abs(wo.dot(its.getNormal()));

        if(bsdfPdf == 0.0f || bsdfValue == a3Spectrum::zero())
            break;

        // Monte Carlo Estimate
        throughput *= bsdfValue * cosTheta / bsdfPdf;

        depth++;

        ray.set(its.p, wo);

        // 执行求交测试
        if(!scene.intersect(ray, &its))
        {
            if(isDelta)
                // 直接光照中Delta形式bsdf被忽略计算
                // 添加可能有的无限远区域光照
                Li += throughput * scene.le(ray);

            break;
        }
    }

    return Li + Le;
}
 