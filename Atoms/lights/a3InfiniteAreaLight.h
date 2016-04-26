﻿#ifndef A3_INFINITEAREA_LIGHT_H
#define A3_INFINITEAREA_LIGHT_H

#include <lights/a3Light.h>

class a3ImageDecoder;

class a3InfiniteAreaLight : public a3Light
{
public:
    a3InfiniteAreaLight(const std::string& filePath);

    ~a3InfiniteAreaLight();

    virtual a3Spectrum le(const a3Ray& ray) const;

    virtual a3Spectrum sampleL(const t3Vector3f& normal, 
                               t3Vector3f& wo, const t3Vector3f& p, float* pdf, 
                               const a3LightSample& sample, a3VisibilityTester& vis) const;

    virtual bool isDeltaDistribution() const;

    virtual float pdf(const t3Vector3f&p, const t3Vector3f& wi);

    a3ImageDecoder* image;
};

#endif 
 