﻿#include <core/a3Scene.h>
#include <shapes/a3Shape.h>
#include <core/a3Ray.h>
#include <core/a3Intersection.h>
#include <lights/a3Light.h>
#include <core/log/a3Log.h>

a3Scene::a3Scene()
{

}

bool a3Scene::intersect(const a3Ray& ray, a3Intersection* intersection) const
{
    float minT = FLT_MAX;
    float t = FLT_MAX;

    for(int i = 0; i<objects.size(); i++)
    {
        if((objects[i]->intersect(ray, &t)) && t > A3_TOLERANCE_FLOAT && t < minT)
        {
            minT = t;

            // 序号代替ID
            intersection->shape = objects[i];
        }
    }

    if(minT < FLT_MAX && minT > A3_TOLERANCE_FLOAT)
    {
        intersection->t = minT;

        // 接触点初始化
        intersection->p = (ray)(minT);

        return true;
    }
    else
    {
        // 用于valid判断
        intersection->t = FLT_MAX;

        return false;
    }
}

bool a3Scene::intersect(const a3Ray& ray) const
{
    float minT = FLT_MAX;
    float t = FLT_MAX;

    for(int i = 0; i<objects.size(); i++)
    {
        if((objects[i]->intersect(ray, &t)) && t > A3_TOLERANCE_FLOAT && t < minT)
            minT = t;
    }

    if(minT < ray.maxT && minT > ray.minT)
        return true;
    else
        return false;
}

bool a3Scene::addShape(a3Shape* shape)
{
    if(!shape)
    {
        a3Log::error("空的形状对象指针%d\n", shape);
        return false;
    }

    objects.push_back(shape);
    return true;
}

bool a3Scene::addLight(a3Light* light)
{
    if(!light)
    {
        a3Log::error("空的形状对象指针%d\n", light);
        return false;
    }

    lights.push_back(light);
    return true;
}

a3Spectrum a3Scene::le(const a3Ray& ray) const
{
    a3Spectrum le;

    // 添加可能有的无限远区域光照
    for(auto l : lights)
        le += l->le(ray);

    return le;
}

 