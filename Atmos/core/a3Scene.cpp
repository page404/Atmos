﻿#include <core/a3Scene.h>
#include <core/log/a3Log.h>
#include <core/a3Ray.h>
#include <core/random/a3Random.h>
#include <core/a3Record.h>

#include <shapes/a3Shape.h>
#include <lights/a3Light.h>
#include <accelerators/a3PrimitiveSet.h>

a3Scene::a3Scene()
{

}

// free the lights and shape outside
a3Scene::~a3Scene()
{
}

bool a3Scene::addShape(a3Shape* shape)
{
    if(!check()) return false;

    return primitiveSet->addShape(shape);
}

bool a3Scene::addLight(a3Light* light)
{
    if(!light)
    {
        a3Log::error("a3Scene::addLight() not allowed nullptr.\n", light);
        return false;
    }

    lights.push_back(light);
    return true;
}

bool a3Scene::check() const
{
    if(!primitiveSet)
    {
        a3Log::error("Before a3Scene::render() was called, primitiveSet: %d not allocated\n", primitiveSet);
        return false;
    }

    return true;
}

bool a3Scene::intersect(const a3Ray& ray, a3IntersectRecord* its) const
{
    bool find = primitiveSet->intersect(ray, its);
    if(find)
    {
        its->localToWorld = its->shape->getShadeToWorld(*its);
        its->worldToLocal = its->localToWorld.getInverse();
        return true;
    }
    else
        return false;
}

bool a3Scene::intersect(const a3Ray& ray) const
{
    return primitiveSet->intersect(ray);
}

a3Spectrum a3Scene::sampleDirect(a3LightSamplingRecord & lRec) const
{
    if(lights.size() == 0)
        return a3Spectrum();

    int numLights = lights.size();
    float lightSelectPdf = 1.0f / numLights;

    // uniform sampling lights
    int index = a3Random::randomInt(0, numLights - 1);
    index = index >= numLights ? numLights - 1 : index;

    a3Spectrum L = lights[index]->sampleDirect(lRec);

    if(lRec.pdf != 0)
    {
        // test visibility ray(lightPoint -> hitPoint)
        a3Ray ray(lRec.p, lRec.d, A3_EPSILON, lRec.distance * (1.0f - A3_EPSILON_SHADOW));

        if(intersect(ray))
            // sth. between sampled point and hitpoint
            return a3Spectrum::zero();

        // lights pdf
        lRec.pdf *= lightSelectPdf;
        lRec.light = lights[index];

        // sample one light equal to all of it
        return L * numLights;
    }
    else
        // can't be sampled on the light
        return a3Spectrum::zero();
}

float a3Scene::pdfLightDirect(const a3LightSamplingRecord & lRec) const
{
    a3Light* light = lRec.light;
    if(light)
        return light->pdf(lRec) / lights.size();
    else
        return 0.0f;
}

a3Spectrum a3Scene::Le(const t3Vector3f& d, const a3IntersectRecord& its) const
{
    if(its.isLight())
        return its.Le(d);
    else
        return a3Spectrum::zero();
}

a3Light * a3Scene::getEnvironmentLight() const
{
    for(auto l : lights)
        if(l->isEnvironment())
            return l;

    return NULL;
}

a3Spectrum a3Scene::evalEnvironment(const a3Ray& ray) const
{
    a3Spectrum L;

    // add radiance from environment light
    for(auto l : lights)
        if(l->isEnvironment())
            L += l->evalEnvironment(ray);

    return L;
}




 